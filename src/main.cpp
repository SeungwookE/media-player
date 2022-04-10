#include <iostream>
#include <fstream>

#include <stdlib.h>
#include <stdio.h>
#include <GLFW/glfw3.h>

#include "video_reader.hpp"

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

	window = glfwCreateWindow(1000, 620, "Hello World", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// set range of view to rendering
	// glViewport(0, 0, 640, 480);
	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    
    // opening all resources for ffmpeg library
	VideoReaderState vr_state;
	if (!video_reader_open(&vr_state, "/Users/seunguklee/dev/mtk-test.mp4")) {
		cout << "Couldn't open video file\n";
		return 1;
	}

	glfwMakeContextCurrent(window);

	// Generate Texture
	GLuint tex_handle;
	glGenTextures(1, &tex_handle);
	glBindTexture(GL_TEXTURE_2D, tex_handle);
	glBindTexture(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Allocate frame buffer
	const int frame_width = vr_state.width;
	const int frame_height = vr_state.height;
	uint8_t* frame_data = new uint8_t[frame_width * frame_height * 4];
	
    double first_frame_time;
    
	// render loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set up orphographic projection
        int window_width, window_height;
        glfwGetFramebufferSize(window, &window_width, &window_height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, window_width, window_height, 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);

        // Read a new frame and load it into texture
        int64_t pts;
        if (!video_reader_read_frame(&vr_state, frame_data, &pts)) {
            printf("Couldn't load video frame\n");
            return 1;
        }
        
        static bool first_frame = true;
        if (first_frame) {
            glfwSetTime(0.0);
            first_frame = false;
        }
        
        double pt_in_seconds = pts * (double)vr_state.time_base.num / (double)vr_state.time_base.den;
        while (pt_in_seconds > glfwGetTime()) {
            glfwWaitEventsTimeout(pt_in_seconds - glfwGetTime());
        }

        glBindTexture(GL_TEXTURE_2D, tex_handle);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame_width, frame_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, frame_data);

        // Render whatever you want
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, tex_handle);
        glBegin(GL_QUADS);
            glTexCoord2d(0,0); glVertex2i(200, 200);
            glTexCoord2d(1,0); glVertex2i(200 + frame_width, 200);
            glTexCoord2d(1,1); glVertex2i(200 + frame_width, 200 + frame_height);
            glTexCoord2d(0,1); glVertex2i(200, 200 + frame_height);
        glEnd();
        glDisable(GL_TEXTURE_2D);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


	// glfwTerminate();
	video_reader_close(&vr_state);

	return 0;
}

           
