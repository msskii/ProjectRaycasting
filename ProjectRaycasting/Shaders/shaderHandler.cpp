//
//  shaderHandler.cpp
//  ProjectRaycasting
//
//  Created by Keanu Gleixner on 07.04.19.
//  Copyright © 2019 Keanu Gleixner. All rights reserved.
//

#include "shaderHandler.hpp"

Shader::Shader(const char *vertSrc, const char *fragSrc) {
    GLuint vid = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vid, 1, &vertSrc, nullptr);
    glCompileShader(vid);
    
    GLint status;
    glGetShaderiv(vid, GL_COMPILE_STATUS, &status);
    if(status != GL_TRUE) {
        printf("[ERROR] Couldn't compile vertex shader:\n%s\n", vertSrc);
        glGetShaderiv(vid, GL_INFO_LOG_LENGTH, &status);
        char* errorlog = (char*) malloc(status + 1);
        glGetShaderInfoLog(vid, status, &status, errorlog);
        printf("%s\n", errorlog);
        free(errorlog);
#ifdef DEBUG_GL
        exit(1);
#endif
    }
    
    GLuint fid = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fid, 1, &fragSrc, nullptr);
    glCompileShader(fid);
    
    glGetShaderiv(fid, GL_COMPILE_STATUS, &status);
    if(status != GL_TRUE) {
        printf("[ERROR] Couldn't compile fragment shader:\n%s\n", fragSrc);
        glGetShaderiv(fid, GL_INFO_LOG_LENGTH, &status);
        char* errorlog = (char*) malloc(status + 1);
        glGetShaderInfoLog(fid, status, &status, errorlog);
        printf("%s\n", errorlog);
        free(errorlog);
#ifdef DEBUG_GL
        exit(1);
#endif
    }
    
    shaderID = glCreateProgram();
    glAttachShader(shaderID, vid);
    glAttachShader(shaderID, fid);
    glLinkProgram(shaderID);
    glDeleteShader(vid);
    glDeleteShader(fid);
    
    glGetProgramiv(shaderID, GL_LINK_STATUS, &status);
    if(status != GL_TRUE) {
        printf("[ERROR] Couldn't link shader:\n");
        glGetProgramiv(shaderID, GL_INFO_LOG_LENGTH, &status);
        char* errorlog = (char*) malloc(status + 1);
        glGetProgramInfoLog(shaderID, status, &status, errorlog);
        printf("%s\n", errorlog);
        free(errorlog);
#ifdef DEBUG_GL
        exit(1);
#endif
    }
}


static util::filedata loadShaderRecursive(const char *filePath, int depth = 9) {
    if(!depth) return util::readFile(filePath); // Don't load more than 10 files recursively, exit condition
    
    util::filedata fd = util::readFile(filePath);
    
    printf("[INFO] Loading shader: %s\n", filePath);
    
    int s = 0;
    while(s < fd.size) {
        if(fd.data[s++] == '\n') { // newline char
            while(fd.data[s] == ' ' || fd.data[s] == '\t') ++s;
            if(fd.data[s] == '#') { // We're in a preprocessor statement
                int len = 0; // Length of directive
                int start = s; // Start at current offset in file
                while(fd.data[s++] != '\n') ++len; // Search for next newline
                //len--;
                //s--;
                
                // Load the preprocessor directive into str
                char *str = (char*) malloc(len + 1);
                memcpy(str, fd.data + start, len);
                str[len] = 0;
                // Check if is a not known directive (Filesystem, ...)
                if(!memcmp(str, "#include ", 9)) {
                    int i = 9;
                    while(str[i++] != '"') ; // Search for beginning of string
                    int j = i + 1;
                    while(str[j++] != '"') ; // Search for end of string
                    
                    char *filename = (char*) malloc(j - i);
                    memcpy(filename, str + i, j - i);
                    filename[j - i - 1] = 0;
                    
                    util::filedata loadedFile = loadShaderRecursive(filename, depth - 1);
                    
                    // data + start until j+1
                    uint64_t newsize = fd.size - s + start + loadedFile.size;
                    char *newFile = (char*) malloc((size_t) newsize + 1);
                    memcpy(newFile, fd.data, start);
                    memcpy(newFile + start, loadedFile.data, (size_t) loadedFile.size);
                    memcpy(newFile + start + loadedFile.size, fd.data + start + j + 1, (size_t) fd.size - s);
                    newFile[newsize] = 0;
                    
                    free(loadedFile.data); // Free newly loaded file
                    free(filename); // Free the filename string
                    free(fd.data); // Free the datas of the file, as we have a new one
                    fd.data = (uint8_t*) newFile; // Set the pointer in fd to the new one
                    fd.size = newsize; // Update the size
                    
                    //printf("%s", fd.data);
                    s = start - 1; // Go back to the start of the line
                }
                
                --s;
                free(str);
            }
        }
    }
    
    return fd;
}

Shader *loadFromFiles(const char *vertPath, const char *fragPath) {
    return new Shader((const char*) loadShaderRecursive(vertPath).data, (const char*) loadShaderRecursive(fragPath).data);
}

void Shader::uniformi(const char *name, std::vector<float> v){
    switch (v.size()) {
        case 1:
            glUniform1f(glGetUniformLocation(shaderID, name), v[0]);
            break;
        case 2:
            glUniform2f(glGetUniformLocation(shaderID, name), v[0], v[1]);
            break;
        case 3:
            glUniform3f(glGetUniformLocation(shaderID, name), v[0], v[1], v[2]);
            break;
        case 4:
            glUniform4f(glGetUniformLocation(shaderID, name), v[0], v[1], v[2], v[3]);
        default:
            break;
    }
}
