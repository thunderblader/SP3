#include "Scene01.h"
#include "GL\glew.h"
#include "Application.h"
#include "KeyboardController.h"
#include "MouseController.h"
#include "Terrain\LoadHmap.h"
#include "Physics\Physics.h"
#include "Particle\Particle.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

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
	m_control = new Controller();
	m_control->LoadConfig("Data//Config.ini", param_physics);
	sound_engine = createIrrKlangDevice();

	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	m_TerrainHeight = 30.f;
	m_TerrainWidth = 1000;

	m_speed = 40.f;

	Math::InitRNG();

	//m_ballCount = 0;

	Score = 0;
	time_limit = 0;
	item_id = 0;
	Level_data = 0;
	Gold = 0;

	move_shop = false;
	free_look = false;
	in_shop = false;
	purchased = false;
	file.Init(&m_goList);
	currlevel = 1;
	newlevel = 1;
	std::string leveltext = "Image//Level0";
	leveltext += to_string(currlevel);
	leveltext += ".csv";
	file.Load(false, leveltext);
	 
	GameObject* playerObj = FetchGO();
	GameObject* bombObj = FetchGO();
	m_player = Player::GetInstance();
	m_player->Init(playerObj, bombObj, GameObject::GO_PLAYER, Vector3(-m_TerrainWidth + 10, 1, 0), Vector3(5, 4, 1), param_physics.massCart, param_physics.acceleration, param_physics.speedLimit);
	m_player->SetHeightmap(&m_heightMap, m_TerrainWidth, m_TerrainHeight);
	Camera_Control(0);

	m_control->SetPlayer(m_player);

	file.Load(true, "Image//shop_data.csv");

	shop.Load_Shop();

	item_node = Tree::getInstance();

	//m_ghost = new GameObject(GameObject::GO_BALL);

	SpawnPowerups();

	SpawnEnemies();
	enemyList[0]->SetPlayerObj(playerObj, bombObj);
	 
	m_objectCount = &playerObj->m_totalGameObjects;
	m_particleCount = 0;
	MAX_PARTICLE = 1000;

	wind = Math::RandFloatMinMax(-10,10);

	display = true;
	menustate = MENU;

	debug = false;
	bossDie = false;
	deathrotation = 0;
	
	coinanim = new SpriteAnimation();
	coinanim->Set(dynamic_cast<SpriteMesh*>(meshList[GEO_COIN]), 0, 5, -1, 1, true);
	sledYetiOnScreen = false;
}

GameObject* Scene01::FetchGO()
{
	//Exercise 2a: implement FetchGO()
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (!go->GetActive() && go->type != GameObject::GO_PLAYER
			&& go->type != GameObject::GO_BOMB && go->type != GameObject::GO_SCREEN)
		{
			go->SetActive(true);
			return go;
		}
	}

	//Exercise 2b: increase object count every time an object is set to active
	for (unsigned i = 0; i < 10; ++i)
	{
		m_goList.push_back(new GameObject(GameObject::GO_NONE));
	}

	m_goList[m_goList.size() - 1]->SetActive(true);

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

	case GameObject::GO_BRICK:
	{
		if (go1->type == GameObject::GO_BOMB)
		{
			Vector3 w0 = go2->pos;
			Vector3 b1 = go1->pos;
			Vector3 N = go2->dir.Normalized();
			Vector3 NP = go2->dir.Cross(Vector3(0, 0, 1));
			float r = go1->scale.x;
			float h = go2->scale.x;
			float l = go2->scale.y;
			if ((w0 - b1).Dot(N) < 0)
				N = -N;

			return go1->vel.Dot(N) > 0 &&
				(abs((w0 - b1).Dot(N)) < (r + h * 0.5f)) &&
				(abs((w0 - b1).Dot(NP)) < (r + l * 0.5f));
		}
	}

	case GameObject::GO_PU_SPEED:
	{
		Vector3 dis = go1->pos - go2->pos;
		Vector3 rel = go1->vel - go2->vel;
		float combinedRadiusSq = (go1->scale.x * 0.2f + go2->scale.x) * (go1->scale.x * 0.2f + go2->scale.x);

		return (rel.Dot(dis) < 0 &&
			dis.LengthSquared() <= combinedRadiusSq);
	}

	case GameObject::GO_PU_RANGE:
	{
		Vector3 dis = go1->pos - go2->pos;
		Vector3 rel = go1->vel - go2->vel;
		float combinedRadiusSq = (go1->scale.x * 0.2f + go2->scale.x) * (go1->scale.x * 0.2f + go2->scale.x);

		return (rel.Dot(dis) < 0 &&
			dis.LengthSquared() <= combinedRadiusSq);
	}

	case GameObject::GO_BOSS:
	{
		if (go1->type == GameObject::GO_BOMB)
		{
			Vector3 dis = go1->pos - go2->pos;
			Vector3 rel = go1->vel - go2->vel;
			float combinedRadiusSq = (go1->scale.x + go2->scale.x / 2) * (go1->scale.x + go2->scale.x / 2);

			return (rel.Dot(dis) < 0 &&
				dis.LengthSquared() <= combinedRadiusSq);
		}
	}

	case GameObject::GO_COIN:
	{
		Vector3 dis = go1->pos - go2->pos;
		Vector3 rel = go1->vel - go2->vel;
		float combinedRadiusSq = (go1->scale.x + go2->scale.x / 2) * (go1->scale.x + go2->scale.x / 2);

		return (rel.Dot(dis) < 0 &&
			dis.LengthSquared() <= combinedRadiusSq);
	}
	case GameObject::GO_SLEDYETI:
	{
		Vector3 dis = go1->pos - go2->pos;
		Vector3 rel = go1->vel - go2->vel;
		float combinedRadiusSq = (go1->scale.x + go2->scale.x / 3) * (go1->scale.x + go2->scale.x / 3);

		return (rel.Dot(dis) < 0 &&
			dis.LengthSquared() <= combinedRadiusSq);
	}
	}

	return 0;
}

