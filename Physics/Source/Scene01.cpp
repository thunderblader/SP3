#include "Scene01.h"
#include "GL\glew.h"
#include "Application.h"
#include "KeyboardController.h"
#include "MouseController.h"
#include "SoundEngine.h"
#include "Terrain\LoadHmap.h"
#include "Physics\Physics.h"
#include "Particle\Particle.h"

#include <sstream>
#include <fstream>

using std::ifstream;
using std::istringstream;
using std::ofstream;
using std::ios;
using std::getline;
using std::cout;
using std::endl;

Scene01::Scene01()
{
}

Scene01::~Scene01()
{
}

void Scene01::Init()
{
	SceneBase::Init();

	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	m_TerrainHeight = 20.f;
	m_TerrainWidth = 500;

	m_speed = 40.f;

	Math::InitRNG();

	m_objectCount = 0;
	m_ballCount = 0;

	free_look = false;

	file.Init(&m_goList);
	file.Load(false, "Image//Test_Level.csv");

	

	file.Load(true, "Image//shop_data.csv");

	shop.Load_Shop();

	shop.Purchase_Item(0);

	m_ghost = new GameObject(GameObject::GO_BALL);

	m_player = Player::GetInstance();
	m_player->Init(FetchGO(), FetchGO(), GameObject::GO_BLOCK, Vector3(-50, 25, 0), Vector3(5, 4, 1), 1.f, 50.f);
	m_player->SetHeightmap(&m_heightMap, m_TerrainWidth, m_TerrainHeight);
	m_control = new Controller(m_player);
	m_control->LoadConfig("Data//Config.ini", param_physics);
	Enemy* enemy = new Enemy();
	enemy->Init(FetchGO(), GameObject::GO_ENEMY_SNOWYETI, Vector3(0.f, 20.f, 0.f), Vector3(5.f, 5.f, 5.f));
	enemyList.push_back(enemy);
	
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			GameObject *bricks = FetchGO();
			bricks->active = true;
			bricks->type = GameObject::GO_BRICK;
			bricks->dir.Set(0, 1, 0);
			bricks->pos.Set(30 + j * 10, 2.5 + 5 * i, 0);
			bricks->scale.Set(5, 5, 1);
		}
	}

	//Vector3 test1, test2;
	//Physics<Vector3>::K1CalcTime(test1, test1, test1);
	//float test3 = 1 / test2;

	m_particleCount = 0;
	MAX_PARTICLE = 100;
}

GameObject* Scene01::FetchGO()
{
	//Exercise 2a: implement FetchGO()
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (!go->active)
		{
			go->active = true;
			++m_objectCount;
			return go;
		}
	}

	//Exercise 2b: increase object count every time an object is set to active
	for (unsigned i = 0; i < 10; ++i)
	{
		m_goList.push_back(new GameObject(GameObject::GO_BALL));
	}

	m_goList[m_goList.size() - 1]->active = true;
	++m_objectCount;

	return m_goList[m_goList.size() - 1];
}

bool Scene01::CheckCollision(GameObject * go1, GameObject * go2, float dt)
{
	switch (go2->type)
	{
	case GameObject::GO_BALL:
	{
		Vector3 dis = go1->pos - go2->pos;
		Vector3 rel = go1->vel - go2->vel;
		float combinedRadiusSq = (go1->scale.x + go2->scale.x) * (go1->scale.x + go2->scale.x);

		return (rel.Dot(dis) < 0 &&
			dis.LengthSquared() <= combinedRadiusSq);
	}

	case GameObject::GO_BLOCK:
	{
		Vector3 w0 = go2->pos;
		Vector3 b1 = go1->pos;
		Vector3 N = go2->dir;
		Vector3 NP = go2->dir.Cross(Vector3(0.f, 0.f, 1.f));
		float r = go1->scale.x;
		float h = go2->scale.y;
		float l = go2->scale.x;

		if ((w0 - b1).Dot(N) < 0)
			N = -N;

		return (go1->vel.Dot(N) > 0 &&
			abs((w0 - b1).Dot(N)) < r + h * 0.5f) &&
			(abs((w0 - b1).Dot(NP)) < r + l * 0.5f);
	}

	case GameObject::GO_PLAYER:
	{
		Vector3 w0 = go2->pos;
		Vector3 b1 = go1->pos;
		Vector3 N = go2->dir;
		Vector3 NP = go2->dir.Cross(Vector3(0.f, 0.f, 1.f));
		float r = go1->scale.x;
		float h = go2->scale.y;
		float l = go2->scale.x;

		if ((w0 - b1).Dot(N) < 0)
			N = -N;

		return (go1->vel.Dot(N) > 0 &&
			abs((w0 - b1).Dot(N)) < r + h * 0.5f) &&
			(abs((w0 - b1).Dot(NP)) < r + l * 0.5f);
	}
	}

	return 0;
}

