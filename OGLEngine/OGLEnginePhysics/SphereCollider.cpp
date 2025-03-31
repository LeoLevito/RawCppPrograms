#include "SphereCollider.h"
#include "CollisionManager.h"
#include "ShaderManager.h"
#include "Graphics.h"
#include "MeshManager.h"

SphereCollider::SphereCollider()
{
	position = { 0.0f,0.0f,0.0f };
	radius = { 1.0f };

	hasGotFirstPosition = false;


	MeshManager::Get().currentlyLoadingMesh = true;
	MeshManager::Get().IsAvailableMemoryOK = true; //okay, this may become an edge case problem if the computer's RAM is already filled up when loading the scene.
	MeshManager::Get().IsObjSizeOK = true; //we know this to be true because we've loaded it before.
	sphereColliderVisualizer = MeshManager::Get().LoadMesh("../Meshes/Cubes/BlenderSphere");
	MeshManager::Get().currentlyLoadingMesh = false;
	sphereColliderVisualizer->bufferMesh();
}

SphereCollider::~SphereCollider()
{
}

void SphereCollider::SetPosition(glm::vec3 pos)
{
	if (hasGotFirstPosition == false) 
	{
		hasGotFirstPosition = true;
	}
	position = pos;
}

void SphereCollider::Update()
{
	if (drawDebugLines)
	{
		glm::mat4 myTrans = glm::mat4(1.0f);

		//use shader for line rendering
		ShaderManager::Get().lineShader->Use();
		ShaderManager::Get().lineShader->SetMatrix4(myTrans, "transform"); //apperently there's a better way to do this compared to using a Uniform type variable inside the vertex shader, Shader Buffer Storage Object, something like that, where we can have even more variables inside the shader and update them.
		ShaderManager::Get().lineShader->SetMatrix4(Camera::Get().myView, "view");
		ShaderManager::Get().lineShader->SetMatrix4(Camera::Get().projection, "projection");
		//ShaderManager::Get().shader->SetVector3(Camera::Get().myPosition, "viewPos"); //Doesn't really make sense to update this here but whatever.
		glm::vec3 color = { 0,1,0 };
		ShaderManager::Get().lineShader->SetVector3(color, "vertexColor");

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //I need to introduce a LateUpdate for this.
		sphereColliderVisualizer->Draw(); 
	}
}

void SphereCollider::DrawImgui()
{
	//Collider::DrawImgui();

	if (ImGui::DragFloat("Radius", &radius, .01f))
	{
	}

	if (ImGui::Checkbox("Draw Debug Lines", &drawDebugLines))
	{
	}
}

void SphereCollider::Serialization(std::fstream& file)
{
	file.write(reinterpret_cast<char*>(&radius), sizeof(float));
}

void SphereCollider::Deserialization(std::fstream& file)
{
	file.read(reinterpret_cast<char*>(&radius), sizeof(float));
}
