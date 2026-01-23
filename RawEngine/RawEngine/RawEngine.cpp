#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "core/mesh.h"
#include "core/assimpLoader.h"
#include "core/texture.h"
#include "Camera.h"
#include "Scene.h"
#include "SceneManager.h"

//#define MAC_CLION
#define VSTUDIO

#ifdef MAC_CLION
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#endif

#ifdef VSTUDIO
// Note: install imgui with:
//     ./vcpkg.exe install imgui[glfw-binding,opengl3-binding]
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#endif

int g_width = 800;
int g_height = 600;

bool shiftPressed = false; // just for debugging

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	shiftPressed = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
}

//input with w and s

void checkMovement(GLFWwindow* window, Camera& cam)
{
	float x, y, z;
	x = 0; // cam.getPosition().x;
	y = 0; // cam.getPosition().y;
	z = 0;// cam.getPosition().z;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		//z -= 0.01f;
		cam.moveForward(-0.01f);
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cam.moveForward(0.01f);
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cam.moveRight(-0.01f);
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cam.moveRight(0.01f);
	}
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cam.moveUp(0.01f);
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) cam.moveUp(-0.01f);

	cam.rotationMouse(window);

	/*if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		cam.rotate(glm::vec3(0, 1, 0), 0.001f);
	}
	else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		cam.rotate(glm::vec3(0, 1, 0), -0.001f);*/
	//}

	//cam.translate(glm::vec3 (x,y,z));
	//if (x != 0 || y != 0 || z != 0) 
	//{
	//    auto cp = cam.getPosition();
	//    printf("Input: Camera is now at (%f,%f,%f)\n", cp.x, cp.y, cp.z);
	//}
	   // printf("Camera is now at (%f,%f,%f)\n", cam.getPosition());
}

void framebufferSizeCallback(GLFWwindow* window,
	int width, int height) {
	g_width = width;
	g_height = height;
	glViewport(0, 0, width, height);
}

std::string readFileToString(const std::string& filePath) {
	std::ifstream fileStream(filePath, std::ios::in);
	if (!fileStream.is_open()) {
		printf("Could not open file: %s\n", filePath.c_str());
		return "";
	}
	std::stringstream buffer;
	buffer << fileStream.rdbuf();
	return buffer.str();
}

