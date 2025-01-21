#pragma once
class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();
	void CreateDepthTexture();


	unsigned int depthMapFBO;

	unsigned int depthMap;

	const unsigned int SHADOW_WIDTH = 1024;
	const unsigned int SHADOW_HEIGHT = 1024;


private:
};

