//
//  rayCaster.hpp
//  ProjectRaycasting
//
//  Created by Keanu Gleixner on 08.04.19.
//  Copyright © 2019 Keanu Gleixner. All rights reserved.
//

#ifndef rayCaster_hpp
#define rayCaster_hpp

#include <stdio.h>
#include <vector>
#include "math.h"
#include "../Util/PhysicsUtil.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class RayCaster{
private:
    int amountRaysHorizontal;
    int amountRaysVertical;
    double horizontalFOVAngle;
    double verticalFOVAngle;
    
    double horizontalAngle;
    
    std::vector<bool> outPut;
    std::vector<float> tempres;
    
    glm::vec3 *directionRays;
public:
    RayCaster(int resH,  int resV, double hFOVAngle, double vFOVAngle);
    void castDemRaysBoi();
    std::vector<bool> getOutput(glm::vec3 cameraPos, glm::vec3 cameraAngle);
    float minDist(glm::vec3 pos);
    bool rayWalk(glm::vec3 dir, glm::vec3 Position);
    
    
    std::vector<std::vector<float>> Spheres;
    
    void getVerticies(std::vector<float> &verticies, std::vector<unsigned int> &indices, glm::vec3 cameraPos, glm::vec3 cameraAngle);
    
    void testPrint();
    
};

#endif /* rayCaster_hpp */
