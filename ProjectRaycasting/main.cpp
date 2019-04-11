//
//  main.cpp
//  ProjectRaycasting
//
//  Created by Keanu Gleixner on 04.04.19.
//  Copyright © 2019 Keanu Gleixner. All rights reserved.
//

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include <OpenGl/glu.h>
#include <GLUT/glut.h>

#include "window.hpp"

int main(int argc, const char * argv[]) {
    
    //create the window
    Window *window = new Window(500,500);
    
    window->run();
    
    
    // Terminate GLFW
    glfwTerminate();
    return 0;
}
