//
//  window.cpp
//  ProjectRaycasting
//
//  Created by Keanu Gleixner on 04.04.19.
//  Copyright © 2019 Keanu Gleixner. All rights reserved.
//

#include "window.hpp"


Window::Window(int window_w, int window_h){
    window_height = window_h;
    window_width = window_w;
    windowinit(window_w, window_h);
}

int Window::windowinit(int window_w, int window_h){
    
    // Initialize GLFW
    glfwInit();
    
    // Define version and compatibility settings
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
    
    if (window_width == 0 && window_height == 0){
        const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        window_width = mode->width;
        window_height = mode->height;
    }
    // Create OpenGL window and context
    window = glfwCreateWindow(window_width, window_height, "OpenGL", NULL, NULL);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwMakeContextCurrent(window);
    
    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    // Check for window creation failure
    if (!window)
    {
        // Terminate GLFW
        glfwTerminate();
        return -1;
    }
    glViewport(0, 0, window_width, window_height);
    
    
    //init graphics handler
    graphicsHandler.init();
    
    return 0;
}


int Window::run(){
    std::chrono::high_resolution_clock fpsLimiter;
    std::chrono::high_resolution_clock::time_point timeStartFPSCounter = fpsLimiter.now();
    std::chrono::high_resolution_clock::time_point timeStartFrame = fpsLimiter.now();
    std::chrono::high_resolution_clock::time_point timeEndFrame;
    
    while (!glfwWindowShouldClose(window)) {
        //poll events and swap buffer
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        //handle input
        handleUserInput();
        
        //override the buffer with black
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //rendering
        graphicsHandler.render();
        
        
        
        //FPS stuff
        //fps counter
        if ((fpsLimiter.now() - timeStartFPSCounter) >= std::chrono::milliseconds(1000)) {
            printf("fps: %d \n",fps);
            fps = 0;
            timeStartFPSCounter = fpsLimiter.now();
        }
        //fps limiter
        if(DESIRED_FPS != 0){
            timeEndFrame = fpsLimiter.now();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000 / DESIRED_FPS) - (timeEndFrame - timeStartFrame));
            timeStartFrame = fpsLimiter.now();
        }
        fps++;
    }
    return 0;
}

void Window::framebuffer_size_callback(GLFWwindow *window, int width, int height){
    glViewport(0, 0, width, height);
    window_width = width;
    window_height = height;
    printf("width:%d  height:%d \n", width, height);
    
}

void Window::handleUserInput(){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        graphicsHandler.cameraPos[2] += 0.1f;
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        graphicsHandler.cameraPos[2] -= 0.1f;
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        graphicsHandler.cameraPos[0] -= 0.1f;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        graphicsHandler.cameraPos[0] += 0.1f;
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        graphicsHandler.cameraPos[1] += 0.1f;
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        graphicsHandler.cameraPos[1] -= 0.1f;
    }
    
}
