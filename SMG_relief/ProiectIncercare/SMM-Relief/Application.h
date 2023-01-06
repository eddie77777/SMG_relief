#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>

#pragma comment (lib, "glfw3dll.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "OpenGL32.lib")

#include "Camera.h"
#include "Skybox.h"
#include "Terrain.h"
#include "ProcessInput.cpp"

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

static double deltaTime = 0.0f;
static double lastFrame = 0.0f;

static glm::vec3 lightIntensityValue(0.6f);
static float skyLight = lightIntensityValue.x;

class Application
{
public:
    Application() = default;
    ~Application();

    void Run();

private:
    bool InitWindow();

    void Render(Skybox& skybox, Terrain& terrain);

    static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
    static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yOffset);
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void CheckIfCollisionExists(Terrain& terrain);

private:
    GLFWwindow* window;
    static Camera* camera;
    bool rayTriangleIntersection(glm::vec3 rayOrigin, glm::vec3 rayDirection,
        glm::vec3 v0, glm::vec3 v1, glm::vec3 v2,
        float& t);

private:
    static Shader skyboxShader;
    static Shader mapShader;
    static Shader rainbowShader;
    Shader signShader;
};