#include "all_headers.h"

using namespace glm;

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

			_vec2 pos4;
			Vertex * v4;
			pos4.x = (x + 1) * X_SCALAR + X_TRANSLATE;
			pos4.z = (z + 1) * Z_SCALAR + Z_TRANSLATE;

			v4 = &(DepthMap.find(pos4)->second);
			glm::vec3 p4 = v4->Position;

			float yDistP1P2 = glm::abs(p2.y - p1.y);
			float yDistP4P2 = glm::abs(p2.y - p4.y);

			float yDistP1P3 = glm::abs(p3.y - p1.y);
			float yDistP4P3 = glm::abs(p3.y - p4.y);

			float maxHorizontalYDist = glm::max(yDistP1P3, yDistP4P2);
			float maxVerticalYDist = glm::max(yDistP1P2, yDistP4P3);

			if (maxHorizontalYDist <= HEIGHT_SCALAR && maxVerticalYDist <= HEIGHT_SCALAR) {
				// Normals for v1, v2, v3
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

				// Normals for v3, v2, v4
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
			}
			else {
				int numOfHorizontalTriangle = glm::ceil(maxHorizontalYDist / HEIGHT_SCALAR);
				numOfHorizontalTriangle = glm::max(numOfHorizontalTriangle, 1);

				int numOfVerticalTriangle = glm::ceil(maxVerticalYDist / HEIGHT_SCALAR);
				numOfVerticalTriangle = glm::max(numOfVerticalTriangle, 1);

				std::vector<vec3> leftPos;
				std::vector<vec3> topPos;
				std::vector<vec3> rightPos;
				std::vector<vec3> bottomPos;
				for (int i = 0; i <= numOfVerticalTriangle; ++i) {
					vec3 newLeftPos = p1 + (p2 - p1) * ((float)i / numOfVerticalTriangle);
					leftPos.push_back(newLeftPos);

					vec3 newRightPos = p3 + (p4 - p3) * ((float)i / numOfVerticalTriangle);
					rightPos.push_back(newRightPos);
				}

				for (int i = 0; i <= numOfHorizontalTriangle; ++i) {
					vec3 newTopPos = p1 + (p3 - p1) * ((float)i / numOfHorizontalTriangle);
					topPos.push_back(newTopPos);

					vec3 newBottomPos = p2 + (p4 - p2) * ((float)i / numOfHorizontalTriangle);
					bottomPos.push_back(newBottomPos);
				}

				glm::vec3 normal = vec3(0.0f, 1.0f, 0.0f);

				for (int i = 0; i < numOfVerticalTriangle; ++i) {
					for (int j = 0; j < numOfHorizontalTriangle; ++j) {
						vec3 newPoint1 = leftPos[i] + (rightPos[i] - leftPos[i]) * ((float)j / numOfHorizontalTriangle);
						vec3 newPoint2 = leftPos[i + 1] + (rightPos[i + 1] - leftPos[i + 1]) * ((float)j / numOfHorizontalTriangle);
						vec3 newPoint3 = topPos[j + 1] + (bottomPos[j + 1] - topPos[j + 1]) * ((float)i / numOfVerticalTriangle);
						vec3 newPoint4 = topPos[j + 1] + (bottomPos[j + 1] - topPos[j + 1]) * ((float)(i + 1) / numOfVerticalTriangle);
						
						Vertex newV1;
						Vertex newV2;
						Vertex newV3;
						Vertex newV4;

						newV1.Position = newPoint1;						
						newV2.Position = newPoint2;						
						newV3.Position = newPoint3;						
						newV4.Position = newPoint4;

						// normal for v1, v2, v3
						glm::vec3 u = newPoint1 - newPoint2;
						glm::vec3 v = newPoint1 - newPoint3;
						normal = glm::cross(u, v);
						normal = glm::normalize(normal);

						newV1.Normal = normal;
						newV1.TexCoords = glm::vec2(p1.x / MAX_X_POS, p1.z / MAX_Z_POS);

						newV2.Normal = normal;
						newV2.TexCoords = glm::vec2(p2.x / MAX_X_POS, p2.z / MAX_Z_POS);

						newV3.Normal = normal;
						newV3.TexCoords = glm::vec2(p3.x / MAX_X_POS, p3.z / MAX_Z_POS);

						// Normals for v3, v2, v4
						u = newPoint2 - newPoint4;
						v = newPoint2 - newPoint3;
						normal = glm::cross(u, v);
						normal = glm::normalize(normal);

						newV4.Normal = normal;
						newV4.TexCoords = glm::vec2(p4.x / MAX_X_POS, p4.z / MAX_Z_POS);

						vertices.push_back(newV1);
						vertices.push_back(newV2);
						vertices.push_back(newV3);

						vertices.push_back(newV3);
						vertices.push_back(newV2);
						vertices.push_back(newV4);
					}
				}

				float angle = glm::dot(normal, glm::vec3(1, 0, 0));
				if (angle < 0.10) {
				SpawnMap.insert(std::pair<_vec2, bool>(pos4, false));
				}
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