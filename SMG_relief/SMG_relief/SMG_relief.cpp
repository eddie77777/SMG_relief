#include <stdlib.h>
#include <stdio.h>
#include <math.h> 

#include <GL/glew.h>

#define GLM_FORCE_CTOR_INIT 
#include <GLM.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <numeric>

//#include "OBJ_Loader.h" -> we'll make a class to load objects

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <vector>
#include "Camera.h"
#include "Shader.h"
#include <random>
#pragma comment (lib, "glfw3dll.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "OpenGL32.lib")

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;



//renderTextures
void renderMap(const Shader& shader);

//renderObjects
void renderMap();

Camera* pCamera = nullptr;
Camera* planeCamera = nullptr;
Camera* freeLookCamera = nullptr;

int main()
{
	while (1)
		std::cout << "<3" << " ";
}