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

		GEO_MENU,
		GEO_MENU2,
		GEO_MENU3,
		GEO_MENU4,
		GEO_MENU5,
		GEO_START,
		GEO_START2,
		GEO_LS,
		GEO_LS2,
		GEO_LS3,
		GEO_OPTIONS,
		GEO_OPTIONS2,
		GEO_INSTRUCTIONS,
		GEO_EXIT,

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
	void RenderMeshIn2D(Mesh *mesh, bool enableLight, float sizeX = 1.f, float sizeY = 1.f, float x = 0.0f, float y = 0.0f);
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