void Scene01::CollisionResponse(GameObject * go1, GameObject * go2)
{
	Vector3 N;
	switch (go2->type)
	{
	case GameObject::GO_BALL:
		u1 = go1->vel;
		N = (go2->pos - go1->pos).Normalized();
		u2 = go2->vel;
		u1N = u1.Dot(N) * N;
		u2N = u2.Dot(N) * N;
		go1->vel = u1 + ((2.f * m2) / (m1 + m2)) * (u2N - u1N);
		go2->vel = u2 + ((2.f * m1) / (m1 + m2)) * (u1N - u2N);
		break;

	case GameObject::GO_BLOCK:
		N = go2->dir;
		go1->vel = go1->vel - (2 * go1->vel.Dot(N)) * N;
		break;

	case GameObject::GO_PLAYER:
		N = go2->dir;
		go1->vel = go1->vel - (2 * go1->vel.Dot(N)) * N;
		break;
	}
}

void Scene01::BombCollision(GameObject * go1, GameObject * go2)
{
	Vector3 w0 = go2->pos;
	Vector3 b1 = go1->pos;
	Vector3 N = go2->dir;
	Vector3 NP = N.Cross(Vector3(0, 0, 1));
	float l = go2->scale.y;
	float r = go1->scale.x;
	float h = go2->scale.x;
	if ((w0 - b1).Dot(N) < 0)
		N = -N;

	Vector3 detect(Math::Clamp((b1 - w0).x, 0.f, h / 2), Math::Clamp((b1 - w0).y, 0.f, l / 2), 0);
	detect += w0;

	if ((detect - b1).Length() < r)
	{
		if (!go1->boom)
		{
			go1->vel.SetZero();
			go1->boom = true;

			go2->active = false;
		}
	}
}

void Scene01::UpdateParticles(double dt)
{
	if (m_player->GetVel().Length() > 5 && m_particleCount < MAX_PARTICLE)
	{
		ParticleObject* particle = GetParticle();
		particle->type = ParticleObject_TYPE::P_SPARK;
		particle->scale.Set(1, 1, 1);
		particle->vel.Set(Math::RandFloatMinMax(-5, 0), Math::RandFloatMinMax(5, 0), 0);
		particle->rotationSpeed = Math::RandFloatMinMax(20, 40);
		//particle->pos.Set(Math::RandFloatMinMax(-1700, 1700), 1200, Math::RandFloatMinMax(-1700, 1700));
		particle->pos = m_player->GetPlayerPos();
	}

	std::vector<ParticleObject*>::iterator it, end;
	end = particleList.end();
	for (it = particleList.begin(); it != end; ++it)
	{
		ParticleObject* particle = (ParticleObject*)*it;
		if (!particle->isActive)
			continue;
		if (particle->type == ParticleObject_TYPE::P_SPARK)
		{
			particle->vel.y += -9.8f * (float)dt;
			particle->pos += particle->vel * (float)dt * 10.0f;
			particle->rotation += particle->rotationSpeed * (float)dt;
			if (particle->pos.y <= 1)
			{
				particle->isActive = false;
				--m_particleCount;
			}
		}
		//if (particle->type == ParticleObject_TYPE::P_SMOKE)
		//{
		//	particle->vel -= m_gravity  * 0.5f * (float)dt;
		//	particle->pos += particle->vel * (float)dt * 10.0f;
		//	particle->rotation += particle->rotationSpeed * (float)dt;

		//	if (particle->pos.y > smokepos.y + 100)
		//	{
		//		particle->isActive = false;
		//		--m_particleCount;
		//	}
		//}
	}
}