void Scene01::CollisionResponse(GameObject * go1, GameObject * go2)
{
	Vector3	N;
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

	case GameObject::GO_BRICK:
		if (go1->type == GameObject::GO_BOMB)
		{
			if (!m_player->GetExploded())
			{
				go1->vel.SetZero();
				m_player->SetExploded(true);
				go2->SetActive(false);
			}

			for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
			{
				GameObject *go3 = static_cast<GameObject *>(*it);
				if (!go3->GetActive())
					continue;

				if (go3->type == GameObject::GO_BRICK)
				{
					Vector3 pos = go1->pos - go3->pos;
					pos.x = Math::Clamp(pos.x, 0.f, go3->scale.x);
					pos.y = Math::Clamp(pos.y, 0.f, go3->scale.y);
					Score += 5;
					pos += go3->pos;
					if ((pos - go1->pos).Length() > 2.5 && (pos - go1->pos).Length() < 10 * m_player->GetPowStats().range)
					{
						float energy = (30 - (pos - go1->pos).Length()) / 30 * 10;

						Vector3 explosion = (go1->pos - pos).Normalized() * energy;
						go3->vel -= explosion;
					}
				}
			}
		}
		break;

	case GameObject::GO_PU_SPEED:
		go2->SetActive(false);
		sound_engine->play2D("Sound//getitem.wav");
		Score += 5;
		m_player->AddPowSpdCount();
		m_player->AddPowSpd(0.5f);
		break;

	case GameObject::GO_PU_RANGE:
		go2->SetActive(false);
		sound_engine->play2D("Sound//getitem.wav");
		Score += 5;
		m_player->AddPowRangeCount();
		m_player->AddPowRange(0.5f);
		break;

	case GameObject::GO_BOSS:
		if (go1->type != GameObject::GO_PLAYER)
		{
			if (!m_player->GetExploded())
			{
				go1->vel.SetZero();
				m_player->SetExploded(true);
				go2->vel.Set(Math::RandFloatMinMax(10, 20), Math::RandFloatMinMax(10, 20), 0);
				go2->pos.z = -0.5;
				bossDie = true;
				Score += 50;
			}
		}
		break;
	case GameObject::GO_COIN:
		go2->SetActive(false);
		sound_engine->play2D("Sound//getitem.wav");
		shop.Add_gold(10);
		Score += 10;
		break;

	case GameObject::GO_SLEDYETI:
		if (!m_player->GetTumble())
 			m_player->SetTumble(true);
		break;
	}
}

