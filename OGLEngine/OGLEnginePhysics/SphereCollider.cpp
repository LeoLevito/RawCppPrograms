#include "SphereCollider.h"
#include "CollisionManager.h"
#include "ShaderManager.h"
#include "Graphics.h"
#include "MeshManager.h"
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>

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

void SphereCollider::SetRotation(glm::vec3 rot)
{
	rotation = rot;
}

void SphereCollider::Update()
{
	if (drawDebugLines)
	{
		DrawDebugLines();
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

void SphereCollider::DrawDebugLines()
{
	glm::quat myRotationQuaternion = glm::quat(glm::radians(rotation)); //https://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
	glm::mat4 rotationMatrix = glm::toMat4(myRotationQuaternion);

	glm::mat4 myTrans = glm::mat4(1.0f);
	myTrans = glm::translate(myTrans, position); //translate first so that each object rotates independently.
	myTrans = myTrans * rotationMatrix;



	//use shader for line rendering
	glm::mat4 defaultTrans = glm::mat4(1.0f);
	ShaderManager::Get().lineShader->Use(); //issue with this is if I have a mesh component created after this collider component, then this lineShader->Use() will override the shader->Use() in Graphics::Render(). Also an issue if you enable this debug draw on an object higher in the hierarchy. I should look into doing shader->Use() inside the MeshComponent.
	ShaderManager::Get().lineShader->SetMatrix4(defaultTrans, "transform"); //apperently there's a better way to do this compared to using a Uniform type variable inside the vertex shader, Shader Buffer Storage Object, something like that, where we can have even more variables inside the shader and update them.
	ShaderManager::Get().lineShader->SetMatrix4(Camera::Get().myView, "view");
	ShaderManager::Get().lineShader->SetMatrix4(Camera::Get().projection, "projection");

	glm::vec3 color = { 0,1,0 };
	ShaderManager::Get().lineShader->SetVector3(color, "vertexColor");

	int roundingAmount = 32;
	float divByroundingAmount = (360.f / roundingAmount);

	for (int i = 0; i < roundingAmount; i++) //x axis circle
	{
		float x = 0 + radius * glm::cos(glm::radians(divByroundingAmount * i));
		float y = 0 + radius * glm::sin(glm::radians(divByroundingAmount * i));
		float z = 0;

		float x1 = 0 + radius * glm::cos(glm::radians(divByroundingAmount * (i + 1)));
		float y1 = 0 + radius * glm::sin(glm::radians(divByroundingAmount * (i + 1)));
		float z1 = 0;

		glm::vec3 xyz = { x, y, z };
		glm::vec3 xyz1 = { x1, y1, z1 };
		glm::vec3 point1world = glm::vec3(myTrans * glm::vec4(xyz, 1.0f));
		glm::vec3 point2world = glm::vec3(myTrans * glm::vec4(xyz1, 1.0f));
		Graphics::Get().DrawLine(point1world, point2world);
	}
	for (int i = 0; i < roundingAmount; i++) //z axis circle
	{
		float x = 0;
		float y = 0 + radius * glm::sin(glm::radians(divByroundingAmount * i));
		float z = 0 + radius * glm::cos(glm::radians(divByroundingAmount * i));

		float x1 = 0;
		float y1 = 0 + radius * glm::sin(glm::radians(divByroundingAmount * (i + 1)));
		float z1 = 0 + radius * glm::cos(glm::radians(divByroundingAmount * (i + 1)));

		glm::vec3 xyz = { x, y, z };
		glm::vec3 xyz1 = { x1, y1, z1 };
		glm::vec3 point1world = glm::vec3(myTrans * glm::vec4(xyz, 1.0f));
		glm::vec3 point2world = glm::vec3(myTrans * glm::vec4(xyz1, 1.0f));
		Graphics::Get().DrawLine(point1world, point2world);
	}
	for (int i = 0; i < roundingAmount; i++)//y axis circle
	{
		float x = 0 + radius * glm::sin(glm::radians(divByroundingAmount * i));
		float y = 0;
		float z = 0 + radius * glm::cos(glm::radians(divByroundingAmount * i));

		float x1 = 0 + radius * glm::sin(glm::radians(divByroundingAmount * (i + 1)));
		float y1 = 0;
		float z1 = 0 + radius * glm::cos(glm::radians(divByroundingAmount * (i + 1)));

		glm::vec3 xyz = { x, y, z };
		glm::vec3 xyz1 = { x1, y1, z1 };
		glm::vec3 point1world = glm::vec3(myTrans * glm::vec4(xyz, 1.0f));
		glm::vec3 point2world = glm::vec3(myTrans * glm::vec4(xyz1, 1.0f));
		Graphics::Get().DrawLine(point1world, point2world);
	}

	color = { 1,1,1 };
	ShaderManager::Get().lineShader->SetVector3(color, "vertexColor");

	glm::vec3 camToCircleDirection = position - Camera::Get().myPosition;

	glm::quat myRotationQuaternion3 = glm::quatLookAt(glm::normalize(camToCircleDirection), glm::vec3(0.0f, 1.0f, 0.0f)); //woah it just works!
	glm::mat4 rotationMatrix3 = glm::toMat4(myRotationQuaternion3);

	glm::mat4 myTrans3 = glm::mat4(1.0f);
	myTrans3 = glm::translate(myTrans3, position); //translate first so that each object rotates independently.
	myTrans3 = myTrans3 * rotationMatrix3;
	for (int i = 0; i < roundingAmount; i++)
	{
		float x = 0 + radius * glm::sin(glm::radians(divByroundingAmount * i));
		float y = 0 + radius * glm::cos(glm::radians(divByroundingAmount * i));
		float z = 0;

		float x1 = 0 + radius * glm::sin(glm::radians(divByroundingAmount * (i + 1)));
		float y1 = 0 + radius * glm::cos(glm::radians(divByroundingAmount * (i + 1)));
		float z1 = 0;

		glm::vec3 xyz = { x, y, z };
		glm::vec3 xyz1 = { x1, y1, z1 };
		glm::vec3 point1world = glm::vec3(myTrans3 * glm::vec4(xyz, 1.0f));
		glm::vec3 point2world = glm::vec3(myTrans3 * glm::vec4(xyz1, 1.0f));
		Graphics::Get().DrawLine(point1world, point2world);
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
