#include "all_headers.h"

GLRenderer::GLRenderer(GLFWwindow * _win){ win = _win; }

// Loads the shaders and programs, initializes opengl params
// Mesh and Terrain Setup themselves
void GLRenderer::PrepareScene()
{
	// Grey background
	glClearColor(0.170f, 0.170f, 0.200f, 0.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Default wireframe

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

	status += (int) m_pVertSh->Load("minimal.vert");
	status += (int) m_pFragSh->Load("minimal.frag");

	status += (int) m_pVertSh->Compile();
	status += (int) m_pFragSh->Compile();

	m_pProgram->AttachShader(m_pVertSh);
	m_pProgram->AttachShader(m_pFragSh);

	status += (int) m_pProgram->Link();

	// Get a handle for the "MVP" uniform
	MatrixID = glGetUniformLocation(m_pProgram->GetID(), "MVP");
	M_MatrixID = glGetUniformLocation(m_pProgram->GetID(), "M");
	V_MatrixID = glGetUniformLocation(m_pProgram->GetID(), "V");


	if (status != 5 ) {
		std::cout << "Error during loading/compiling/linking of shaders!!\n";
	}

	/*^^^^^^^^^^^^^^^^^^ Repeat above for terrain shaders ^^^^^^^^^^^^^^^*/
	// Terrain shaders
	m_pProgram_terrain = new GLProgram();
	m_pVertSh_terrain = new GLShader(GL_VERTEX_SHADER);
	m_pFragSh_terrain = new GLShader(GL_FRAGMENT_SHADER);

	status = 0;

	status += (int)m_pVertSh_terrain->Load("terrain.vert");
	status += (int)m_pFragSh_terrain->Load("terrain.frag");

	status += (int)m_pVertSh_terrain->Compile();
	status += (int)m_pFragSh_terrain->Compile();

	m_pProgram_terrain->AttachShader(m_pVertSh_terrain);
	m_pProgram_terrain->AttachShader(m_pFragSh_terrain);

	status += (int)m_pProgram_terrain->Link();

	// Get a handle for the "MVP" uniform
	MatrixID_terrain = glGetUniformLocation(m_pProgram_terrain->GetID(), "MVP");

	if (status != 5) {
		std::cout << "Error during loading/compiling/linking of shaders!!\n";
	}

	SetData();
}


// Used to properly spawn models in the world
// Model constructor creates and setups the meshes & textures
void GLRenderer::AddModelFromSaved(GLchar* path, std::string name, bool centered) {
	int index = -1;
	bool data_found = false;
	bool can_copy = false;

	for (int i = 0; i < model_data.size(); i++) {
		if (model_data[i].name == name) {

			if (model_data[i].instanced == false) {
				index = i;
				data_found = true;
				break;
			}
			else {
				can_copy = true;
			}
		}
	}

	if (can_copy) {

		for (int j = 0; j < m_Models.size(); j++) {
			if (m_Models[j]->name == name) {
				CopyModel(m_Models[j]);
				break;
			}
		}
	}
	else {
		m_Models.push_back(new Model(path, name, centered));
	}

	if (data_found) {
		model_data[index].instanced = true;

		m_Models.back()->Transform(model_data[index].transformation);
		m_Models.back()->centered = model_data[index].centered;

		// Hacky, only needed for ground, won't be needed with terrain
		m_Models.back()->scale = glm::vec3(model_data[index].transformation[0].x, model_data[index].transformation[1].y, model_data[index].transformation[2].z);
	}

}



// Fills the ModelData array and loads all the models defined in "Object Matrices.txt"
// Add custom models here, or in the above file
void GLRenderer::SetData()
{
	ReadModelMatrices();

	m_Terrain.SetupTerrain(); // Prepare Terrain

	m_Models.push_back(new Model("./models/flyertug/FlyerTug(obj).obj", "first_person"));	// First person model
	m_Models.back()->scale = glm::vec3(0.35, 0.35, 0.35);
	
	for (int i = 0; i < model_data.size(); i++) {
		AddModelFromSaved(&model_data[i].path[0], model_data[i].name, model_data[i].centered);
	}
	/*
	AddModelFromSaved("./models/dice/D6.obj", "ground");	// Ground box

	AddModelFromSaved("./models/words/boing.obj", "boing");
	AddModelFromSaved("./models/teapot.obj", "center");
	AddModelFromSaved("./models/Tree/Tree.obj", "tree");
	AddModelFromSaved("./models/truck/truck.obj", "truck");

	AddModelFromSaved("./models/dice/D6.obj", "dice");
	AddModelFromSaved("./models/garbage/Garbage_Bin.obj", "garbage");
	AddModelFromSaved("./models/horse/horse-obj.obj", "horse");
	AddModelFromSaved("./models/nanosuit/nanosuit.obj", "nanosuit");
	AddModelFromSaved("./models/Tree/Tree.obj", "tree");

	AddModelFromSaved("./models/abandoned1/abandoned1.obj", "abandoned1");
	AddModelFromSaved("./models/building3/building3.obj", "building3");
	AddModelFromSaved("./models/building2/building2.obj", "building2");
	AddModelFromSaved("./models/building6/building6.obj", "building6");
	AddModelFromSaved("./models/building10/building10.obj", "building10");
	AddModelFromSaved("./models/building11/building11.obj", "building11");
	AddModelFromSaved("./models/skyscraper4/skyscraper4.obj", "skyscraper4");
	AddModelFromSaved("./models/destroyed_house1/old_house1.obj", "destroyed_house1");

	AddModelFromSaved("./models/Dobby/Dobby.obj", "Dobby");
	AddModelFromSaved("./models/road1/road1.obj", "road1");
	AddModelFromSaved("./models/road3/road3.obj", "road3");
	AddModelFromSaved("./models/street_cones1/street_cones1.obj", "street_cones1");*/

	// Big i.e. slow ones:
	//m_Models.push_back(new Model("./models/Fantasy Colony/Fantasy Colony.obj"));
	//m_Models.push_back(new Model("./models/habitat/Habitat.obj"));
	//m_Models.push_back(new Model("./models/miami/Miami 2525.obj"));
}


// Main part of game loop.
// Meshes & Terrain draws themselves
void GLRenderer::DrawScene()
{
	std::async(&GLRenderer::GroundDetection, this).get();
	std::async(&GLRenderer::CollisionDetection, this);	// In Parallel
	std::async(&GLRenderer::RayTracing, this);			// In Parallel

	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (glfwWindowShouldClose(win)) return;

	UpdateMatricesFromInputs();		// Update camera position and view / projection matrices
	HandleModelManipulation();

	// Draw Terrain
	m_pProgram_terrain->Use();
	MVP = Projection * View;
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	m_Terrain.Draw(win);

	// Draw Models
	m_pProgram->Use();

	for (int i = 0; i < m_Models.size(); i++) {

		for (int j = 0; j < m_Models[i]->meshes.size(); j++) {

			glUniformMatrix4fv(M_MatrixID, 1, GL_FALSE, &m_Models[i]->meshes[j].ModelMat[0][0]);

			glUniformMatrix4fv(V_MatrixID, 1, GL_FALSE, &View[0][0]);

			MVP = Projection * View * m_Models[i]->meshes[j].ModelMat;
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
			m_Models[i]->meshes[j].Draw(win);
		}
	}

	// Swap buffers
	glfwSwapBuffers(win);

	HandleSpawning();
}


// Basic bounding-box detection
// TODO: Based on depth map of terrain
void GLRenderer::GroundDetection() {
	glm::vec3 currPosition = position + 50.0f * direction;
	currPosition = currPosition + 50.0f * - up;

	for (int j = 0; j < m_Models.size(); j++) {
		if (glfwWindowShouldClose(win)) { return; }
		if (m_Models[j]->name == "ground") {
			float ground_plane = m_Models[j]->meshes[0].ModelMat[3].y + m_Models[j]->scale.y * m_Models[j]->meshes[0].max.y; // Hacky.. won't be needed when we have terrain. (only used for ground)
			if (currPosition.y <= ground_plane) {
				position.y += speed / 5;
			}
			return;
		}
	}
	return;
}


// Very basic sphere-based ray intersection
void GLRenderer::RayTracing() {
	bool intersected = false;
	glm::vec3 intersection_position;
	glm::vec3 intersection_normal;
	float dist = 0.0f;

	for (int j = 0; j < m_Models.size(); j++) {
		if (glfwWindowShouldClose(win)) { return; }
		if (m_Models[j]->name == "first_person") continue;
		if (m_Models[j]->name == "ground") continue;

		for (int k = 0; k < m_Models[j]->meshes.size(); k++) {
			Mesh * mesh = &m_Models[j]->meshes[k];

			glm::vec3 mesh_pos(mesh->ModelMat * mesh->centroid);
			glm::vec3 max(mesh->ModelMat * mesh->max);
			max -= mesh_pos;
			float radius = sqrt(pow(max.x, 2) + pow(max.y, 2) + pow(max.z, 2));
			glm::vec3 pos;
			glm::vec3 norm;
			float new_dist;

			if (glm::intersectRaySphere(position, direction, mesh_pos, radius, pos, norm)) {
				new_dist = sqrt(pow(pos.x - position.x, 2) + pow(pos.y - position.y, 2) + pow(pos.z - position.z, 2));

				if (dist == 0.0f || new_dist < dist) {
					// this one is closer than old one
					dist = new_dist;
					intersection_position = pos;
					intersected = true;
					selected = m_Models[j];
				}
			}
			
		}
	}

	system("cls");
	if (intersected) {
		std::cout << "RAY CROSSED: " << selected->name << "\n";
		
	}
	else {
		selected = &null_model;
	}

	return;
}



// Very basic box-based
bool GLRenderer::CollisionDetection() {
	glm::vec3 currPosition = position + 35.0f * direction;
	bool hit = false;

	for (int j = 0; j < m_Models.size(); j++) {
		if (glfwWindowShouldClose(win)) { return false; }
		if (m_Models[j]->name == "first_person") continue;
		if (m_Models[j]->name == "ground") continue;

		for (int k = 0; k < m_Models[j]->meshes.size(); k++) {
			Mesh * mesh = &m_Models[j]->meshes[k];

			glm::vec3 max(mesh->ModelMat * mesh->max);
			glm::vec3 min(mesh->ModelMat * mesh->min);

			if (currPosition.x <= max.x && currPosition.x >= min.x || currPosition.x >= max.x && currPosition.x <= min.x) {
				if (currPosition.y <= max.y && currPosition.y >= min.y || currPosition.y >= max.y && currPosition.y <= min.y) {
					if (currPosition.z <= max.z && currPosition.z >= min.z || currPosition.z >= max.z && currPosition.z <= min.z) {
						system("cls");
						std::cout << "PENETRATED: " << mesh->parent->name << "\n";

						glm::vec3 mesh_pos(mesh->ModelMat * mesh->centroid);

						glm::vec3 push_back = currPosition - mesh_pos - direction;

						push_back = glm::normalize(push_back);
						push_back *= speed / 10;

						position += push_back;
						
						hit = true;
					}
				}
			}
		}
	}
	return hit;
}


void GLRenderer::Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}


