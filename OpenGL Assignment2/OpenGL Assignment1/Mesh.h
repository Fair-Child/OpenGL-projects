#include "all_headers.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;		// Faces
	glm::vec2 TexCoords;	// Texture coords, also known as UVs
};

struct Texture {
	GLuint id;
	std::string type;
	aiString path;
};

struct MaterialInfo
{
	glm::vec3 Ka;			//Ambient reflectivity
	glm::vec3 Kd;			//Diffuse reflectivity
	glm::vec3 Ks;			//Specular reflectivity
	float Shininess;		//Specular shininess factor
};

// Child of Model, holds the vertices, textures, buffers, etc...
// Sets itself up at construction
// Draws itself
class Mesh {
	friend class GLRenderer;
	friend class Model;

public:
	Mesh(std::vector<Vertex> _vertices, std::vector<GLuint> _indices, std::vector<Texture> _textures, Model* _parent, bool _centered = true);
	Mesh(Mesh * copy_this, int a);

protected:

	int mID;
	static int unique_ID;
	Model* parent;

	std::vector<Vertex> vertices;
	std::vector<Texture> textures;
	std::vector<GLuint> indices;

	void Draw(GLFWwindow * win);

	// Predefined transformations
	//void Spin(GLFWwindow * win);		// TODO: generalize, normalize speed
	//void Revolve(GLFWwindow * win);

	glm::mat4 ModelMat;					// Model matrix

	bool centered = true;				// Whether mesh's orgin is at its centeroid or not
	glm::vec4 centroid;					// Average center of Mesh
	glm::vec4 max;
	glm::vec4 min;
	GLfloat radius;

	GLfloat vertical_angle = 0.0f;
	GLfloat horizontal_angle = 0.0f;
	GLfloat change_value = 0.001f;

private:
	// Buffers
	GLuint VAO, VBO[2], EBO;

	void AddVertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 uv);
	void SetupMesh();
};