#include "all_headers.h"

int GLRenderer::pos = 0;

void GLRenderer::PrepareSplineScene()
{
	// Grey background
	glClearColor(0.170f, 0.170f, 0.200f, 0.0f);

	glPointSize(6.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
}

void GLRenderer::PrepareTrainScene() {
	// Grey background
	glClearColor(0.170f, 0.170f, 0.200f, 0.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Default wireframe

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Enable backface culling
	glEnable(GL_CULL_FACE);

	/*vvvvvvvvvvvvvvvvvvvv Loading shaders vvvvvvvvvvvvvvvvv*/

	// All purpose shaders
	m_pProgram = new GLProgram();
	m_pVertSh = new GLShader(GL_VERTEX_SHADER);
	m_pFragSh = new GLShader(GL_FRAGMENT_SHADER);

	int status = 0;

	status += (int)m_pVertSh->Load("lit.vert");
	status += (int)m_pFragSh->Load("lit.frag");

	status += (int)m_pVertSh->Compile();
	status += (int)m_pFragSh->Compile();

	m_pProgram->AttachShader(m_pVertSh);
	m_pProgram->AttachShader(m_pFragSh);

	status += (int)m_pProgram->Link();

	m_pProgram->Use();

	// Get a handle for the uniforms
	MVP_MatrixID = glGetUniformLocation(m_pProgram->GetID(), "MVP");
	M_MatrixID = glGetUniformLocation(m_pProgram->GetID(), "M");
	V_MatrixID = glGetUniformLocation(m_pProgram->GetID(), "V");

	if (status != 5) {
		std::cout << "Error during loading/compiling/linking of shaders!!\n";
	}

	for (int i = 0; i < InterpolatedPoints.size(); i++) {
		InterpolatedPoints[i].z = InterpolatedPoints[i].y;
		InterpolatedPoints[i].y = 0;
	}

	PlaceRails();
	PrepareLights();
	
	TrainPrepared = true;

	mciSendString(L"open \"choo.mp3\" type mpegvideo alias choo", NULL, 0, NULL);
}

void GLRenderer::PrepareLights() {

	/*		  type_light_index		*/
	LightInfo point_light_0;

	point_light_0.Type = 0; //Point
	point_light_0.LightPosition_worldspace = InterpolatedPoints[0];
	point_light_0.LightColor = glm::vec3(0, 1, 0);
	point_light_0.LightPower = 0.3f;

	point_light_0.Type_ID = glGetUniformLocation(m_pProgram->GetID(), "Lights[0].Type");
	glUniform1i(point_light_0.Type_ID, point_light_0.Type);

	point_light_0.LightPosition_worldspace_ID = glGetUniformLocation(m_pProgram->GetID(), "Lights[0].LightPosition_worldspace");
	glUniform3fv(point_light_0.LightPosition_worldspace_ID, 1, &point_light_0.LightPosition_worldspace[0]);

	point_light_0.LightColor_ID = glGetUniformLocation(m_pProgram->GetID(), "Lights[0].LightColor");
	glUniform3fv(point_light_0.LightColor_ID, 1, &point_light_0.LightColor[0]);

	point_light_0.LightPower_ID = glGetUniformLocation(m_pProgram->GetID(), "Lights[0].LightPower");
	glUniform1f(point_light_0.LightPower_ID, point_light_0.LightPower);	



	LightInfo point_light_1;

	point_light_1.Type = 0; //Point
	point_light_1.LightPosition_worldspace = InterpolatedPoints[InterpolatedPoints.size() - 1];
	point_light_1.LightColor = glm::vec3(1, 0, 0);
	point_light_1.LightPower = 0.3f;

	point_light_1.Type_ID = glGetUniformLocation(m_pProgram->GetID(), "Lights[1].Type");
	glUniform1i(point_light_1.Type_ID, point_light_1.Type);

	point_light_1.LightPosition_worldspace_ID = glGetUniformLocation(m_pProgram->GetID(), "Lights[1].LightPosition_worldspace");
	glUniform3fv(point_light_1.LightPosition_worldspace_ID, 1, &point_light_1.LightPosition_worldspace[0]);

	point_light_1.LightColor_ID = glGetUniformLocation(m_pProgram->GetID(), "Lights[1].LightColor");
	glUniform3fv(point_light_1.LightColor_ID, 1, &point_light_1.LightColor[0]);

	point_light_1.LightPower_ID = glGetUniformLocation(m_pProgram->GetID(), "Lights[1].LightPower");
	glUniform1f(point_light_1.LightPower_ID, point_light_1.LightPower);



	LightInfo spot_light_2;

	spot_light_2.Type = 1; // Spot light
	spot_light_2.LightPosition_worldspace = position;
	spot_light_2.LightPosition_worldspace.y /= 2;
	spot_light_2.SpotLightDirection_worldspace = direction;
	spot_light_2.LightColor = glm::vec3(0.355f, 0.247f, 0.141f);
	spot_light_2.LightPower = 0.5f;
	spot_light_2.CutOff = cos(glm::radians(12.5f));
	spot_light_2.OuterCutOff = cos(glm::radians(15.0f));
	spot_light_2.dynamic = true;

	spot_light_2.Type_ID = glGetUniformLocation(m_pProgram->GetID(), "Lights[2].Type");
	glUniform1i(spot_light_2.Type_ID, spot_light_2.Type);

	spot_light_2.LightPosition_worldspace_ID = glGetUniformLocation(m_pProgram->GetID(), "Lights[2].LightPosition_worldspace");
	glUniform3fv(spot_light_2.LightPosition_worldspace_ID, 1, &spot_light_2.LightPosition_worldspace[0]);

	spot_light_2.SpotLightDirection_worldspace_ID = glGetUniformLocation(m_pProgram->GetID(), "Lights[2].SpotLightDirection_worldspace");
	glUniform3fv(spot_light_2.SpotLightDirection_worldspace_ID, 1, &spot_light_2.SpotLightDirection_worldspace[0]);

	spot_light_2.LightColor_ID = glGetUniformLocation(m_pProgram->GetID(), "Lights[2].LightColor");
	glUniform3fv(spot_light_2.LightColor_ID, 1, &spot_light_2.LightColor[0]);

	spot_light_2.LightPower_ID = glGetUniformLocation(m_pProgram->GetID(), "Lights[2].LightPower");
	glUniform1f(spot_light_2.LightPower_ID, spot_light_2.LightPower);

	spot_light_2.CutOff_ID = glGetUniformLocation(m_pProgram->GetID(), "Lights[2].CutOff");
	glUniform1f(spot_light_2.CutOff_ID, spot_light_2.CutOff);

	spot_light_2.OuterCutOff_ID = glGetUniformLocation(m_pProgram->GetID(), "Lights[2].OuterCutOff");
	glUniform1f(spot_light_2.OuterCutOff_ID, spot_light_2.OuterCutOff);


	m_Lights.push_back(point_light_0);
	m_Lights.push_back(point_light_1);
	m_Lights.push_back(spot_light_2);
}


void GLRenderer::StartSpline() {

	UserPoints.clear();
	InterpolatedPoints.clear();

	int num_of_points = 0;
	std::string input = "";

	while (num_of_points <= 3) {
		std::cout << "Enter the number of points for the Berzier spline: \n";
		try {
			std::cin >> input;
			num_of_points = std::stoi(input);

			if (num_of_points <= 3) {
				std::cout << "Number must be greater than 3!\n";
			}
		}
		catch (...) {
			std::cout << "Wrong input!\n";
			num_of_points = 0;
			input = "";
		}
	}

	NUM_OF_POINTS = num_of_points;
}


void GLRenderer::PlaceRails()
{
	m_Models.push_back(new Model("./models/dice/D6.obj", "ground"));
	m_Models.back()->Scale(glm::vec3(10, 1, 10));
	m_Models.back()->Translate(glm::vec3(0, -10.00, 0));

	Model copy_this = Model("./models/dice/D6.obj", "railing");

	glm::vec3 previous_rail;
	glm::vec3 tangent;
	float angle = 0;
	glm::vec3 rot_axis = glm::vec3(0,1,0);

	//remove duplicates
	InterpolatedPoints.erase(std::unique(InterpolatedPoints.begin(), InterpolatedPoints.end()), InterpolatedPoints.end());


	for (int i = 0; i < InterpolatedPoints.size(); i++) {

		InterpolatedPoints[i] *= RAIL_SPACING;
		glm::vec3 rail_position = InterpolatedPoints[i];

		if (i != 0) {
			tangent = rail_position - previous_rail;
			tangent = glm::normalize(tangent);

			angle = glm::dot(tangent, glm::vec3(1,0,0));
			rot_axis = glm::cross(tangent, glm::vec3(1, 0, 0));

			Tangents.push_back(tangent);
		}		
		previous_rail = rail_position;	
		

		m_Models.push_back(new Model(&copy_this));
		m_Models.back()->Translate(rail_position);
		m_Models.back()->Rotate(angle, rot_axis);
		m_Models.back()->Scale(RAILING_SCALE);
				
	}

	StartTrain();
}

void GLRenderer::StartTrain() {

	position = InterpolatedPoints[0];
	position.y += 0.025f;

	direction = Tangents[0];

}

void GLRenderer::UpdateTrainPosition(GLFWwindow* win) {

	Sleep(150);

	if (!pause) {
		++pos;

		if (pos == Tangents.size()) {
			pos = 0;
		}

		if (Tangents[pos].x == 0 && Tangents[pos].y == 0 && Tangents[pos].z == 0) {
			++pos;
		}

		position = InterpolatedPoints[pos];
		position.y += 0.025f;
		direction = Tangents[pos];

		m_Lights[2].LightPosition_worldspace = position;
		m_Lights[2].LightPosition_worldspace.y /= 2;
		m_Lights[2].SpotLightDirection_worldspace = direction;

		glUniform3fv(m_Lights[2].LightPosition_worldspace_ID, 1, &m_Lights[2].LightPosition_worldspace[0]);
		glUniform3fv(m_Lights[2].SpotLightDirection_worldspace_ID, 1, &m_Lights[2].SpotLightDirection_worldspace[0]);
	}

	Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.01f, 10000.0f);
	View = glm::lookAt(position, position + direction, up);
}


void GLRenderer::DrawTrainScene(GLFWwindow* win) {

	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	UpdateTrainPosition(win);

	// Draw Rails
	for (int i = 0; i < m_Models.size(); i++) {

		for (int j = 0; j < m_Models[i]->meshes.size(); j++) {

			glUniformMatrix4fv(M_MatrixID, 1, GL_FALSE, &m_Models[i]->meshes[j].ModelMat[0][0]);

			glUniformMatrix4fv(V_MatrixID, 1, GL_FALSE, &View[0][0]);

			MVP = Projection * View * m_Models[i]->meshes[j].ModelMat;
			glUniformMatrix4fv(MVP_MatrixID, 1, GL_FALSE, &MVP[0][0]);
			m_Models[i]->meshes[j].Draw(win);
		}
	}

	HandleUserInputTrain(win);

	// Swap buffers
	glfwSwapBuffers(win);
}

void GLRenderer::HandleUserInputTrain(GLFWwindow* win){
	// Static -> Called only first time
	static double last_time_clicked = 1;

	// Time since last click
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - last_time_clicked);


	if (deltaTime > 0.2) {

		if (glfwGetKey(win, GLFW_KEY_BACKSPACE) == GLFW_PRESS) {

			pause = true;
			pos = 0;
			StartTrain();

			m_Lights[2].LightPosition_worldspace = position;
			m_Lights[2].LightPosition_worldspace.y /= 2;
			m_Lights[2].SpotLightDirection_worldspace = direction;

			glUniform3fv(m_Lights[2].LightPosition_worldspace_ID, 1, &m_Lights[2].LightPosition_worldspace[0]);
			glUniform3fv(m_Lights[2].SpotLightDirection_worldspace_ID, 1, &m_Lights[2].SpotLightDirection_worldspace[0]);

			last_time_clicked = currentTime;
		}

		if (glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS) {

			mciSendString(L"play choo from 0", NULL, 0, NULL);
			pause = false;
			last_time_clicked = currentTime;
		}


		if (glfwGetKey(win, GLFW_KEY_ENTER) == GLFW_PRESS) {


			if (m_Lights[2].LightPower == 0.0f) {
				m_Lights[2].LightPower = 0.5f;
			}
			else {
				m_Lights[2].LightPower = 0.0f;
			}

			glUniform1f(m_Lights[2].LightPower_ID, m_Lights[2].LightPower);

			last_time_clicked = currentTime;
		}

		if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS){
			position += direction * deltaTime * 1.0f;
		}
		// Move backward
		if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS){
			position -= direction * deltaTime * 1.0f;
		}

	}
}