void Scene01::UpdateParticles(double dt)
{
	if (m_particleCount < (int)MAX_PARTICLE)
	{	
		if (m_player->GetVel().Length() > 90 && !m_player->GetLaunched())
		{
			ParticleObject* particle = GetParticle();
			particle->type = ParticleObject_TYPE::P_SPARK;
			particle->scale.Set(1, 1, 1);
			particle->vel.Set(Math::RandFloatMinMax(-1, 1), Math::RandFloatMinMax(-1, 5), 0);
			particle->rotationSpeed = Math::RandFloatMinMax(20, 40);
			//particle->pos.Set(Math::RandFloatMinMax(-1700, 1700), 1200, Math::RandFloatMinMax(-1700, 1700));
			particle->pos = m_player->GetPlayerPos();
			particle->pos.y -= m_player->GetPlayerObj().scale.y*0.4f;
			if (m_player->GetVel().x > 0)
			{
				particle->pos.x -= m_player->GetPlayerObj().scale.x*0.2f;
			}
			else
			{
				particle->pos.x += m_player->GetPlayerObj().scale.x*0.2f;
			}
		}
		for (int i = 0; i < 5; ++i)
		{
			ParticleObject* particle = GetParticle();
			particle->type = ParticleObject_TYPE::P_RAIN;
			particle->scale.Set(1, 3, 1);
			particle->vel.Set(Math::RandFloatMinMax(wind - 1, wind + 1), param_physics.gravity, 0.f);
			particle->rotationSpeed = 0;
			particle->rotation = Math::RadianToDegree(atan2(particle->vel.Normalized().y, particle->vel.Normalized().x)) - 270;
			particle->pos.Set(Math::RandFloatMinMax(-m_TerrainWidth*1.5f, m_TerrainWidth*1.5f), m_worldHeight*1.5f, 0);
		}
		if (m_player->GetExploded() && m_player->GetPlayerBomb().scale.x == 5)
		{
			for (int i = 0; i < 10; ++i)
			{
				ParticleObject* particle = GetParticle();
				particle->type = ParticleObject_TYPE::P_EXPLOSION;
				particle->scale.Set(1, 1, 1);
				particle->vel.Set(Math::RandFloatMinMax(-1, 1), Math::RandFloatMinMax(-1, 5), 0);
				particle->rotationSpeed = 0;
				particle->rotation = Math::RadianToDegree(atan2(particle->vel.Normalized().y, particle->vel.Normalized().x)) - 270;
				particle->pos = m_player->GetPlayerPos();
				sound_engine->play2D("Sound//blast.mp3");
			}
		}
		
	}

	std::vector<ParticleObject*>::iterator it, end;
	end = particleList.end();
	for (it = particleList.begin(); it != end; ++it)
	{
		ParticleObject* particle = (ParticleObject*)*it;
		if (!particle->isActive)
			continue;
		particle->vel.y += param_physics.gravity * (float)dt;
		particle->pos += particle->vel * (float)dt * 10.0f;
		if (particle->type == ParticleObject_TYPE::P_SPARK)
		{
			particle->rotation += particle->rotationSpeed * (float)dt;
		}
		else if (particle->type == ParticleObject_TYPE::P_RAIN)
		{

		}
		else if (particle->type == ParticleObject_TYPE::P_EXPLOSION)
		{
			
		}
		if (particle->pos.y <= 1)
		{
			particle->isActive = false;
			--m_particleCount;
		}
	}
}

