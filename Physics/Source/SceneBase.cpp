#include "SceneBase.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "KeyboardController.h"
#include "SoundEngine.h"
#include "Particle\Particle.h"
#include "SpriteAnimation.h"

#include <sstream>

SceneBase::SceneBase()
{
}

SceneBase::~SceneBase()
{
}

void SceneBase::Init()
{
	// Black background
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 
	
	glEnable(GL_CULL_FACE);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	m_programID = LoadShaders( "Shader//comg.vertexshader", "Shader//comg.fragmentshader" );
	
	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	//m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
	//m_parameters[U_VIEW] = glGetUniformLocation(m_programID, "V");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	
	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(0, 20, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);
	
	glUniform1i(m_parameters[U_NUMLIGHTS], 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

	camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_BALL] = MeshBuilder::GenerateSphere("ball", Color(0.f, 0.f, 0.f), 10, 10, 1.f);
	meshList[GEO_SNOWBALL] = MeshBuilder::GenerateSphere("snowball", Color(1.f, 1.f, 1.f), 10, 10, 1.f);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(0.f, 0.f, 0.f), 1.f);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(0.f, 0.f, 0.f), 1.f);
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);

	meshList[GEO_FOREGROUND] = MeshBuilder::GenerateQuad("foreground", Color(0.f, 0.f, 0.f), 1.f);
	meshList[GEO_FOREGROUND]->textureID = LoadTGA("Image//foreground.tga");
	meshList[GEO_BACKGROUND] = MeshBuilder::GenerateQuad("background", Color(0.f, 0.f, 0.f), 1.f);
	meshList[GEO_BACKGROUND]->textureID = LoadTGA("Image//background.tga");
	meshList[GEO_TERRAIN] = MeshBuilder::GenerateTerrain("GEO_TERRAIN", "Image//heightmap1.raw", m_heightMap);
	meshList[GEO_TERRAIN]->textureID = LoadTGA("Image//terrain.tga");
	meshList[GEO_CART] = MeshBuilder::GenerateQuad("cart", Color(0.f, 0.f, 0.f), 1.f);
	meshList[GEO_CART]->textureID = LoadTGA("Image//cart.tga");
	meshList[GEO_BRICK] = MeshBuilder::GenerateQuad("brick", Color(0.f, 0.f, 0.f), 1.f);
	meshList[GEO_BRICK]->textureID = LoadTGA("Image//brick.tga");
	meshList[GEO_BOMB] = MeshBuilder::GenerateQuad("bomb", Color(0.f, 0.f, 0.f), 1.f);
	meshList[GEO_BOMB]->textureID = LoadTGA("Image//bomb.tga");
	meshList[GEO_BOOM] = MeshBuilder::GenerateQuad("boom", Color(0.f, 0.f, 0.f), 1.f);
	meshList[GEO_BOOM]->textureID = LoadTGA("Image//boom.tga");
	meshList[GEO_SCREEN] = MeshBuilder::GenerateQuad("Screen", Color(0.f, 0.f, 0.f), 1.f);
	meshList[GEO_SCREEN]->textureID = LoadTGA("Image//Screen.tga");

	meshList[GEO_SPRITE_YETI] = MeshBuilder::GenerateSprite("yeti", 4, 4, 1.f);
	meshList[GEO_SPRITE_YETI]->textureID = LoadTGA("Image//Sprite_YetiLeft.tga");

	meshList[GEO_COIN] = MeshBuilder::GenerateSprite("coin", 1, 6, 1.f);
	meshList[GEO_COIN]->textureID = LoadTGA("Image//coin.tga");
	meshList[GEO_SLEDYETI] = MeshBuilder::GenerateSprite("sledyeti", 3, 5, 1.f);
	meshList[GEO_SLEDYETI]->textureID = LoadTGA("Image//sledyeti.tga");

	meshList[GEO_BOSS] = MeshBuilder::GenerateQuad("boss", Color(0.f, 0.f, 0.f), 1.f);
	meshList[GEO_BOSS]->textureID = LoadTGA("Image//boss.tga");

	meshList[GEO_HUD_SCORE] = MeshBuilder::GenerateQuad("hud_score", Color(0.f, 0.f, 0.f), 1.f);
	meshList[GEO_HUD_SCORE]->textureID = LoadTGA("Image//hud_score.tga");
	meshList[GEO_HUD_CHANCE] = MeshBuilder::GenerateQuad("hud_chance", Color(0.f, 0.f, 0.f), 1.f);
	meshList[GEO_HUD_CHANCE]->textureID = LoadTGA("Image//hud_chance.tga");
	meshList[GEO_HUD_LEVEL1] = MeshBuilder::GenerateQuad("hud_level1", Color(0.f, 0.f, 0.f), 1.f);
	meshList[GEO_HUD_LEVEL1]->textureID = LoadTGA("Image//hud_level1.tga");
	meshList[GEO_HUD_LEVEL2] = MeshBuilder::GenerateQuad("hud_level2", Color(0.f, 0.f, 0.f), 1.f);
	meshList[GEO_HUD_LEVEL2]->textureID = LoadTGA("Image//hud_level2.tga");
	meshList[GEO_HUD_LEVEL3] = MeshBuilder::GenerateQuad("hud_level3", Color(0.f, 0.f, 0.f), 1.f);
	meshList[GEO_HUD_LEVEL3]->textureID = LoadTGA("Image//hud_level3.tga");
	meshList[GEO_HUD_LEVEL4] = MeshBuilder::GenerateQuad("hud_level4", Color(0.f, 0.f, 0.f), 1.f);
	meshList[GEO_HUD_LEVEL4]->textureID = LoadTGA("Image//hud_level4.tga");
	meshList[GEO_HUD_LEVEL5] = MeshBuilder::GenerateQuad("hud_level5", Color(0.f, 0.f, 0.f), 1.f);
	meshList[GEO_HUD_LEVEL5]->textureID = LoadTGA("Image//hud_level5.tga");
	meshList[GEO_NO_1] = MeshBuilder::GenerateQuad("hud_1", Color(0.f, 0.f, 0.f), 1.f);
	meshList[GEO_NO_1]->textureID = LoadTGA("Image//Number//hud_1.tga");
	meshList[GEO_NO_2] = MeshBuilder::GenerateQuad("hud_2", Color(0.f, 0.f, 0.f), 1.f);
	meshList[GEO_NO_2]->textureID = LoadTGA("Image//Number//hud_2.tga");
	meshList[GEO_NO_3] = MeshBuilder::GenerateQuad("hud_3", Color(0.f, 0.f, 0.f), 1.f);
	meshList[GEO_NO_3]->textureID = LoadTGA("Image//Number//hud_3.tga");
	meshList[GEO_NO_4] = MeshBuilder::GenerateQuad("hud_4", Color(0.f, 0.f, 0.f), 1.f);
	meshList[GEO_NO_4]->textureID = LoadTGA("Image//Number//hud_4.tga");
	meshList[GEO_NO_5] = MeshBuilder::GenerateQuad("hud_5", Color(0.f, 0.f, 0.f), 1.f);
	meshList[GEO_NO_5]->textureID = LoadTGA("Image//Number//hud_5.tga");
	meshList[GEO_NO_6] = MeshBuilder::GenerateQuad("hud_6", Color(0.f, 0.f, 0.f), 1.f);
	meshList[GEO_NO_6]->textureID = LoadTGA("Image//Number//hud_6.tga");
	meshList[GEO_NO_7] = MeshBuilder::GenerateQuad("hud_7", Color(0.f, 0.f, 0.f), 1.f);
	meshList[GEO_NO_7]->textureID = LoadTGA("Image//Number//hud_7.tga");
	meshList[GEO_NO_8] = MeshBuilder::GenerateQuad("hud_8", Color(0.f, 0.f, 0.f), 1.f);
	meshList[GEO_NO_8]->textureID = LoadTGA("Image//Number//hud_8.tga");
	meshList[GEO_NO_9] = MeshBuilder::GenerateQuad("hud_9", Color(0.f, 0.f, 0.f), 1.f);
	meshList[GEO_NO_9]->textureID = LoadTGA("Image//Number//hud_9.tga");
	meshList[GEO_NO_0] = MeshBuilder::GenerateQuad("hud_0", Color(0.f, 0.f, 0.f), 1.f);
	meshList[GEO_NO_0]->textureID = LoadTGA("Image//Number//hud_0.tga");

	meshList[GEO_PU_SPEED] = MeshBuilder::GenerateQuad("pu_speed", Color(0.f, 0.f, 0.f), 1.f);
	meshList[GEO_PU_SPEED]->textureID = LoadTGA("Image//pu_speed.tga");
	meshList[GEO_PU_RANGE] = MeshBuilder::GenerateQuad("pu_range", Color(0.f, 0.f, 0.f), 1.f);
	meshList[GEO_PU_RANGE]->textureID = LoadTGA("Image//pu_range.tga");

	meshList[GEO_PARTICLE_SPARK] = MeshBuilder::GenerateQuad("GEO_PARTICLE_SPARK", Color(1, 1, 1), 1.f);
	meshList[GEO_PARTICLE_SPARK]->textureID = LoadTGA("Image//spark.tga");
	meshList[GEO_PARTICLE_RAIN] = MeshBuilder::GenerateQuad("GEO_PARTICLE_RAIN", Color(1, 1, 1), 1.f);
	meshList[GEO_PARTICLE_RAIN]->textureID = LoadTGA("Image//rain.tga");

	meshList[GEO_PARTICLE_EXPLOSION] = MeshBuilder::GenerateQuad("GEO_PARTICLE_EXPLOSION", Color(1, 1, 1), 1.f);
	meshList[GEO_PARTICLE_EXPLOSION]->textureID = LoadTGA("Image//explosion.tga");

	meshList[GEO_CLIFF] = MeshBuilder::GenerateQuad("GEO_CLIFF", Color(1, 1, 1), 1.f);
	meshList[GEO_CLIFF]->textureID = LoadTGA("Image//cliff.tga");

	meshList[GEO_MENU] = MeshBuilder::GenerateQuad("MENU", Color(1, 1, 1), 1.f);
	meshList[GEO_MENU]->textureID = LoadTGA("Image//menu1.tga");
	meshList[GEO_MENU2] = MeshBuilder::GenerateQuad("MENU2", Color(1, 1, 1), 1.f);
	meshList[GEO_MENU2]->textureID = LoadTGA("Image//menu2.tga");
	meshList[GEO_MENU3] = MeshBuilder::GenerateQuad("MENU3", Color(1, 1, 1), 1.f);
	meshList[GEO_MENU3]->textureID = LoadTGA("Image//menu3.tga");
	meshList[GEO_MENU4] = MeshBuilder::GenerateQuad("MENU4", Color(1, 1, 1), 1.f);
	meshList[GEO_MENU4]->textureID = LoadTGA("Image//menu4.tga");
	meshList[GEO_MENU5] = MeshBuilder::GenerateQuad("MENU5", Color(1, 1, 1), 1.f);
	meshList[GEO_MENU5]->textureID = LoadTGA("Image//menu5.tga");

	meshList[GEO_LEVEL1] = MeshBuilder::GenerateQuad("level", Color(1, 1, 1), 1.f);
	meshList[GEO_LEVEL1]->textureID = LoadTGA("Image//level1.tga");
	meshList[GEO_LEVEL2] = MeshBuilder::GenerateQuad("level2", Color(1, 1, 1), 1.f);
	meshList[GEO_LEVEL2]->textureID = LoadTGA("Image//level2.tga");
	meshList[GEO_LEVEL3] = MeshBuilder::GenerateQuad("level3", Color(1, 1, 1), 1.f);
	meshList[GEO_LEVEL3]->textureID = LoadTGA("Image//level3.tga");
	meshList[GEO_LEVEL4] = MeshBuilder::GenerateQuad("level4", Color(1, 1, 1), 1.f);
	meshList[GEO_LEVEL4]->textureID = LoadTGA("Image//level4.tga");
	meshList[GEO_LEVEL5] = MeshBuilder::GenerateQuad("level5", Color(1, 1, 1), 1.f);
	meshList[GEO_LEVEL5]->textureID = LoadTGA("Image//level5.tga");
	meshList[GEO_LEVELBACK] = MeshBuilder::GenerateQuad("levelback", Color(1, 1, 1), 1.f);
	meshList[GEO_LEVELBACK]->textureID = LoadTGA("Image//levelback.tga");

	meshList[GEO_LS] = MeshBuilder::GenerateQuad("ls", Color(1, 1, 1), 1.f);
	meshList[GEO_LS]->textureID = LoadTGA("Image//ls.tga");
	meshList[GEO_LS2] = MeshBuilder::GenerateQuad("ls2", Color(1, 1, 1), 1.f);
	meshList[GEO_LS2]->textureID = LoadTGA("Image//ls2.tga");
	meshList[GEO_LS3] = MeshBuilder::GenerateQuad("ls3", Color(1, 1, 1), 1.f);
	meshList[GEO_LS3]->textureID = LoadTGA("Image//ls3.tga");

	meshList[GEO_OPTIONS] = MeshBuilder::GenerateQuad("options1", Color(1, 1, 1), 1.f);
	meshList[GEO_OPTIONS]->textureID = LoadTGA("Image//options1.tga");
	meshList[GEO_OPTIONS2] = MeshBuilder::GenerateQuad("options2", Color(1, 1, 1), 1.f);
	meshList[GEO_OPTIONS2]->textureID = LoadTGA("Image//options2.tga");
	meshList[GEO_OPTIONS3] = MeshBuilder::GenerateQuad("options3", Color(1, 1, 1), 1.f);
	meshList[GEO_OPTIONS3]->textureID = LoadTGA("Image//options3.tga");

	meshList[GEO_WIN1] = MeshBuilder::GenerateQuad("win1", Color(1, 1, 1), 1.f);
	meshList[GEO_WIN1]->textureID = LoadTGA("Image//win1.tga");
	meshList[GEO_WIN2] = MeshBuilder::GenerateQuad("win2", Color(1, 1, 1), 1.f);
	meshList[GEO_WIN2]->textureID = LoadTGA("Image//win2.tga");
	meshList[GEO_WIN3] = MeshBuilder::GenerateQuad("win3", Color(1, 1, 1), 1.f);
	meshList[GEO_WIN3]->textureID = LoadTGA("Image//win3.tga");

	meshList[GEO_LOSE1] = MeshBuilder::GenerateQuad("lose1", Color(1, 1, 1), 1.f);
	meshList[GEO_LOSE1]->textureID = LoadTGA("Image//lose1.tga");
	meshList[GEO_LOSE2] = MeshBuilder::GenerateQuad("lose2", Color(1, 1, 1), 1.f);
	meshList[GEO_LOSE2]->textureID = LoadTGA("Image//lose2.tga");

	meshList[GEO_INSTRUCTIONS] = MeshBuilder::GenerateQuad("instructions", Color(1, 1, 1), 1.f);
	meshList[GEO_INSTRUCTIONS]->textureID = LoadTGA("Image//instructions.tga");

	meshList[GEO_SHOP] = MeshBuilder::GenerateQuad("Shop", Color(1, 1, 1), 1.f);
	meshList[GEO_SHOP]->textureID = LoadTGA("Image//Shop.tga");
	meshList[GEO_SIZE] = MeshBuilder::GenerateQuad("Size", Color(1, 1, 1), 1.f);
	meshList[GEO_SIZE]->textureID = LoadTGA("Image//Size.tga");
	meshList[GEO_SPEED] = MeshBuilder::GenerateQuad("Speed", Color(1, 1, 1), 1.f);
	meshList[GEO_SPEED]->textureID = LoadTGA("Image//Speed.tga");
	meshList[GEO_JUMP] = MeshBuilder::GenerateQuad("Jump", Color(1, 1, 1), 1.f);
	meshList[GEO_JUMP]->textureID = LoadTGA("Image//Jump.tga");
	meshList[GEO_BOOST] = MeshBuilder::GenerateQuad("Boost", Color(1, 1, 1), 1.f);
	meshList[GEO_BOOST]->textureID = LoadTGA("Image//Boost.tga");

	meshList[GEO_GOLD] = MeshBuilder::GenerateQuad("gold", Color(1, 1, 1), 1.f);
	meshList[GEO_GOLD]->textureID = LoadTGA("Image//gold.tga");
	meshList[GEO_PRICE] = MeshBuilder::GenerateQuad("price", Color(1, 1, 1), 1.f);
	meshList[GEO_PRICE]->textureID = LoadTGA("Image//price.tga");
	meshList[GEO_BOUGHT] = MeshBuilder::GenerateQuad("bought", Color(1, 1, 1), 1.f);
	meshList[GEO_BOUGHT]->textureID = LoadTGA("Image//bought.tga"); 
	meshList[GEO_INSUFFICIENT] = MeshBuilder::GenerateQuad("insufficient", Color(1, 1, 1), 1.f);
	meshList[GEO_INSUFFICIENT]->textureID = LoadTGA("Image//insufficient.tga");
	meshList[GEO_LIMIT] = MeshBuilder::GenerateQuad("limit", Color(1, 1, 1), 1.f);
	meshList[GEO_LIMIT]->textureID = LoadTGA("Image//limit.tga");

	//CSoundEngine::GetInstance()->Init(); 
	//
	//
	//CSoundEngine::GetInstance()->AddSound("Jump", "Image//Mario-jump-sound.mp3");
	//CSoundEngine::GetInstance()->AddSound("background-music", "Image//BGM.mp3");
	//CSoundEngine::GetInstance()->AddSound("blast", "Image//Blast.mp3");
	//CSoundEngine::GetInstance()->AddSound("toss", "Image//Toss.mp3");
	//CSoundEngine::GetInstance()->AddSound("getitem", "Image//Getitem.wav");

	bLightEnabled = false;
}

