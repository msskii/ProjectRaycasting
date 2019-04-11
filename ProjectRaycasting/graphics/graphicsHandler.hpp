//
//  graphicsHandler.hpp
//  ProjectRaycasting
//
//  Created by Keanu Gleixner on 06.04.19.
//  Copyright © 2019 Keanu Gleixner. All rights reserved.
//

#ifndef graphicsHandler_hpp
#define graphicsHandler_hpp

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include <OpenGl/glu.h>
#include <GLUT/glut.h>

#include <stdio.h>
#include <vector>
#include "../Shaders/shaderHandler.hpp"
#include "rayCaster.hpp"

#include "../OpenCL/RayCL.hpp"
#include "../Util/PhysicsUtil.h"

class graphicsHandler{
private:
    std::vector<float> verticies;
    std::vector<unsigned int> indices;
    
    unsigned int VBO,EBO;
    unsigned int VAO;
    GLuint texID;
    
    Shader *shader;
    RayCaster *rayCaster;
    
public:
    glm::vec3 cameraPos;
    glm::vec3 cameraAngle;
    

    graphicsHandler();
    void init();
    void render();
    void updateBuffers();
    
    
    
};

#endif /* graphicsHandler_hpp */