void GLRenderer::HandleModelManipulation() {

	OutputModelMatrices();

	if (selected->name != "ground") {
		if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
			ModelTranslate(direction * 5.0f, selected);
		}

		if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
			ModelTranslate(-direction * 5.0f, selected);
		}

		if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_5) == GLFW_PRESS){
			ModelScale(glm::vec3(1.01f, 1.01f, 1.01f), selected);
		}

		if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_4) == GLFW_PRESS){
			ModelScale(glm::vec3(0.99f, 0.99f, 0.99f), selected);
		}

	}
}

// TODO: make it procedural, based on current position and based on file/algorithm
void GLRenderer::HandleSpawning() {

	if (glfwGetKey(win, GLFW_KEY_P) == GLFW_PRESS) {
		// Static -> Called only first time
		static double last_time_spawned = 3;

		// Time since last frame
		double currentTime = glfwGetTime();
		float deltaTime = float(currentTime - last_time_spawned);

		if (deltaTime > 1) {
			CopyModelAtMyLocation(selected);
		}
		last_time_spawned = currentTime;
	}
}


// TODO: move to somewhere around me, based on some algorithm -> note: avoid stacking
void GLRenderer::CopyModelAtMyLocation(Model* copy_this) {
	m_Models.push_back(new Model(copy_this));

	m_Models.back()->Translate(position);
}

