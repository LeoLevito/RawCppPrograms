#include "SphereCollider.h"
#include "CollisionManager.h"
#include "ShaderManager.h"
#include "Graphics.h"
#include "MeshManager.h"
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <iostream>

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
	//use shader for line rendering
	glm::mat4 defaultTrans = glm::mat4(1.0f);
	ShaderManager::Get().lineShader->Use(); //issue with this is if I have a mesh component created after this collider component, then this lineShader->Use() will override the shader->Use() in Graphics::Render(). Also an issue if you enable this debug draw on an object higher in the hierarchy. I should look into doing shader->Use() inside the MeshComponent.
	ShaderManager::Get().lineShader->SetMatrix4(defaultTrans, "transform"); //apperently there's a better way to do this compared to using a Uniform type variable inside the vertex shader, Shader Buffer Storage Object, something like that, where we can have even more variables inside the shader and update them.
	ShaderManager::Get().lineShader->SetMatrix4(Camera::Get().myView, "view");
	ShaderManager::Get().lineShader->SetMatrix4(Camera::Get().projection, "projection");

	//set up matrices for 3 axis circles
	glm::quat myRotationQuaternion = glm::quat(glm::radians(rotation)); //https://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
	glm::mat4 rotationMatrix = glm::toMat4(myRotationQuaternion);

	glm::mat4 AxisTrans = glm::mat4(1.0f);
	AxisTrans = glm::translate(AxisTrans, position); //translate first so that each object rotates independently.
	AxisTrans = AxisTrans * rotationMatrix;


	//calculate position and radius of circle facing the camera. Principle is this: https://astronomy.stackexchange.com/questions/37068/how-to-calculate-how-much-of-the-earths-or-another-planets-surface-is-visibl image from user Yellow Sky helped me visualize my trigonometry calculations below.
	float fraction; //fraction of sphere surface seen by camera. Not used anywhere asid from std::cout below.
	float D = glm::distance(position, Camera::Get().myPosition);
	fraction = ((D - radius) / (2 * D)); //https://www.phpsciencelabs.com/sphere-visible-fraction-of-surface-calculator/index.php

	float b;
	float c = glm::distance(position, Camera::Get().myPosition); //distance to closest point on surface

	b = glm::sqrt(glm::pow(c, 2) - glm::pow(radius, 2)); //distance to true horizon.

	float rAngle = glm::acos((glm::pow(c, 2) + glm::pow(b, 2) - glm::pow(radius, 2)) / (2 * c * b)); //radians, //pow by 2 is squared, //https://www.calculator.net/triangle-calculator.html?vc=&vx=&vy=2.82843&va=&vz=1&vb=90&angleunits=d&x=Calculate where one side is 3 units long because my camera is positioned at (0,0,3) at start.
	float height = radius * glm::sin(rAngle) / glm::sin(glm::radians(90.f)); //https://www.calculator.net/triangle-calculator.html?vc=90&vx=&vy=&va=19.4712&vz=1&vb=&angleunits=d&x=Calculate based on results from link above, get 'height' (distance from center of sphere) at which the spherical cap's true horizon line is located.
	float radiusAtHeight = radius * glm::sin(glm::radians(180.f - 90 - glm::degrees(rAngle))) / glm::sin(glm::radians(90.f)); //same link as line above, calculate radius of true horizon.

	//std::cout << "Visible fraction: " << fraction << ", distance to true horizon: " << b << ", rAngle: " << rAngle << ", height: " << height << ", radiusAtHeight: " << radiusAtHeight << std::endl;
	glm::vec3 positionOf4thCircle = position + glm::normalize(Camera::Get().myPosition - position) * (radius - (radius - height)); //https://discussions.unity.com/t/trying-to-get-closest-points-to-mesh-on-a-sphere/696587 answer by JoeStrout, with modification from me to calculate position at which to perform the circle line drawing. If camera is closer to the sphere, the position will be closer to the camera, if the camera is farther from the sphere, the position will be closer to the sphere's center.
	//std::cout << "positionOf4thCircle: " << positionOf4thCircle.x << " " << positionOf4thCircle.y << " " << positionOf4thCircle.z << std::endl;


	//set up matrices for 4th circle facing the camera.
	glm::vec3 camToCircleDirection = positionOf4thCircle - Camera::Get().myPosition;

	glm::quat myRotationQuaternion3 = glm::quatLookAt(glm::normalize(camToCircleDirection), glm::vec3(0.0f, 1.0f, 0.0f)); //woah it just works!
	glm::mat4 rotationMatrix3 = glm::toMat4(myRotationQuaternion3);

	glm::mat4 FacingCameraTrans = glm::mat4(1.0f);
	FacingCameraTrans = glm::translate(FacingCameraTrans, positionOf4thCircle); //translate first so that each object rotates independently.
	FacingCameraTrans = FacingCameraTrans * rotationMatrix3;


	//actually draw debug lines
	glm::vec3 color = { 0,1,0 };
	ShaderManager::Get().lineShader->SetVector3(color, "vertexColor");
	ActuallyDrawDebugLines(AxisTrans, 0, radius);
	ActuallyDrawDebugLines(AxisTrans, 1, radius);
	ActuallyDrawDebugLines(AxisTrans, 2, radius);

	color = { 1,1,1 };
	ShaderManager::Get().lineShader->SetVector3(color, "vertexColor");
	ActuallyDrawDebugLines(FacingCameraTrans, 3, radiusAtHeight);
}