void SceneBase::Update(double dt)
{
	//Keyboard Section
	if(KeyboardController::GetInstance()->IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if(KeyboardController::GetInstance()->IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if(KeyboardController::GetInstance()->IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(KeyboardController::GetInstance()->IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	fps = (float)(1.f / dt);
}

void SceneBase::RenderText(Mesh* mesh, std::string text, Color color)
{
	if(!mesh || mesh->textureID <= 0)
		return;
	
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void SceneBase::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if(!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneBase::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if(enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);
		
		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{	
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	if(mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void SceneBase::RenderSpriteMesh(SpriteMesh * mesh, bool enableLight, int m_currentFrame)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render(m_currentFrame);
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void SceneBase::RenderMeshIn2D(Mesh *mesh, bool enableLight, double m_width, double m_height, float sizeX, float sizeY, float x, float y)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, m_width, 0, m_height, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(sizeX, sizeY, 1.f);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneBase::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SceneBase::RenderAllParticles()
{
	glDisable(GL_DEPTH_TEST);
	std::vector <ParticleObject*>::iterator it, end;
	end = particleList.end();

	for (it = particleList.begin(); it != end; ++it)
	{
		ParticleObject* particle = (ParticleObject*)*it;
		if (!particle->isActive)
			continue;
		RenderParticles(particle);
	}
	glEnable(GL_DEPTH_TEST);
}

ParticleObject * SceneBase::GetParticle(void)
{
	std::vector <ParticleObject*>::iterator it, end;
	end = particleList.end();

	for (it = particleList.begin(); it != end; ++it)
	{
		ParticleObject* particle = (ParticleObject*)*it;
		if (!particle->isActive)
		{
			particle->isActive = true;
			++m_particleCount;
			return particle;
		}
	}
	for (unsigned int i = 0; i < 10; ++i)
	{
		ParticleObject* particle = new ParticleObject(ParticleObject_TYPE::P_SPARK);
		particleList.push_back(particle);
	}

	ParticleObject* particle = particleList.back();
	particle->isActive = true;
	++m_particleCount;
	return particle;
}

void SceneBase::RenderParticles(ParticleObject * particle)
{
	switch (particle->type)
	{
	case ParticleObject_TYPE::P_SPARK:
		modelStack.PushMatrix();
		modelStack.Translate(particle->pos.x, particle->pos.y, particle->pos.z);
		modelStack.Rotate(particle->rotation, 0.0f, 0.0f, 1.0f);
		modelStack.Scale(particle->scale.x, particle->scale.y, particle->scale.z);
		RenderMesh(meshList[GEO_PARTICLE_SPARK], false);
		modelStack.PopMatrix();
		break;
	case ParticleObject_TYPE::P_RAIN:
		modelStack.PushMatrix();
		modelStack.Translate(particle->pos.x, particle->pos.y, particle->pos.z);
		modelStack.Rotate(particle->rotation, 0.0f, 0.0f, 1.0f);
		modelStack.Scale(particle->scale.x, particle->scale.y, particle->scale.z);
		RenderMesh(meshList[GEO_PARTICLE_RAIN], false);
		modelStack.PopMatrix();
		break;
	case ParticleObject_TYPE::P_EXPLOSION:
		modelStack.PushMatrix();
		modelStack.Translate(particle->pos.x, particle->pos.y, particle->pos.z);
		modelStack.Rotate(particle->rotation, 0.0f, 0.0f, 1.0f);
		modelStack.Scale(particle->scale.x, particle->scale.y, particle->scale.z);
		RenderMesh(meshList[GEO_PARTICLE_EXPLOSION], false);
		modelStack.PopMatrix();
		break;
	default:
		break;
	}
}

void SceneBase::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}
