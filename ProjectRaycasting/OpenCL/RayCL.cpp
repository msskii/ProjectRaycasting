//
//  RayCL.cpp
//  ProjectRaycasting
//
//  Created by Aaron Hodel on 11.04.19.
//  Copyright © 2019 Keanu Gleixner. All rights reserved.
//

#include "RayCL.hpp"


static void onError(const char *errinfo, const void *privateInfo, size_t cb, void *userInfo) {
    
}

static float* screenSize = new float[2]{ 500, 500 };
static size_t* screenSizet = new size_t[2]{ (size_t)screenSize[0], (size_t) screenSize[1] };

static unsigned char* colorData = new unsigned char[4 * screenSize[0] * screenSize[1]];

cl_device_id device;
cl_context context;
cl_command_queue queue;

cl_program program;
cl_kernel getOutput;

cl_mem colorOut;
cl_event evt;

void initCL() {
    cl_platform_id selectedPlatform;
    clGetPlatformIDs(1, &selectedPlatform, NULL);
    clGetDeviceIDs(selectedPlatform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    
    cl_context_properties *props = new cl_context_properties[3]{
        CL_CONTEXT_PLATFORM, (cl_context_properties)selectedPlatform, // only config: Which platform...
        (cl_context_properties)0 // Terminator
    };
    
    context = clCreateContext(props, 1, &device, onError, NULL, NULL);
    
    
    util::filedata d = util::readFile("assets/test.cl");
    //printf("[GLEAN][DEBUG] Compiling CL-Source: \n%s\n", d.data);
    program = clCreateProgramWithSource(context, 1, (const char**)&d.data, NULL, NULL);
    cl_int buildErr = clBuildProgram(program, 1, &device, "", NULL, NULL);
    
    if (buildErr != CL_SUCCESS) {
        printf("[GLEAN][ERROR] Couldn't compile OpenCL program: \n");
        size_t len = 0;
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &len);
        char *errorLog = new char[len + 1];
        errorLog[len] = 0;
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, len, errorLog, NULL);
        printf("[GLEAN][ERROR] %s\n", errorLog);
        delete[] errorLog;
    }
    
    queue = clCreateCommandQueue(context, device, 0, NULL);
    
    colorOut = clCreateBuffer(context, CL_MEM_READ_WRITE, screenSize[0] * screenSize[1] * sizeof(cl_uchar4), NULL, NULL);
    getOutput = clCreateKernel(program, "getOutput", NULL);
    
    cl_int err = clSetKernelArg(getOutput, 2, sizeof(cl_float2), screenSize);
    err = clSetKernelArg(getOutput, 3, sizeof(colorOut), &colorOut);
}

unsigned char* testRay(math::Vector<4> cameraPos, math::Vector<2> cameraAngles) {
    cl_int err = clSetKernelArg(getOutput, 0, sizeof(cl_float4), &cameraPos[0]);
    err = clSetKernelArg(getOutput, 1, sizeof(cl_float2), &cameraAngles[0]);
    
    err = clEnqueueNDRangeKernel(queue, getOutput, 2, NULL, screenSizet, NULL, NULL, NULL, &evt);
    clWaitForEvents(1, &evt);
    
    clEnqueueReadBuffer(queue, colorOut, CL_TRUE, 0, sizeof(cl_uchar4) * screenSize[0] * screenSize[1], colorData, NULL, NULL, NULL);
    return colorData;
}