Model* GLRenderer::CopyModel(Model* copy_this) {
	m_Models.push_back(new Model(copy_this));

	return m_Models.back();
}

// Source: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-6-keyboard-and-mouse/
void GLRenderer::UpdateMatricesFromInputs(){
	int width, height;
	glfwGetWindowSize(win, &width, &height);

	// Static -> Called only first time
	static double lastTime = glfwGetTime();

	// Time since last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Mouse pos
	double xpos, ypos;
	glfwGetCursorPos(win, &xpos, &ypos);

	// Convert movement into yaw / pitch
	horizontalAngle += mouseSpeed * float(width / 2 - xpos);
	verticalAngle += mouseSpeed * float(height / 2 - ypos);

	// Direction (Spherical to Cartesian), trickiest part...
	direction = glm::vec3(
					sin(horizontalAngle) * cos(verticalAngle),
					sin(verticalAngle),
					cos(verticalAngle) * cos(horizontalAngle)
					);

	// Right vector
	right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
		);

	// Up vector
	up = glm::cross(right, direction);


	// Movement with Mouse
	// Move forward
	if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS){
		position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS){
		position -= direction * deltaTime * speed;
	}

	// Strafe right
	if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS){
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS){
		position -= right * deltaTime * speed;
	}
	// Strafe up
	if (glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS){
		position += up * deltaTime * speed;
	}

	// Resize window with Page Up / Down
	if (glfwGetKey(win, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS){
		glfwSetWindowSize(win, width - width/100, height - height/100);
		Reshape(width - width / 100, height - height / 100);
	}

	if (glfwGetKey(win, GLFW_KEY_PAGE_UP) == GLFW_PRESS){
		glfwSetWindowSize(win, width + width / 100, height + height / 100);
		Reshape(width + width / 100, height + height / 100);
	}

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	Projection = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100000.0f);

	// Camera matrix
	View = glm::lookAt(position, position + direction, up);	

	for (int i = 0; i < m_Models.size(); i++) {
		if (m_Models[i]->name == "first_person") {
			Model* first_person = m_Models[i];
			glm::vec3 scale = first_person->scale;

			ModelReset(first_person);

			ModelTranslate(position, first_person);

			ModelRotate(horizontalAngle - 3.14f * 2, up, first_person);
			ModelRotate(verticalAngle, right, first_person);

			ModelTranslate(glm::vec3(3, -20, 35), first_person);

			ModelRotate(0.1f, right, first_person);
			ModelRotate(0.05f, up, first_person);

			ModelScale(scale, first_person);

		}
	}
	// Reset mouse position for next frame
	glfwSetCursorPos(win, width / 2, height / 2);

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}


