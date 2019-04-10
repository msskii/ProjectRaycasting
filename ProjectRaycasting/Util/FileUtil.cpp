//
//  FileUtil.cpp
//  ProjectRaycasting
//
//  Created by Keanu Gleixner on 07.04.19.
//  Copyright © 2019 Keanu Gleixner. All rights reserved.
//

#include "FileUtil.hpp"

util::filedata util::readFile(const char *path) {
#ifdef __APPLE__
    FILE *f = fopen(path, "rb");
#else
    FILE *f;
    fopen_s(&f, path, "rb");
#endif
    
    if(!f) {
        printf("[ERROR] File %s could not be opened\n", path);
        return { nullptr, 0 };
    }
    
    filedata fd;
    fseek(f, 0, SEEK_END);
    fd.size = ftell(f);
    fd.data = (uint8_t*) malloc(fd.size + 1);
    fd.data[fd.size] = 0;
    fseek(f, 0, SEEK_SET);
    fread(fd.data, 1, fd.size, f);
    fclose(f);
    
    return fd;
}