void Scene01::Update(double dt)
{
	SceneBase::Update(dt);

	if (display)
	{
		Menu(dt);
		if (!sound_engine->isCurrentlyPlaying("Sound//mainmenu.mp3"))
		{
			sound_engine->play2D("Sound//mainmenu.mp3", true);
		}
		return;
	}

	if (sound_engine->isCurrentlyPlaying("Sound//mainmenu.mp3"))
	{
		sound_engine->stopAllSounds();
	}

	if (!sound_engine->isCurrentlyPlaying("Sound//gameplay.mp3"))
	{
		sound_engine->play2D("Sound//gameplay.mp3", true);
	}

	static bool CurrentTry = false;
	if (m_player->GetLaunched() && !CurrentTry)
	{
		ClearEnemyProj();
		ClearPowerUps();
		SpawnPowerups();
		CurrentTry = true;
	}
	else if (!m_player->GetLaunched())
		CurrentTry = false;

	if (newlevel != currlevel && !m_player->GetExploded())
	{
		Reset(newlevel);
	}

	coinanim->Update(dt);
	if (KeyboardController::GetInstance()->IsKeyPressed('I'))
	{
		file.Save_Data(Level_data, Score, Gold, item_node->root);
		if (in_shop == false)
		{
			in_shop = true;
		}
		else
		{
			in_shop = false;
		}
	}
	if (in_shop)
	{
		Shop_Update(dt);
		return;
	}

	Camera_Control(dt);
	UpdateParticles(dt);

	if (KeyboardController::GetInstance()->IsKeyPressed('L'))
	{
		file.Save_Data(Level_data, Score, Gold, item_node->root);
	}
	if (KeyboardController::GetInstance()->IsKeyPressed('K'))
	{
		//file.Load_Data(item_node->root);
	
	}
	if (KeyboardController::GetInstance()->IsKeyPressed('8'))
	{
		debug = !debug;
	}
	if (KeyboardController::GetInstance()->IsKeyPressed('9'))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if (KeyboardController::GetInstance()->IsKeyPressed('0'))
	{
		m_speed += 0.1f;
	}

	if (m_player->GetTries() <= 0)
	{
		menustate = LOSE1;
		display = true;
	}

	m_player->Update(dt);
	m_control->Update(dt);

	sledYetiOnScreen = false;
	vector<Enemy*>::iterator it;
	for (it = enemyList.begin(); it != enemyList.end(); )
	{
		(*it)->Update(dt);

		switch ((*it)->GetType())
		{
		case GameObject::GO_SLEDYETI:
			if (!sledYetiOnScreen && (*it)->GetPos().x > camera.position.x - m_worldWidth)
			{
				sledYetiOnScreen = true;
			}
			break;

		case GameObject::GO_ENEMY_SNOWYETI:
			if (!(*it)->GetProjFired() && !(*it)->GetProjActive() && (*it)->GetCurAnimFrame() == 11 && (*it)->GetState() == Enemy::ENEMY_STATE::ATTACK)
			{
				(*it)->PushProjectile(FetchGO(), Vector3(1.f, 1.f, 1.f), 10.f);
				(*it)->SetProjFired(true);
			}
			else if ((*it)->GetCurAnimFrame() == 12)
				(*it)->SetProjFired(false);
			break;

		default:
			break;
		}
		
		if (!(*it)->GetActive())
		{
			(*it)->Exit();
			delete *it;
			it = enemyList.erase(it);
		}
		else
			++it;
	}

	if (!sledYetiOnScreen && camera.position.x + m_worldWidth*1.5f < 0)
	{
		Enemy* enemy;
		enemy = new Enemy();
		enemy->Init(FetchGO(), GameObject::GO_SLEDYETI, Vector3(camera.position.x + m_worldWidth*1.5f, 0.5f, 0), Vector3(10.f, 10.f, 1.f), 5.f);
		enemy->SetSpriteAnim(meshList[GEO_SLEDYETI], 0, 13, -1, 1, true);
		enemyList.push_back(enemy);
	}

	//Physics Simulation Section
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->GetActive())
		{
			if (go->type == GameObject::GO_BRICK)
			{
				if (!go->vel.IsZero())
				{
					go->pos += go->vel * static_cast<float>(dt);
					go->vel += Vector3(0, param_physics.gravity, 0) * (float)dt;
				}
			}
			if (go->type == GameObject::GO_BOSS)
			{
				if (bossDie)
				{
					if (go->scale.x > 0.001)
					{
						go->scale *= 0.95f;
						deathrotation += 10;
					}
					else if (go->scale.x < 0.001)
					{
						go->SetActive(false);
						deathrotation = 0;
						bossDie = false;
						menustate = WIN1;
						display = true;
					}

					go->pos += go->vel * static_cast<float>(dt);
					go->vel += Vector3(0, param_physics.gravity, 0) * (float)dt;
				}
			}
			if ((go->type == GameObject::GO_BOMB && !m_player->GetExploded()) || go->type == GameObject::GO_PLAYER || go->type == GameObject::GO_SLEDYETI)
			{
				if (go->vel.Length() < 3)
					go->vel.IsZero();
				if (go->type == GameObject::GO_BOMB)
				{
					go->pos += go->vel * (float)dt * 0.2f;
				}
				else
				{
					go->pos += go->vel * (float)dt;
				}
				Physics::K1(go->vel, Vector3(wind / go->mass, param_physics.gravity * go->mass * 2.f, 0), (float)dt, go->vel);
				if (go->pos.y <= (m_TerrainHeight * ReadHeightMap(m_heightMap, (go->pos.x + m_TerrainWidth * 0.5f) / m_TerrainWidth, 0.f)) + go->scale.y * 0.5f && go->pos.x < 0 - go->scale.x && go->pos.x > -m_TerrainWidth)
				{
					go->pos.y = (m_TerrainHeight * ReadHeightMap(m_heightMap, (go->pos.x + m_TerrainWidth * 0.5f) / m_TerrainWidth, 0.f)) + go->scale.y * 0.5f;
					float backCart = ReadHeightMap(m_heightMap, ((go->pos.x + m_TerrainWidth * 0.5f) - go->scale.x * 0.5f) / m_TerrainWidth, 0.f);
					float frontCart = ReadHeightMap(m_heightMap, ((go->pos.x + m_TerrainWidth * 0.5f) + go->scale.x * 0.5f) / m_TerrainWidth, 0.f);
					float theta = atan2((m_TerrainHeight * backCart) - (m_TerrainHeight * frontCart), -go->scale.x);
					Vector3 tempnormal;
					tempnormal = Vector3(sin(-theta), cos(-theta), 0).Normalize();
					go->dir = tempnormal;

					go->vel = go->vel - (1.1f*go->vel.Dot(tempnormal) * tempnormal);
					go->vel.x = go->vel.x - go->vel.x * param_physics.frictionTerrain * (float)dt; // friction
				}
				
				for (std::vector<GameObject *>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
				{
					GameObject *go2 = (GameObject *)*it2;

					if (!go2->GetActive() || ((go->type != GameObject::GO_BOMB && go2->type != GameObject::GO_BOMB)
						&& (go->type != GameObject::GO_PLAYER && go2->type != GameObject::GO_PLAYER)))// || (go->type != GameObject::GO_BRICK && go2->type != GameObject::GO_BRICK))
						continue;

					GameObject *goA, *goB;
					if (go->type == GameObject::GO_BOMB || go->type == GameObject::GO_PLAYER)
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

						CollisionResponse(goA, goB);

						v1 = goA->vel;
						v2 = goB->vel;

						break;
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

	case GameObject::GO_PLAYER:
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z+0.1f);
		if (!m_player->GetExploded())
			modelStack.Rotate(m_player->GetBombspin()*10 ,0, 0, 1);
		modelStack.Rotate(Math::RadianToDegree(atan2(go->dir.y, go->dir.x)) + 90, 0.f, 0.f, 1.f);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_CART], false);
		break;

	case GameObject::GO_PROJ_SNOWBALL:
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_SNOWBALL], false);
		break;

	case GameObject::GO_BRICK:
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BRICK], false);
		break;

	case GameObject::GO_BOMB:
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z-0.1f);
		if (!m_player->GetExploded())
			modelStack.Rotate(m_player->GetBombspin() ,0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		if (!m_player->GetExploded())
		{
			RenderMesh(meshList[GEO_BOMB], false);
		}
		else if (m_player->GetExploded() && go->scale.x < 10)
			RenderMesh(meshList[GEO_BOOM], false);
		break;

	case GameObject::GO_PU_SPEED:
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_PU_SPEED], false);
		break;

	case GameObject::GO_PU_RANGE:
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_PU_RANGE], false);
		break;

	case GameObject::GO_BOSS:
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(deathrotation, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BOSS], false);
		break;

	case GameObject::GO_COIN:
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderSpriteMesh(coinanim->GetMesh(), false, coinanim->GetCurFrame());
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

	for (int i = -4; i < 4; ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate((m_worldHeight * 2.f - 0.2f) * (1 + i) + (camera.position.x / m_TerrainWidth) * 900.f, m_worldHeight * 0.5f, -1.f);
		modelStack.Scale(m_worldHeight * 2.f, m_worldHeight, 1.f);
		RenderMesh(meshList[GEO_BACKGROUND], false);
		modelStack.PopMatrix();
	}

	for (int i = -5; i < 5; ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate((m_worldHeight * 2.f - .5f) * (1 + i) + (camera.position.x / m_TerrainWidth) * 600.f, m_worldHeight * 0.4f, -0.8f);
		modelStack.Scale(m_worldHeight * 2.f, m_worldHeight, 1.f);
		RenderMesh(meshList[GEO_FOREGROUND], false);
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate(95, 0, -0.3f);
	modelStack.Scale(50, 25, 1); // values varies.
	RenderMesh(meshList[GEO_CLIFF], false);
	modelStack.PopMatrix();

	std::vector<Enemy*>::iterator it, end;
	end = enemyList.end();
	for (it = enemyList.begin(); it != end; ++it)
	{
		Enemy* enemy = (Enemy*)*it;
		SpriteAnimation* anim = enemy->GetSprite();
		modelStack.PushMatrix();
		modelStack.Translate(Vector3(enemy->GetPos().x, enemy->GetPos().y, enemy->GetPos().z -0.2f));
		if (enemy->GetType() == GameObject::GO_SLEDYETI)
			modelStack.Rotate(enemy->GetRot(), 0, 0, 1);
		modelStack.Scale(enemy->GetScale());
		RenderSpriteMesh(anim->GetMesh(), false, anim->GetCurFrame());
		modelStack.PopMatrix();
	}

	{
		modelStack.PushMatrix();
		modelStack.Translate(-m_TerrainWidth * 0.5f, 0, 0);
		modelStack.Scale(m_TerrainWidth, m_TerrainHeight, 1); // values varies.
		RenderMesh(meshList[GEO_TERRAIN], false);
		modelStack.PopMatrix();
	}
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->GetActive())
		{
			RenderGO(go);
		}
	}

	RenderAllParticles();
	RenderHUD();
	//On screen text
	std::ostringstream ss;
	if (in_shop == false)
	{
		if (debug)
		{
			ss.str("");
			ss << "Pos Offset: " << (m_player->GetPlayerPos().x / m_TerrainWidth);
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 12);

			ss.str("");
			ss << "Objects: " << *m_objectCount;
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 9);

			ss.precision(3);
			ss.str("");
			ss << "Speed: " << m_player->GetVel().Length();
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 6);

			ss.precision(5);
			ss.str("");
			ss << "FPS: " << fps;
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);
		}
	}
	else
	{
		Shop_Render();
	}
	RenderMenu();
}

