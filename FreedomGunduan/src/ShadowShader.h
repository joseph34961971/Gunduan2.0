#pragma once

#include <ShaderObject.h>

class ShadowShader : public ShaderObject
{
public:
	static unsigned int SHADOW_WIDTH;
	static unsigned int SHADOW_HEIGHT;
	ShadowShader();
	~ShadowShader();

	bool Init();
	void SetMMat(const GLfloat* value);
	void SetLSMat(const GLfloat* value);
	void GeneratingDepthMap();
	GLuint getDepthMapFBO();
	GLuint getDepthMapID();

private:
	GLuint mLocation;
	GLuint lsLocation;
	GLuint depthMapFBO;
	GLuint depthMapID;
};