// Detach shader programs, delete the pointers
// Sleep to let the asynchronous functions finish
void GLRenderer::DestroyScene()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	m_pProgram->DetachShader(m_pVertSh);
	m_pProgram->DetachShader(m_pFragSh);

	m_pProgram_terrain->DetachShader(m_pVertSh_terrain);
	m_pProgram_terrain->DetachShader(m_pFragSh_terrain);

	Sleep(3000);

	for (int i = 0; i < m_Models.size(); i++) {
		delete m_Models[i];
	}
	delete m_pProgram;
	delete m_pVertSh;
	delete m_pFragSh;

	delete m_pProgram_terrain;
	delete m_pVertSh_terrain;
	delete m_pFragSh_terrain;
}


// Based on specific model
void GLRenderer::ModelCenter(Model* update_this) {
	update_this->Center();
}

void GLRenderer::ModelTranslate(glm::vec3 change, Model* update_this) {
	update_this->Translate(change);
}

void GLRenderer::ModelScale(glm::vec3 change, Model* update_this) {
	update_this->Scale(change);
}

void GLRenderer::ModelRotate(float angle, glm::vec3 vector, Model* update_this){
	update_this->Rotate(angle * 1, vector);
}

void GLRenderer::ModelReset(Model* update_this){
	update_this->Reset();
}

void GLRenderer::ModelTransform(glm::mat4 tm, Model* update_this) {
	update_this->Transform(tm);
}

// Based on name
void GLRenderer::ModelCenter(std::string name) {
	for (int i = 0; i < m_Models.size(); i++) {
		if (m_Models[i]->name == name) {
			m_Models[i]->Center();
		}
	}
}

void GLRenderer::ModelTranslate(glm::vec3 change, std::string name) {
	for (int i = 0; i < m_Models.size(); i++) {
		if (m_Models[i]->name == name) {
			m_Models[i]->Translate(change);
		}
	}
}

void GLRenderer::ModelScale(glm::vec3 change, std::string name) {
	for (int i = 0; i < m_Models.size(); i++) {
		if (m_Models[i]->name == name) {
			m_Models[i]->Scale(change);
		}
	}
}

