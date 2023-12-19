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

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "lib/stb_image.h";

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

	GLuint VBO, VAO;

	GLuint numVertices;

	GLuint textures[20];
	GLuint numTextures = 0;

	const static GLuint shadersNumber = 1;

	const char* VertexShaderSource[shadersNumber] = {
		R"(
		#version 330 core

		layout (location = 0) in vec3 position;
		layout (location = 1) in vec2 texCoord;

		out vec2 textureCoord;

		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;

		void main() {
			gl_Position = projection * view * model * vec4(position, 1.0);
			textureCoord = texCoord;
		}
		)"
	};

	const char* FragShaderSources[shadersNumber] = {
		R"(
		#version 330 core

		in vec2 textureCoord;

		out vec4 fragColor;

		uniform sampler2D textures[20];
		uniform int numTextures;

		void main() {
			vec4 finalColor = vec4(1.0);

			for (int i = 0; i < numTextures; ++i) {
				vec4 textureColor = texture(textures[i], textureCoord);
				finalColor *= textureColor;
			}

			fragColor = finalColor;
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
		numVertices = 36;
		GLfloat cube[] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,   1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,   1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f,   0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 1.0f,   1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 1.0f,   0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 1.0f,   0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 1.0f,   1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f,   1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 1.0f,   1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,   1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 1.0f,   0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f,   1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 1.0f,   1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 1.0f,   1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f,   0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f,   0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,   1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 1.0f,   0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f,   0.0f, 1.0f
		};


		//GLuint VBO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);


		glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
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

	void loadTexture(const char* texturePath, GLuint& textureID) {
		int width, height, channels;
		unsigned char* image = stbi_load(texturePath, &width, &height, &channels, STBI_rgb);

		if (!image) {
			std::cerr << "Failed to load texture: " << texturePath << std::endl;
			return;
		}

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(image);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
public:
	Painter(PainterState& painterState) : state(painterState) {}

	PainterState state;

	GLfloat angle = 0.0f;

	void loadModel(const std::string& path) {
		numTextures = 0;
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			std::cerr << "Error loading model: " << importer.GetErrorString() << std::endl;
			return;
		}

		int count = 0;

		std::string modelDirectory = path;
		modelDirectory = modelDirectory.substr(0, modelDirectory.find_last_of('\\'));

		for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {

			aiMesh* mesh = scene->mMeshes[i];

			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			for (unsigned int j = 0; j < AI_TEXTURE_TYPE_MAX; ++j) {
				aiTextureType textureType = static_cast<aiTextureType>(j);
				aiString texturePath;

				if (material->GetTexture(textureType, 0, &texturePath) == AI_SUCCESS) {
					std::string fullPath = modelDirectory + '\\' + texturePath.C_Str();
					GLuint textureID;
					loadTexture(fullPath.c_str(), textureID);
					
					textures[numTextures++] = textureID;
				}
			}

			count += mesh->mNumVertices;

			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);

			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(aiVector3D) * mesh->mNumVertices, &mesh->mVertices[0], GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);

			/*if (mesh->HasTextureCoords(0)) {
				GLuint textureVBO;
				glGenBuffers(1, &textureVBO);
				glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(aiVector3D) * mesh->mNumVertices, &mesh->mTextureCoords[0][0], GL_STATIC_DRAW);

				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
				glEnableVertexAttribArray(1);
			}*/

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindVertexArray(0);
		}
		numVertices = count;
	}


	void Draw() {
		glEnable(GL_DEPTH_TEST);

		GLint currentAttrib;
		glUseProgram(Programs[0]);

		glUniform1i(glGetUniformLocation(Programs[0], "numTextures"), numTextures);

		for (int i = 0; i < numTextures; ++i) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, textures[i]);
			glUniform1i(glGetUniformLocation(Programs[0], ("textures[" + std::to_string(i) + "]").c_str()), i);
		}

		angle += 0.005;
		rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.5f, 0.0f));
		auto viewMatrix = state.camera.getViewMatrix();
		auto projectionMatrix = state.camera.getProjectionMatrix();
		glBindVertexArray(VAO);
		glUniformMatrix4fv(glGetUniformLocation(Programs[0], "model"), 1, GL_FALSE, glm::value_ptr(rotationMatrix));
		glUniformMatrix4fv(glGetUniformLocation(Programs[0], "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(glGetUniformLocation(Programs[0], "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		glDrawArrays(GL_TRIANGLES, 0, numVertices);
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
