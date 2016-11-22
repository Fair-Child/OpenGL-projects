#include "all_headers.h"


class Model
{
	friend class GLRenderer;

public:
	Model(GLchar* path, std::string _name = "unspecified", bool _centered = true);
	Model(Model * copy_this);

protected:
	int mID;
	static int unique_ID;
	std::string name;
	bool centered = true;

	void Draw(GLFWwindow* win);

	void Translate(glm::vec3 change);
	void Rotate(float angle, glm::vec3 vector);
	void Scale(glm::vec3 change);

private:
	std::vector<Mesh> meshes;
	std::string directory;

	void LoadModel(std::string path);
	void SetupModel();

	//Below functions have been taken from source: http://www.learnopengl.com/#!Model-Loading/Model
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	GLint TextureFromFile(const char* path, std::string directory);
};