GLuint generateShader(const std::string& shaderPath, GLuint shaderType) {
	printf("Loading shader: %s\n", shaderPath.c_str());
	const std::string shaderText = readFileToString(shaderPath);
	const GLuint shader = glCreateShader(shaderType);
	const char* s_str = shaderText.c_str();
	glShaderSource(shader, 1, &s_str, nullptr);
	glCompileShader(shader);
	GLint success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		printf("Error! Shader issue [%s]: %s\n", shaderPath.c_str(), infoLog);
	}
	return shader;
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(g_width, g_height, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize GLAD\n");
		return -1;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	//Setup platforms
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 400");

	glEnable(GL_DEPTH_TEST);

	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	const GLuint modelVertexShader = generateShader("shaders/modelVertex.vs", GL_VERTEX_SHADER);
	const GLuint vertexShader = generateShader("shaders/vertex.vs", GL_VERTEX_SHADER);
	const GLuint fragmentShader = generateShader("shaders/fragment.fs", GL_FRAGMENT_SHADER);
	const GLuint textureShader = generateShader("shaders/texture.fs", GL_FRAGMENT_SHADER);
	const GLuint framebufferShader = generateShader("shaders/framebuffer.fs", GL_FRAGMENT_SHADER);
	const GLuint framebufferVertexShader = generateShader("shaders/framebuffer.vs", GL_VERTEX_SHADER);
	const GLuint framebufferVertexShader2 = generateShader("shaders/framebuffer2.vs", GL_VERTEX_SHADER);
	const GLuint framebufferShader2 = generateShader("shaders/framebuffer2.fs", GL_FRAGMENT_SHADER);
	// TODO: load the postprocessing shaders, create a shader program
	int success;
	Camera cam;
	SceneManager sceneManager;
	Scene scene_1("scene_1");
	Scene scene_2("scene_2");

	bool s_1_active = true;
	//scene1.AddObj(&suzanne);
	//sceneManager.scenes. = 

	char infoLog[512];
	const unsigned int modelShaderProgram = glCreateProgram();
	glAttachShader(modelShaderProgram, modelVertexShader);
	glAttachShader(modelShaderProgram, fragmentShader);
	glLinkProgram(modelShaderProgram);
	glGetProgramiv(modelShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(modelShaderProgram, 512, NULL, infoLog);
		printf("Error! Making Shader Program: %s\n", infoLog);
	}
	const unsigned int textureShaderProgram = glCreateProgram();
	glAttachShader(textureShaderProgram, modelVertexShader);
	glAttachShader(textureShaderProgram, textureShader);
	glLinkProgram(textureShaderProgram);
	glGetProgramiv(textureShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(textureShaderProgram, 512, NULL, infoLog);
		printf("Error! Making Shader Program: %s\n", infoLog);
	}

	const unsigned int framebufferShaderProgram = glCreateProgram();
	glAttachShader(framebufferShaderProgram, framebufferVertexShader);
	glAttachShader(framebufferShaderProgram, framebufferShader);
	glLinkProgram(framebufferShaderProgram);
	glGetProgramiv(framebufferShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(framebufferShaderProgram, 512, NULL, infoLog);
		printf("Error! Making Shader Program: %s\n", infoLog);
	}

	const unsigned int framebufferShaderProgram2 = glCreateProgram();
	glAttachShader(framebufferShaderProgram2, framebufferVertexShader2);
	glAttachShader(framebufferShaderProgram2, framebufferShader2);
	glLinkProgram(framebufferShaderProgram2);
	glGetProgramiv(framebufferShaderProgram2, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(framebufferShaderProgram2, 512, NULL, infoLog);
		printf("Error! Making Shader Program: %s\n", infoLog);
	}

	glDeleteShader(modelVertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(textureShader);
	glDeleteShader(framebufferShader);
	glDeleteShader(framebufferShader2);

	//FRAMEBUFFERS

	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	unsigned int textureColourbuffer;
	glGenTextures(1, &textureColourbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColourbuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, g_width, g_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColourbuffer, 0);

	// option1:
	unsigned int depthColorbuffer;
	glGenTextures(1, &depthColorbuffer);
	glBindTexture(GL_TEXTURE_2D, depthColorbuffer);

	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, 800, 600, 0,
		GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
	);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthColorbuffer, 0);

	// option2: -- what's wrong?
	//GLuint depthBuffer;
	//glGenRenderbuffers(1, &depthBuffer);
	//glBindRenderbuffer(GL_RENDER, depthBuffer);

	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, g_width, g_height);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
		printf("executing victory dance!\n");
	}
	else {
		printf("Something is wrong: %d\n", glCheckFramebufferStatus(GL_FRAMEBUFFER));
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//framebuffer 2 -> we're not using any of this

	//unsigned int framebuffer2;
	//glGenFramebuffers(1, &framebuffer2);
	//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);

	//unsigned int textureColourbuffer2;
	//glGenTextures(1, &textureColourbuffer2);
	//glBindTexture(GL_TEXTURE_2D, textureColourbuffer2);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, g_width, g_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColourbuffer2, 0);

	//glGenRenderbuffers(1, &depthBuffer);
	//glBindRenderbuffer(GL_RENDER, depthBuffer);

	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, g_width, g_height);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//end framebuffers

	core::Texture cmgtGatoTexture("textures/CMGaTo_crop.png");

	std::shared_ptr<core::Texture> furTexture = std::make_shared<core::Texture>("textures/fur.png");
	std::shared_ptr<core::Texture> woodTexture = std::make_shared<core::Texture>("textures/wood.png");
	std::shared_ptr<core::Texture> leatherTexture = std::make_shared<core::Texture>("textures/leather.png");

	core::Mesh quad = core::Mesh::generateQuad();
	core::Model quadModel({ quad });
	quadModel.translate(glm::vec3(0, 0, -2.5));
	quadModel.scale(glm::vec3(5, 5, 1));

	core::Model postprocessingQuad({ core::Mesh::generateQuad() });
	core::Model postprocessingQuad2({ core::Mesh::generateQuad() });

	core::Model suzanne = core::AssimpLoader::loadModel("models/nonormalmonkey.obj");
	Material suzanne_mat(modelShaderProgram, glm::vec3(1.0, 0.0, 0.0), 40);
	suzanne.SetMaterial(suzanne_mat);
	suzanne.material.SetTexture(furTexture);

	core::Model couch = core::AssimpLoader::loadModel("models/Couch_Small1.fbx");
	Material couch_mat(modelShaderProgram, glm::vec3(0.0, 1.0, 0.0), 20);
	couch.SetMaterial(couch_mat);
	couch.material.SetTexture(leatherTexture);

	core::Model table = core::AssimpLoader::loadModel("models/model.obj");
	Material table_mat(modelShaderProgram, glm::vec3(0.0, 0.0, 1.0), 80);
	table.SetMaterial(table_mat);
	table.material.SetTexture(woodTexture);

	couch.rotate(vec3(1, 0, 0), 225);
	couch.translate(vec3(-2, 0, 0));
	table.translate(vec3(2, 0, 0));


	//auto suzanne = std::make_shared<core::Model>(
	  //  core::AssimpLoader::loadModel("models/nonormalmonkey.obj"));
	//auto couch = std::make_shared<core::Model>(
	  //  core::AssimpLoader::loadModel("models/Couch_Small1.fbx"));
	//auto table = std::make_shared<core::Model>(
	  //  core::AssimpLoader::loadModel("models/model.obj"));

	//core::Model suzanne = core::AssimpLoader::loadModel("models/nonormalmonkey.obj");


	glm::vec4 clearColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	glClearColor(clearColor.r,
		clearColor.g, clearColor.b, clearColor.a);



	//VP glm::translate(glm::mat4(1), glm::vec3(0,0,-10)); // 
	glm::mat4 view = glm::lookAt(cam.cameraPos, cam.cameraTarget, cam.up);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(g_width) / static_cast<float>(g_height), 0.1f, 100.0f);

	GLint mvpMatrixUniform = glGetUniformLocation(modelShaderProgram, "mvpMatrix");
	GLint textureModelUniform = glGetUniformLocation(textureShaderProgram, "mvpMatrix");
	GLint textureUniform = glGetUniformLocation(textureShaderProgram, "text");
	GLint ppTextureUniform = glGetUniformLocation(framebufferShaderProgram, "textureUniform");
	GLint ppTextureUniform2 = glGetUniformLocation(framebufferShaderProgram2, "textureUniform2");
	GLint brightnessLocation = glGetUniformLocation(framebufferShaderProgram2, "brightness");
	GLint contrastLocation = glGetUniformLocation(framebufferShaderProgram2, "contrast");
	GLint hueLocation = glGetUniformLocation(framebufferShaderProgram2, "hue");
	GLint lightDirectionUniform = glGetUniformLocation(modelShaderProgram, "lightDirection");
	GLint mMatrixUniform = glGetUniformLocation(modelShaderProgram, "mMatrixUniform");
	GLint objectColor = glGetUniformLocation(modelShaderProgram, "objectColor");
	GLint lightColor = glGetUniformLocation(modelShaderProgram, "lightColor");
	GLint ambientIntensity = glGetUniformLocation(modelShaderProgram, "ambientIntensity");
	GLint ambientLightColor = glGetUniformLocation(modelShaderProgram, "ambientLightColor");
	GLint lightPos = glGetUniformLocation(modelShaderProgram, "lightPos");
	GLint camPos = glGetUniformLocation(modelShaderProgram, "camPos");
	GLint specularColor = glGetUniformLocation(modelShaderProgram, "specularColor");
	GLint shininess = glGetUniformLocation(modelShaderProgram, "shininess");
	GLint adsUvGridTexUniform = glGetUniformLocation(modelShaderProgram, "uvGridText");
	//GLint framebuffer_is_active = glGetUniformLocation(framebufferShaderProgram, "framebuffer_is_active");

	// Scene starting point (TODO: make class. Also maybe make GameObject class)
	//std::vector<core::Model*> scene1; // pointers? copies? references?

	// Here's where you add more stuff to your scene



	scene_1.AddObj(&couch);
	scene_1.AddObj(&table);
	scene_2.AddObj(&suzanne);

	//bool framebuffer_is_active = true;

	double currentTime = glfwGetTime();
	double finishFrameTime = 0.0;
	float deltaTime = 0.0f;
	float rotationStrength = 100.0f;

	bool framebuffer_is_active = true;
	int postProcessingEffect = 0; // 0=edge detect, 1=brightness etc.
	
	//bool framebuffer2_is_active = false;
	float contrast = 1.0f;
	float brightness = 0.15f; // TODO: tweak shader such that these values make sense....
	float hue = 1.0f;


	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		// why does this break your renderer?:
		if (framebuffer_is_active)
		{
			//printf("Using frame buffer 1\n");
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			glClearColor(0.1, 0.1, 0.1, 0.1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//framebuffer2_is_active = false;
		//}
		//else if//(framebuffer2_is_active)
		//{
		//	printf("Using frame buffer 2\n");
		//	framebuffer_is_active = 
		//	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);
		//	glClearColor(0.1, 0.1, 0.1, 0.1);
		//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		} else
		{
			//printf("Using no frame buffer\n");

			glBindFramebuffer(GL_FRAMEBUFFER, 0); // render to screen 
			glClearColor(0.1, 0.1, 0.1, 0.1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}


		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Raw Engine v2");
		ImGui::SliderFloat("Brightness", &brightness, -1.0f, 1.0f);
		ImGui::SliderFloat("Contrast", &contrast, -1.0f, 1.0f);
		ImGui::SliderFloat("Hue", &hue, 0.0f, 360.0f);

		if (ImGui::Button("Post processing toggle")) {
			printf("I was clicked!\n");
			framebuffer_is_active = !framebuffer_is_active;
			printf("%d\n", framebuffer_is_active);
		}
		if (ImGui::Button("Post processing effect selection")) {
			printf("I was clicked!\n");
			postProcessingEffect = 1 - postProcessingEffect;
			printf("%d\n", postProcessingEffect);
		}

		ImGui::Text("Hello :)");
		ImGui::End();

		processInput(window);
		suzanne.rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(rotationStrength) * static_cast<float>(deltaTime));
		//suzanne.translate(glm::vec3(0, 0, 0.1));


		checkMovement(window, cam);
		view = glm::lookAt(cam.cameraPos, cam.cameraPos + cam.cameraFront, glm::vec3(0,1,0)); // TODO (maybe): the camera stores its own matrix - not always looking at the same point
		//projection = glm::perspective(glm::radians(45.0f), static_cast<float>(g_width) / static_cast<float>(g_height), 0.1f, 100.0f);
		// Hint: look at model.cpp

		//view = glm::translate(glm::mat4(1), cam.getPosition());
		//projection;

		if (shiftPressed) {
			auto cp = cam.getPosition();
			printf("Main: Camera is now at (%f,%f,%f)\n", cp.x, cp.y, cp.z);
		}

		glUseProgram(modelShaderProgram);
		// REMEMBER THIS!
		//  object       |
		// Model matrix  v
		//  world
		// View matrix     v     
		//  camera (this is the space that has camera position as origin)
		// Projection matrix    v
		//  screen space / homogeneous space (...more or less)

		glUniform3f(lightDirectionUniform, 1, 0, 0);
		glUniform4f(objectColor, 1.0f, 1.0f, 1.0f, 1);
		glUniform4f(lightColor, 255.0f / 255.0f, 166.0f / 255.0f, 248.0f / 255.0f, 0.2f); // 4th coord is irrelevant...
		glUniform1f(ambientIntensity, 0.59f);
		glUniform4f(ambientLightColor, 0.84f, 0.7f, 0.72f, 0.2f);
		glUniform3f(camPos, cam.getPosition().x, cam.getPosition().y, cam.getPosition().z);
		glUniform3f(lightPos, 4, 0, 0);
		glUniform1f(shininess, 0.5f);
		glUniform4f(specularColor, 0.39f, 0.34f, 0.45f, 0.2f);

		/*for (int i = 0; i < scene1.size(); i++) {
			// TODO: change uniforms (=material properties) and possibly shader per model
			core::Model* obj = scene1[i];
			glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, glm::value_ptr(projection * view * (*obj).getModelMatrix()));
			// TODO: just pass the model matrix as well
			glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, glm::value_ptr((*obj).getModelMatrix()));
			obj->render();
		}*/

		/*glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, glm::value_ptr(projection * view * (couch).getModelMatrix()));
		glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, glm::value_ptr((couch).getModelMatrix()));
		glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, glm::value_ptr(projection * view * (suzanne).getModelMatrix()));
		glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, glm::value_ptr((suzanne).getModelMatrix()));
		glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, glm::value_ptr(projection * view * (table).getModelMatrix()));
		glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, glm::value_ptr((table).getModelMatrix()));*/

		glBindVertexArray(0);



		glUseProgram(textureShaderProgram);
		glUniformMatrix4fv(textureModelUniform, 1, GL_FALSE, glm::value_ptr(projection * view * quadModel.getModelMatrix()));
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(textureUniform, 0);
		glBindTexture(GL_TEXTURE_2D, cmgtGatoTexture.getId());
		quadModel.render();
		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);


		if ((glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS))
			s_1_active = true;
		else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
			s_1_active = false;

		if (s_1_active)
		{
			glUseProgram(modelShaderProgram);
			//glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, glm::value_ptr(projection * view * scene_1.objs[1]->getModelMatrix()));
			//glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, glm::value_ptr(scene_1.objs[0]->getModelMatrix()));
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, couch.material.texture->getId());
			glUniform1i(adsUvGridTexUniform, 1);

			scene_1.renderScene(
				modelShaderProgram,
				view,
				projection,
				mvpMatrixUniform,
				mMatrixUniform
			);
		}
		else
		{
			glUseProgram(modelShaderProgram);
			//glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, glm::value_ptr(projection * view * scene_2.objs[0]->getModelMatrix()));
			//glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, glm::value_ptr(scene_2.objs[0]->getModelMatrix()));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, suzanne.material.texture->getId());
			glUniform1i(adsUvGridTexUniform, 0);

			scene_2.renderScene(
				modelShaderProgram,
				view,
				projection,
				mvpMatrixUniform,
				mMatrixUniform
			);
		}
		// TODO: what if ..?
		if (framebuffer_is_active) {
			if (postProcessingEffect == 0) // wrong variable name!
			{ // edge detection:
				//printf("Using edge detection post processing\n");
				glBindFramebuffer(GL_FRAMEBUFFER, 0); // render to screen
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



				glUseProgram(framebufferShaderProgram);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureColourbuffer);
				glUniform1i(ppTextureUniform, 0);
				postprocessingQuad.render();


			}
			else if (postProcessingEffect == 1)// brightness, contrast, etc.: 
			{
				//printf("Using brightness etc. post processing\n");

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



				glUseProgram(framebufferShaderProgram2);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureColourbuffer);


				glUniform1i(ppTextureUniform2, 0);
				glUniform1f(brightnessLocation, brightness);
				glUniform1f(contrastLocation, contrast);
				glUniform1f(hueLocation, hue);
				postprocessingQuad.render();


			}
		} else { // no frame buffer active
			// :-)
		}

		//glEnable(GL_DEPTH_TEST);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
		finishFrameTime = glfwGetTime();
		deltaTime = static_cast<float>(finishFrameTime - currentTime);
		currentTime = finishFrameTime;
	}

	glDeleteProgram(modelShaderProgram);
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;

}