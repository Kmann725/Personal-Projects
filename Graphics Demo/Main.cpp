#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"

void FramebufferCallback(GLFWwindow* window, int width, int height);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void ProcessInput(GLFWwindow* window);

const float SCREEN_WIDTH = 1200.0f;
const float SCREEN_HEIGHT = 700.0f;

Camera camera = Camera(glm::vec3(0.0f, 1.0f, 0.3f), glm::vec3(0.0f, 1.0f, 0.0f));
float prevX = 1200 / 2.0f;
float prevY = 700 / 2.0f;
bool firstMouse = true;

float dt = 0.0f;
float prevFrame = 0.0f;

glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::perspective(glm::radians(45.0f), SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);

bool wireframe = false;
bool pressed1 = false;

float rotation = 0.0f;

int main() {
	srand((unsigned int)time(0));

	// Initializing GLFW window and context
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Graphics Demo", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Setting callbacks for receiving player inputs
	glfwSetFramebufferSizeCallback(window, FramebufferCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetKeyCallback(window, KeyCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader carShader = Shader("car.vs", "car.fs");
	Shader interiorShader = Shader("car.vs", "car.fs");
	Shader tireShader = Shader("object.vs", "object.fs");
	Shader roadShader = Shader("object.vs", "object.fs");
	Shader grassShader = Shader("object.vs", "object.fs");

	float carVerts[] = {
		// bottom of car														// parentheses indicate duplicate position with different normal
		-1.0f, -1.0f, -1.0f,			0.0f, -1.0f, 0.0f,								// 0
		-1.0f, -1.0f, 1.0f,				0.0f, -1.0f, 0.0f,								// 1
		-0.8f, -1.0f, -1.0f,			0.0f, -1.0f, 0.0f,								// 2
		-0.8f, -1.0f, -0.65f,			0.0f, -1.0f, 0.0f,								// 3
		-0.8f, -1.0f, 0.65f,			0.0f, -1.0f, 0.0f,								// 4
		-0.8f, -1.0f, 1.0f,				0.0f, -1.0f, 0.0f,								// 5
		-0.5f, -1.0f, -1.0f,			0.0f, -1.0f, 0.0f,								// 6
		-0.5f, -1.0f, -0.65f,			0.0f, -1.0f, 0.0f,								// 7
		-0.5f, -1.0f, 0.65f,			0.0f, -1.0f, 0.0f,								// 8
		-0.5f, -1.0f, 1.0f,				0.0f, -1.0f, 0.0f,								// 9
		0.5f, -1.0f, -1.0f,				0.0f, -1.0f, 0.0f,								// 10
		0.5f, -1.0f, -0.65f,			0.0f, -1.0f, 0.0f,								// 11
		0.5f, -1.0f, 0.65f,				0.0f, -1.0f, 0.0f,								// 12
		0.5f, -1.0f, 1.0f,				0.0f, -1.0f, 0.0f,								// 13
		0.8f, -1.0f, -1.0f,				0.0f, -1.0f, 0.0f,								// 14
		0.8f, -1.0f, -0.65f,			0.0f, -1.0f, 0.0f,								// 15
		0.8f, -1.0f, 0.65f,				0.0f, -1.0f, 0.0f,								// 16
		0.8f, -1.0f, 1.0f,				0.0f, -1.0f, 0.0f,								// 17
		1.0f, -1.0f, -1.0f,				0.0f, -1.0f, 0.0f,								// 18
		1.0f, -1.0f, 1.0f,				0.0f, -1.0f, 0.0f,								// 19

		// front right wheel well
		-0.8f, -0.7f, -1.0f,			1.0f, 0.0f, 0.0f,								// 20
		-0.8f, -0.7f, -0.65f,			1.0f, 0.0f, 0.0f,								// 21
		-0.65f, -0.6f, -1.0f,			0.5547f, -0.83205f, 0.0f,						// 22
		-0.65f, -0.6f, -0.65f,			0.0f, 0.0f, -1.0f,								// 23
		-0.5f, -0.7f, -1.0f,			-1.0f, 0.0f, 0.0f,								// 24
		-0.5f, -0.7f, -0.65f,			-1.0f, 0.0f, 0.0f,								// 25
		-0.8f, -1.0f, -1.0f,			1.0f, 0.0f, 0.0f,								// 26 (2)
		-0.8f, -1.0f, -0.65f,			1.0f, 0.0f, 0.0f,								// 27 (3)
		-0.8f, -1.0f, -0.65f,			0.0f, 0.0f, -1.0f,								// 28 (3)
		-0.5f, -1.0f, -1.0f,			-1.0f, 0.0f, 0.0f,								// 29 (6)
		-0.5f, -1.0f, -0.65f,			-1.0f, 0.0f, 0.0f,								// 30 (7)
		-0.5f, -1.0f, -0.65f,			0.0f, 0.0f, -1.0f,								// 31 (7)
		-0.8f, -0.7f, -1.0f,			0.5547f, -0.83205f, 0.0f,						// 32 (20)
		-0.8f, -0.7f, -0.65f,			0.0f, 0.0f, -1.0f,								// 33 (21)
		-0.8f, -0.7f, -0.65f,			0.5547f, -0.83205f, 0.0f,						// 34 (21)
		-0.5f, -0.7f, -0.65f,			0.0f, 0.0f, -1.0f,								// 35 (25)
		-0.5f, -0.7f, -0.65f,			-0.5547f, -0.83205f, 0.0f,						// 36 (25)
		-0.5f, -0.7f, -1.0f,			-0.5547f, -0.83205f, 0.0f,						// 37 (24)
		-0.65f, -0.6f, -1.0f,			-0.5547f, -0.83205f, 0.0f,						// 38 (22)
		-0.65f, -0.6f, -0.65f,			0.5547f, -0.83205f, 0.0f,						// 39 (23)
		-0.65f, -0.6f, -0.65f,			-0.5547f, -0.83205f, 0.0f,						// 40 (23)

		// front left wheel well
		-0.8f, -0.7f, 1.0f,				1.0f, 0.0f, 0.0f,								// 41
		-0.8f, -0.7f, 0.65f,			1.0f, 0.0f, 0.0f,								// 42
		-0.65f, -0.6f, 1.0f,			0.5547f, -0.83205f, 0.0f,						// 43
		-0.65f, -0.6f, 0.65f,			0.0f, 0.0f, 1.0f,								// 44
		-0.5f, -0.7f, 1.0f,				-1.0f, 0.0f, 0.0f,								// 45
		-0.5f, -0.7f, 0.65f,			-1.0f, 0.0f, 0.0f,								// 46
		-0.8f, -1.0f, 0.65f,			1.0f, 0.0f, 0.0f,								// 47 (4)
		-0.8f, -1.0f, 0.65f,			0.0f, 0.0f, 1.0f,								// 48 (4)
		-0.8f, -1.0f, 1.0f,				1.0f, 0.0f, 0.0f,								// 49 (5)
		-0.5f, -1.0f, 0.65f,			-1.0f, 0.0f, 0.0f,								// 50 (8)
		-0.5f, -1.0f, 0.65f,			0.0f, 0.0f, 1.0f,								// 51 (8)
		-0.5f, -1.0f, 1.0f,				-1.0f, 0.0f, 0.0f,								// 52 (9)
		-0.5f, -0.7f, 0.65f,			0.0f, 0.0f, 1.0f,								// 53 (46)
		-0.5f, -0.7f, 0.65f,			-0.5547f, -0.83205f, 0.0f,						// 54 (46)
		-0.8f, -0.7f, 0.65f,			0.0f, 0.0f, 1.0f,								// 55 (42)
		-0.8f, -0.7f, 0.65f,			0.5547f, -0.83205f, 0.0f,						// 56 (42)
		-0.8f, -0.7f, 1.0f,				0.5547f, -0.83205f, 0.0f,						// 57 (41)
		-0.5f, -0.7f, 1.0f,				-0.5547f, -0.83205f, 0.0f,						// 58 (45)
		-0.65f, -0.6f, 1.0f,			-0.5547f, -0.83205f, 0.0f,						// 59 (43)
		-0.65f, -0.6f, 0.65f,			0.5547f, -0.83205f, 0.0f,						// 60 (44)
		-0.65f, -0.6f, 0.65f,			-0.5547f, -0.83205f, 0.0f,						// 61 (44)

		// back right wheel well
		0.5f, -0.7f, -1.0f,				1.0f, 0.0f, 0.0f,								// 62
		0.5f, -0.7f, -0.65f,			1.0f, 0.0f, 0.0f,								// 63
		0.65f, -0.6f, -1.0f,			0.5547f, -0.83205f, 0.0f,						// 64
		0.65f, -0.6f, -0.65f,			0.0f, 0.0f, -1.0f,								// 65
		0.8f, -0.7f, -1.0f,				-1.0f, 0.0f, 0.0f,								// 66
		0.8f, -0.7f, -0.65f,			-1.0f, 0.0f, 0.0f,								// 67
		0.5f, -1.0f, -1.0f,				1.0f, 0.0f, 0.0f,								// 68 (10)
		0.5f, -1.0f, -0.65f,			1.0f, 0.0f, 0.0f,								// 69 (11)
		0.5f, -1.0f, -0.65f,			0.0f, 0.0f, -1.0f,								// 70 (11)
		0.8f, -1.0f, -1.0f,				-1.0f, 0.0f, 0.0f,								// 71 (14)
		0.8f, -1.0f, -0.65f,			-1.0f, 0.0f, 0.0f,								// 72 (15)
		0.8f, -1.0f, -0.65f,			0.0f, 0.0f, -1.0f,								// 73 (15)
		0.5f, -0.7f, -1.0f,				0.5547f, -0.83205f, 0.0f,						// 74 (62)
		0.5f, -0.7f, -0.65f,			0.0f, 0.0f, -1.0f,								// 75 (63)
		0.5f, -0.7f, -0.65f,			0.5547f, -0.83205f, 0.0f,						// 76 (63)
		0.65f, -0.6f, -1.0f,			-0.5547f, -0.83205f, 0.0f,						// 77 (64)
		0.65f, -0.6f, -0.65f,			0.5547f, -0.83205f, 0.0f,						// 78 (65)
		0.65f, -0.6f, -0.65f,			-0.5547f, -0.83205f, 0.0f,						// 79 (65)
		0.8f, -0.7f, -1.0f,				-0.5547f, -0.83205f, 0.0f,						// 80 (66)
		0.8f, -0.7f, -0.65f,			0.0f, 0.0f, -1.0f,								// 81 (67)
		0.8f, -0.7f, -0.65f,			-0.5547f, -0.83205f, 0.0f,						// 82 (67)

		// back left wheel well
		0.5f, -0.7f, 1.0f,				1.0f, 0.0f, 0.0f,								// 83
		0.5f, -0.7f, 0.65f,				1.0f, 0.0f, 0.0f,								// 84
		0.65f, -0.6f, 1.0f,				0.5547f, -0.83205f, 0.0f,						// 85
		0.65f, -0.6f, 0.65f,			0.0f, 0.0f, 1.0f,								// 86
		0.8f, -0.7f, 1.0f,				-1.0f, 0.0f, 0.0f,								// 87
		0.8f, -0.7f, 0.65f,				-1.0f, 0.0f, 0.0f,								// 88
		0.5f, -1.0f, 0.65f,				1.0f, 0.0f, 0.0f,								// 89 (12)
		0.5f, -1.0f, 0.65f,				0.0f, 0.0f, 1.0f,								// 90 (12)
		0.5f, -1.0f, 1.0f,				1.0f, 0.0f, 0.0f,								// 91 (13)
		0.8f, -1.0f, 0.65f,				-1.0f, 0.0f, 0.0f,								// 92 (16)
		0.8f, -1.0f, 0.65f,				0.0f, 0.0f, 1.0f,								// 93 (16)
		0.8f, -1.0f, 1.0f,				-1.0f, 0.0f, 0.0f,								// 94 (17)
		0.5f, -0.7f, 1.0f,				0.5547f, -0.83205f, 0.0f,						// 95 (83)
		0.5f, -0.7f, 0.65f,				0.0f, 0.0f, 1.0f,								// 96 (84)
		0.5f, -0.7f, 0.65f,				0.5547f, -0.83205f, 0.0f,						// 97 (84)
		0.65f, -0.6f, 1.0f,				-0.5547f, -0.83205f, 0.0f,						// 98 (85)
		0.65f, -0.6f, 0.65f,			0.5547f, -0.83205f, 0.0f,						// 99 (86)
		0.65f, -0.6f, 0.65f,			-0.5547f, -0.83205f, 0.0f,						// 100 (86)
		0.8f, -0.7f, 1.0f,				-0.5547f, -0.83205f, 0.0f,						// 101 (87)
		0.8f, -0.7f, 0.65f,				0.0f, 0.0f, 1.0f,								// 102 (88)
		0.8f, -0.7f, 0.65f,				-0.5547f, -0.83205f, 0.0f,						// 103 (88)

		// rear bumper
		1.0f, -0.5f, -1.0f,				1.0f, 0.0f, 0.0f,								// 104
		1.0f, -0.5f, 1.0f,				1.0f, 0.0f, 0.0f,								// 105
		1.0f, -1.0f, -1.0f,				1.0f, 0.0f, 0.0f,								// 106 (18)
		1.0f, -1.0f, 1.0f,				1.0f, 0.0f, 0.0f,								// 107 (19)

		// top view of car
		// front of car
		-1.0f, -0.5f, -1.0f,			-1.0f, 0.0f, 0.0f,								// 108
		-1.0f, -0.5f, 1.0f,				-1.0f, 0.0f, 0.0f,								// 109
		-1.0f, -1.0f, -1.0f,			-1.0f, 0.0f, 0.0f,								// 110 (0)
		-1.0f, -1.0f, 1.0f,				-1.0f, 0.0f, 0.0f,								// 111 (1)

		-0.95f, -0.4f, -0.85f,			-0.894427f, 0.447214f, 0.0f,					// 112
		-0.95f, -0.4f, 0.85f,			-0.894427f, 0.447214f, 0.0f,					// 113
		-1.0f, -0.5f, -1.0f,			-0.894427f, 0.447214f, 0.0f,					// 114 (108)
		-1.0f, -0.5f, 1.0f,				-0.894427f, 0.447214f, 0.0f,					// 115 (109)

		-0.9f, -0.4f, -1.0f,			-0.688247f, 0.688247f, -0.229416f,				// 116
		-1.0f, -0.5f, -1.0f,			-0.688247f, 0.688247f, -0.229416f,				// 117 (108)
		-0.95f, -0.4f, -0.85f,			-0.688247f, 0.688247f, -0.229416f,				// 118 (112)

		-0.9f, -0.4f, 1.0f,				-0.688247f, 0.688247f, 0.229416f,				// 119
		-1.0f, -0.5f, 1.0f,				-0.688247f, 0.688247f, 0.229416f,				// 120 (109)
		-0.95f, -0.4f, 0.85f,			-0.688247f, 0.688247f, 0.229416f,				// 121 (113)

		-0.85f, -0.3f, -0.85f,			-0.688247f, 0.688247f, -0.229416f,				// 122
		-0.95f, -0.4f, -0.85f,			-0.688247f, 0.688247f, -0.229416f,				// 123 (112)

		-0.85f, -0.3f, 0.85f,			-0.688247f, 0.688247f, 0.229416f,				// 124
		-0.95f, -0.4f, 0.85f,			-0.688247f, 0.688247f, 0.229416f,				// 125 (113)

		-0.85f, -0.3f, -0.85f,			-0.707107f, 0.707107f, 0.0f,					// 126 (122)
		-0.95f, -0.4f, -0.85f,			-0.707107f, 0.707107f, 0.0f,					// 127 (112)
		-0.85f, -0.3f, 0.85f,			-0.707107f, 0.707107f, 0.0f,					// 128 (124)
		-0.95f, -0.4f, 0.85f,			-0.707107f, 0.707107f, 0.0f,					// 129 (113)

		-0.7f, -0.3f, -1.0f,			-0.408248f, 0.816497f, -0.408248f,				// 130
		-0.9f, -0.4f, -1.0f,			-0.408248f, 0.816497f, -0.408248f,				// 131 (116)
		-0.85f, -0.3f, -0.85f,			-0.408248f, 0.816497f, -0.408248f,				// 132 (122)
		-0.65f, -0.2f, -0.85f,			-0.408248f, 0.816497f, -0.408248f,				// 133

		-0.7f, -0.3f, 1.0f,				-0.408248f, 0.816497f, 0.408248f,				// 134
		-0.9f, -0.4f, 1.0f,				-0.408248f, 0.816497f, 0.408248f,				// 135 (119)
		-0.85f, -0.3f, 0.85f,			-0.408248f, 0.816497f, 0.408248f,				// 136 (124)
		-0.65f, -0.2f, 0.85f,			-0.408248f, 0.816497f, 0.408248f,				// 137

		-0.85f, -0.3f, -0.85f,			-0.447214f, 0.894427f, 0.0f,					// 138 (122)
		-0.65f, -0.2f, -0.85f,			-0.447214f, 0.894427f, 0.0f,					// 139 (133)
		-0.85f, -0.3f, 0.85f,			-0.447214f, 0.894427f, 0.0f,					// 140 (124)
		-0.65f, -0.2f, 0.85f,			-0.447214f, 0.894427f, 0.0f,					// 141 (137)

		-0.35f, -0.3f, -1.0f,			0.0f, 0.83205f, -0.5547f,						// 142
		-0.35f, -0.2f, -0.85f,			0.0f, 0.83205f, -0.5547f,						// 143
		-0.65f, -0.2f, -0.85f,			0.0f, 0.83205f, -0.5547f,						// 144 (133)
		-0.7f, -0.3f, -1.0f,			0.0f, 0.83205f, -0.5547f,						// 145 (130)

		-0.35f, -0.3f, 1.0f,			0.0f, 0.83205f, 0.5547f,						// 146
		-0.35f, -0.2f, 0.85f,			0.0f, 0.83205f, 0.5547f,						// 147
		-0.65f, -0.2f, 0.85f,			0.0f, 0.83205f, 0.5547f,						// 148 (137)
		-0.7f, -0.3f, 1.0f,				0.0f, 0.83205f, 0.5547f,						// 149 (134)

		-0.35f, -0.2f, -0.85f,			0.0f, 1.0f, 0.0f,								// 150 (143)
		-0.35f, -0.2f, 0.85f,			0.0f, 1.0f, 0.0f,								// 151 (147)
		-0.65f, -0.2f, -0.85f,			0.0f, 1.0f, 0.0f,								// 152 (133)
		-0.65f, -0.2f, 0.85f,			0.0f, 1.0f, 0.0f,								// 153 (137)

		// roof of car
		-0.15f, 0.2f, -0.9f,			-0.242536f, 0.970143f, 0.0f,					// 154
		-0.15f, 0.2f, 0.9f,				-0.242536f, 0.970143f, 0.0f,					// 155
		0.05f, 0.25f, -0.9f,			-0.242536f, 0.970143f, 0.0f,					// 156
		0.05f, 0.25f, 0.9f,				-0.242536f, 0.970143f, 0.0f,					// 157

		0.05f, 0.25f, -0.9f,			0.0f, 1.0f, 0.0f,								// 158 (156)
		0.05f, 0.25f, 0.9f,				0.0f, 1.0f, 0.0f,								// 159 (157)
		0.3f, 0.25f, -0.9f,				0.0f, 1.0f, 0.0f,								// 160
		0.3f, 0.25f, 0.9f,				0.0f, 1.0f, 0.0f,								// 161

		0.3f, 0.25f, -0.9f,				0.242536f, 0.970143f, 0.0f,						// 162 (160)
		0.3f, 0.25f, 0.9f,				0.242536f, 0.970143f, 0.0f,						// 163 (161)
		0.5f, 0.2f, -0.9f,				0.242536f, 0.970143f, 0.0f,						// 164
		0.5f, 0.2f, 0.9f,				0.242536f, 0.970143f, 0.0f,						// 165

		-0.35f, -0.3f, -1.0f,			-0.874475f, 0.403604f, -0.269069f,				// 166 (142)
		-0.35f, -0.2f, -0.85f,			-0.874475f, 0.403604f, -0.269069f,				// 167 (143)
		-0.15f, 0.2f, -0.9f,			-0.874475f, 0.403604f, -0.269069f,				// 168 (154)

		-0.15f, 0.2f, -0.8f,			-0.857143f, 0.428571f, -0.285714f,				// 169
		-0.35f, -0.2f, -0.85f,			-0.857143f, 0.428571f, -0.285714f,				// 170 (143)
		-0.15f, 0.2f, -0.9f,			-0.857143f, 0.428571f, -0.285714f,				// 171 (154)

		-0.35f, -0.3f, 1.0f,			-0.874475f, 0.403604f, 0.269069f,				// 172 (146)
		-0.35f, -0.2f, 0.85f,			-0.874475f, 0.403604f, 0.269069f,				// 173 (147)
		-0.15f, 0.2f, 0.9f,				-0.874475f, 0.403604f, 0.269069f,				// 174 (155)

		-0.15f, 0.2f, 0.8f,				-0.857143f, 0.428571f, 0.285714f,				// 175
		-0.35f, -0.2f, 0.85f,			-0.857143f, 0.428571f, 0.285714f,				// 176 (147)
		-0.15f, 0.2f, 0.9f,				-0.857143f, 0.428571f, 0.285714f,				// 177 (155)

		-0.35f, -0.3f, -1.0f,			-0.0523554f, 0.209421f, -0.976423f,				// 178 (142)
		-0.265f, -0.3f, -1.0f,			-0.0523554f, 0.209421f, -0.976423f,				// 179
		-0.15f, 0.2f, -0.9f,			-0.0523554f, 0.209421f, -0.976423f,				// 180 (154)
		-0.075f, 0.21875f, -0.9f,		-0.0523554f, 0.209421f, -0.976423f,				// 181

		-0.35f, -0.3f, 1.0f,			-0.0523554f, 0.209421f, 0.976423f,				// 182 (146)
		-0.265f, -0.3f, 1.0f,			-0.0523554f, 0.209421f, 0.976423f,				// 183
		-0.15f, 0.2f, 0.9f,				-0.0523554f, 0.209421f, 0.976423f,				// 184 (155)
		-0.075f, 0.21875f, 0.9f,		-0.0523554f, 0.209421f, 0.976423f,				// 185

		// back of the car
		1.0f, -0.5f, -1.0f,				0.970142f, 0.242536f, 0.0f,						// 186 (104)
		1.0f, -0.5f, 1.0f,				0.970142f, 0.242536f, 0.0f,						// 187 (105)
		0.95f, -0.3f, -0.85f,			0.970142f, 0.242536f, 0.0f,						// 188
		0.95f, -0.3f, 0.85f,			0.970142f, 0.242536f, 0.0f,						// 189

		0.95f, -0.3f, -0.85f,			0.447214f, 0.894427f, 0.0f,						// 190 (188)
		0.95f, -0.3f, 0.85f,			0.447214f, 0.894427f, 0.0f,						// 191 (189)
		0.85f, -0.25f, -0.85f,			0.447214f, 0.894427f, 0.0f,						// 192
		0.85f, -0.25f, 0.85f,			0.447214f, 0.894427f, 0.0f,						// 193

		0.85f, -0.25f, -0.85f,			0.242536f, 0.970142f, 0.0f,						// 194 (192)
		0.85f, -0.25f, 0.85f,			0.242536f, 0.970142f, 0.0f,						// 195 (193)
		0.65f, -0.2f, -0.85f,			0.242536f, 0.970142f, 0.0f,						// 196
		0.65f, -0.2f, 0.85f,			0.242536f, 0.970142f, 0.0f,						// 197

		1.0f, -0.5f, -1.0f,				0.426401f, 0.639602f, -0.639602f,				// 198 (104)
		0.85f, -0.25f, -0.85f,			0.426401f, 0.639602f, -0.639602f,				// 199 (192)
		0.85f, -0.4f, -1.0f,			0.426401f, 0.639602f, -0.639602f,				// 200

		0.95f, -0.3f, -0.85f,			0.309427f, 0.618853f, -0.721995f,				// 201 (188)
		1.0f, -0.5f, -1.0f,				0.309427f, 0.618853f, -0.721995f,				// 202 (104)
		0.85f, -0.25f, -0.85f,			0.309427f, 0.618853f, -0.721995f,				// 203 (192)

		0.85f, -0.25f, -0.85f,			0.174078f, 0.696311f, -0.696311f,				// 204 (192)
		0.85f, -0.4f, -1.0f,			0.174078f, 0.696311f, -0.696311f,				// 205 (200)
		0.65f, -0.35f, -1.0f,			0.174078f, 0.696311f, -0.696311f,				// 206
		0.65f, -0.2f, -0.85f,			0.174078f, 0.696311f, -0.696311f,				// 207 (196)

		1.0f, -0.5f, 1.0f,				0.426401f, 0.639602f, 0.639602f,				// 208 (105)
		0.85f, -0.25f, 0.85f,			0.426401f, 0.639602f, 0.639602f,				// 209 (193)
		0.85f, -0.4f, 1.0f,				0.426401f, 0.639602f, 0.639602f,				// 210

		0.95f, -0.3f, 0.85f,			0.309427f, 0.618853f, 0.721995f,				// 211 (189)
		1.0f, -0.5f, 1.0f,				0.309427f, 0.618853f, 0.721995f,				// 212 (105)
		0.85f, -0.25f, 0.85f,			0.309427f, 0.618853f, 0.721995f,				// 213 (193)

		0.85f, -0.25f, 0.85f,			0.174078f, 0.696311f, 0.696311f,				// 214 (193)
		0.85f, -0.4f, 1.0f,				0.174078f, 0.696311f, 0.696311f,				// 215 (210)
		0.65f, -0.35f, 1.0f,			0.174078f, 0.696311f, 0.696311f,				// 216
		0.65f, -0.2f, 0.85f,			0.174078f, 0.696311f, 0.696311f,				// 217 (197)

		0.5f, 0.2f, -0.9f,				0.857143f, 0.428571f, -0.285714f,				// 218 (164)
		0.5f, 0.2f, -0.8f,				0.857143f, 0.428571f, -0.285714f,				// 219
		0.65f, -0.2f, -0.85f,			0.857143f, 0.428571f, -0.285714f,				// 220 (196)

		0.65f, -0.35f, -1.0f,			0.904534f, 0.301511f, -0.301511f,				// 221 (206)
		0.5f, 0.2f, -0.9f,				0.904534f, 0.301511f, -0.301511f,				// 222 (164)
		0.65f, -0.2f, -0.85f,			0.904534f, 0.301511f, -0.301511f,				// 223 (196)

		0.5f, 0.2f, 0.9f,				0.857143f, 0.428571f, 0.285714f,				// 224 (165)
		0.5f, 0.2f, 0.8f,				0.857143f, 0.428571f, 0.285714f,				// 225
		0.65f, -0.2f, 0.85f,			0.857143f, 0.428571f, 0.285714f,				// 226 (197)

		0.65f, -0.35f, 1.0f,			0.904534f, 0.301511f, 0.301511f,				// 227 (216)
		0.5f, 0.2f, 0.9f,				0.904534f, 0.301511f, 0.301511f,				// 228 (165)
		0.65f, -0.2f, 0.85f,			0.904534f, 0.301511f, 0.301511f,				// 229 (197)

		0.65f, -0.35f, -1.0f,			0.132628f, 0.212205f, -0.968183f,				// 230 (206)
		0.57f, -0.3f, -1.0f,			0.132628f, 0.212205f, -0.968183f,				// 231
		0.5f, 0.2f, -0.9f,				0.132628f, 0.212205f, -0.968183f,				// 232 (164)

		0.5f, 0.2f, -0.9f,				0.0506701f, 0.20268f, -0.977933f,				// 233 (164)
		0.57f, -0.3f, -1.0f,			0.0506701f, 0.20268f, -0.977933f,				// 234 (231)
		0.425f, 0.21875f, -0.9f,		0.0506701f, 0.20268f, -0.977933f,				// 235

		0.65f, -0.35f, 1.0f,			0.132628f, 0.212205f, 0.968183f,				// 236 (216)
		0.57f, -0.3f, 1.0f,				0.132628f, 0.212205f, 0.968183f,				// 237
		0.5f, 0.2f, 0.9f,				0.132628f, 0.212205f, 0.968183f,				// 238 (165)

		0.5f, 0.2f, 0.9f,				0.0506701f, 0.20268f, 0.977933f,				// 239 (165)
		0.57f, -0.3f, 1.0f,				0.0506701f, 0.20268f, 0.977933f,				// 240 (237)
		0.425f, 0.21875f, 0.9f,			0.0506701f, 0.20268f, 0.977933f,				// 241

		// right side of car
		-1.0f, -1.0f, -1.0f,			0.0f, 0.0f, -1.0f,								// 242 (0)
		-1.0f, -0.5f, -1.0f,			0.0f, 0.0f, -1.0f,								// 243 (108)
		-0.8f, -1.0f, -1.0f,			0.0f, 0.0f, -1.0f,								// 244 (2)
		-0.8f, -0.7f, -1.0f,			0.0f, 0.0f, -1.0f,								// 245 (20)
		-0.9f, -0.4f, -1.0f,			0.0f, 0.0f, -1.0f,								// 246 (116)
		-0.65f, -0.6f, -1.0f,			0.0f, 0.0f, -1.0f,								// 247 (22)
		-0.7f, -0.3f, -1.0f,			0.0f, 0.0f, -1.0f,								// 248 (130)
		-0.265f, -0.3f, -1.0f,			0.0f, 0.0f, -1.0f,								// 249 (179)
		-0.5f, -0.7f, -1.0f,			0.0f, 0.0f, -1.0f,								// 250 (24)
		-0.5f, -1.0f, -1.0f,			0.0f, 0.0f, -1.0f,								// 251 (6)
		0.5f, -1.0f, -1.0f,				0.0f, 0.0f, -1.0f,								// 252 (10)
		0.5f, -0.7f, -1.0f,				0.0f, 0.0f, -1.0f,								// 253 (62)
		0.57f, -0.3f, -1.0f,			0.0f, 0.0f, -1.0f,								// 254 (231)
		0.65f, -0.6f, -1.0f,			0.0f, 0.0f, -1.0f,								// 255 (64)
		0.65f, -0.35f, -1.0f,			0.0f, 0.0f, -1.0f,								// 256 (206)
		0.85f, -0.4f, -1.0f,			0.0f, 0.0f, -1.0f,								// 257 (200)
		0.8f, -0.7f, -1.0f,				0.0f, 0.0f, -1.0f,								// 258 (66)
		1.0f, -0.5f, -1.0f,				0.0f, 0.0f, -1.0f,								// 259 (104)
		0.8f, -1.0f, -1.0f,				0.0f, 0.0f, -1.0f,								// 260 (14)
		1.0f, -1.0f, -1.0f,				0.0f, 0.0f, -1.0f,								// 261 (18)

		0.22f, 0.25f, -0.9f,			-0.0523554f, 0.209421f, -0.976423f,				// 262
		0.16f, 0.25f, -0.9f,			-0.0523554f, 0.209421f, -0.976423f,				// 263
		0.22f, -0.3f, -1.0f,			-0.0523554f, 0.209421f, -0.976423f,				// 264
		0.16f, -0.3f, -1.0f,			-0.0523554f, 0.209421f, -0.976423f,				// 265
		0.05f, 0.25f, -0.9f,			-0.0523554f, 0.209421f, -0.976423f,				// 266 (156)
		0.16f, 0.15f, -0.9181818f,		-0.0523554f, 0.209421f, -0.976423f,				// 267
		0.05f, 0.15f, -0.9181818f,		-0.0523554f, 0.209421f, -0.976423f,				// 268
		-0.075f, 0.21875f, -0.9f,		-0.0523554f, 0.209421f, -0.976423f,				// 269 (181)
		-0.113f, 0.115f, -0.92f,		-0.0523554f, 0.209421f, -0.976423f,				// 270
		0.3f, 0.25f, -0.9f,				-0.0523554f, 0.209421f, -0.976423f,				// 271 (160)
		0.3f, 0.15f, -0.9181818f,		-0.0523554f, 0.209421f, -0.976423f,				// 272
		0.22f, 0.15f, -0.9181818f,		-0.0523554f, 0.209421f, -0.976423f,				// 273
		0.425f, 0.21875f, -0.9f,		-0.0523554f, 0.209421f, -0.976423f,				// 274 (235)
		0.454f, 0.115f, -0.92f,			-0.0523554f, 0.209421f, -0.976423f,				// 275
	};

	unsigned int carIndices[]{
		// bottom of car
		1, 0, 5,
		0, 2, 5,
		4, 3, 15,
		4, 15, 16,
		7, 6, 10,
		7, 10, 11,
		9, 8, 13,
		8, 12, 13,
		17, 14, 18,
		17, 18, 19,

		// front right wheel well
		20, 26, 27,
		20, 27, 21,
		33, 28, 31,
		33, 31, 35,
		33, 35, 23,
		25, 30, 29,
		25, 29, 24,
		22, 32, 34,
		22, 34, 39,
		37, 38, 40,
		37, 40, 36,

		// front left wheel well
		42, 49, 41,
		42, 47, 49,
		53, 48, 51,
		53, 48, 55,
		53, 55, 44,
		45, 52, 46,
		50, 52, 46,
		43, 57, 56,
		43, 56, 60,
		59, 61, 58,
		61, 54, 58,

		// back right wheel well
		62, 68, 63,
		68, 69, 63,
		75, 70, 81,
		70, 73, 81,
		75, 81, 65,
		72, 71, 66,
		72, 66, 67,
		74, 64, 76,
		64, 78, 76,
		77, 80, 79,
		80, 82, 79,

		// back left wheel well
		83, 84, 91,
		91, 84, 89,
		93, 90, 102,
		102, 90, 96,
		102, 96, 86,
		88, 87, 94,
		88, 94, 92,
		85, 95, 97,
		85, 97, 99,
		98, 100, 101,
		101, 100, 103,

		// rear bumper
		104, 106, 107,
		104, 107, 105,

		// top view of car
		108, 111, 110,
		109, 111, 108,
		115, 114, 112,
		115, 112, 113,
		117, 118, 116,
		120, 121, 119,
		123, 116, 122,
		125, 119, 124,
		126, 127, 128,
		127, 129, 128,
		130, 132, 131,
		133, 130, 132,
		136, 135, 134,
		136, 134, 137,
		139, 138, 141,
		138, 140, 141,
		143, 142, 145,
		143, 145, 144,
		146, 147, 149,
		147, 149, 148,
		150, 152, 153,
		150, 153, 151,
		156, 154, 155,
		156, 155, 157,
		160, 158, 159,
		160, 159, 161,
		164, 162, 163,
		164, 163, 165,
		168, 166, 167,
		171, 170, 169,
		174, 173, 172,
		175, 176, 177,
		178, 179, 180,
		179, 180, 181,
		182, 183, 184,
		183, 184, 185,
		189, 187, 186,
		189, 186, 188,
		193, 191, 190,
		193, 190, 192,
		197, 195, 194,
		197, 194, 196,
		199, 200, 198,
		201, 202, 203,
		204, 205, 206,
		204, 206, 207,
		209, 210, 208,
		213, 212, 211,
		217, 215, 216,
		217, 215, 214,
		218, 219, 220,
		221, 222, 223,
		224, 226, 225,
		228, 227, 229,
		232, 230, 231,
		234, 233, 235,
		238, 237, 236,
		241, 240, 239,

		// right side of car
		244, 242, 243,
		245, 244, 243,
		246, 245, 243,
		248, 245, 246,
		248, 247, 245,
		249, 247, 248,
		249, 250, 247,
		249, 251, 250,
		249, 252, 251,
		253, 252, 249,
		254, 253, 249,
		254, 255, 253,
		256, 255, 254,
		257, 255, 256,
		257, 258, 255,
		257, 259, 258,
		259, 261, 258,
		258, 261, 260,
		263, 262, 264,
		263, 264, 265,
		263, 267, 268,
		263, 268, 266,
		266, 268, 270,
		266, 270, 269,
		273, 262, 272,
		262, 272, 271,
		272, 271, 274,
		274, 275, 272
	};

	float tireVerts[] = {
		// back face
		-0.25f, 1.0f, -1.0f,
		0.25f, 1.0f, -1.0f,
		0.6f, 0.85f, -1.0f,
		0.8f, 0.65f, -1.0f,
		1.0f, 0.25f, -1.0f,
		1.0f, -0.25f, -1.0f,
		0.8f, -0.65f, -1.0f,
		0.6f, -0.85f, -1.0f,
		0.25f, -1.0f, -1.0f,
		-0.25f, -1.0f, -1.0f,
		-0.6f, -0.85f, -1.0f,
		-0.8f, -0.65f, -1.0f,
		-1.0f, -0.25f, -1.0f,
		-1.0f, 0.25f, -1.0f,
		-0.8f, 0.65f, -1.0f,
		-0.6f, 0.85f, -1.0f,

		// front face
		- 0.25f, 1.0f, 1.0f,
		0.25f, 1.0f, 1.0f,
		0.6f, 0.85f, 1.0f,
		0.8f, 0.65f, 1.0f,
		1.0f, 0.25f, 1.0f,
		1.0f, -0.25f, 1.0f,
		0.8f, -0.65f, 1.0f,
		0.6f, -0.85f, 1.0f,
		0.25f, -1.0f, 1.0f,
		-0.25f, -1.0f, 1.0f,
		-0.6f, -0.85f, 1.0f,
		-0.8f, -0.65f, 1.0f,
		-1.0f, -0.25f, 1.0f,
		-1.0f, 0.25f, 1.0f,
		-0.8f, 0.65f, 1.0f,
		-0.6f, 0.85f, 1.0f
	};

	unsigned int tireIndices[] = {
		0, 15, 1,
		15, 2, 1,
		15, 3, 2,
		15, 14, 3,
		14, 4, 3,
		14, 13, 4,
		13, 5, 4,
		13, 12, 5,
		12, 11, 5,
		11, 6, 5,
		11, 10, 6,
		10, 7, 6,
		10, 9, 7,
		9, 8, 7,

		16, 31, 17,
		31, 18, 17,
		31, 19, 18,
		31, 30, 19,
		30, 20, 19,
		30, 29, 20,
		29, 21, 20,
		29, 28, 21,
		28, 27, 21,
		27, 22, 21,
		27, 26, 22,
		26, 23, 22,
		26, 25, 23,
		25, 24, 23,

		0, 17, 16,
		0, 1, 17,
		1, 18, 17,
		1, 2, 18,
		2, 19, 18,
		2, 3, 19,
		3, 20, 19,
		3, 4, 20,
		4, 21, 20,
		4, 5, 21,
		5, 22, 21,
		5, 6, 22,
		6, 23, 22,
		6, 7, 23,
		7, 24, 23,
		7, 8, 24,
		8, 25, 24,
		8, 9, 25,
		9, 26, 25,
		9, 10, 26,
		10, 27, 26,
		10, 11, 27,
		11, 28, 27,
		11, 12, 28,
		12, 29, 28,
		12, 13, 29,
		13, 30, 29,
		13, 14, 30,
		14, 31, 30,
		14, 15, 31,
		15, 16, 31,
		15, 0, 16
	};

	glm::vec3 tirePos[] = {
		glm::vec3(-1.62f, -0.95f, -4.08f),
		glm::vec3(-1.62f, -0.95f, -5.92f),
		glm::vec3(1.62f, -0.95f, -4.08f),
		glm::vec3(1.62f, -0.95f, -5.92f)
	};

	float roadVerts[] = {
		-5.0f, -1.0f, -5.0f,
		5.0f, -1.0f, 5.0f,
		-5.0f, -1.0f, 5.0f,

		-5.0f, -1.0f, -5.0f,
		5.0f, -1.0f, -5.0f,
		5.0f, -1.0f, 5.0f
	};

	glm::vec3 roadTilePos[] = {
		glm::vec3(70.0f, -0.26f, -3.0f),
		glm::vec3(60.0f, -0.26f, -3.0f),
		glm::vec3(50.0f, -0.26f, -3.0f),
		glm::vec3(40.0f, -0.26f, -3.0f),
		glm::vec3(30.0f, -0.26f, -3.0f),
		glm::vec3(20.0f, -0.26f, -3.0f),
		glm::vec3(10.0f, -0.26f, -3.0f),
		glm::vec3(0.0f, -0.26f, -3.0f),
		glm::vec3(-10.0f, -0.26f, -3.0f),
		glm::vec3(-20.0f, -0.26f, -3.0f),
		glm::vec3(-30.0f, -0.26f, -3.0f),
		glm::vec3(-40.0f, -0.26f, -3.0f),
		glm::vec3(-50.0f, -0.26f, -3.0f),
		glm::vec3(-60.0f, -0.26f, -3.0f),
		glm::vec3(-70.0f, -0.26f, -3.0f),
		glm::vec3(-80.0f, -0.26f, -3.0f)
	};

	glm::vec3 grassTilePos[] = {
		// on left side of road
		glm::vec3(70.0f, -0.26f, 12.0f),
		glm::vec3(60.0f, -0.26f, 12.0f),
		glm::vec3(50.0f, -0.26f, 12.0f),
		glm::vec3(40.0f, -0.26f, 12.0f),
		glm::vec3(30.0f, -0.26f, 12.0f),
		glm::vec3(20.0f, -0.26f, 12.0f),
		glm::vec3(10.0f, -0.26f, 12.0f),
		glm::vec3(0.0f, -0.26f, 12.0f),
		glm::vec3(-10.0f, -0.26f, 12.0f),
		glm::vec3(-20.0f, -0.26f, 12.0f),
		glm::vec3(-30.0f, -0.26f, 12.0f),
		glm::vec3(-40.0f, -0.26f, 12.0f),
		glm::vec3(-50.0f, -0.26f, 12.0f),
		glm::vec3(-60.0f, -0.26f, 12.0f),
		glm::vec3(-70.0f, -0.26f, 12.0f),
		glm::vec3(-80.0f, -0.26f, 12.0f),

		// on right side of road
		glm::vec3(70.0f, -0.26f, -18.0f),
		glm::vec3(60.0f, -0.26f, -18.0f),
		glm::vec3(50.0f, -0.26f, -18.0f),
		glm::vec3(40.0f, -0.26f, -18.0f),
		glm::vec3(30.0f, -0.26f, -18.0f),
		glm::vec3(20.0f, -0.26f, -18.0f),
		glm::vec3(10.0f, -0.26f, -18.0f),
		glm::vec3(0.0f, -0.26f, -18.0f),
		glm::vec3(-10.0f, -0.26f, -18.0f),
		glm::vec3(-20.0f, -0.26f, -18.0f),
		glm::vec3(-30.0f, -0.26f, -18.0f),
		glm::vec3(-40.0f, -0.26f, -18.0f),
		glm::vec3(-50.0f, -0.26f, -18.0f),
		glm::vec3(-60.0f, -0.26f, -18.0f),
		glm::vec3(-70.0f, -0.26f, -18.0f),
		glm::vec3(-80.0f, -0.26f, -18.0f)
	};

	unsigned int carVBO, carVAO, carEBO;

	glGenVertexArrays(1, &carVAO);
	glGenBuffers(1, &carVBO);
	glGenBuffers(1, &carEBO);

	glBindBuffer(GL_ARRAY_BUFFER, carVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(carVerts), carVerts, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, carEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(carIndices), carIndices, GL_STATIC_DRAW);

	glBindVertexArray(carVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int tireVBO, tireVAO, tireEBO;

	glGenVertexArrays(1, &tireVAO);
	glGenBuffers(1, &tireVBO);
	glGenBuffers(1, &tireEBO);

	glBindBuffer(GL_ARRAY_BUFFER, tireVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tireVerts), tireVerts, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tireEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tireIndices), tireIndices, GL_STATIC_DRAW);

	glBindVertexArray(tireVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int roadVBO, roadVAO;

	glGenVertexArrays(1, &roadVAO);
	glGenBuffers(1, &roadVBO);

	glBindBuffer(GL_ARRAY_BUFFER, roadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(roadVerts), roadVerts, GL_STATIC_DRAW);

	glBindVertexArray(roadVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	carShader.Use();
	carShader.SetVec3("Color", glm::vec3(0.8f, 0.0f, 0.0f));
	carShader.SetVec3("LightPos", camera.pos);
	carShader.SetVec3("LightColor", glm::vec3(1.0f, 1.0f, 1.0f));

	interiorShader.Use();
	interiorShader.SetVec3("Color", glm::vec3(0.8f, 0.0f, 0.0f));
	interiorShader.SetVec3("LightPos", camera.pos);
	interiorShader.SetVec3("LightColor", glm::vec3(1.0f, 1.0f, 1.0f));

	tireShader.Use();
	tireShader.SetVec3("Color", glm::vec3(0.05f, 0.05f, 0.05f));

	roadShader.Use();
	roadShader.SetVec3("Color", glm::vec3(0.43f, 0.43f, 0.42f));

	grassShader.Use();
	grassShader.SetVec3("Color", glm::vec3(0.045f, 0.25f, 0.065f));

	int faces = 0;
	float timer = 1.0f;

	// Loops execution of main gameplay functions
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = (float)glfwGetTime();
		dt = currentFrame - prevFrame;
		prevFrame = currentFrame;

		ProcessInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		view = camera.GetViewMatrix();

		rotation += (450.0f * dt);

		carShader.Use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		model = glm::scale(model, glm::vec3(2.5f, 1.0f, 1.1f));
		carShader.SetMat4("model", model);
		carShader.SetMat4("projection", projection);
		carShader.SetMat4("view", view);
		carShader.SetVec3("LightPos", camera.pos);

		/*
		if (faces < ((sizeof(carIndices) / sizeof(unsigned int)) / 3)) {
			glBindVertexArray(carVAO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, carEBO);
			glDrawElements(GL_TRIANGLES, faces * 3, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			timer -= dt;

			if (timer <= 0) {
				faces += 1;
				timer = 0.1f;
				cout << faces << endl;
			}
		}
		else {
			glBindVertexArray(carVAO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, carEBO);
			glDrawElements(GL_TRIANGLES, sizeof(carIndices), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		*/
		
		glBindVertexArray(carVAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, carEBO);
		glDrawElements(GL_TRIANGLES, sizeof(carIndices), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		

		tireShader.Use();
		tireShader.SetMat4("projection", projection);
		tireShader.SetMat4("view", view);

		
		for (int i = 0; i < 4; i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, tirePos[i]);
			model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.18f));
			tireShader.SetMat4("model", model);

			glBindVertexArray(tireVAO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tireEBO);
			glDrawElements(GL_TRIANGLES, sizeof(tireIndices), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		

		roadShader.Use();
		roadShader.SetMat4("projection", projection);
		roadShader.SetMat4("view", view);

		for (int i = 0; i < 16; i++) {
			roadTilePos[i].x += (15.0f * dt);

			if (roadTilePos[i].x >= 80.0f) {
				roadTilePos[i].x = -80.0f;
			}

			model = glm::mat4(1.0f);
			model = glm::translate(model, roadTilePos[i]);
			roadShader.SetMat4("model", model);

			glBindVertexArray(roadVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}

		grassShader.Use();
		grassShader.SetMat4("projection", projection);
		grassShader.SetMat4("view", view);

		for (int i = 0; i < 32; i++) {
			grassTilePos[i].x += (15.0f * dt);

			if (grassTilePos[i].x >= 80.0f) {
				grassTilePos[i].x = -80.0f;
			}

			model = glm::mat4(1.0f);
			model = glm::translate(model, grassTilePos[i]);
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 2.0f));
			roadShader.SetMat4("model", model);

			glBindVertexArray(roadVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void ProcessInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, dt);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, dt);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, dt);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, dt);
	}
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_1 && action == GLFW_PRESS && !pressed1) {
		if (wireframe == false) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			wireframe = true;
			pressed1 = true;
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			wireframe = false;
			pressed1 = true;
		}
	}
	else if (key == GLFW_KEY_1 && action == GLFW_RELEASE) {
		pressed1 = false;
	}
}

void FramebufferCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos) {
	float xpos = static_cast<float>(xPos);
	float ypos = static_cast<float>(yPos);

	if (firstMouse) {
		prevX = xpos;
		prevY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - prevX;
	float yoffset = ypos - prevY;
	prevX = xpos;
	prevY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	
}