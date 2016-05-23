#include "all_headers.h"

float Terrain::color[3] = { 0.092, 0.184, 0.092 };


void Terrain::InsertPoint(float x, float depth, float z) {

	_vec2 temp;
	temp.x = x;
	temp.z = z;
	DepthMap.insert(std::pair<_vec2, float>(temp, depth));
}

void Terrain::InsertPoint(glm::vec3 vertex) {
	_vec2 temp;
	temp.x = vertex.x;
	temp.z = vertex.z;
	DepthMap.insert(std::pair<_vec2, float>(temp, vertex.y));
}

void Terrain::Draw(GLFWwindow * win) {

	// To remove
	glDisable(GL_CULL_FACE);

	// Draw the triangles
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glEnable(GL_CULL_FACE);
}

void Terrain::GenerateDepthMap() {

	// These points should come from some kind of random source
	InsertPoint(glm::vec3(1000, 1000, 1000));
	InsertPoint(glm::vec3(500, 0, 500));
	InsertPoint(glm::vec3(1500, 0, 1500));

}

void Terrain::LoadVertices() {
	// This should push back the points from the depth map into the vertices IN THE PROPER ORDER for openGL to make triangles/or other shape we define in draw
	// GL_POLYGON might be the easiest?

	// Probably also need a tangent map at each point, to get smooth interpolated results >.<

	// It seems easier to pre-define certain shapes that we can "apply" to a plane surface to deform it based on some algorithm

	// Need to research procedural generation / voxels / surface splines !

	// TO REMOVE:
	vertices.push_back(glm::vec3(1000, 1000, 1000));
	vertices.push_back(glm::vec3(500, 0, 500));
	vertices.push_back(glm::vec3(1500, 0, 1500));
}


void Terrain::SetupTerrain() {

	//srand(time(NULL));

	GenerateDepthMap();
	LoadVertices();

	std::vector<GLfloat> g_color_buffer_data;

	for (int i = 0; i < vertices.size(); i++)  {
		// color based on location
		g_color_buffer_data.push_back(vertices[i].x / 10000);
		g_color_buffer_data.push_back(vertices[i].y / 1000);
		g_color_buffer_data.push_back(vertices[i].z / 10000);
	}	

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO[0]);
	glGenBuffers(1, &VBO[1]);
	glBindVertexArray(VAO);

	// Vertex Positions	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &(vertices[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,                  // attribute. Must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,     // stride
		NULL				// array buffer offset
		);

	// Vertex Colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, g_color_buffer_data.size() * sizeof(GLfloat), &(g_color_buffer_data[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}