void Scene01::RenderHUD()
{
	float scaleX, scaleY, posX, posY;
	scaleX = 20.f;
	scaleY = 10.f;
	posX = scaleX * 0.5f + 0.5f;
	posY = m_worldHeight - scaleY * 0.5f;

	// Render Lives/Chances/Tries
	RenderMeshIn2D(meshList[GEO_HUD_CHANCE], false, m_worldWidth, m_worldHeight, scaleX, scaleY, posX, posY);
	for (unsigned int i = 0; i < m_player->GetTries(); ++i)
	{
		posX = 25.5f + (i * 8.f);
		posY = m_worldHeight - 5.5f;
		RenderMeshIn2D(meshList[GEO_CART], false, m_worldWidth, m_worldHeight, 5.f, 4.f, posX, posY);
	}
	/*****************/

	// Render Level
	posX = m_worldWidth * 0.5f;
	posY = m_worldHeight - scaleY - scaleY * 0.5f;

	switch (currlevel)
	{
	case 1:
		RenderMeshIn2D(meshList[GEO_HUD_LEVEL1], false, m_worldWidth, m_worldHeight, scaleX, scaleY, posX, posY);
		break;
	case 2:
		RenderMeshIn2D(meshList[GEO_HUD_LEVEL2], false, m_worldWidth, m_worldHeight, scaleX, scaleY, posX, posY);
		break;
	case 3:
		RenderMeshIn2D(meshList[GEO_HUD_LEVEL3], false, m_worldWidth, m_worldHeight, scaleX, scaleY, posX, posY);
		break;
	case 4:
		RenderMeshIn2D(meshList[GEO_HUD_LEVEL4], false, m_worldWidth, m_worldHeight, scaleX, scaleY, posX, posY);
		break;
	case 5:
		RenderMeshIn2D(meshList[GEO_HUD_LEVEL5], false, m_worldWidth, m_worldHeight, scaleX, scaleY, posX, posY);
		break;
	default: break;
	}
	/*****************/

	// Render Score
	string ss = to_string(Score);
	RenderMeshIn2D(meshList[GEO_HUD_SCORE], false, m_worldWidth, m_worldHeight, scaleX, scaleY, m_worldWidth * 0.5f, m_worldHeight - 5.f);

	for (unsigned i = 0; i < ss.size(); ++i)
	{
		posX = m_worldWidth * 0.5f + scaleX * 0.5f + (i * 5.f);
		posY = m_worldHeight - 5.f;

		switch (ss[i])
		{
		case '1':
			RenderMeshIn2D(meshList[GEO_NO_1], false, m_worldWidth, m_worldHeight, scaleY, scaleY, posX, posY);
			break;
		case '2': 
			RenderMeshIn2D(meshList[GEO_NO_2], false, m_worldWidth, m_worldHeight, scaleY, scaleY, posX, posY);
			break;
		case '3': 
			RenderMeshIn2D(meshList[GEO_NO_3], false, m_worldWidth, m_worldHeight, scaleY, scaleY, posX, posY);
			break;
		case '4': 
			RenderMeshIn2D(meshList[GEO_NO_4], false, m_worldWidth, m_worldHeight, scaleY, scaleY, posX, posY);
			break;
		case '5': 
			RenderMeshIn2D(meshList[GEO_NO_5], false, m_worldWidth, m_worldHeight, scaleY, scaleY, posX, posY);
			break;
		case '6': 
			RenderMeshIn2D(meshList[GEO_NO_6], false, m_worldWidth, m_worldHeight, scaleY, scaleY, posX, posY);
			break;
		case '7': 
			RenderMeshIn2D(meshList[GEO_NO_7], false, m_worldWidth, m_worldHeight, scaleY, scaleY, posX, posY);
			break;
		case '8': 
			RenderMeshIn2D(meshList[GEO_NO_8], false, m_worldWidth, m_worldHeight, scaleY, scaleY, posX, posY);
			break;
		case '9': 
			RenderMeshIn2D(meshList[GEO_NO_9], false, m_worldWidth, m_worldHeight, scaleY, scaleY, posX, posY);
			break;
		case '0': 
			RenderMeshIn2D(meshList[GEO_NO_0], false, m_worldWidth, m_worldHeight, scaleY, scaleY, posX, posY);
			break;
		default: break;
		}
	}
	/*****************/

	// Render Powerup
	unsigned int SpdCount = m_player->GetPowSpdCount();
	unsigned int RangeCount = m_player->GetPowRangeCount();
	posX = m_worldWidth - 5.f;
	posY = (m_worldHeight - 6.f * SpdCount - 6.f * RangeCount) * 0.5f ;

	RenderMeshIn2D(meshList[GEO_HUD_PBORDER], false, m_worldWidth, m_worldHeight, 8.f, 7.f * SpdCount + 7.f * RangeCount, posX, m_worldHeight * 0.5f + 3.f);

	for (unsigned int i = 0; i < SpdCount; ++i)
	{
		posY += 6.f;
		RenderMeshIn2D(meshList[GEO_PU_SPEED], false, m_worldWidth, m_worldHeight, 5.f, 5.f, posX, posY);
	}

	for (unsigned int i = 0; i < RangeCount; ++i)
	{
		posY += 6.f;
		RenderMeshIn2D(meshList[GEO_PU_RANGE], false, m_worldWidth, m_worldHeight, 5.f, 5.f, posX, posY);
	}
	/*****************/
}

