#include "ShadowShader.h"

unsigned int ShadowShader::SHADOW_WIDTH = 8192;
unsigned int ShadowShader::SHADOW_HEIGHT = 8192;

ShadowShader::ShadowShader()
{
}


ShadowShader::~ShadowShader()
{
}

bool ShadowShader::Init()
{
	if (!ShaderObject::Init())
	{
		return false;
	}

	if (!AddShader(GL_VERTEX_SHADER, ResourcePath::shaderPath + "shadow.vert"))
	{
		return false;
	}

	if (!AddShader(GL_FRAGMENT_SHADER, ResourcePath::shaderPath + "shadow.frag"))
	{
		return false;
	}

	if (!Finalize())
	{
		return false;
	}

	mLocation = GetUniformLocation("model");
	if (mLocation == -1)
	{
		puts("Get uniform loaction error: model");
		return false;
	}

	lsLocation = GetUniformLocation("lightSpaceMatrix");
	if (lsLocation == -1)
	{
		puts("Get uniform loaction error: lightSpaceMatrix");
		return false;
	}

	GeneratingDepthMap();
	
	return true;
}

void ShadowShader::SetMMat(const GLfloat *value)
{
	glUniformMatrix4fv(mLocation, 1, GL_FALSE, value);
}

void ShadowShader::SetLSMat(const GLfloat *value)
{
	glUniformMatrix4fv(lsLocation, 1, GL_FALSE, value);
}

void ShadowShader::GeneratingDepthMap()
{
	//************************************************************************
	//
	// * generate a new texture
	//
	//========================================================================
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMapID);
	glBindTexture(GL_TEXTURE_2D, depthMapID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapID, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint ShadowShader::getDepthMapFBO()
{
	return depthMapFBO;
}

GLuint ShadowShader::getDepthMapID()
{
	return depthMapID;
}