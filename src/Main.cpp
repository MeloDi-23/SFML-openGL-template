#include <SFML/Window.hpp>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <iostream>

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glsimple.hpp>
GLint load_shader();
void draw_openGL(float);
GLuint shader_from_file(const char* file, GLuint type);
void init_gl();
void prepare_cams();
void prepare_models();
void delete_resources();
extern float ratio;
float ratio = 0.f;
int main() {
    // Set up the SFML window with an OpenGL context
    sf::Window window(sf::VideoMode(1000, 800), "OpenGL with SFML", sf::Style::Default, sf::ContextSettings(24));
    window.setVerticalSyncEnabled(true);
    auto size = window.getSize();
    ratio = (float)size.x/size.y;

    init_gl();
    load_shader();
    prepare_models();
    prepare_cams();

	bool pause = false;
    // Main loop
    sf::Clock clock;

    while (window.isOpen()) {
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Button::Left)
					pause = !pause;
				break;
			default:
				break;
			}
        }
        auto time = clock.getElapsedTime();

        clock.restart();
        draw_openGL(time.asMilliseconds());

        // Display the window contents
        window.display();
    }
    delete_resources();
    return 0;
}