void Scene01::SpawnPowerups()
{
	GameObject* obj;
	float rX = -m_TerrainWidth + 50.f;
	float rY = 0.f;
	while (rX < -100.f)
	{
		rY = 0.f;
		obj = FetchGO();

		if (Math::RandIntMinMax(0, 5))
		{
			rX += Math::RandFloatMinMax(20.f, 50.f);
			rY += Math::RandFloatMinMax(15.f, 30.f);
			obj->type = GameObject::GO_COIN;
		}
		else
		{
			rX += Math::RandFloatMinMax(30.f, 100.f);
			rY += Math::RandFloatMinMax(15.f, 20.f);
			obj->type = (GameObject::GAMEOBJECT_TYPE)Math::RandIntMinMax(GameObject::GO_PU_SPEED, GameObject::GO_PU_RANGE);
		}

		obj->SetActive(true);
		obj->pos.Set(rX, m_TerrainHeight * ReadHeightMap(m_heightMap, (rX + m_TerrainWidth * 0.5f) / m_TerrainWidth, 0.f) + rY, 0.f);
		obj->scale.Set(5.f, 5.f, 5.f);
	}
}

void Scene01::SpawnEnemies()
{
	Enemy* enemy;
	float rX = -m_TerrainWidth + 200.f;
	float rY;

	for (unsigned i = 0; rX < -200.f; ++i)
	{
		rX += Math::RandFloatMinMax(100.f, 200.f);
		rY = Math::RandFloatMinMax(60.f, 80.f);
		enemy = new Enemy();
		enemy->Init(FetchGO(), GameObject::GO_ENEMY_SNOWYETI, Vector3(rX, rY, 0.f), Vector3(10.f, 10.f, 1.f));
		enemy->SetSpriteAnim(meshList[GEO_SPRITE_YETI], 0, 15, 0, 1.f, true);
		enemyList.push_back(enemy);
	}

	enemy = new Enemy();
	enemy->Init(FetchGO(), GameObject::GO_SLEDYETI, Vector3(-m_TerrainWidth + m_worldWidth + 20.f, 0.5f, 0), Vector3(10.f, 10.f, 1.f), 5.f);
	enemy->SetSpriteAnim(meshList[GEO_SLEDYETI], 0, 13, -1, 1, true);
	enemyList.push_back(enemy);
	sledYetiOnScreen = true;

	enemy->SetHeightMap(&m_heightMap, m_TerrainWidth, m_TerrainHeight);
}

