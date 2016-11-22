#pragma once

//--- defines
#define _AFXDLL
#define _CRT_SECURE_NO_DEPRECATE
#define GLEW_STATIC

//--- includes
#include <SDKDDKVer.h>
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <iostream>
#include <fstream>
#include <afxwin.h>
#include <vector>

//Sound
#include <Mmsystem.h>
#include <mciapi.h>
#pragma comment(lib, "Winmm.lib")


//--- assimp
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

//--- soil
#include <SOIL.h>


//--- openGL Stuff
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/spline.hpp>

//--- My Classes
#include "Mesh.h"
#include "Model.h"
#include "GLShader.h"
#include "GLProgram.h"
#include "GLRenderer.h"
#include "GLWindow.h"