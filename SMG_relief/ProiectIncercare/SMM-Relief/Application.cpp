#include "Application.h"

Camera* Application::camera = nullptr;
Shader Application::skyboxShader;
Shader Application::mapShader;

void Application::Run()
{
	if (!InitWindow())
	{
		std::cout << "Couldn't initialize GLFW window !" << std::endl;
		return;
	}

	camera = new Camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 400.0f, 300.0f));

	std::filesystem::path localPath = std::filesystem::current_path();
	std::string resourcesFolder = localPath.string() + "\\Resources";

	Skybox skybox(resourcesFolder, skyboxShader);
	Terrain terrain(resourcesFolder, mapShader);

	Render(skybox, terrain);
}

Application::~Application()
{
	mapShader.Delete();
	skyboxShader.Delete();
	glfwTerminate();

	delete camera;
}

bool Application::InitWindow()
{
	if (!glfwInit())
		return false;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Explorare relief real", NULL, NULL);
	glfwMakeContextCurrent(window);

	glewInit();
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);
	glfwSetKeyCallback(window, KeyCallback);

	glfwSetCursorPos(window, static_cast<double>(SCR_WIDTH) / 2, static_cast<double>(SCR_HEIGHT) / 2);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}

void Application::CheckIfCollisionExists(Terrain& terrain)
{
	glm::vec3 rayOrigin = camera->GetPosition();
	glm::vec3 rayDirection = glm::normalize(glm::vec3(camera->GetViewMatrix() * glm::vec4(0, 0, -1, 0)));

	// Check if the ray intersects any of the triangles in the terrain
	bool collided = false;
	float tMin = FLT_MAX;
	/*GLuint vertexBuffer = terrain.map.VBO;
	GLuint indexBuffer = terrain.map.EBO;
	glm::vec3* vertices = (glm::vec3*)glMapBuffer(vertexBuffer, GL_READ_ONLY);
	unsigned int* indices = (unsigned int*)glMapBuffer(indexBuffer, GL_READ_ONLY);*/
	
	
	
	for (int i = 0; i < terrain.map.vertices.size(); i += 3)
	{
		// Get the vertices of the triangle
		glm::vec3 v0, v1, v2;
		v0 = glm::vec3(terrain.map.vertices[i].Position);
		v1 = glm::vec3(terrain.map.vertices[i+1].Position);
		v2 = glm::vec3(terrain.map.vertices[i+2].Position);

		// Check if the ray intersects the triangle using the triangle-ray intersection algorithm
		float t;
		if (rayTriangleIntersection(rayOrigin, rayDirection, v0, v1, v2, t))
		{
			// There is a collision!
			// Update the collision distance if this triangle is closer
			if (t < tMin)
			{
				tMin = t;
				collided = true;
				break;
			}
		}
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

	if (collided)
	{
		// Move the camera above the terrain
		float cameraX = rayOrigin.x + rayDirection.x * 10 ;
		float cameraY = rayOrigin.y + rayDirection.y * 10;
		float cameraZ = rayOrigin.z + rayDirection.z * 10;
		glm::vec3 lastPos(cameraX, cameraY, cameraZ);
		camera->position = lastPos;
		//camera->Reset(SCR_WIDTH,SCR_HEIGHT);
	}
			
}

bool Application::rayTriangleIntersection(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, float& t)
{
	// Compute the triangle's normal
	glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

	// Compute the distance from the ray origin to the triangle
	float distance = glm::dot(normal, v0 - rayOrigin) / glm::dot(normal, rayDirection);

	// Check if the triangle is behind the ray
	if (distance < 0) return false;

	// Compute the intersection point
	glm::vec3 intersection = rayOrigin + distance * rayDirection;

	// Check if the intersection point is inside the triangle
	glm::vec3 u = v1 - v0;
	glm::vec3 v = v2 - v0;
	glm::vec3 w = intersection - v0;
	float uu = glm::dot(u, u);
	float uv = glm::dot(u, v);
	float vv = glm::dot(v, v);
	float wu = glm::dot(w, u);
	float wv = glm::dot(w, v);
	float denom = uv * uv - uu * vv;
	float s = (uv * wv - vv * wu) / denom;
	float tt = (uv * wu - uu * wv) / denom;
	if (s >= 0 && tt >= 0 && s + tt <= 1)
	{
		// The intersection point is inside the triangle
		  // The ray intersects the triangle
		t = distance;
		return true;
	}

	// The intersection point is outside the triangle
	return false;
}

void Application::Render(Skybox& skybox, Terrain& terrain)
{
	while (!glfwWindowShouldClose(window))
	{
		// Per-frame time logic
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Lighting
		float clearR = 0.07f + skyLight / 2.f - 0.1f;
		float clearG = 0.13f + skyLight / 2.f - 0.1f;
		float clearB = 0.17 + skyLight / 2.f - 0.1f;
		glClearColor(clearR, clearG, clearB, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Checking if camera's position intersects the object
		//CheckIfCollisionExists(terrain);

		// Input
		ProcessInput(window, camera, deltaTime);

		// Render here
		terrain.Render(camera, mapShader);
		skybox.Render(camera, skyboxShader);

		// Swap front and back buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Application::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	camera->Reshape(width, height);
}

void Application::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	camera->MouseControl((float)xpos, (float)ypos);
}

void Application::ScrollCallback(GLFWwindow* window, double xoffset, double yOffset)
{
	camera->ProcessMouseScroll((float)yOffset);
}

void Application::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (tolower(key) == 'i' && lightIntensityValue.x < 0.9f)
	{
		lightIntensityValue += glm::vec3(0.05f);
	}

	if (tolower(key) == 'o' && lightIntensityValue.x > 0.2f)
	{
		lightIntensityValue -= glm::vec3(0.05f);
	}

	skyboxShader.Use();
	skyboxShader.SetVec3("lightColor", lightIntensityValue);
	mapShader.Use();
	mapShader.SetVec3("lightColor", lightIntensityValue);

	skyLight = lightIntensityValue.x;
}


