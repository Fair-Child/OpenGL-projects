#include "all_headers.h"

int Terrain::MAX_X_POS = 0;
int Terrain::MAX_Z_POS = 0;

int Terrain::HEIGHT_SCALAR = 250;
int Terrain::X_SCALAR = 100;
int Terrain::Z_SCALAR = 100;


void Terrain::InsertPoint(Vertex v) {
	_vec2 temp;
	temp.x = v.Position.x;
	temp.z = v.Position.z;

	DepthMap.insert(std::pair<_vec2, Vertex>(temp, v));
}

void Terrain::Draw(GLFWwindow * win) {

	glDisable(GL_CULL_FACE);
	
	glBindTexture(GL_TEXTURE_2D, terrain_texture);
	glActiveTexture(GL_TEXTURE0);

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

			Vertex v;
			v.Position.x = i * X_SCALAR + X_TRANSLATE;
			v.Position.y = HEIGHT_SCALAR*(_r + _g + _b);
			v.Position.z = j * Z_SCALAR + Z_TRANSLATE;
			InsertPoint(v);

		}
	}

	SOIL_free_image_data(pixels_);

	ModifyTerrain();
}

void Terrain::ModifyTerrain() {

	for (int i = 0; i < NUM_OF_LINES; i++) {
		_vec2 start;
		_vec2 it;
		int x_increment = (rand() % 2) - 1;
		int z_increment = (rand() % 2) - 1;
		if (x_increment == 0 && z_increment == 0) {
			x_increment = 1;
		}
		float height = rand() % 50 + 1;
		int width = rand() % 100 + 1;

		auto pt = DepthMap.begin();
		std::advance(pt, rand() % DepthMap.size());
		start = pt->first;

		it = start;

		do {
			_vec2 jt = it;
			_vec2 ojt = it;

			if (DepthMap.count(it) <= 0) {
				break;
			}

			for (int j = 1; j < width; j++) {

				if (DepthMap.count(jt) <= 0 || DepthMap.count(ojt) <= 0) {
					break;
				}

				if (j * 10 > height) {
					//break;
				}

				DepthMap.find(jt)->second.Position.y += height - j * 10;

				if (j != 1) {
					DepthMap.find(ojt)->second.Position.y += height - j * 10;
				}

				if (x_increment == 1 || x_increment == -1) {
					jt.z += Z_SCALAR;
					ojt.z -= Z_SCALAR;

				} else if (z_increment == 1 || z_increment == -1) {

					jt.x += X_SCALAR;
					ojt.x -= X_SCALAR;
				}

			}			

			it.x += x_increment * X_SCALAR;
			it.z += z_increment * Z_SCALAR;

		} while (true);
		

	}
}

void Terrain::LoadVertices() {
	int x = 0;
	int z = 0;

	float u = 0.0f;
	float v = 0.0f;

	for (int j = 0; j < MAX_Z_POS; j++) {

		x = 0;
		for (int i = 0; i < MAX_X_POS; i ++) {

			_vec2 pos1;
			Vertex * v1;
			pos1.x = x * X_SCALAR + X_TRANSLATE;
			pos1.z = z * Z_SCALAR + Z_TRANSLATE;

			v1 = &(DepthMap.find(pos1)->second);
			glm::vec3 p1 = v1->Position;

			_vec2 pos2;
			Vertex * v2;
			pos2.x = x * X_SCALAR + X_TRANSLATE;
			pos2.z = (z + 1) * Z_SCALAR + Z_TRANSLATE;

			v2 = &(DepthMap.find(pos2)->second);
			glm::vec3 p2 = v2->Position;


			_vec2 pos3;
			Vertex * v3;
			pos3.x = (x + 1) * X_SCALAR + X_TRANSLATE;
			pos3.z = z * Z_SCALAR + Z_TRANSLATE;

			v3 = &(DepthMap.find(pos3)->second);
			glm::vec3 p3 = v3->Position;

			// Normals
			glm::vec3 u = p1 - p2;
			glm::vec3 v = p1 - p3;
			glm::vec3 normal = glm::cross(u, v);
			normal = glm::normalize(normal);

			v1->Normal = normal;
			v1->TexCoords = glm::vec2(p1.x / MAX_X_POS, p1.z / MAX_Z_POS);
			vertices.push_back(*v1);

			v2->Normal = normal;
			v2->TexCoords = glm::vec2(p2.x / MAX_X_POS, p2.z / MAX_Z_POS);
			vertices.push_back(*v2);

			v3->Normal = normal;
			v3->TexCoords = glm::vec2(p3.x / MAX_X_POS, p3.z / MAX_Z_POS);
			vertices.push_back(*v3);


			_vec2 pos4;
			Vertex * v4;
			pos4.x = (x + 1) * X_SCALAR + X_TRANSLATE;
			pos4.z = (z + 1) * Z_SCALAR + Z_TRANSLATE;

			v4 = &(DepthMap.find(pos4)->second);
			glm::vec3 p4 = v4->Position;

			// Normals
			u = p2 - p4;
			v = p2 - p3;
			normal = glm::cross(u, v);
			normal = glm::normalize(normal);

			vertices.push_back(*v3);
			vertices.push_back(*v2);

			v4->Normal = normal;
			v4->TexCoords = glm::vec2(p4.x / MAX_X_POS, p4.z / MAX_Z_POS);
			vertices.push_back(*v4);

			float angle = glm::dot(normal, glm::vec3(1, 0, 0));
			if (angle < 0.10) {
				SpawnMap.insert(std::pair<_vec2, bool>(pos4, false));
			}			

			x++;
		}

		z++;
	}
}

bool Terrain::CheckNothingNearby(_vec2 pos) {

	bool nothing_nearby = true;

	for (int i = 0; i < 50; i++) {
		for (int j = 0; j < 50; j++) {			

			_vec2 test;
			test.x = pos.x + i;
			test.z = pos.z + j;
			
			if (SpawnMap.count(test) > 0 && SpawnMap.find(test)->second == true) {
				nothing_nearby = false;
				break;
			}
		}
	}

	return nothing_nearby;
}


void Terrain::SetupTerrain() {

	srand(time(NULL));

	GenerateDepthMap();
	LoadVertices();

	int tex_num = 0;
	tex_num = rand() % NUM_OF_TEXTURES + 1;

	glGenTextures(1, &terrain_texture);

	std::string tex_path = "./models/terrain" + std::to_string(tex_num) + ".jpg";
	int width, height;

	unsigned char* image = SOIL_load_image(tex_path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, terrain_texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	SOIL_free_image_data(image);


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));

	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
}