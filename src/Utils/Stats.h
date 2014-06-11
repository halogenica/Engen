#pragma once

#include "System/Common.h"

namespace gen
{

class Stats
{
public:
	Stats();
	~Stats();

	void OnLostDevice();
	void OnResetDevice();

	void AddVertices(unsigned int n);
	void SubVertices(unsigned int n);
	void AddTriangles(unsigned int n);
	void SubTriangles(unsigned int n);
    void AddRigidBodies(unsigned int n);
	void SubRigidBodies(unsigned int n);

    void SetCursorPosition(float x, float y);
	void SetTriCount(unsigned int n);
	void SetVertexCount(unsigned int n);
    void SetRigidBodyCount(unsigned int n);

	void UpdateFPS(float dt);
    void UpdateUPS(float dt);
    void Display();


	float m_FPS;
    float m_UPS;

private:
	// Prevent copying
	Stats(const Stats& rhs);
	Stats& operator=(const Stats& rhs);

	ci::gl::TextureFontRef  m_textFont;
    ci::Rectf               m_textBoundsRect;
	unsigned int            m_numTris;
	unsigned int            m_numVertices;
    unsigned int            m_numRigidBodies;
    float                   m_cursorX;
    float                   m_cursorY;
};
    
}