void GLRenderer::HandleUserInputSpline(GLFWwindow* win){
	int width, height;
	glfwGetWindowSize(win, &width, &height);

	// Mouse pos
	double xpos, ypos;
	glfwGetCursorPos(win, &xpos, &ypos);

	// Static -> Called only first time
	static double last_time_spawned = 1;

	// Time since last click
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - last_time_spawned);

	if (deltaTime > 0.15) {

		if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && UserPoints.size() < NUM_OF_POINTS) {
			AddNewPoint(win, (float)xpos, (float)ypos);
			last_time_spawned = currentTime;
		}

		if (glfwGetKey(win, GLFW_KEY_L) == GLFW_PRESS) {
			DRAW_MODE = GL_LINE_STRIP;
			last_time_spawned = currentTime;
		}

		if (glfwGetKey(win, GLFW_KEY_P) == GLFW_PRESS) {
			DRAW_MODE = GL_POINTS;
			last_time_spawned = currentTime;
		}

		if (glfwGetKey(win, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
			DRAW_MODE = GL_LINES;
			last_time_spawned = currentTime;
		}

		if (glfwGetKey(win, GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
			std::cout << "\n";
			TrainReady = false;
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glfwSwapBuffers(win);
			StartSpline();
			last_time_spawned = currentTime;
		}

		if (glfwGetKey(win, GLFW_KEY_ENTER) == GLFW_PRESS && UserPoints.size() >= 4) {
			InterpolatedPoints.clear();
			InterpolatePoints(0);
			TrainReady = true;
			last_time_spawned = currentTime;
		}

		if (glfwGetKey(win, GLFW_KEY_F5) == GLFW_PRESS && TrainReady) {

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glfwSwapBuffers(win);

			PrepareTrainScene();

			last_time_spawned = currentTime;
		}
	}
}

