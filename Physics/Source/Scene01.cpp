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

	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	m_TerrainHeight = 30.f;
	m_TerrainWidth = 1000;

	m_speed = 40.f;

	Math::InitRNG();

	m_ballCount = 0;


	m_tries = 3;
	Score = 0;
	time_limit = 0;
	item_id = 0;
	Level_data = 0;

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

	m_control->SetPlayer(m_player);

	file.Load(true, "Image//shop_data.csv");

	shop.Load_Shop();

	shop.Purchase_Item(1);
	item_node = Tree::getInstance();

	m_ghost = new GameObject(GameObject::GO_BALL);

	Enemy* enemy;
	float rX = -m_TerrainWidth + 200.f;
	float rY;

	for (unsigned i = 0; rX < -200.f; ++i)
	{
		rX += Math::RandFloatMinMax(100.f, 200.f);
		rY = Math::RandFloatMinMax(50.f, 70.f);
		enemy = new Enemy();
		enemy->Init(FetchGO(), GameObject::GO_ENEMY_SNOWYETI, Vector3(rX, rY, 0.f), Vector3(10.f, 10.f, 1.f));
		enemy->SetSpriteAnim(meshList[GEO_SPRITE_YETI]);
		enemyList.push_back(enemy);
	}

	enemy->SetPlayerObj(playerObj);
	enemy->SetBombObj(bombObj);
	enemy->SetHeightMap(&m_heightMap, m_TerrainWidth, m_TerrainHeight);

	SpawnPowerups();
	 
	m_objectCount = &playerObj->m_totalGameObjects;
	m_particleCount = 0;
	MAX_PARTICLE = 1000;

	wind = Math::RandFloatMinMax(-10,10);

	display = true;
	menustate = MENU;

	debug = false;
	
	coinanim = new SpriteAnimation();
	coinanim->Set(dynamic_cast<SpriteMesh*>(meshList[GEO_COIN]), 0, 5, 0, 1, true);
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

	case GameObject::GO_PU_POWER:
	{
		Vector3 dis = go1->pos - go2->pos;
		Vector3 rel = go1->vel - go2->vel;
		float combinedRadiusSq = (go1->scale.x * 0.2f + go2->scale.x) * (go1->scale.x * 0.2f + go2->scale.x);

		return (rel.Dot(dis) < 0 &&
			dis.LengthSquared() <= combinedRadiusSq);
	}

	case GameObject::GO_BOSS:
	{
		Vector3 dis = go1->pos - go2->pos;
		Vector3 rel = go1->vel - go2->vel;
		float combinedRadiusSq = (go1->scale.x + go2->scale.x / 2) * (go1->scale.x + go2->scale.x / 2);

		return (rel.Dot(dis) < 0 &&
			dis.LengthSquared() <= combinedRadiusSq);
	}

	case GameObject::GO_COIN:
	{
		Vector3 dis = go1->pos - go2->pos;
		Vector3 rel = go1->vel - go2->vel;
		float combinedRadiusSq = (go1->scale.x + go2->scale.x / 2) * (go1->scale.x + go2->scale.x / 2);

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
		if (go1->type != GameObject::GO_PLAYER)
		{
			if (!m_player->GetExploded())
			{
				go1->vel.SetZero();
				m_player->SetExploded(true);
				go2->SetActive(false);

				if (m_tries > 0)
					--m_tries;
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

					pos += go3->pos;
					if ((pos - go1->pos).Length() > 2.5 && (pos - go1->pos).Length() < 10)
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
		break;

	case GameObject::GO_PU_RANGE:
		go2->SetActive(false);
		sound_engine->play2D("Sound//getitem.wav");
		break;

	case GameObject::GO_PU_POWER:
		go2->SetActive(false);
		sound_engine->play2D("Sound//getitem.wav");
		break;

	case GameObject::GO_BOSS:
		if (!m_player->GetExploded())
		{
			go1->vel.SetZero();
			m_player->SetExploded(true);
			go2->SetActive(false);
			++newlevel;
		}
		break;
	case GameObject::GO_COIN:
		go2->SetActive(false);
		sound_engine->play2D("Sound//getitem.wav");
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
	if (newlevel != currlevel && !m_player->GetExploded())
	{
		currlevel = newlevel;
		m_tries = 3;

		for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			GameObject *go = (GameObject *)*it;
			if (go->GetActive() && go->type != GameObject::GO_BOMB
				&& go->type != GameObject::GO_PLAYER && go->type != GameObject::GO_ENEMY_SNOWYETI)
			{
				go->SetActive(false);
				go->type = GameObject::GO_NONE;
			}
		}

		std::string leveltext = "Image//Level0";
		leveltext += to_string(currlevel);
		leveltext += ".csv";
		file.Load(false, leveltext);
		leveltext = "Image//heightmap";
		leveltext += to_string(currlevel);
		leveltext += ".raw";
		meshList[GEO_TERRAIN] = MeshBuilder::GenerateTerrain("GEO_TERRAIN", leveltext, m_heightMap);
		meshList[GEO_TERRAIN]->textureID = LoadTGA("Image//terrain.tga");

		SpawnPowerups();

		wind = Math::RandFloatMinMax(-10, 10);
	}

	coinanim->Update(dt);

	if (KeyboardController::GetInstance()->IsKeyPressed('I'))
	{
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
		//file.Load_Data();
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

	m_player->Update(dt);
	m_control->Update(dt);

	vector<Enemy*>::iterator it, end;
	end = enemyList.end();
	for (it = enemyList.begin(); it != end; ++it)
	{
		(*it)->Update(dt);

		if (!(*it)->GetProjFired() && !(*it)->GetProjActive() && (*it)->GetCurAnimFrame() == 11)
		{
			(*it)->PushProjectile(FetchGO(), Vector3(1.f, 1.f, 1.f), 30.f);
			(*it)->SetProjFired(true);
		}
		else if ((*it)->GetCurAnimFrame() == 12)
			(*it)->SetProjFired(false);

		if (!(*it)->GetActive())
		{
			delete *it;
			enemyList.erase(it);
		}
	}

	//Mouse Section
	//if (MouseController::GetInstance()->IsButtonPressed(MouseController::LMB))
	//{
	//	std::cout << "LBUTTON DOWN" << std::endl;

	//	double x, y;
	//	MouseController::GetInstance()->GetMousePosition(x, y);
	//	int w = Application::GetWindowWidth();
	//	int h = Application::GetWindowHeight();

	//	m_ghost->pos.Set((float)(x / w * m_worldWidth) + camera.position.x, (float)(m_worldHeight - (y / h * m_worldHeight) + camera.position.y), 0.f);
	//	//m_ghost->pos.Set((float)(x / w * m_worldWidth), m_worldHeight * 0.5f, 0.f);
	//}
	//else if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
	//{
	//	std::cout << "LBUTTON UP" << std::endl;

	//	//Exercise 6: spawn small GO_BALL
	//	GameObject *go = FetchGO();
	//	go->pos = m_ghost->pos;
	//	go->scale.Set(1.f, 1.f, 1.f);
	//	go->mass = 1.f;

	//	double x, y;
	//	MouseController::GetInstance()->GetMousePosition(x, y);
	//	int w = Application::GetWindowWidth();
	//	int h = Application::GetWindowHeight();
	//	go->vel.Set(m_ghost->pos.x - (float)(x / w * m_worldWidth) - camera.position.x, m_ghost->pos.y - (float)(m_worldHeight - (y / h * m_worldHeight) + camera.position.y), 0.f);
	//	go->scale.Set(Math::Clamp(go->vel.Length(), 2.f, 10.f), Math::Clamp(go->vel.Length(), 2.f, 10.f), 0.f);
	//}
	if (MouseController::GetInstance()->IsButtonPressed(MouseController::RMB))
	{
		std::cout << "RBUTTON DOWN" << std::endl;

		double x, y;
		MouseController::GetInstance()->GetMousePosition(x, y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();

		m_ghost->pos.Set((float)(x / w * m_worldWidth) + camera.position.x, (float)(m_worldHeight - (y / h * m_worldHeight) + camera.position.y), 0.f);
		//m_ghost->pos.Set((float)(x / w * _worldWidth), m_worldHeight * 0.5f, 0.f);
	}
	else if (MouseController::GetInstance()->IsButtonReleased(MouseController::RMB))
	{
		std::cout << "RBUTTON UP" << std::endl;

		GameObject *go = FetchGO();
		go->pos = m_ghost->pos;
		go->type = GameObject::GO_BOMB;
		//go->boom = false;
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
			if ((go->type == GameObject::GO_BOMB && !m_player->GetExploded()) || go->type == GameObject::GO_PLAYER)
			{
				//go->vel.x = go->vel.x - go->vel.x * 1.f * (float)dt;
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

					//if (theta > 3.14159)
					//tempnormal = Vector3(0, 1, 0).Normalize();
					//else
					tempnormal = Vector3(sin(-theta), cos(-theta), 0).Normalize();
					go->dir = tempnormal;

					go->vel = go->vel - (1.1f*go->vel.Dot(tempnormal) * tempnormal);
					go->vel.x = go->vel.x - go->vel.x * param_physics.frictionTerrain * (float)dt; // friction
				}
				/*if ((go->pos.x < 0 + go->scale.x && go->vel.x < 0) || (go->pos.x > m_worldWidth - go->scale.x && go->vel.x > 0))
				{
				go->vel.x = -go->vel.x;
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
				float backCart = ReadHeightMap(m_heightMap, ((go->pos.x + m_TerrainWidth * 0.5f) - go->scale.x * 0.5f) / m_TerrainWidth, 0.f);
				float frontCart = ReadHeightMap(m_heightMap, ((go->pos.x + m_TerrainWidth * 0.5f) + go->scale.x * 0.5f) / m_TerrainWidth, 0.f);
				float theta = atan2((m_TerrainHeight * backCart) - (m_TerrainHeight * frontCart), -go->scale.x);
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

						//initialMomentum = m1 * u1 + m2 * u2;

						CollisionResponse(goA, goB);

						v1 = goA->vel;
						v2 = goB->vel;

						//finalMomentum = m1 * v1 + m2 * v2;

						//initialKE = 0.5f * m1 * u1.Dot(u1) + 0.5f * m2 * u2.Dot(u2);
						//finalKE = 0.5f * m1 * v1.Dot(v1) + 0.5f * m2 * v2.Dot(v2);

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
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
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

	case GameObject::GO_PU_POWER:
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_PU_POWER], false);
		break;

	case GameObject::GO_BOSS:
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
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

	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->GetActive())
		{
			RenderGO(go);
		}
	}
	std::vector<Enemy*>::iterator it, end;
	end = enemyList.end();
	for (it = enemyList.begin(); it != end; ++it)
	{
		Enemy* enemy = (Enemy*)*it;
		SpriteAnimation* anim = enemy->GetSprite();
		modelStack.PushMatrix();
		modelStack.Translate(enemy->GetPos());
		modelStack.Scale(enemy->GetScale());
		RenderSpriteMesh(anim->GetMesh(), false, anim->GetCurFrame());
		modelStack.PopMatrix();
	}

	{
		modelStack.PushMatrix();
		modelStack.Translate(-m_TerrainWidth * 0.5f, 0, 1);
		modelStack.Scale(m_TerrainWidth, m_TerrainHeight, 1); // values varies.
		RenderMesh(meshList[GEO_TERRAIN], false);
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate(60, 0, -0.1f);
	modelStack.Scale(50, 25, 1); // values varies.
	RenderMesh(meshList[GEO_CLIFF], false);
	modelStack.PopMatrix();

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
		float scaleX, scaleY;
		scaleX = 20.f;
		scaleY = 10.f;

		modelStack.Translate(camera.position.x + 65, camera.position.y + 50, 1);
		modelStack.Scale((float)Application::GetWindowWidth(), 85, 1);
		RenderMesh(meshList[GEO_SCREEN], false);

		RenderMeshIn2D(meshList[GEO_SHOP], false, m_worldWidth, m_worldHeight, scaleX, scaleY, m_worldWidth * 0.5f, m_worldHeight - scaleY - scaleY * 0.5f);
		ss.str("");
		ss << item_node->root.name;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 15, 42);
		ss.str("");
		ss << "ID: " << item_node->root.data;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 15, 39);
		ss.str("");
		ss << "Price: " << item_node->root.price;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 15, 36);
		ss.str("");
		ss << "Gold: " << shop.gold;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 15, 15);
		if (buy_item == true)
		{
			if (purchased == true)
			{
				ss.str("");
				ss << "You bought:" << item_node->root.name;
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 15, 18);
			}
			else
			{
				ss.str("");
				ss << "You no money";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 15, 18);
				ss.str("");
				ss << "It costs: " << item_node->root.price;
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 15, 21);
			}
		}
	}

	RenderMenu();
}

void Scene01::RenderHUD()
{
	float scaleX, scaleY;
	scaleX = 20.f;
	scaleY = 10.f;

	RenderMeshIn2D(meshList[GEO_HUD_CHANCE], false, m_worldWidth, m_worldHeight, scaleX, scaleY, scaleX * 0.5f + 0.5f, m_worldHeight - scaleY * 0.5f);
	for (unsigned int i = 0; i < m_tries; ++i)
	{
		RenderMeshIn2D(meshList[GEO_CART], false, m_worldWidth, m_worldHeight, 5.f, 4.f, 25.5f + (i * 8.f), m_worldHeight - 5.5f);
	}

	switch (currlevel)
	{
	case 1:
		RenderMeshIn2D(meshList[GEO_HUD_LEVEL1], false, m_worldWidth, m_worldHeight, scaleX, scaleY, m_worldWidth * 0.5f, m_worldHeight - scaleY - scaleY * 0.5f);
		break;
	case 2:
		RenderMeshIn2D(meshList[GEO_HUD_LEVEL2], false, m_worldWidth, m_worldHeight, scaleX, scaleY, m_worldWidth * 0.5f, m_worldHeight - scaleY - scaleY * 0.5f);
		break;
	case 3:
		RenderMeshIn2D(meshList[GEO_HUD_LEVEL3], false, m_worldWidth, m_worldHeight, scaleX, scaleY, m_worldWidth * 0.5f, m_worldHeight - scaleY - scaleY * 0.5f);
		break;
	case 4:
		RenderMeshIn2D(meshList[GEO_HUD_LEVEL4], false, m_worldWidth, m_worldHeight, scaleX, scaleY, m_worldWidth * 0.5f, m_worldHeight - scaleY - scaleY * 0.5f);
		break;
	case 5:
		RenderMeshIn2D(meshList[GEO_HUD_LEVEL5], false, m_worldWidth, m_worldHeight, scaleX, scaleY, m_worldWidth * 0.5f, m_worldHeight - scaleY - scaleY * 0.5f);
		break;
	default: break;
	}

	string ss = to_string(Score);
	RenderMeshIn2D(meshList[GEO_HUD_SCORE], false, m_worldWidth, m_worldHeight, scaleX, scaleY, m_worldWidth * 0.5f, m_worldHeight - 5.f);

	for (unsigned i = 0; i < ss.size(); ++i)
	{
		switch (ss[i])
		{
		case '1':
			RenderMeshIn2D(meshList[GEO_NO_1], false, m_worldWidth, m_worldHeight, scaleY, scaleY, m_worldWidth * 0.5f + scaleX * 0.5f + (i * 5.f), m_worldHeight - 5.f);
			break;
		case '2': 
			RenderMeshIn2D(meshList[GEO_NO_2], false, m_worldWidth, m_worldHeight, scaleY, scaleY, m_worldWidth * 0.5f + scaleX * 0.5f + (i * 5.f), m_worldHeight - 5.f);
			break;
		case '3': 
			RenderMeshIn2D(meshList[GEO_NO_3], false, m_worldWidth, m_worldHeight, scaleY, scaleY, m_worldWidth * 0.5f + scaleX * 0.5f + (i * 5.f), m_worldHeight - 5.f);
			break;
		case '4': 
			RenderMeshIn2D(meshList[GEO_NO_4], false, m_worldWidth, m_worldHeight, scaleY, scaleY, m_worldWidth * 0.5f + scaleX * 0.5f + (i * 5.f), m_worldHeight - 5.f);
			break;
		case '5': 
			RenderMeshIn2D(meshList[GEO_NO_5], false, m_worldWidth, m_worldHeight, scaleY, scaleY, m_worldWidth * 0.5f + scaleX * 0.5f + (i * 5.f), m_worldHeight - 5.f);
			break;
		case '6': 
			RenderMeshIn2D(meshList[GEO_NO_6], false, m_worldWidth, m_worldHeight, scaleY, scaleY, m_worldWidth * 0.5f + scaleX * 0.5f + (i * 5.f), m_worldHeight - 5.f);
			break;
		case '7': 
			RenderMeshIn2D(meshList[GEO_NO_7], false, m_worldWidth, m_worldHeight, scaleY, scaleY, m_worldWidth * 0.5f + scaleX * 0.5f + (i * 5.f), m_worldHeight - 5.f);
			break;
		case '8': 
			RenderMeshIn2D(meshList[GEO_NO_8], false, m_worldWidth, m_worldHeight, scaleY, scaleY, m_worldWidth * 0.5f + scaleX * 0.5f + (i * 5.f), m_worldHeight - 5.f);
			break;
		case '9': 
			RenderMeshIn2D(meshList[GEO_NO_9], false, m_worldWidth, m_worldHeight, scaleY, scaleY, m_worldWidth * 0.5f + scaleX * 0.5f + (i * 5.f), m_worldHeight - 5.f);
			break;
		case '0': 
			RenderMeshIn2D(meshList[GEO_NO_0], false, m_worldWidth, m_worldHeight, scaleY, scaleY, m_worldWidth * 0.5f + scaleX * 0.5f + (i * 5.f), m_worldHeight - 5.f);
			break;
		default: break;
		}
	}
}

void Scene01::SpawnPowerups()
{
	GameObject* obj;
	float rX = -m_TerrainWidth + 50.f;
	float rY;
	for (unsigned i = 0; rX < -100.f; ++i)
	{
		obj = FetchGO();
		if (Math::RandIntMinMax(0, 1))
			obj->type = (GameObject::GAMEOBJECT_TYPE)Math::RandIntMinMax(GameObject::GO_PU_SPEED, GameObject::GO_PU_POWER);
		else
			obj->type = GameObject::GO_COIN;
		obj->SetActive(true);
		rX += Math::RandFloatMinMax(30.f, 100.f);
		rY = Math::RandFloatMinMax(15.f, 20.f);
		obj->pos.Set(rX, m_TerrainHeight * ReadHeightMap(m_heightMap, (rX + m_TerrainWidth * 0.5f) / m_TerrainWidth, 0.f) + rY, 0.f);
		obj->scale.Set(5.f, 5.f, 5.f);
	}
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
