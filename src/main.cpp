#include <iostream>
#include <fstream>

#include <stdlib.h>
#include <stdio.h>
#include <GLFW/glfw3.h>

#include "video_reader.hpp"

using namespace std;

bool load_frame(const char* filename, int* width, int* height, unsigned char** data);

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

	// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// set range of view to rendering
	// glViewport(0, 0, 640, 480);
	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	int frame_width, frame_height;
	unsigned char* frame_data;

	VideoReaderState vr_state;
	if (!video_reader_open(&vr_state, "/Users/seunguklee/dev/squat.mp4")) {
		cout << "Couldn't open video file\n";
		return 1;
	}

	const int frame_width = vr_state.width;
	const int frame_height = vr_state.height;
	uint8_t* frame_data = new uint8_t[frame_width * frame_height * 4];
	
	if (!video_reader_read_frame(&vr_state, frame_data)) {
		cout << "Couldn't load video frame\n";
		return 1;
	}
	video_reader_close(&vr_state);

	if (!load_frame("/Users/seunguklee/dev/squat.mp4", &frame_width, &frame_height, &frame_data)) {
		printf("Couldn't load video frame\n");
		return 1;
	}

	glfwMakeContextCurrent(window);

	GLuint tex_handle;
	glGenTextures(1, &tex_handle);
	glBindTexture(GL_TEXTURE_2D, tex_handle);
	glBindTexture(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame_width, frame_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, frame_data);


	// render loop
	while (!glfwWindowShouldClose(window)) {

		// input
		// processInput(window);

		// rendering
		// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// glDrawPixels(100, 100, GL_RGB, GL_UNSIGNED_BYTE, data);


		// glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// glClear(GL_COLOR_BUFFER_BIT);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set up orphographic projection
		int window_width, window_height;
		glfwGetFramebufferSize(window, &window_width, &window_height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, window_width, 0, window_height, -1, 1);
		glMatrixMode(GL_MODELVIEW);

		// Renderging texture data
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex_handle);
		glBegin(GL_QUADS);
			glTexCoord2d(0, 0); glVertex2i(200, 200);
			glTexCoord2d(1, 0); glVertex2i(200 + frame_width, 200);
			glTexCoord2d(1, 1); glVertex2i(200 + frame_width, 200 + frame_height);
			glTexCoord2d(0, 1); glVertex2i(200, 200 + frame_height);
		glEnd();
		glDisable(GL_TEXTURE_2D);

		glfwSwapBuffers(window);
		glfwWaitEvents();
		// check and call events and swap the buffers
		// glfwPollEvents();
		// glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

           
