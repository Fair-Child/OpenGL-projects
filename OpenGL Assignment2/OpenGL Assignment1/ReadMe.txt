COMP 371 - Computer Graphics
Assignment 2

Student Name: 	Jafar Abbas
Student ID: 	26346650


Program Controls:

	Same as assignment description

	F5		  - Start Train mode


Classes:

	GLWindow	- Handles creation of window and creating opengl context
	GLShader	- Handles loading shaders and compiling them
	GLProgram	- Combines shaders into a program and sets them to be used by GPU
	GlRenderer	- Prepares the scene, computes matrices and coordinates drawing
	Model		- Regroups related meshes, loads OBJ files into vertices, coordinates transformations
	Mesh		- Holds vertices, performs transformations, prepares the VAO/VBO, draws itself

Sources used: 

	www.opengl-tutorial.org   - mouse movements
	www.learnopengl.com 	  - classes inspiration and handling indices for mesh loading