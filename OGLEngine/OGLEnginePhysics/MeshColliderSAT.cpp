#include "MeshColliderSAT.h"


MeshColliderSAT::MeshColliderSAT()
{
	position = glm::vec3(0, 0, 0);
	//UpdateBounds();
}

void MeshColliderSAT::SetPosition(glm::vec3 pos)
{
	position = pos;
}

void MeshColliderSAT::SetRotation(glm::vec3 rot)
{
	rotation = rot;
}

void MeshColliderSAT::SetScale(glm::vec3 scaleIn)
{
	scale = scaleIn;
}

void MeshColliderSAT::UpdateBounds()
{
	//position = dynamic_cast<MeshComponent*>(owner->components[i])->position; //need to get a reference to the transform component if there is one.
	//rotation = dynamic_cast<TransformComponent*>(owner->components[i])->rotation;
	//scale = dynamic_cast<TransformComponent*>(owner->components[i])->scale;

	//myCollider->SetPosition(position);
	//myCollider->SetRotation(rotation);
	//myCollider->SetScale(scale);
	//corners.clear(); //this has a random chance of happing right when collisionmanager is in the process of accessing it, which will cause a vector subscript out of range error.
	for (size_t i = 0; i < topParent->components.size(); i++)
	{
		if (dynamic_cast<MeshComponent*>(topParent->components[i])) //checking if owner has a component of type TransformComponent. Is of-type correct word-use in this case?
		{
			MeshComponent* mycomp = dynamic_cast<MeshComponent*>(topParent->components[i]);
			corners = mycomp->mesh->indexed_vertices;


			std::vector<glm::vec3> edges;
			std::vector<glm::vec3> normals;
			glm::vec3 edge = corners[corners.size() - 1] - corners[0];

			edges.push_back(edge);

			for (int i = 1; i < corners.size(); i++)
			{
				edge = corners[i] - corners[i - 1];

				edges.push_back(edge);
			}

			for (int i = 0; i < edges.size(); i++)
			{
				for (int j = i + 1; j < edges.size(); j++)
				{
					normals.push_back(glm::normalize(glm::cross(edges[i], edges[j])));
				}
			}

			normalVector = normals;
		}
	}









	//normalVector.clear();
	//normalVector.insert(normalVector.end(), normals.begin(), normals.end());

	////std::unordered_set<glm::vec3> normals;
	//std::vector<glm::highp_vec3> normals;

	//glm::vec3 edge = corners[corners.size() - 1] - corners[0];

	//glm::cross()
	////cross product edges to get face normals?

	//glm::vec3 perp = glm::vec3(edge.x, edge.y, edge.z); //THIS IS PROBABLY NOT RIGHT, NEED TO RESEARCH 3D SAT EQUIVALENT.

	//normals.push_back(glm::normalize(perp));

	//for (int i = 1; i < corners.size(); i++)
	//{
	//	edge = corners[i] - corners[i - 1];

	//	perp = glm::vec3(edge.x, edge.y, edge.z); //THIS IS PROBABLY NOT RIGHT, NEED TO RESEARCH 3D SAT EQUIVALENT.

	//	normals.push_back(glm::normalize(perp));
	//}
	//normalVector.clear();
	//normalVector.insert(normalVector.end(), normals.begin(), normals.end());
}

void MeshColliderSAT::DrawImgui()
{
}
