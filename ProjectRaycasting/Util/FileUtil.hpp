//
//  FileUtil.hpp
//  ProjectRaycasting
//
//  Created by Keanu Gleixner on 07.04.19.
//  Copyright © 2019 Keanu Gleixner. All rights reserved.
//

#ifndef FileUtil_hpp
#define FileUtil_hpp

#include <stdio.h>

#include <stdint.h>
#include <fstream>
#include <stdlib.h>
#include <string>

namespace util {
    
    typedef struct {
        uint8_t *data;
        uint64_t size;
    } filedata;
    
    extern filedata readFile(const char *path);
    
}

#endif /* FileUtil_hpp */