void Scene01::ClearPowerUps()
{
	vector<GameObject*>::iterator it;
	for (it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		if ((*it)->type == GameObject::GO_PU_RANGE || (*it)->type == GameObject::GO_PU_SPEED || (*it)->type == GameObject::GO_COIN)
		{
			(*it)->SetActive(false);
			(*it)->type = GameObject::GO_NONE;
		}
	}
}

void Scene01::ClearEnemies()
{
	while (!enemyList.empty())
	{
		Enemy* obj = enemyList.back();
		obj->Exit();
		delete obj;
		enemyList.pop_back();
	}
}

void Scene01::ClearEnemyProj()
{
	vector<Enemy*>::iterator it;
	for (it = enemyList.begin(); it != enemyList.end(); )
	{
		if ((*it)->GetType() == GameObject::GO_SLEDYETI)
		{
			(*it)->Exit();
			delete *it;
			it = enemyList.erase(it);
		}
		else
		{
			(*it)->ClearProjectile();
			++it;
		}
	}
}

void Scene01::Reset(int _level)
{
	ClearEnemies();

	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->GetActive() && go->type != GameObject::GO_BOMB
			&& go->type != GameObject::GO_PLAYER)
		{
			go->SetActive(false);
			go->type = GameObject::GO_NONE;
		}
	}

	currlevel = _level;

	std::string leveltext = "Image//Level0";
	leveltext += to_string(currlevel);
	leveltext += ".csv";
	file.Load(false, leveltext);
	leveltext = "Image//heightmap";
	leveltext += to_string(currlevel);
	leveltext += ".raw";

	if (meshList[GEO_TERRAIN])
	{
		delete meshList[GEO_TERRAIN];
		meshList[GEO_TERRAIN] = nullptr;
	}

	meshList[GEO_TERRAIN] = MeshBuilder::GenerateTerrain("GEO_TERRAIN", leveltext, m_heightMap);
	meshList[GEO_TERRAIN]->textureID = LoadTGA("Image//terrain.tga");

	SpawnPowerups();
	SpawnEnemies();
	m_player->SetTries(3);
	wind = Math::RandFloatMinMax(-10, 10);
	display = false;
}

void Scene01::Exit()
{
	SceneBase::Exit();
	ClearEnemies();

	vector<Tree*> deleteTree;
	item_node->CreateList(item_node, deleteTree);

	while (!deleteTree.empty())
	{
		Tree* dnode = deleteTree.back();
		delete dnode;
		deleteTree.pop_back();
	}

	if (coinanim)
	{
		delete coinanim;
		coinanim = nullptr;
	}

	if (m_player)
	{
		m_player->Destroy();
		m_player = nullptr;
	}

	if (m_control)
	{
		delete m_control;
		m_control = nullptr;
	}

	//Cleanup GameObjects
	while (m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
}