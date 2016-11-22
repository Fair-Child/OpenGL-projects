#include "all_headers.h"

#define INTERPOLATION_ACCURACY 0.025f	// Smaller = More points
#define RAIL_SPACING 2.0f				// Multiplies the [-1,1] coordinates
#define RAILING_SCALE glm::vec3(0.003,0.0005,0.0005)

struct LightInfo {
	int Type;								//	0: point light, 1: spot light, 2: directed light

	glm::vec3 La;							// Ambient light intensity
	glm::vec3 Ld;							// Diffuse light intensity
	glm::vec3 Ls;							// Specular light intensity

	glm::vec3 LightPosition_worldspace;		// Light Position in world space
	glm::vec3 SpotLightDirection_worldspace;// Spot Light Direction in world space

	float CutOff;							// For Spotlight
	float OuterCutOff;						// For Spotlight

	glm::vec3 LightColor;					// For simplicity, to be removed
	float LightPower;						// For simplicity, to be removed

	bool dynamic = false;

	GLuint Type_ID;
	GLuint LightPosition_worldspace_ID;
	GLuint SpotLightDirection_worldspace_ID;
	GLuint La_ID;
	GLuint Ld_ID;
	GLuint Ls_ID;
	GLuint LightColor_ID;
	GLuint LightPower_ID;
	GLuint CutOff_ID;
	GLuint OuterCutOff_ID;
};

class GLRenderer
{
public:
	// Spline stuff
	void StartSpline();
	void PrepareSplineScene();
	void DrawSplineScene(GLFWwindow* win);

	// Train stuff
	void StartTrain();
	void PrepareTrainScene();
	void DrawTrainScene(GLFWwindow* win);

	void DestroyScene();
	void Draw(GLFWwindow* win);

protected:
	// Train stuff
	void PlaceRails();
	void UpdateTrainPosition(GLFWwindow* win);

	// Spline stuff
	void HandleUserInputSpline(GLFWwindow* win);
	void HandleUserInputTrain(GLFWwindow* win);
	void AddNewPoint(GLFWwindow* win, float xpos, float ypos);
	void InterpolatePoints(int index);
	void AddNewInterpolatedPoint(glm::vec3 pt);
	void RecursiveSubdivision(float u0, float u1, float max_line_length, int last_index);

	void linearly_interpolate(glm::vec3& dest, const glm::vec3& a, const glm::vec3& b, const float t);				// Simple linear interpolation between two points
	void bezier(glm::vec3 &dest, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d, const float t);

protected:
	GLProgram* m_pProgram;			// Program
	GLShader*  m_pVertSh;			// Vertex shader
	GLShader*  m_pFragSh;			// Fragment shader

	bool TrainReady = false;
	bool TrainPrepared = false;
	bool pause = true;

	std::vector<Model *> m_Models;	// Rail models container
	std::vector<LightInfo> m_Lights;
	void PrepareLights();

	int DRAW_MODE = GL_LINES;
	int NUM_OF_POINTS;

	static int pos;
	std::vector<glm::vec3> UserPoints;
	std::vector<glm::vec3> InterpolatedPoints;
	std::vector<glm::vec3> Tangents;
	GLuint VAO[2], VBO[2];

	GLuint MVP_MatrixID;			// handle for "MVP" uniform
	GLuint M_MatrixID;				// handle for "M" uniform
	GLuint V_MatrixID;				// handle for "V" uniform

	glm::mat4 Projection;			// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 View;					// Camera matrix
	glm::mat4 MVP;					// Our ModelViewProjection : multiplication of our 3 matrices

	float horizontalAngle = 3.14f;				// Initial horizontal angle : toward -Z		(angle around Y)
	float verticalAngle = 0.0f;					// Initial vertical angle : none			(angle around X)
		
	glm::vec3 position = glm::vec3(0, 0, 10); // Camera Information
	glm::vec3 up = glm::vec3(0, 1, 0);
	glm::vec3 right = glm::vec3(1, 0, 0);
	glm::vec3 direction;
};