// OpenGL Assignment1.cpp : Defines the entry point for the console application.
//

#include "all_headers.h"

int _tmain(int argc, _TCHAR* argv[])
{
	// Add breakpoint @ memory leak
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(5476);

	bool statusOkay = false;

	GLWindow myWindowManager;

	statusOkay = myWindowManager.InitializeGLFW();
	statusOkay = myWindowManager.CreateGLFWWindow("OpenGL Window", 1024, 768);
	statusOkay = myWindowManager.InitializeGLEW();

	if (!statusOkay) { return 0; }

	myWindowManager.SetKeyListener();
	
	GLRenderer myRenderer;
	myRenderer.PrepareSplineScene();
	myRenderer.StartSpline();

	do{
		// Clear & Draw
		myRenderer.Draw(myWindowManager.window);

		// Handle events
		glfwPollEvents();

	} while (!glfwWindowShouldClose(myWindowManager.window));

	myRenderer.DestroyScene();
	glfwTerminate();

	system("pause");

	return 0;
}

