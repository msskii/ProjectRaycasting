//
//  graphicsHandler.cpp
//  ProjectRaycasting
//
//  Created by Keanu Gleixner on 06.04.19.
//  Copyright © 2019 Keanu Gleixner. All rights reserved.
//

#include "graphicsHandler.hpp"
#include "../config.h"

#include <iostream>


graphicsHandler::graphicsHandler(){
    
}

void graphicsHandler::init(){
    //generating the vertex buffer and vertex array
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    
    //load shader
    shader = loadFromFiles("./assets/image.vert", "./assets/image.frag");
    shader->bind();
    
    //set uniforms
    std::vector<float> test = {0.0f,1.0f,0.0f};
    shader->uniformi("test", test);
    
    //update buffers
    updateBuffers();
    
    //set attrib pointer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    //init raycaster
    rayCaster = new RayCaster(200,200,PI_HALF,PI_HALF);
    std::vector<std::vector<float>> Spheres;
    std::vector<float> Sphere = {0.0f, 0.0f, 3.0f, 1.0f};
    Spheres.push_back(Sphere);
    rayCaster->Spheres = Spheres;
    rayCaster->getVerticies(verticies, indices, cameraPos, cameraAngle);
    updateBuffers();
    
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    cameraPos.z = -1;
    
    initCL();
}

void graphicsHandler::render() {    
    unsigned char* values = testRay(physics::createVec(cameraPos.x, cameraPos.y, cameraPos.z, 0), physics::createVec(cameraAngle.x, cameraAngle.y));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 500, 500, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, values);
    
    glBindVertexArray(VAO);

    shader->bind();
    shader->uniformi("tex", 0);
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
}

void graphicsHandler::updateBuffers(){
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 5 * 4, new float[5 * 4] {
        -1,  1, 0, 0, 0,
        1,  1, 0, 1, 0,
        1, -1, 0, 1, 1,
        -1, -1, 0, 0, 1
    }, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, new unsigned int[6] {0, 1, 2, 2, 3, 0}, GL_STATIC_DRAW);}
