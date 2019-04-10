//
//  shaderHandler.hpp
//  ProjectRaycasting
//
//  Created by Keanu Gleixner on 07.04.19.
//  Copyright © 2019 Keanu Gleixner. All rights reserved.
//

#ifndef shaderHandler_hpp
#define shaderHandler_hpp

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include <OpenGl/glu.h>
#include <GLUT/glut.h>

#include "../Util/FileUtil.hpp"
#include <stdio.h>
#include <vector>



class Shader {
private:
    GLuint shaderID;
    
    
public:
    Shader(const char *vertSrc, const char *fragSrc);
    
    inline void uniformi(const char *name, int v) { glUniform1i(glGetUniformLocation(shaderID, name), v); };
    inline void uniformi(const char *name, float v) { glUniform1f(glGetUniformLocation(shaderID, name), v); };

    void uniformi(const char *name, std::vector<float> v);
    
    inline void bind() { glUseProgram(shaderID); }
    
};

extern Shader *loadFromFiles(const char *vertPath, const char *fragPath);

#endif /* shaderHandler_hpp */
