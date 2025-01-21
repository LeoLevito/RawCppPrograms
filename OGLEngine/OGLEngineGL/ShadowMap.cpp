#include "ShadowMap.h"
#include <GLAD/glad.h>

ShadowMap::ShadowMap()
{
	glGenFramebuffers(1, &depthMapFBO);
	CreateDepthTexture();
}

ShadowMap::~ShadowMap()
{
}

void ShadowMap::CreateDepthTexture()
{
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0); //last 0 supposed to be NULL
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //repeat texture in y direction
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //repeat texture in x direction
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //mipmapping setup, minifying function, apply what texture filter to use in the last argument.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //mipmapping setup, magnification function, apply what texture filter to use in the last argument.

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE); //explicitly tell opengl to not render any color data.
	glReadBuffer(GL_NONE); //explicitly tell opengl to not render any color data.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
