//
//  rayCaster.cpp
//  ProjectRaycasting
//
//  Created by Keanu Gleixner on 08.04.19.
//  Copyright © 2019 Keanu Gleixner. All rights reserved.
//

#include "rayCaster.hpp"
#include "math.h"




#define minStep 0.05f
#define maxDist 10.0f

RayCaster::RayCaster(int resH,  int resV, double hFOVAngle, double vFOVAngle) : amountRaysHorizontal(resH), amountRaysVertical(resV), horizontalFOVAngle(hFOVAngle), verticalFOVAngle(vFOVAngle){
   glm::vec4 viewDir(0.0f,0.0f,1.0f,1.0f);
    directionRays = new glm::vec3[amountRaysHorizontal * amountRaysVertical];
    for (int i = 0; i < (amountRaysVertical * amountRaysHorizontal); i++) {
        float verticalAngle = (verticalFOVAngle / (amountRaysVertical - 1) * int(i/amountRaysVertical)) - verticalFOVAngle / 2;
        glm::mat4 rotatex(1.0f);
        rotatex = glm::rotate(rotatex, float(verticalAngle), glm::vec3(1.0f,0.0f,0.0f));
        //left n right
        glm::mat4 rotatez(1.0f);
        float horizontalAngle = (horizontalFOVAngle / (amountRaysHorizontal - 1) * int(i % amountRaysHorizontal)) - horizontalFOVAngle / 2;
        rotatez = glm::rotate(rotatez, float(horizontalAngle), glm::vec3(0.0f,1.0f,0.0f));
        
        directionRays[i] = rotatex * rotatez * viewDir;
    }
}

std::vector<bool> RayCaster::getOutput(glm::vec3 cameraPos, glm::vec3 cameraAngle){
    outPut.clear();
    
    //cast rays from botleft to topright
    glm::vec3 position;
    bool hitssmth;
    float distWalked;
    float stepSize;
    for (int i = 0; i < (amountRaysVertical * amountRaysHorizontal); i++) {
        hitssmth = false;
        position = cameraPos;
        distWalked = 0;
        while (distWalked < maxDist) {
            stepSize = minDist(position);
            
            //it hits
            if(stepSize < minStep){
                hitssmth = true;
                break;
            }
            //else walk
            distWalked += stepSize;
            position += directionRays[i] * stepSize;
        }
        outPut.push_back(hitssmth);
    }
    
    return outPut;
}

bool RayCaster::rayWalk(glm::vec3 dir, glm::vec3 Position){
    float distWalked = 0;
    while (distWalked < maxDist) {
        float stepSize = minDist(Position);
        
        //it hits
        if(stepSize < minStep){
            return true;
        }
        //else walk
        distWalked += stepSize;
        Position += dir * stepSize;
    }
    return false;
}


void RayCaster::testPrint(){
    std::string output;
    for(int i = 0; i < amountRaysVertical; i++){
        for (int j = 0; j < amountRaysHorizontal; j++) {
            if (outPut[i * amountRaysHorizontal+ j]) {
                output.append("1 ");
            }else output.append("0 ");
        }
        output.append("\n");
    }
    output.append("\n");
    printf(output.data());
}


float RayCaster::minDist(glm::vec3 pos){
    float min = 1000;
    for (int i = 0; i < Spheres.size(); i++) {
        float sphereRadius = Spheres[i][3];
        float distToSphere = sqrt(pow(pos[0] - Spheres[i][0],2) + pow(pos[1] - Spheres[i][1],2) + pow(pos[2] - Spheres[i][2],2)) - sphereRadius;
        if (distToSphere < min) {
            min = distToSphere;
        }
    }
    return min;
}

void RayCaster::getVerticies(std::vector<float> &verticies, std::vector<unsigned int> &indices, glm::vec3 cameraPos, glm::vec3 cameraAngle){
    verticies.clear();
    indices.clear();
    
    glm::vec3 colour(0.5f);
    
    getOutput(cameraPos, cameraAngle);
    for (int i = 0; i < amountRaysVertical; i++) {
        for (int j = 0; j < amountRaysHorizontal; j++) {
            //position
            verticies.push_back((j * 2.0f / amountRaysHorizontal) - 1.0f);
            verticies.push_back((i * 2.0f / amountRaysVertical) - 1.0f);
            verticies.push_back(0.0f);
            //color
            if (outPut[i * amountRaysHorizontal + j]) {
                verticies.push_back(colour[0]);
                verticies.push_back(colour[1]);
                verticies.push_back(colour[2]);
            }else{
                verticies.push_back(0.0f);
                verticies.push_back(0.0f);
                verticies.push_back(0.0f);
            }
        }
    }
    
    //indicies
    for (int i = 0; i < amountRaysVertical - 1; i++) {
        for (int j = 0; j < amountRaysHorizontal - 1; j++) {
            
            indices.push_back(i * amountRaysHorizontal + j);
            indices.push_back((i + 1) * amountRaysHorizontal + j);
            indices.push_back(i * amountRaysHorizontal + j + 1);
            
            indices.push_back((i + 1) * amountRaysHorizontal + j + 1);
            indices.push_back((i + 1) * amountRaysHorizontal + j);
            indices.push_back(i * amountRaysHorizontal + j + 1);
            
        }
    }
}
