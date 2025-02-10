#include "BoxCollider.h"
#include <gtx/hash.hpp> //required for the unordered set
#include <gtx/polar_coordinates.hpp> //required for the unordered set
#include <unordered_set>
#include <string>
#include <iostream>


//https://textbooks.cs.ksu.edu/cis580/04-collisions/04-separating-axis-theorem/

BoxCollider::BoxCollider()
{
	position = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
	//UpdateBounds();
}

void BoxCollider::SetPosition(glm::vec3 pos)
{
	position = pos;
}

void BoxCollider::SetRotation(glm::vec3 rot)
{
	rotation = rot;
}

void BoxCollider::SetScale(glm::vec3 scaleIn)
{
	scale = scaleIn;
}

void BoxCollider::UpdateBounds()
{
	std::vector<glm::vec3> edges;
	std::vector<glm::vec3> normals;

	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, position); //translate first so that each object rotates independently.
	trans = glm::rotate(trans, rotation.x, glm::vec3(1, 0, 0)); //matrix, angle, axis.
	trans = glm::rotate(trans, rotation.y, glm::vec3(0, 1, 0));
	trans = glm::rotate(trans, rotation.z, glm::vec3(0, 0, 1));
	trans = glm::scale(trans, scale);

	

	//glm::rotate(trans, rotation.x, glm::vec3(1, 0, 0)
	//glm::rotate(trans, rotation.y, glm::vec3(0, 1, 0));
	//glm::rotate(trans, rotation.z, glm::vec3(0, 0, 1));
	//ShaderManager::Get().shader->SetMatrix4(trans, "transform"); //apperently there's a better way to do this compared to using a Uniform type variable inside the vertex shader, Shader Buffer Storage Object, something like that, where we can have even more variables inside the shader and update them.
	//ShaderManager::Get().shader->SetMatrix4(Camera::Get().myView, "view");
	//ShaderManager::Get().shader->SetMatrix4(Camera::Get().projection, "projection");
	//ShaderManager::Get().shader->SetVector3(Camera::Get().myPosition, "viewPos"); //Doesn't really make sense to update this here but whatever.

	//gl_Position = projection * view * transform * vec4(aPos, 1.0f);


	//glm::vec3 rotatedpoint = position + (rotation * (glm::vec3(-1.0f, -1.0f, 1.0f) - position));


	corners.clear(); //this HAD a random chance of happening right when collisionmanager is in the process of accessing it, which would cause a vector subscript out of range error. I've fixed this by only running this function in the collision manager's Process() function.


	//glm::vec3 xAxis = glm::vec3(1, 0, 0);
	//glm::vec3 yAxis = glm::vec3(0, 1, 0);
	//glm::vec3 zAxis = glm::vec3(0, 0, 1);

	//specify vertices of cube.
	glm::vec3 corner1 = glm::vec3(-1.0f, -1.0f, 1.0f);
	glm::vec3 corner2 = glm::vec3(1.0f, -1.0f, 1.0f);
	glm::vec3 corner3 = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 corner4 = glm::vec3(-1.0f, 1.0f, 1.0f);

	glm::vec3 corner5 = glm::vec3(-1.0f, -1.0f, -1.0f);
	glm::vec3 corner6 = glm::vec3(1.0f, -1.0f, -1.0f);
	glm::vec3 corner7 = glm::vec3(1.0f, 1.0f, -1.0f);
	glm::vec3 corner8 = glm::vec3(-1.0f, 1.0f, -1.0f);

	//https://stackoverflow.com/questions/36358621/multiply-vec3-with-mat4-using-glm oh my god this came in clutch. This one here is good for fundamental understanding of rotating one point around another: https://stackoverflow.com/questions/45357715/how-to-rotate-point-around-another-one
	corner1 = glm::vec3(trans * glm::vec4(corner1, 1.0f));
	corner2 = glm::vec3(trans * glm::vec4(corner2, 1.0f));
	corner3 = glm::vec3(trans * glm::vec4(corner3, 1.0f));
	corner4 = glm::vec3(trans * glm::vec4(corner4, 1.0f));

	corner5 = glm::vec3(trans * glm::vec4(corner5, 1.0f));
	corner6 = glm::vec3(trans * glm::vec4(corner6, 1.0f));
	corner7 = glm::vec3(trans * glm::vec4(corner7, 1.0f));
	corner8 = glm::vec3(trans * glm::vec4(corner8, 1.0f));

	//glm::vec3 pos1;
	//glm::vec3 pos2;
	//glm::vec3 pos3;
	//glm::vec3 pos4;

	//glm::vec3 pos5;
	//glm::vec3 pos6;
	//glm::vec3 pos7;
	//glm::vec3 pos8;

	//pos1.x = corner1.x * xAxis.x + corner1.y * yAxis.x + corner1.z * zAxis.x + position.x;
	//pos1.y = corner1.x * xAxis.y + corner1.y * yAxis.y + corner1.z * zAxis.y + position.y;
	//pos1.z = corner1.x * xAxis.z + corner1.y * yAxis.z + corner1.z * zAxis.z + position.z;

	//corner1 = (corner1 - position) * glm::polar(rotation) + position;
	//corner2 = (corner2 - position) * glm::polar(rotation) + position;
	//corner3 = (corner3 - position) * glm::polar(rotation) + position;
	//corner4 = (corner4 - position) * glm::polar(rotation) + position;

	//corner5 = (corner5 - position) * glm::polar(rotation) + position;
	//corner6 = (corner6 - position) * glm::polar(rotation) + position;
	//corner7 = (corner7 - position) * glm::polar(rotation) + position;
	//corner8 = (corner8 - position) * glm::polar(rotation) + position;

	std::cout << corner1.x << " " << corner1.y << " " << corner1.z << std::endl;

	corners.push_back(corner1);
	corners.push_back(corner2);
	corners.push_back(corner3);
	corners.push_back(corner4);

	corners.push_back(corner5);
	corners.push_back(corner6);
	corners.push_back(corner7);
	corners.push_back(corner8);

	//corners.push_back(position + (glm::vec3(-1.0f, -1.0f,  1.0f) * scale));
	//corners.push_back(position + (glm::vec3( 1.0f, -1.0f,  1.0f) * scale));
	//corners.push_back(position + (glm::vec3( 1.0f,  1.0f,  1.0f) * scale));
	//corners.push_back(position + (glm::vec3(-1.0f,  1.0f,  1.0f) * scale));

	//corners.push_back(position + (glm::vec3(-1.0f, -1.0f, -1.0f) * scale));
	//corners.push_back(position + (glm::vec3( 1.0f, -1.0f, -1.0f) * scale));
	//corners.push_back(position + (glm::vec3( 1.0f,  1.0f, -1.0f) * scale));
	//corners.push_back(position + (glm::vec3(-1.0f,  1.0f, -1.0f) * scale));



											  //NOTE: these comments may or may not be correct, the top edge of the front face may actually be the right or bottom one. This is just a way for me to keep track of the edges in a clockwise convention.
	//front edges
	edges.push_back(corners[0] - corners[1]); //0 top edge of front face
	edges.push_back(corners[1] - corners[2]); //1 right edge of front face
	edges.push_back(corners[2] - corners[3]); //2 bottom edge of front face
	edges.push_back(corners[3] - corners[0]); //3 left edge of front face

	//back edges
	edges.push_back(corners[4] - corners[5]); //4 top edge of back face
	edges.push_back(corners[5] - corners[6]); //5 right edge of back face
	edges.push_back(corners[6] - corners[7]); //6 bottom edge of back face
	edges.push_back(corners[7] - corners[4]); //7 left edge of back face

	//inbetween edges / rest of edges
	edges.push_back(corners[0] - corners[4]); //8 top edge of left face
	edges.push_back(corners[1] - corners[5]); //9 top edge of right face
	edges.push_back(corners[2] - corners[6]); //10 bottom edge of right face
	edges.push_back(corners[3] - corners[7]); //11 bottom edge of left face

	//calculate face normals
	normals.push_back(glm::cross(edges[0], edges[1])); //front face
	normals.push_back(glm::cross(edges[4], edges[5])); //back face
	normals.push_back(glm::cross(edges[0], edges[8])); //top face
	normals.push_back(glm::cross(edges[2], edges[10])); //bottom face
	normals.push_back(glm::cross(edges[1], edges[9])); //right face
	normals.push_back(glm::cross(edges[3], edges[11])); //left face

	normalVector = normals;

	////set up normals, I'm not gonna have to do this with actual meshes since I already index normals with those.
	//std::vector<glm::vec3> edges;
	//std::vector<glm::vec3> normals;

	//glm::vec3 edge = corners[corners.size() - 1] - corners[0];

	//edges.push_back(edge);

	//for (int i = 1; i < corners.size(); i++)
	//{
	//	edge = corners[i] - corners[i - 1];

	//	edges.push_back(edge);
	//}

	//for (int i = 0; i < edges.size(); i++)
	//{
	//	for (int j = i + 1; j < edges.size(); j++)
	//	{
	//		normals.push_back(glm::normalize(glm::cross(edges[i], edges[j])));
	//	}
	//}
	//normalVector.clear();
	//normalVector.insert(normalVector.end(), normals.begin(), normals.end());






	////std::unordered_set<glm::vec3> normals;
	//std::vector<glm::highp_vec3> normals;

	//glm::vec3 edge = corners[corners.size() - 1] - corners[0];
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

void BoxCollider::DrawImgui()
{
	Collider::DrawImgui();
	//if (ImGui::DragFloat3("Width Height Depth", &widthHeightDepth.x, .01f)) //now box is set in constructor, but can expand it to be able to change size of it.
	//{
	//}
	ImGui::Text("scale");
	ImGui::Text(std::to_string(scale.x).c_str());
	ImGui::Text(std::to_string(scale.y).c_str());
	ImGui::Text(std::to_string(scale.z).c_str());
}