void Scene01::Update(double dt)
{
	SceneBase::Update(dt);
	Camera_Control(dt);
	UpdateParticles(dt);
	if (KeyboardController::GetInstance()->IsKeyPressed('L'))
	{
		//file.Save_Data(Level, Score, Gold);
	}
	if (KeyboardController::GetInstance()->IsKeyPressed('K'))
	{
		//file.Load_Data();
	}
	if (KeyboardController::GetInstance()->IsKeyPressed('I'))
	{
		shop.Purchase_Item(0);
	}
	if (KeyboardController::GetInstance()->IsKeyPressed('V'))
	{
		shop.Purchase_Item(1);
	}
	if (KeyboardController::GetInstance()->IsKeyPressed('B'))
	{
		shop.Purchase_Item(2);
	}
	if (KeyboardController::GetInstance()->IsKeyPressed('N'))
	{
		shop.Purchase_Item(3);
	}
	if (KeyboardController::GetInstance()->IsKeyPressed('M'))
	{
		shop.Purchase_Item(4);
	}

	if (KeyboardController::GetInstance()->IsKeyPressed('9'))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if (KeyboardController::GetInstance()->IsKeyPressed('0'))
	{
		m_speed += 0.1f;
	}

	if (KeyboardController::GetInstance()->IsKeyPressed('U')) // Debug key snow yeti shooting
	{
		enemyList[0]->PushProjectile(FetchGO(), m_player->GetPlayerPos(), Vector3(1.f, 1.f, 1.f), 10.f);
	}

	m_player->Update(dt);
	m_control->Update(dt);
	
	vector<Enemy*>::iterator it, end;
	end = enemyList.end();
	for (it = enemyList.begin(); it != end; ++it) (*it)->Update(dt);

	//Mouse Section
	if (MouseController::GetInstance()->IsButtonPressed(MouseController::LMB))
	{
		std::cout << "LBUTTON DOWN" << std::endl;

		double x, y;
		MouseController::GetInstance()->GetMousePosition(x, y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();

		m_ghost->pos.Set((float)(x / w * m_worldWidth) + camera.position.x, (float)(m_worldHeight - (y / h * m_worldHeight) + camera.position.y), 0.f);
		//m_ghost->pos.Set((float)(x / w * m_worldWidth), m_worldHeight * 0.5f, 0.f);
	}
	else if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
	{
		std::cout << "LBUTTON UP" << std::endl;

		//Exercise 6: spawn small GO_BALL
		GameObject *go = FetchGO();
		go->pos = m_ghost->pos;
		go->scale.Set(1.f, 1.f, 1.f);
		go->mass = 1.f;

		double x, y;
		MouseController::GetInstance()->GetMousePosition(x, y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		go->vel.Set(m_ghost->pos.x - (float)(x / w * m_worldWidth) - camera.position.x, m_ghost->pos.y - (float)(m_worldHeight - (y / h * m_worldHeight) + camera.position.y), 0.f);
		go->scale.Set(Math::Clamp(go->vel.Length(), 2.f, 10.f), Math::Clamp(go->vel.Length(), 2.f, 10.f), 0.f);
	}
	if (MouseController::GetInstance()->IsButtonPressed(MouseController::RMB))
	{
		std::cout << "RBUTTON DOWN" << std::endl;

		double x, y;
		MouseController::GetInstance()->GetMousePosition(x, y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();

		m_ghost->pos.Set((float)(x / w * m_worldWidth) + camera.position.x, (float)(m_worldHeight - (y / h * m_worldHeight) + camera.position.y), 0.f);
		//m_ghost->pos.Set((float)(x / w * m_worldWidth), m_worldHeight * 0.5f, 0.f);
	}
	else if (MouseController::GetInstance()->IsButtonReleased(MouseController::RMB))
	{
		std::cout << "RBUTTON UP" << std::endl;

		GameObject *go = FetchGO();
		go->pos = m_ghost->pos;
		go->type = GameObject::GO_BOMB;
		go->boom = false;
		go->scale.Set(2, 2, 2);
		go->mass = 1.5f * 1.5f * 1.5f;

		double x, y;
		MouseController::GetInstance()->GetMousePosition(x, y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		go->vel.Set(m_ghost->pos.x - (float)(x / w * m_worldWidth) - camera.position.x, m_ghost->pos.y - (float)(m_worldHeight - (y / h * m_worldHeight) + camera.position.y), 0.f);
	}

	//Physics Simulation Section

	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			//Exercise 7: handle out of bound game objects
			if (go->type == GameObject::GO_BALL || go->type == GameObject::GO_BLOCK)
			{
				go->vel.x = go->vel.x - go->vel.x * 1.f * (float)dt;
				if (go->vel.Length() < 3)
					go->vel.IsZero();
				Physics::K1(go->vel.y, (-9.8f * go->mass * 2.f), (float)dt, go->vel.y);
				//go->vel.y = go->vel.y - 9.8f * go->mass  * (float)dt;
				go->pos += go->vel * (float)dt * m_speed;
				if (go->pos.y <= (m_TerrainHeight * ReadHeightMap(m_heightMap, (go->pos.x + m_TerrainWidth * 0.5f) / m_TerrainWidth, 0.f)) + go->scale.y * 0.5f)
				{
					go->pos.y = (m_TerrainHeight * ReadHeightMap(m_heightMap, (go->pos.x + m_TerrainWidth * 0.5f) / m_TerrainWidth, 0.f)) + go->scale.y * 0.5f;
					float theta = atan2((m_TerrainHeight
						* ReadHeightMap(m_heightMap, ((go->pos.x + m_TerrainWidth*0.5f) - go->scale.x * 0.5f) / m_TerrainWidth, 0.f)) - (m_TerrainHeight
						* ReadHeightMap(m_heightMap, ((go->pos.x + m_TerrainWidth*0.5f) + go->scale.x * 0.5f) / m_TerrainWidth, 0.f)), -go->scale.x);
					Vector3 tempnormal;

					//if (theta > 3.14159)
						//tempnormal = Vector3(0, 1, 0).Normalize();
					//else
						tempnormal = Vector3(sin(-theta), cos(-theta), 0).Normalize();
					go->dir = tempnormal;
					go->vel = go->vel - (go->vel.Dot(tempnormal) * tempnormal);
					go->vel.x = go->vel.x - go->vel.x * 5.f * (float)dt;
				}
				/*if ((go->pos.x < 0 + go->scale.x && go->vel.x < 0) || (go->pos.x > m_worldWidth - go->scale.x && go->vel.x > 0))
				{
					go->vel.x = -go->vel.x;
				}

				if ((go->pos.y > m_worldHeight - go->scale.y && go->vel.y > 0))
				{
					go->vel.y = -go->vel.y;
				}

				if (go->pos.x < 0 - go->scale.x || go->pos.x > m_worldWidth + go->scale.x ||
					go->pos.y < 0 - go->scale.y || go->pos.y > m_worldHeight + go->scale.y ||
					(go->pos.y < 0 - go->scale.y && go->vel.y < 0))
				{
					go->active = false;
					--m_objectCount;
					continue;
				}*/
				//if (go->pos.x + go->scale.x > m_worldWidth && go->vel.x > 0)
				//{
				//	go->vel.x = -go->vel.x;
				//}
				//else if (go->pos.x - go->scale.x < 0 && go->vel.x < 0)
				//{
				//	go->vel.x = -go->vel.x;
				//}
				//else if (go->pos.y + go->scale.y > m_worldHeight && go->vel.y > 0)
				//{
				//	//go->vel.y = -go->vel.y;
				//}
				//else if (go->pos.y - go->scale.y < 0 && go->vel.y < 0)
				//{
				//	go->vel.y = -go->vel.y;
				//}

				for (std::vector<GameObject *>::iterator it2 = it + 1; it2 != m_goList.end(); ++it2)
				{
					GameObject *go2 = (GameObject *)*it2;

					if (!go2->active || (go->type != GameObject::GO_BALL && go2->type != GameObject::GO_BALL))
						continue;

					GameObject *goA, *goB;
					if (go->type == GameObject::GO_BALL)
					{
						goA = go;
						goB = go2;
					}
					else
					{
						goA = go2;
						goB = go;
					}

					if (CheckCollision(goA, goB, (float)dt))
					{
						m1 = goA->mass;
						m2 = goB->mass;
						u1 = goA->vel;
						u2 = goB->vel;

						initialMomentum = m1 * u1 + m2 * u2;

						CollisionResponse(goA, goB);

						v1 = goA->vel;
						v2 = goB->vel;

						finalMomentum = m1 * v1 + m2 * v2;

						initialKE = 0.5f * m1 * u1.Dot(u1) + 0.5f * m2 * u2.Dot(u2);
						finalKE = 0.5f * m1 * v1.Dot(v1) + 0.5f * m2 * v2.Dot(v2);

						break;
					}
				}
			}

			if (go->type == GameObject::GO_BOMB)
			{
				go->pos += go->vel * static_cast<float>(dt);
				go->vel += Vector3(0, -9.8, 0) * dt;
				if (go->boom)
				{
					if(go->scale.x < 5)
						go->scale *= 1.2;
					if (go->scale.x > 5)
					{
						go->boom = false;
						go->active = false;
					}

					for (std::vector<GameObject *>::iterator it2 = it + 1; it2 != m_goList.end(); ++it2)
					{
						GameObject *go2 = static_cast<GameObject *>(*it2);
						if (go2->active)
							continue;

						if (go2->type == GameObject::GO_BRICK)
						{
							Vector3 pos = go->pos - go2->pos;
							pos.x = Math::Clamp(pos.x, 0.f, go2->scale.x);
							pos.y = Math::Clamp(pos.y, 0.f, go2->scale.y);

							pos += go2->pos;

							if ((pos - go->pos).Length() > 5 && (pos - go->pos).Length() < 30)
							{
								float energy = (30 - (pos - go->pos).Length()) / 30 * 2;

								Vector3 explosion = (go->pos - pos).Normalized() * energy;
								go2->vel -= explosion;
							}
						}
					}
				}
				else
					for (std::vector<GameObject *>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
					{
						GameObject *go2 = (GameObject *)*it2;
						if (go2->active)
						{
							if(go2->type == GameObject::GO_BRICK)
							{
								go2->pos += go2->vel * static_cast<float>(dt);
								if (!go2->vel.IsZero())
									go2->vel += (Vector3(0, 0, 0) - go2->vel) * dt;
								BombCollision(go, go2);
							}
						}
					}
			}
		}
	}
}


void Scene01::RenderGO(GameObject *go)
{
	modelStack.PushMatrix();

	switch (go->type)
	{
	case GameObject::GO_BALL:
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BALL], false);
		break;

	case GameObject::GO_BLOCK:
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2(go->dir.y, go->dir.x)) + 90, 0.f, 0.f, 1.f);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_CART], false);
		break;

	case GameObject::GO_PLAYER:
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_CUBE], false);
		break;

	case GameObject::GO_ENEMY_SNOWYETI:
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_CUBE], false);
		break;

	case GameObject::GO_TEMP:
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_CUBE], false);
	case GameObject::GO_BRICK:
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BRICK], false);
		break;

	case GameObject::GO_BOMB:
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		if (!go->boom)
			RenderMesh(meshList[GEO_BOMB], false);
		if (go->boom)
			RenderMesh(meshList[GEO_BOOM], false);
		break;
	}

	modelStack.PopMatrix();
}

