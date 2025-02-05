#include "BoxCollider.h"
#include <gtx/hash.hpp> //required for the unordered set
#include <unordered_set>

//https://textbooks.cs.ksu.edu/cis580/04-collisions/04-separating-axis-theorem/

BoxCollider::BoxCollider(ColliderComponent& componentRef)
{
	compRef = &componentRef;
	position = glm::vec3(0, 0, 0);
	corners.clear();
	corners.push_back(position + glm::vec3(-0.5f, -0.5f, 0.5f));
	corners.push_back(position + glm::vec3(0.5f, -0.5f, 0.5f));
	corners.push_back(position + glm::vec3(0.5f, 0.5f, 0.5f));
	corners.push_back(position + glm::vec3(-0.5f, 0.5f, 0.5f));

	corners.push_back(position + glm::vec3(-0.5f, -0.5f, -0.5f));
	corners.push_back(position + glm::vec3(0.5f, -0.5f, -0.5f));
	corners.push_back(position + glm::vec3(0.5f, 0.5f, -0.5f));
	corners.push_back(position + glm::vec3(-0.5f, 0.5f, -0.5f));

	auto normals = new std::unordered_set<glm::vec3>();

	auto edge = corners[corners.size() - 1] - corners[0];

	auto perp = new glm::vec3(edge.z, edge.y, edge.x); //this is currently not right

	normals->insert(glm::normalize(*perp));

	for (int i = 1; i < corners.size(); i++)
	{
		edge = corners[i] - corners[i - 1];

		perp = new glm::vec3(edge.z, edge.y, edge.x); //this is currently not right

		normals->insert(glm::normalize(*perp));
	}
	normalVector.clear();
	normalVector.insert(normalVector.end(), normals->begin(), normals->end());
}

void BoxCollider::SetPosition(glm::vec3 pos)
{
	position = pos;
	corners.clear(); //this has a random chance of happing right when collisionmanager is in the process of accessing it, which will cause a vector subscript out of range error.
	corners.push_back(position + glm::vec3(-0.5f, -0.5f, 0.5f));
	corners.push_back(position + glm::vec3(0.5f, -0.5f, 0.5f));
	corners.push_back(position + glm::vec3(0.5f, 0.5f, 0.5f));
	corners.push_back(position + glm::vec3(-0.5f, 0.5f, 0.5f));

	corners.push_back(position + glm::vec3(-0.5f, -0.5f, -0.5f));
	corners.push_back(position + glm::vec3(0.5f, -0.5f, -0.5f));
	corners.push_back(position + glm::vec3(0.5f, 0.5f, -0.5f));
	corners.push_back(position + glm::vec3(-0.5f, 0.5f, -0.5f));
	
	auto normals = new std::unordered_set<glm::vec3>();

	auto edge = corners[corners.size() - 1] - corners[0];

	auto perp = new glm::vec3(edge.z, edge.y, edge.x); //THIS IS PROBABLY NOT RIGHT, NEED TO RESEARCH 3D SAT EQUIVALENT.

	normals->insert(glm::normalize(*perp));

	for (int i = 1; i < corners.size(); i++)
	{
		edge = corners[i] - corners[i - 1];

		perp = new glm::vec3(edge.z, edge.y, edge.x); //THIS IS PROBABLY NOT RIGHT, NEED TO RESEARCH 3D SAT EQUIVALENT.

		normals->insert(glm::normalize(*perp));
	}
	normalVector.clear();
	normalVector.insert(normalVector.end(), normals->begin(), normals->end());
}

void BoxCollider::DrawImgui()
{
	Collider::DrawImgui();
	//if (ImGui::DragFloat3("Width Height Depth", &widthHeightDepth.x, .01f)) //now box is set in constructor, but can expand it to be able to change size of it.
	//{
	//}
}
