#include "all_headers.h"

float Terrain::color[3] = { 0.092, 0.184, 0.092 };


int Terrain::MAX_X_POS = 0;
int Terrain::MAX_Z_POS = 0;

int Terrain::X_INCREMENT = 1;
int Terrain::Z_INCREMENT = 1;


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
	int width, height;

	unsigned char* pixels_ = SOIL_load_image("./models/consider_this_question.bmp", &width, &height, 0, SOIL_LOAD_RGB);

	MAX_X_POS = width;
	MAX_Z_POS = height;

	for (int i = 0; i <= width; i++)
	{
		for (int j = 0; j <= height; j++)
		{

			unsigned char r = pixels_[(i + j * width) * 3 + 0];
			unsigned char g = pixels_[(i + j * width) * 3 + 1];
			unsigned char b = pixels_[(i + j * width) * 3 + 2];

			float _r = (float)r / 255;
			float _g = (float)g / 255;
			float _b = (float)b / 255;

			InsertPoint(i * X_INCREMENT, HEIGHT_SCALAR*(_r + _g + _b), j * Z_INCREMENT);

		}
	}

	SOIL_free_image_data(pixels_);
}

void Terrain::LoadVertices() {
	int x = 0;
	int z = 0;

	for (int j = 0; j < MAX_Z_POS; j++) {

		x = 0;
		for (int i = 0; i < MAX_X_POS; i ++) {

			_vec2 pos1;
			float depth1;
			pos1.x = x;
			pos1.z = z;

			depth1 = DepthMap.find(pos1)->second;

			vertices.push_back(glm::vec3(pos1.x, depth1, pos1.z));


			_vec2 pos2;
			float depth2;
			pos2.x = x;
			pos2.z = z + Z_INCREMENT;

			depth2 = DepthMap.find(pos2)->second;

			vertices.push_back(glm::vec3(pos2.x, depth2, pos2.z));


			_vec2 pos3;
			float depth3;
			pos3.x = x + X_INCREMENT;
			pos3.z = z;

			depth3 = DepthMap.find(pos3)->second;

			vertices.push_back(glm::vec3(pos3.x, depth3, pos3.z));


			_vec2 pos4;
			float depth4;
			pos4.x = x + X_INCREMENT;
			pos4.z = z + Z_INCREMENT;

			depth4 = DepthMap.find(pos4)->second;


			vertices.push_back(glm::vec3(pos2.x, depth2, pos2.z));
			vertices.push_back(glm::vec3(pos3.x, depth3, pos3.z));
			vertices.push_back(glm::vec3(pos4.x, depth4, pos4.z));


			x += X_INCREMENT;
		}

		z += Z_INCREMENT;
	}
}


void Terrain::SetupTerrain() {

	srand(time(NULL));

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