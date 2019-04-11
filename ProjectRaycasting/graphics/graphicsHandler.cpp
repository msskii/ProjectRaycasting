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
    shader = loadFromFiles("./shader.vert", "./shader.frag");
    shader->bind();
    
    //set uniforms
    std::vector<float> test = {0.0f,1.0f,0.0f};
    shader->uniformi("test", test);
    
    //update buffers
    updateBuffers();
    
    //set attrib pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    //init raycaster
    rayCaster = new RayCaster(200,200,PI_HALF,PI_HALF);
    std::vector<std::vector<float>> Spheres;
    std::vector<float> Sphere = {0.0f, 0.0f, 3.0f, 1.0f};
    Spheres.push_back(Sphere);
    rayCaster->Spheres = Spheres;
    rayCaster->getVerticies(verticies, indices, cameraPos, cameraAngle);
    updateBuffers();
}

void graphicsHandler::render(){
    rayCaster->getVerticies(verticies, indices, cameraPos, cameraAngle);
    updateBuffers();
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, int(indices.size()), GL_UNSIGNED_INT, 0);
    
}

void graphicsHandler::updateBuffers(){
    //the vertecies
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticies.size(), verticies.data(), GL_STATIC_DRAW);
    
    //the indicies
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * indices.size(), indices.data(), GL_STATIC_DRAW);
}
