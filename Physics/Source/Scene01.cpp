#include "Scene01.h"
#include "GL\glew.h"
#include "Application.h"
#include "KeyboardController.h"
#include "SoundEngine.h"
#include "Terrain\LoadHmap.h"

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

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	//Physics code here
	m_speed = 40.f;

	Math::InitRNG();

	//Exercise 1: initialize m_objectCount
	m_objectCount = 0;
	m_ballCount = 0;

	free_look = false;

	Unit_Height_Space = 0;
	Unit_Width_Space = 0;

	Level = 4;
	Score = 4;
	Gold = 10000;

	//Load();

	m_ghost = new GameObject(GameObject::GO_BALL);

	m_player = Player::GetInstance();
	m_player->Init(FetchGO(), GameObject::GO_BLOCK, Vector3(25, 25, 0), Vector3(2, 2, 2), 2.f, 50.f);
	m_player->SetHeightmap(&m_heightMap, m_worldWidth, m_worldHeight);
	m_control = new Controller(m_player);

	//Load();

	//test = FetchGO();
	//test->type = GameObject::GO_BALL;
	//test->active = true;
	//test->dir.Set(0, 1, 0);
	//test->pos = Vector3(20, 25, 0);
	//test->vel.Set(0, 0, 0);
	//test->scale.Set(2, 2, 2);
	//test->mass = 1;
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

void Scene01::Update(double dt)
{
	SceneBase::Update(dt);
	Camera_Control(dt);

	if (KeyboardController::GetInstance()->IsKeyPressed('L'))
	{
		//file.Save_Data(Level, Score, Gold);
	}
	if (KeyboardController::GetInstance()->IsKeyPressed('K'))
	{
		//file.Load_Data();
	}

	/*if (Application::IsKeyPressed('9'))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if (Application::IsKeyPressed('0'))
	{
		m_speed += 0.1f;
	}*/

	m_player->Update(dt);
	m_control->Update(dt);

	//Mouse Section
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;

		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();

		m_ghost->pos.Set((float)(x / w * m_worldWidth) + camera.position.x, (float)(m_worldHeight - (y / h * m_worldHeight) + camera.position.y), 0.f);
		//m_ghost->pos.Set((float)(x / w * m_worldWidth), m_worldHeight * 0.5f, 0.f);
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;

		//Exercise 6: spawn small GO_BALL
		GameObject *go = FetchGO();
		go->pos = m_ghost->pos;
		go->scale.Set(1.f, 1.f, 1.f);
		go->mass = 1.f;

		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		go->vel.Set(m_ghost->pos.x - (float)(x / w * m_worldWidth) - camera.position.x, m_ghost->pos.y - (float)(m_worldHeight - (y / h * m_worldHeight) + camera.position.y), 0.f);
		go->scale.Set(Math::Clamp(go->vel.Length(), 2.f, 10.f), Math::Clamp(go->vel.Length(), 2.f, 10.f), 0.f);
	}
	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;

		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();

		m_ghost->pos.Set((float)(x / w * m_worldWidth) + camera.position.x, (float)(m_worldHeight - (y / h * m_worldHeight) + camera.position.y), 0.f);
		//m_ghost->pos.Set((float)(x / w * m_worldWidth), m_worldHeight * 0.5f, 0.f);
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;

		//Exercise 10: spawn large GO_BALL
		GameObject *go = FetchGO();
		go->pos = m_ghost->pos;
		go->scale.Set(1.5f, 1.5f, 1.5f);
		go->mass = 1.5f * 1.5f * 1.5f;

		double x, y;
		Application::GetCursorPos(&x, &y);
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
				go->vel.x = go->vel.x - go->vel.x * 2 * dt;
				if (go->vel.Length() < 3)
					go->vel.IsZero();
				go->vel.y = go->vel.y -9.8*go->mass*2 *dt;
				go->pos += go->vel * (float)dt * m_speed;
				if (go->pos.y <= ((m_worldHeight/2) * ReadHeightMap(m_heightMap, go->pos.x / (m_worldWidth * 2),0))+go->scale.x)
				{
					go->pos.y = ((m_worldHeight / 2) * ReadHeightMap(m_heightMap, go->pos.x / (m_worldWidth * 2), 0)) + go->scale.x;
					float theta = atan2(((m_worldHeight / 2) * ReadHeightMap(m_heightMap, (go->pos.x - 2) / (m_worldWidth * 2), 0)) - ((m_worldHeight / 2) * ReadHeightMap(m_heightMap, (go->pos.x + 2) / (m_worldWidth * 2), 0)), -2);
					Vector3 tempnormal;

					if (theta > 3.14159)
						tempnormal = Vector3(0, 1, 0).Normalize();
					else
						tempnormal = Vector3(1, theta, 0).Normalize();
					go->dir = tempnormal.Cross(Vector3(0,0,1)).Normalized();
					go->vel = go->vel - (go->vel.Dot(tempnormal) * tempnormal);
					go->vel.x = go->vel.x - go->vel.x * 5 * dt;
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
			}

			//Exercise 8a: handle collision between GO_BALL and GO_BALL using velocity swap
			for (std::vector<GameObject *>::iterator it2 = it + 1; it2 != m_goList.end(); ++it2)
			{
				GameObject *go2 = (GameObject *)*it2;

				//Exercise 12: improve inner loop to prevent double collision
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
					//Exercise 8b: store values in auditing variables
					m1 = goA->mass;
					m2 = goB->mass;
					u1 = goA->vel;
					u2 = goB->vel;

					initialMomentum = m1 * u1 + m2 * u2;

					//Exercise 10: handle collision using momentum swap instead
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
	}
}


void Scene01::RenderGO(GameObject *go)
{
	switch (go->type)
	{
	case GameObject::GO_BALL:
		modelStack.PushMatrix();
		//Exercise 4: render a sphere using scale and pos
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BALL], false);
		//Exercise 11: think of a way to give balls different colors
		modelStack.PopMatrix();
		break;

	case GameObject::GO_BLOCK:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2(go->dir.y, go->dir.x)) - 90.f, 0.f, 0.f, 1.f);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_CUBE], false);
		modelStack.PopMatrix();
		break;

	case GameObject::GO_PLAYER:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_CUBE], false);
		modelStack.PopMatrix();
		break;
	}
}

void Scene01::Render()
{
	//m_worldHeight = 100.f;
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
		//modelStack.Translate(m_worldWidth * 0.5, 0, 3);
		modelStack.Scale(m_worldWidth * 2, m_worldHeight/2, 1); // values varies.
		RenderMesh(meshList[GEO_TERRAIN], false);
		modelStack.PopMatrix();
	}

	//On screen text

	std::ostringstream ss;
	//Exercise 5: Render m_objectCount
	ss.str("");
	ss << "Objects: " << m_objectCount;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 9);

	//Exercise 8c: Render initial and final momentum
	ss.str("");
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
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 21);

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

void Scene01::Spawn(int value, int Width_Space, int Height_Space)
{
	if (value == 0)
	{
		spawner = FetchGO();
		spawner->type = GameObject::GO_BALL;
		spawner->active = true;
		spawner->dir.Set(0, 1, 0);
		spawner->pos = Vector3(m_worldWidth - Unit_Width_Space, m_worldHeight - Unit_Height_Space, 0);
		spawner->vel.Set(0, 0, 0);
		spawner->scale.Set(2, 2, 2);
		spawner->mass = 1;
		std::cout << "spawned" << std::endl;
		std::cout << spawner->pos.x << ", " << spawner->pos.y << endl;
	}
	else if (value == 1)
	{

	}
	else
	{

	}
}
