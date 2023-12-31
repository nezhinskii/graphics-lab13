#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <GL/glew.h>

#include "imgui.h" // necessary for ImGui::*, imgui-SFML.h doesn't include imgui.h

#include "imgui-SFML.h" // for ImGui::SFML::* functions and SFML-specific overloads

#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <iostream>
#include "camera.h"
#include "painter.h"
#include "lib/ImGuiFileDialog/ImGuiFileDialog.h"
#include "painter_state.h"

using namespace sf;

void modelPickerWidget(std::string title, std::string* path, Model*& model) {
	if (ImGui::Button(title.c_str()))
		ImGuiFileDialog::Instance()->OpenDialog(title.c_str(), "Choose object", ".obj", ".");
	if ((*path).empty()) {
		ImGui::Text("Empty");
	}
	else {
		ImGui::Text((*path).c_str());
	}

	if (ImGuiFileDialog::Instance()->Display(title.c_str()))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			(*path) = filePathName;
			model = new Model(filePathName);
		}
		else {
			(*path).clear();
		}
		ImGuiFileDialog::Instance()->Close();
	}
}

int main() {
	sf::RenderWindow window(sf::VideoMode(600, 600), "Lab 13", sf::Style::Default, sf::ContextSettings(24));
	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);
	window.setActive(true);

	Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f), 1.0f);
	auto state = PainterState(camera);
	auto painter = Painter(state);

	painter.Init();
	GLboolean firstMouse = true;
	GLfloat lastX = 0, lastY = 0;
	sf::Clock clock;
	GLboolean isFocused = false;
	sf::Vector2i centerWindow;

	if (!ImGui::SFML::Init(window)) return -1;

	sf::Clock deltaClock;
	while (window.isOpen()) {

		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(window, event);
			bool isImGuiHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);

			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::Resized) {
				glViewport(
					0,
					0,
					event.size.width,
					event.size.height
				);
				painter.state.camera.processResize(event.size.width, event.size.height);
			}
			else if (event.type == sf::Event::KeyPressed) {
				painter.state.camera.processKeyboard(event.key.code);
			}
			else if (event.type == sf::Event::MouseMoved && isFocused) {
				GLfloat xoffset = event.mouseMove.x - centerWindow.x;
				GLfloat yoffset = centerWindow.y - event.mouseMove.y;
				lastX = event.mouseMove.x;
				lastY = event.mouseMove.y;
				sf::Mouse::setPosition(centerWindow, window);
				painter.state.camera.processMouseMovement(xoffset, yoffset);
			}

			if (!isImGuiHovered && event.type == sf::Event::MouseButtonPressed) {
				isFocused = true;
				window.setMouseCursorVisible(false);
				window.setMouseCursorGrabbed(true);
				centerWindow.x = window.getSize().x / 2;
				centerWindow.y = window.getSize().y / 2;
				sf::Mouse::setPosition(centerWindow, window);
				auto pos = sf::Mouse::getPosition();
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape || event.type == sf::Event::LostFocus) {
				window.setMouseCursorVisible(true);
				window.setMouseCursorGrabbed(false);
				isFocused = false;
				firstMouse = true;
			}
		}

		ImGui::SFML::Update(window, deltaClock.restart());


		ImGui::Begin("Lab 13");
		modelPickerWidget("Pick central model", &painter.state.centralPath, painter.state.centralModel);
		modelPickerWidget("Pick satellite model", &painter.state.satellitePath, painter.state.satelliteModel);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		painter.Draw();

		ImGui::End();
		ImGui::SFML::Render(window);
		window.display();
	}

	painter.Release();
	return 0;
}