void GLRenderer::Draw(GLFWwindow* win) {
	if (TrainReady && TrainPrepared) {
		DrawTrainScene(win);
	}
	else {
		DrawSplineScene(win);
	}
}


void GLRenderer::DrawSplineScene(GLFWwindow* win)
{
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	HandleUserInputSpline(win);

	glBindVertexArray(VAO[0]);
	glDrawArrays(GL_POINTS, 0, UserPoints.size());

	glBindVertexArray(VAO[1]);
	glDrawArrays(DRAW_MODE, 0, InterpolatedPoints.size());

	// Swap buffers
	glfwSwapBuffers(win);
}


void GLRenderer::InterpolatePoints(int last_index) {

	if (last_index < UserPoints.size() - 3) {

		RecursiveSubdivision(0, 1, INTERPOLATION_ACCURACY, last_index);

		InterpolatePoints(last_index + 3);
	}
}


void GLRenderer::RecursiveSubdivision(float u0, float u1, float max_line_length, int last_index) {
	float umid = (u0 + u1) / 2;

	glm::vec3 bezier_point1;
	glm::vec3 bezier_point2;

	bezier(bezier_point1, UserPoints[last_index], UserPoints[last_index + 1], UserPoints[last_index + 2], UserPoints[last_index + 3], u0);
	bezier(bezier_point2, UserPoints[last_index], UserPoints[last_index + 1], UserPoints[last_index + 2], UserPoints[last_index + 3], u1);

	if (glm::distance(bezier_point1, bezier_point2) >= max_line_length) {

		RecursiveSubdivision(u0, umid, max_line_length, last_index);
		RecursiveSubdivision(umid, u1, max_line_length, last_index);
	}
	else {
		AddNewInterpolatedPoint(bezier_point1);
		AddNewInterpolatedPoint(bezier_point2);
	}
}


