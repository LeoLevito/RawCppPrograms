#pragma once
class ShadowCubeMap
{
public:
	ShadowCubeMap();
	~ShadowCubeMap();
	void CreateDepthCubeMap();

	unsigned int depthCubeMapFBO;

	unsigned int depthCubeMap;

	const unsigned int SHADOW_WIDTH = 1024;
	const unsigned int SHADOW_HEIGHT = 1024;

private:
};

