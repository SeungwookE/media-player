#include <iostream>
#include <fstream>

#include <stdlib.h>
#include <stdio.h>
#include <GLFW/glfw3.h>

using namespace std;

// callback for the case when window resized.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

// process control input
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(int argc, const char** argv) {
	GLFWwindow * window;

	if (!glfwInit()) {
		printf("Couldn't init GLFW\n");
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// set range of view to rendering
	glViewport(0, 0, 640, 480);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 

	unsigned char* data = new unsigned char[100 * 100 * 3];
	for (int y=0; y<100; ++y) {
		for (int x=0; x<100; x++) {
			data[y * 100 * 3 + x * 3    ] = 0xff;
			data[y * 100 * 3 + x * 3 + 1] = 0x00;
			data[y * 100 * 3 + x * 3 + 2] = 0x00;                                                                                                           
		}
	}

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};  

	glfwMakeContextCurrent(window);
	// render loop
	while (!glfwWindowShouldClose(window)) {

		// input
		processInput(window);

		// rendering
		// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// glDrawPixels(100, 100, GL_RGB, GL_UNSIGNED_BYTE, data);


		// glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// glClear(GL_COLOR_BUFFER_BIT);

		

		// check and call events and swap the buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

           