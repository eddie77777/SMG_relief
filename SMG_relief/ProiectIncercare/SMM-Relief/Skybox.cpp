#include "Skybox.h"

Skybox::Skybox(const std::string& resourcesFolder, Shader& skyboxShader)
{
	this->resourcesFolder = resourcesFolder;

	SetSkyboxFaces();
	InitSkybox(skyboxShader);

	skyboxShader.Use();
	skyboxShader.SetVec3("lightColor", glm::vec3(0.6f, 0.6f, 0.6f));
}

void Skybox::Render(Camera* pCamera, Shader& skyboxShader)
{
	glDepthFunc(GL_LEQUAL);
	skyboxShader.Use();

	glm::mat4 viewMatrix = glm::mat4(glm::mat3(pCamera->GetViewMatrix()));
	glm::mat4 projMatrix = pCamera->GetProjectionMatrix();

	skyboxShader.SetMat4("view", viewMatrix);
	skyboxShader.SetMat4("projection", projMatrix);

	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void Skybox::SetSkyboxFaces()
{
	skyboxFaces =
	{
		resourcesFolder + "\\SkyboxTexture\\Left.jpg.png",
		resourcesFolder + "\\SkyboxTexture\\Right.jpg.png",
		resourcesFolder + "\\SkyboxTexture\\Up.jpg.png",
		resourcesFolder + "\\SkyboxTexture\\Down.jpg.png",
		resourcesFolder + "\\SkyboxTexture\\Front.jpg.png",
		resourcesFolder + "\\SkyboxTexture\\Back.jpg.png"
	};
}

void Skybox::InitSkybox(Shader& skyboxShader)
{
	skyboxShader.Set("Skybox.shader");

	const float skyboxVertices[] =
	{
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	skyboxTexture = CreateSkyboxTexture(skyboxFaces);
	skyboxShader.SetInt("skybox", 0);
}
