#ifndef SPRITEANIMATION_H
#define SPRITEANIMATION_H

#include "Mesh.h"
#include "Vertex.h"

#include <string>

class SpriteMesh : public Mesh
{
public:
	SpriteMesh(const std::string &meshName, int row, int col)
		: Mesh(meshName) , m_row(row) , m_col(col) {}
	~SpriteMesh() {}

	virtual void Render(int m_currentFrame);

    int m_row;
    int m_col;

private:
};

class SpriteAnimation
{
public:
	SpriteAnimation();
	~SpriteAnimation();

	void Set(SpriteMesh* _mesh, int startFrame, int endFrame, int repeat, float time, bool active);
	void SetActive(bool _active);

	SpriteMesh* GetMesh();
	int GetCurFrame() const;

	void Update(double dt);

private:
	SpriteMesh* mesh;
	int startFrame;
	int endFrame;
	int repeatCount;
	float animTime;
	bool ended;
	bool animActive;

	float m_currentTime;
	int m_currentFrame;
	int m_playCount;
};

#endif