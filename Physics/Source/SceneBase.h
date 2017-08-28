#ifndef SCENE_BASE_H
#define SCENE_BASE_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "GameObject.h"
#include <vector>

class ParticleObject;
class SpriteMesh;
class SpriteAnimation;

class SceneBase : public Scene
{
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_NUMLIGHTS,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};
public:
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_TEXT,
		GEO_BALL,
		GEO_CUBE,
		GEO_QUAD,
		GEO_PU_SPEED,
		GEO_PU_RANGE,
		GEO_PU_POWER,
		GEO_FOREGROUND,
		GEO_BACKGROUND,
		GEO_TERRAIN,
		GEO_CART,
		GEO_BRICK,
		GEO_SNOWBALL,
		GEO_BOMB,
		GEO_BOOM,
		GEO_SPRITE_YETI,
		GEO_PARTICLE_SPARK,
		GEO_PARTICLE_RAIN,
		GEO_PARTICLE_EXPLOSION,
		GEO_BOSS,
		GEO_SCREEN,

		GEO_HUD_SCORE,
		GEO_HUD_LEVEL1,
		GEO_HUD_LEVEL2,
		GEO_HUD_LEVEL3,
		GEO_HUD_LEVEL4,
		GEO_HUD_LEVEL5,
		GEO_HUD_CHANCE,
		GEO_NO_1,
		GEO_NO_2,
		GEO_NO_3,
		GEO_NO_4,
		GEO_NO_5,
		GEO_NO_6,
		GEO_NO_7,
		GEO_NO_8,
		GEO_NO_9,
		GEO_NO_0,

		GEO_MENU,
		GEO_MENU2,
		GEO_MENU3,
		GEO_MENU4,
		GEO_MENU5,
		GEO_LEVEL1,
		GEO_LEVEL2,
		GEO_LEVEL3,
		GEO_LEVEL4,
		GEO_LEVEL5,
		GEO_LEVELBACK,
		GEO_LS,
		GEO_LS2,
		GEO_LS3,
		GEO_OPTIONS,
		GEO_OPTIONS2,
		GEO_OPTIONS3,
		GEO_INSTRUCTIONS,
		GEO_WIN1,
		GEO_WIN2,
		GEO_WIN3,
		GEO_LOSE1,
		GEO_LOSE2,

		GEO_CLIFF,
		GEO_SHOP,
		GEO_SIZE,
		GEO_SPEED,
		GEO_JUMP,
		GEO_BOOST,
		GEO_COIN,
		GEO_SLEDYETI,
		GEO_GOLD,
		GEO_PRICE,
		GEO_BOUGHT,
		GEO_INSUFFICIENT,
		GEO_LIMIT,

		NUM_GEOMETRY,
	};
public:
	SceneBase();
	~SceneBase();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderSpriteMesh(SpriteMesh* mesh, bool enableLight, int m_currentFrame);
	void RenderMeshIn2D(Mesh *mesh, bool enableLight, double m_width = 160, double m_height = 120, float sizeX = 1.f, float sizeY = 1.f, float x = 0.0f, float y = 0.0f);
	void RenderGO(GameObject *go);

	GameObject* FetchGO();

	void RenderAllParticles();
	ParticleObject* GetParticle(void);
	void RenderParticles(ParticleObject* particle);
protected:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera camera;

	MS modelStack;
	MS viewStack;
	MS projectionStack;

	Light lights[1];

	bool bLightEnabled;

	float fps;

	std::vector<unsigned char> m_heightMap;
	std::vector<ParticleObject*> particleList;

	int m_particleCount;
	unsigned MAX_PARTICLE;
};

#endif