// Modified from http://www.cubic.org/docs/bezier.htm
// Simple linear interpolation between two points
void GLRenderer::linearly_interpolate(glm::vec3& dest, const glm::vec3& a, const glm::vec3& b, const float t) {

	dest.x = a.x + (b.x - a.x)*t;
	dest.y = a.y + (b.y - a.y)*t;
	dest.z = a.z + (b.z - a.z)*t;
}

// Evaluate a point on a bezier-curve using DeCasteljau Subdivision
// t: from 0 to 1.0f
void GLRenderer::bezier(glm::vec3 &dest, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d, const float t) {

	glm::vec3 ab;
	linearly_interpolate(ab, a, b, t);           // point between a and b

	glm::vec3 bc;
	linearly_interpolate(bc, b, c, t);           // point between b and c

	glm::vec3 cd;
	linearly_interpolate(cd, c, d, t);           // point between c and d

	glm::vec3 abbc;
	linearly_interpolate(abbc, ab, bc, t);       // point between ab and bc

	glm::vec3 bccd;
	linearly_interpolate(bccd, bc, cd, t);       // point between bc and cd

	linearly_interpolate(dest, abbc, bccd, t);   // point on the bezier-curve
}

void GLRenderer::AddNewPoint(GLFWwindow * win, float xpos, float ypos) {
	int width, height;
	glfwGetWindowSize(win, &width, &height);

	UserPoints.push_back(glm::vec3((2 * xpos) / width - 1, -((2 * ypos) / height - 1), 0));

	std::cout << "Point Pressed: (" << xpos << ", " << ypos << "), Points remaining: " << NUM_OF_POINTS - UserPoints.size() << "\n";

	glGenVertexArrays(1, &VAO[0]);
	glGenBuffers(1, &VBO[0]);

	glBindVertexArray(VAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, UserPoints.size() * sizeof(glm::vec3), &UserPoints[0], GL_DYNAMIC_DRAW);

	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
}


void GLRenderer::AddNewInterpolatedPoint(glm::vec3 pt) {

	InterpolatedPoints.push_back(pt);

	glGenVertexArrays(1, &VAO[1]);
	glGenBuffers(1, &VBO[1]);

	glBindVertexArray(VAO[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, InterpolatedPoints.size() * sizeof(glm::vec3), &InterpolatedPoints[0], GL_DYNAMIC_DRAW);

	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
}



void GLRenderer::DestroyScene()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	for (int i = 0; i < m_Models.size(); i++) {
		delete m_Models[i];
	}

	if (TrainReady && TrainPrepared) {
		m_pProgram->DetachShader(m_pVertSh);
		m_pProgram->DetachShader(m_pFragSh);

		delete m_pProgram;
		delete m_pVertSh;
		delete m_pFragSh;
	}
}