void Scene01::Render()
{
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projectionStack.LoadMatrix(projection);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
	);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	RenderMesh(meshList[GEO_AXES], false);

	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			RenderGO(go);
		}
	}

	{
		modelStack.PushMatrix();
		modelStack.Translate(-m_TerrainWidth * 0.5f, 0, 1);
		modelStack.Scale(m_TerrainWidth, m_TerrainHeight, 1); // values varies.
		RenderMesh(meshList[GEO_TERRAIN], false);
		modelStack.PopMatrix();
	}

	RenderAllParticles();
	//On screen text

	std::ostringstream ss;
	//Exercise 5: Render m_objectCount
	ss.str("");
	ss << "Objects: " << m_objectCount;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 9);

	//Exercise 8c: Render initial and final momentum
	/*ss.str("");
	ss << "Initial momentum: " << initialMomentum;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 15);

	ss.str("");
	ss << "Final momentum: " << finalMomentum;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 12);

	ss.str("");
	ss << "Initial KE: " << initialKE;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 18);

	ss.str("");
	ss << "Final KE: " << finalKE;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 21);*/

	ss.precision(3);
	ss.str("");
	ss << "Speed: " << m_speed;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 6);

	ss.precision(5);
	ss.str("");
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);

	RenderTextOnScreen(meshList[GEO_TEXT], "Collision", Color(0, 1, 0), 3, 0, 0);
}

void Scene01::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while (m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	if (m_ghost)
	{
		delete m_ghost;
		m_ghost = NULL;
	}
}
