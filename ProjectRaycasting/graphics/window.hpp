//
//  window.hpp
//  ProjectRaycasting
//
//  Created by Keanu Gleixner on 04.04.19.
//  Copyright © 2019 Keanu Gleixner. All rights reserved.
//

#ifndef window_hpp
#define window_hpp

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include <OpenGl/glu.h>
#include <GLUT/glut.h>
#include <stdio.h>

#include "config.h"
#include "graphicsHandler.hpp"

#include <time.h>
#include <chrono>
#include <thread>


class Window{
    
private:
    GLFWwindow* window;
    graphicsHandler graphicsHandler;
    int fps = 0;
    
public:
    
    Window(int window_w, int window_h);
    int windowinit(int window_w, int window_h);
    int run();
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void handleUserInput();
    
};

#endif /* window_hpp */
