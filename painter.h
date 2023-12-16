#pragma once

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include "painter_state.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace sf;

class Painter {

	struct MyTexCoord {
		GLfloat x;
		GLfloat y;
	};

	struct MyVertex {
		GLfloat x;
		GLfloat y;
		GLfloat z;
	};

	struct MyColor {
		GLfloat r;
		GLfloat g;
		GLfloat b;
		GLfloat a;
	};


	GLuint Programs[1];

	GLuint cubeCtVAO;

	const static GLuint shadersNumber = 1;

	const char* VertexShaderSource[shadersNumber] = {
		R"(
		#version 330 core
		layout (location = 0) in vec3 coord;
		layout (location = 1) in vec3 inColor;

		out vec4 color;

		uniform mat4 model;
		uniform mat4 view; 
		uniform mat4 projection;

		void main() {
			gl_Position = projection * view * model * vec4(coord, 1.0);
			color = vec4(inColor, 1.0);
		}
		)"
	};

	const char* FragShaderSources[shadersNumber] = {
		R"(
		#version 330 core
		out vec4 fragColor;

		in vec4 color;

		void main() {
			fragColor = color;
		}
		)"
	};


	void ShaderLog(unsigned int shader)
	{
		int infologLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLen);
		if (infologLen > 1)
		{
			int charsWritten = 0;
			std::vector<char> infoLog(infologLen);
			glGetShaderInfoLog(shader, infologLen, &charsWritten, infoLog.data());
			std::cout << "InfoLog: " << infoLog.data() << std::endl;
		}
	}


	void InitVBO() {
		srand(time(0));
		InitCube();
	}

	void InitCube() {
		GLfloat cube[] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f
		};


		GLuint cubeCtVBO;
		glGenVertexArrays(1, &cubeCtVAO);
		glBindVertexArray(cubeCtVAO);
		glGenBuffers(1, &cubeCtVBO);
		glBindBuffer(GL_ARRAY_BUFFER, cubeCtVBO);


		glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
	}

	void InitShader() {
		GLuint vShaders[shadersNumber];

		for (int i = 0; i < shadersNumber; i++) {
			vShaders[i] = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vShaders[i], 1, &(VertexShaderSource[i]), NULL);

			glCompileShader(vShaders[i]);
			std::cout << "vertex shader" << i << std::endl;
			ShaderLog(vShaders[i]);
		}

		GLuint fShaders[shadersNumber];

		for (int i = 0; i < shadersNumber; i++) {
			fShaders[i] = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fShaders[i], 1, &(FragShaderSources[i]), NULL);
			glCompileShader(fShaders[i]);
			std::cout << "fragment shader" << i << std::endl;
			ShaderLog(fShaders[i]);
		}


		for (int i = 0; i < shadersNumber; i++) {
			Programs[i] = glCreateProgram();
			glAttachShader(Programs[i], vShaders[i]);
			glAttachShader(Programs[i], fShaders[i]);
			glLinkProgram(Programs[i]);
			int link_ok;
			glGetProgramiv(Programs[i], GL_LINK_STATUS, &link_ok);
			if (!link_ok) {
				std::cout << "error attach shaders \n";
				return;
			}
		}
	}

	void ReleaseShader() {
		glUseProgram(0);
		for (int i = 0; i < shadersNumber; i++) {
			glDeleteProgram(Programs[i]);
		}
	}

	void ReleaseVBO() {
	}

	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.5f, 0.0f));

public:
	Painter(PainterState& painterState) : state(painterState) {}

	PainterState state;

	GLfloat angle = 0.0f;

	void Draw() {
		glEnable(GL_DEPTH_TEST);
		GLint currentAttrib;
		glUseProgram(Programs[0]);
		angle += 0.005;
		rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.5f, 0.0f));
		auto viewMatrix = state.camera.getViewMatrix();
		auto projectionMatrix = state.camera.getProjectionMatrix();
		glBindVertexArray(cubeCtVAO);
		glUniformMatrix4fv(glGetUniformLocation(Programs[0], "model"), 1, GL_FALSE, glm::value_ptr(rotationMatrix));
		glUniformMatrix4fv(glGetUniformLocation(Programs[0], "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(glGetUniformLocation(Programs[0], "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glUseProgram(0);
	}

	void Init() {
		glewInit();
		InitShader();
		InitVBO();
	}

	void Release() {
		ReleaseShader();
		ReleaseVBO();
	}

};