void GLRenderer::ModelRotate(float angle, glm::vec3 vector, std::string name){
	for (int i = 0; i < m_Models.size(); i++) {
		if (m_Models[i]->name == name) {
			m_Models[i]->Rotate(angle * 1, vector);
		}
	}
}

void GLRenderer::ModelReset(std::string name) {
	for (int i = 0; i < m_Models.size(); i++) {
		if (m_Models[i]->name == name) {
			m_Models[i]->Reset();
		}
	}
}

void GLRenderer::ModelTransform(glm::mat4 tm, std::string name) {
	for (int i = 0; i < m_Models.size(); i++) {
		if (m_Models[i]->name == name) {
			m_Models[i]->Transform(tm);
		}
	}
}

// Writes all the existing model's Current Transformation Matrix into "Object Matrices.txt"
void GLRenderer::OutputModelMatrices() {
	if (glfwGetKey(win, GLFW_KEY_O) == GLFW_PRESS) {

		std::ofstream File("Object Matrices.txt");
		for (int i = 0; i < m_Models.size(); i++) {

			Model* model = m_Models[i];
			if (model->name == "first_person") continue;

			if (model->meshes.size() > 0) {				

				glm::mat4 matrix = model->meshes[0].ModelMat;
				std::string is_centered = "true";
				if (!model->centered) is_centered = "false";
				File << model->name << "\t[" << model->path << "]\t[" << model->mID << "]\t{" << is_centered << "}\n" << glm::to_string(matrix) << "\n\n";
			}
		}
	}
}

// Read "Object Matrices.txt" and fills the ModelData array, which holds transformation for a model
void GLRenderer::ReadModelMatrices() {

	std::ifstream File("Object Matrices.txt");
	std::string Data;

	glm::mat4 transformation;
	ModelData model_info;

	if (!File.is_open()) return;

	while (!File.eof()) {
		// Get name, path, index and centered
		getline(File, Data);
		if (Data.empty()) continue;

		int sep0 = Data.find_first_of('\t');

		std::string name = Data.substr(0, sep0);

		sep0 = Data.find_first_of('[');
		int sep1 = Data.find_first_of(']');
		std::string path = Data.substr(sep0 + 1, sep1 - sep0 - 1);

		sep0 = Data.rfind('[');
		sep1 = Data.rfind(']');
		int id = std::stoi(Data.substr(sep0 + 1, sep1 - sep0 - 1));

		sep0 = Data.rfind('{');
		sep1 = Data.rfind('}');

		std::string cent = Data.substr(sep0 + 1, sep1 - sep0 - 1);
		bool is_centered = true;
		if (cent == "false") is_centered = false;

		// Get transformation matrix
		getline(File, Data);

		float temp_float;
		sep0 = Data.find_first_of('((');
		sep1 = Data.rfind('))');

		Data = Data.substr(sep0 + 1, sep1 - sep0 - 1);

		glm::vec4 temp_vec;
		std::string temp_string;

		for (int i = 0; i < 4; i++) {

			sep0 = Data.find_first_of('(');
			sep1 = Data.find_first_of(',');

			temp_string = Data.substr(sep0 + 1, sep1 - sep0 - 1);
			temp_float = std::stof(temp_string);
			temp_vec.x = temp_float;

			Data = Data.substr(sep1 + 2, -1);

			sep0 = 0;
			sep1 = Data.find_first_of(',');
			temp_string = Data.substr(sep0, sep1 - sep0 - 1);
			temp_float = std::stof(temp_string);
			temp_vec.y = temp_float;

			Data = Data.substr(sep1 + 2, -1);

			sep0 = 0;
			sep1 = Data.find_first_of(',');
			temp_string = Data.substr(sep0, sep1 - sep0 - 1);
			temp_float = std::stof(temp_string);
			temp_vec.z = temp_float;

			Data = Data.substr(sep1 + 2, -1);

			sep0 = 0;
			sep1 = Data.find_first_of(')');
			temp_string = Data.substr(sep0, sep1 - sep0 - 1);
			temp_float = std::stof(temp_string);
			temp_vec.w = temp_float;

			if (i < 3) {
				Data = Data.substr(sep1 + 2, -1);
			}

			transformation[i] = temp_vec;
		}

		// initialize model data struct
		model_info.id = id;
		model_info.path = path;
		model_info.name = name;
		model_info.transformation = transformation;
		model_info.centered = is_centered;
		model_info.instanced = false;

		model_data.push_back(model_info);
	}
}