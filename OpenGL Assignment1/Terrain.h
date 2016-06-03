#include "all_headers.h"

#define HEIGHT_SCALAR 1000

struct _vec2 {
	float x;
	float z;

	bool operator<(const _vec2& v) const {

		if (x < v.x)
			return true;
		if (v.x < x)
			return false;
		if (z < v.z)
			return true;
		if (v.z < z)
			return false;
		return false;
	}
};

// TODO: Nothing really implemented yet.. only draws a triangle atm.

class Terrain {
	friend class GLRenderer;

protected:
	std::multimap<_vec2, float> DepthMap;
	std::vector<glm::vec3> vertices;

	void InsertPoint(float x, float z, float depth);
	void InsertPoint(glm::vec3 vertex);
	void Draw(GLFWwindow * win);

	static int MAX_X_POS;
	static int X_INCREMENT;

	static int MAX_Z_POS;
	static int Z_INCREMENT;

private:
	void SetupTerrain();
	void GenerateDepthMap();
	void LoadVertices();
	static float color[3];
	GLuint VAO, VBO[2];
};