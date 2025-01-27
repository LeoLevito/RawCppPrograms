#include "ShadowCubeMap.h"
#include <GLAD/glad.h>


ShadowCubeMap::ShadowCubeMap()
{
	glGenFramebuffers(1, &depthCubeMapFBO);
	CreateDepthCubeMap();
}

ShadowCubeMap::~ShadowCubeMap()
{
}

void ShadowCubeMap::CreateDepthCubeMap()
{
	glGenTextures(1, &depthCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
	for (unsigned i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0); //last 0 supposed to be NULL
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //mipmapping setup, minifying function, apply what texture filter to use in the last argument.
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //mipmapping setup, magnification function, apply what texture filter to use in the last argument.
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	glBindFramebuffer(GL_FRAMEBUFFER, depthCubeMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0); //glFramebufferTexture instead of glFramebufferTexture2D
	glDrawBuffer(GL_NONE); //explicitly tell opengl to not render any color data.
	glReadBuffer(GL_NONE); //explicitly tell opengl to not render any color data.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