void SphereCollider::ActuallyDrawDebugLines(glm::mat4 trans, int axis, float radius)
{
	float x;
	float y;
	float z;

	float x1;
	float y1;
	float z1;

	for (int i = 0; i < roundingAmount; i++)
	{
		switch (axis)
		{
		case 0: //x axis circle
			x = 0 + radius * glm::cos(glm::radians(divByRoundingAmount * i));
			y = 0 + radius * glm::sin(glm::radians(divByRoundingAmount * i));
			z = 0;

			x1 = 0 + radius * glm::cos(glm::radians(divByRoundingAmount * (i + 1)));
			y1 = 0 + radius * glm::sin(glm::radians(divByRoundingAmount * (i + 1)));
			z1 = 0;
			break;
		case 1: //z axis circle
			x = 0;
			y = 0 + radius * glm::sin(glm::radians(divByRoundingAmount * i));
			z = 0 + radius * glm::cos(glm::radians(divByRoundingAmount * i));

			x1 = 0;
			y1 = 0 + radius * glm::sin(glm::radians(divByRoundingAmount * (i + 1)));
			z1 = 0 + radius * glm::cos(glm::radians(divByRoundingAmount * (i + 1)));
			break;
		case 2: //y axis circle
			x = 0 + radius * glm::sin(glm::radians(divByRoundingAmount * i));
			y = 0;
			z = 0 + radius * glm::cos(glm::radians(divByRoundingAmount * i));

			x1 = 0 + radius * glm::sin(glm::radians(divByRoundingAmount * (i + 1)));
			y1 = 0;
			z1 = 0 + radius * glm::cos(glm::radians(divByRoundingAmount * (i + 1)));
			break;
		case 3: //facing camera circle
			x = 0 + radius * glm::sin(glm::radians(divByRoundingAmount * i));
			y = 0 + radius * glm::cos(glm::radians(divByRoundingAmount * i));
			z = 0;

			x1 = 0 + radius * glm::sin(glm::radians(divByRoundingAmount * (i + 1)));
			y1 = 0 + radius * glm::cos(glm::radians(divByRoundingAmount * (i + 1)));
			z1 = 0;
			break;
		default:
			break;
		}

		glm::vec3 xyz = { x, y, z };
		glm::vec3 xyz1 = { x1, y1, z1 };
		glm::vec3 point1world = glm::vec3(trans * glm::vec4(xyz, 1.0f));
		glm::vec3 point2world = glm::vec3(trans * glm::vec4(xyz1, 1.0f));
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
