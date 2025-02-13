#include "BoxCollider.h"
#include <gtc/matrix_transform.hpp>
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
	//https://gamedev.stackexchange.com/questions/44500/how-many-and-which-axes-to-use-for-3d-obb-collision-with-sat
	//helpful code answers from Acegikmo (Freya Holmér!!) and Bas Smit!

	//set up transformation for rotation of axes which will later be used to check collision.
	glm::mat4 trans2 = glm::mat4(1.0f);
	trans2 = glm::translate(trans2, glm::vec3(0,0,0));
	trans2 = glm::rotate(trans2, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)); //matrix, angle, axis.
	trans2 = glm::rotate(trans2, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	trans2 = glm::rotate(trans2, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	//set up right, up and forward vectors.
	right   = glm::vec3(1.0f, 0.0f, 0.0f);
	up		= glm::vec3(0.0f, 1.0f, 0.0f);
	forward = glm::vec3(0.0f, 0.0f, 1.0f);
	//apply the object's rotation to the right, up and forward vectors in relation to origo (0,0,0)
	right   = glm::vec3(trans2 * glm::vec4(right,	1.0f));
	up		= glm::vec3(trans2 * glm::vec4(up,		1.0f));
	forward = glm::vec3(trans2 * glm::vec4(forward, 1.0f));


	//https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_05
	//specify vertices of cube.
	glm::vec3 corner1 = glm::vec3(-1.0f, -1.0f, 1.0f);
	glm::vec3 corner2 = glm::vec3(1.0f, -1.0f, 1.0f);
	glm::vec3 corner3 = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 corner4 = glm::vec3(-1.0f, 1.0f, 1.0f);

	glm::vec3 corner5 = glm::vec3(-1.0f, -1.0f, -1.0f);
	glm::vec3 corner6 = glm::vec3(1.0f, -1.0f, -1.0f);
	glm::vec3 corner7 = glm::vec3(1.0f, 1.0f, -1.0f);
	glm::vec3 corner8 = glm::vec3(-1.0f, 1.0f, -1.0f);


	//set up transformation for rotation and scale
	trans = glm::mat4(1.0f);
	trans = glm::translate(trans, position); //translate first so that each object rotates independently.
	trans = glm::rotate(trans, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)); //matrix, angle, axis.
	trans = glm::rotate(trans, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	trans = glm::rotate(trans, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	trans = glm::scale(trans, scale);

	//https://stackoverflow.com/questions/36358621/multiply-vec3-with-mat4-using-glm oh my god this came in clutch. 
	//https://stackoverflow.com/questions/45357715/how-to-rotate-point-around-another-one This one here is good for fundamental understanding of rotating one point around another.
	//apply rotation and scale to each vertex in relation to the objects position (which was specified in the transformation above).
	corner1 = glm::vec3(trans * glm::vec4(corner1, 1.0f));
	corner2 = glm::vec3(trans * glm::vec4(corner2, 1.0f));
	corner3 = glm::vec3(trans * glm::vec4(corner3, 1.0f));
	corner4 = glm::vec3(trans * glm::vec4(corner4, 1.0f));

	corner5 = glm::vec3(trans * glm::vec4(corner5, 1.0f));
	corner6 = glm::vec3(trans * glm::vec4(corner6, 1.0f));
	corner7 = glm::vec3(trans * glm::vec4(corner7, 1.0f));
	corner8 = glm::vec3(trans * glm::vec4(corner8, 1.0f));


	corners.clear(); //this HAD a random chance of happening right when collisionmanager is in the process of accessing it, which would cause a vector subscript out of range error. I've fixed this by only running this function in the collision manager's Process() function.
	//front corners
	corners.push_back(corner1); //top left front
	corners.push_back(corner2); //top right front
	corners.push_back(corner3); //bottom right front
	corners.push_back(corner4); //bottom left front
	//back corners
	corners.push_back(corner5); //top left back
	corners.push_back(corner6); //top right back
	corners.push_back(corner7); //bottom right back
	corners.push_back(corner8); //bottom left back


	glm::vec3 normalizedVector{ 1.0f,1.0f,1.0f };
	extents = glm::vec3(trans * glm::vec4(normalizedVector, 1.0f)); //okay my extents are wrong.
	extents = scale;

	extentsMax = corner3;
	extentsMin = corner5;

	//std::cout << extents.x << " " << extents.y << " " << extents.z << std::endl;


	//Not needed anymore. But could still be nice to have.
											  
	//Calculate edges										  
	std::vector<glm::vec3> edges;

	//front edges							  //NOTE: these comments may or may not be correct, the top edge of the front face may actually be the right or bottom one. This is just a way for me to keep track of the edges in a clockwise convention.
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

	//extents.x = edges[0];
	//extents.y = edges[1];
	//extents.z = edges[9];

	//Calculate face normals
	//https://www.euclideanspace.com/maths/algebra/vectors/applications/normals/index.htm
	std::vector<glm::vec3> normals;
	normals.push_back(glm::normalize(glm::cross(edges[0], edges[1]))); //front face
	normals.push_back(glm::normalize(glm::cross(edges[4], edges[5]))); //back face
	normals.push_back(glm::normalize(glm::cross(edges[0], edges[8]))); //top face
	normals.push_back(glm::normalize(glm::cross(edges[2], edges[10]))); //bottom face
	normals.push_back(glm::normalize(glm::cross(edges[1], edges[9]))); //right face
	normals.push_back(glm::normalize(glm::cross(edges[3], edges[11]))); //left face

	normalVector = normals;

	//we can do an average of these now and make an adjascent std::vector for the normals vector.
	std::vector<glm::vec3> faceNormalVerticesAverages;
	faceNormalVerticesAverages.push_back((corner1 + corner2 + corner3 + corner4) / 4.0f); //front corners
	faceNormalVerticesAverages.push_back((corner5 + corner6 + corner7 + corner8) / 4.0f); //back corners
	faceNormalVerticesAverages.push_back((corner1 + corner2 + corner5 + corner6) / 4.0f); //top corners
	faceNormalVerticesAverages.push_back((corner3 + corner4 + corner7 + corner8) / 4.0f); //bottom corners
	faceNormalVerticesAverages.push_back((corner1 + corner4 + corner5 + corner8) / 4.0f); //left corners
	faceNormalVerticesAverages.push_back((corner2 + corner3 + corner6 + corner7) / 4.0f); //right corners

	averageVector = faceNormalVerticesAverages;

	std::vector<glm::vec3> faceVertices1;
	std::vector<glm::vec3> faceVertices2;
	std::vector<glm::vec3> faceVertices3;
	std::vector<glm::vec3> faceVertices4;
	std::vector<glm::vec3> faceVertices5;
	std::vector<glm::vec3> faceVertices6;
	//front corners
	faceVertices1.push_back(corner1);
	faceVertices1.push_back(corner2);
	faceVertices1.push_back(corner3);
	faceVertices1.push_back(corner4);
	//back corners
	faceVertices2.push_back(corner5);
	faceVertices2.push_back(corner6);
	faceVertices2.push_back(corner7);
	faceVertices2.push_back(corner8);
	//top corners
	faceVertices3.push_back(corner1);
	faceVertices3.push_back(corner2);
	faceVertices3.push_back(corner5);
	faceVertices3.push_back(corner6);
	//bottom corners
	faceVertices4.push_back(corner3);
	faceVertices4.push_back(corner4);
	faceVertices4.push_back(corner7);
	faceVertices4.push_back(corner8);
	//left corners
	faceVertices5.push_back(corner1);
	faceVertices5.push_back(corner4);
	faceVertices5.push_back(corner5);
	faceVertices5.push_back(corner8);
	//right corners
	faceVertices6.push_back(corner2);
	faceVertices6.push_back(corner3);
	faceVertices6.push_back(corner6);
	faceVertices6.push_back(corner7);

	faceCornerVector.clear();

	faceCornerVector.push_back(faceVertices1);
	faceCornerVector.push_back(faceVertices2);
	faceCornerVector.push_back(faceVertices3);
	faceCornerVector.push_back(faceVertices4);
	faceCornerVector.push_back(faceVertices5);
	faceCornerVector.push_back(faceVertices6);
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
