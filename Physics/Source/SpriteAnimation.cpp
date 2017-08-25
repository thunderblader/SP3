#include "SpriteAnimation.h"
#include "GL\glew.h"

SpriteAnimation::SpriteAnimation() : m_currentFrame(0), m_currentTime(0), m_playCount(0)
{
}

SpriteAnimation::~SpriteAnimation()
{
}

void SpriteAnimation::Set(SpriteMesh* _mesh, int _startFrame, int _endFrame, int _repeat, float _time, bool _active)
{
	mesh = _mesh;
	startFrame = _startFrame;
	endFrame = _endFrame;
	repeatCount = _repeat;
	animTime = _time;
	animActive = _active;
}

void SpriteAnimation::SetActive(bool _active)
{
	animActive = _active;
}

SpriteMesh * SpriteAnimation::GetMesh()
{
	return mesh;
}

int SpriteAnimation::GetCurFrame() const
{
	return m_currentFrame;
}

void SpriteAnimation::Update(double dt)
{
	if (this == NULL)
		return;

	if (!animActive)
	{
		return;
	}

	m_currentTime += static_cast<float>(dt);
	int numFrame = Math::Max(1, endFrame - startFrame + 1);
	float frameTime = animTime / numFrame;

	m_currentFrame = Math::Min(endFrame, startFrame + static_cast<int>(m_currentTime / frameTime));

	if (m_currentTime >= animTime)
	{
		if (repeatCount == 0 || repeatCount <= m_playCount)
		{
			animActive = false;
			m_currentTime = 0.f;
			m_currentFrame = startFrame;
			repeatCount = 0;
			m_playCount = 0;
			ended = true;
		}

		if (repeatCount >= 1)
		{
			m_currentTime = 0.f;
			m_currentFrame = startFrame;
			++m_playCount;
		}
	}
}

void SpriteMesh::Render(int m_currentFrame)
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Position));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color)));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color) + sizeof(Vector3)));

	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	if (mode == Mesh::DRAW_LINES)
		glDrawElements(GL_LINES, indexSize, GL_UNSIGNED_INT, (void*)(m_currentFrame * 6 * sizeof(GLint)));
	else if (mode == Mesh::DRAW_TRIANGLE_STRIP)
		glDrawElements(GL_TRIANGLE_STRIP, indexSize, GL_UNSIGNED_INT, (void*)(m_currentFrame * 6 * sizeof(GLint)));
	else
		glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, (void*)(m_currentFrame * 6 * sizeof(GLint)));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}
