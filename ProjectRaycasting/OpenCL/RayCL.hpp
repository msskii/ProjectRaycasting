//
//  RayCL.hpp
//  ProjectRaycasting
//
//  Created by Aaron Hodel on 11.04.19.
//  Copyright © 2019 Keanu Gleixner. All rights reserved.
//

#ifndef RayCL_hpp
#define RayCL_hpp

#include <OpenCL/opencl.h>
#include "../Util/FileUtil.hpp"
#include "../Util//Vector.hpp"

extern void initCL();
extern unsigned char* testRay(math::Vector<4> cameraPos, math::Vector<2> cameraAngles);

#endif /* RayCL_hpp */
