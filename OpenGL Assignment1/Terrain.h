#include "all_headers.h"

struct _vec2 {
	float x;
	float z;

	bool operator<(const _vec2& v) const {
		return (x < v.x) && (z < v.z);
	}
};

// TODO: Nothing really implemented yet.. only draws a triangle atm.

class Terrain {
	friend class GLRenderer;

protected:
	std::map<_vec2, float> DepthMap;
	std::vector<glm::vec3> vertices;


	void InsertPoint(float x, float z, float depth);
	void InsertPoint(glm::vec3 vertex);
	void Draw(GLFWwindow * win);

private:
	void SetupTerrain();
	void GenerateDepthMap();
	void LoadVertices();
	static float color[3];
	GLuint VAO, VBO[2];
};