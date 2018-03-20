#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <vector>
#include <pcl/common/time.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/sample_consensus/sac_model_line.h>
#include <pcl/sample_consensus/ransac.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/cloud_iterator.h>
int main()
{

    //https://streamhpc.com/blog/2013-04-28/opencl-error-codes/
    cl_device_id device_id = NULL;
    cl_context context = NULL;
    cl_command_queue command_queue = NULL;
    cl_mem memobj , resobj ;

    cl_program program = NULL;
    cl_kernel kernel = NULL;
    cl_platform_id platform_id = NULL;
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret;
    int input[10]= {1,2,3,4,5,6,7,8,9,10};

    int length = 10 ;

    pcl::PointXYZ point(0,0,0);

    FILE *fp;
    char fileName[] = "/home/tuan/Desktop/OpenCLBA-Local/OpenCLBA/hello.cl";
    char *source_str;
    size_t source_size;
    size_t work_units=10;
    /* Load the source code containing the kernel*/
    fp = fopen(fileName, "r");
    if (!fp) {
    fprintf(stderr, "Failed to load kernel.\n");
    exit(1);
    }
    source_str = (char*)malloc(0x100000);
    source_size = fread(source_str,1,0x100000, fp);
    fclose(fp);

    ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    std::cout<<ret<<" code"<<std::endl;

    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
    std::cout<<ret<<" code"<<std::endl;

    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
    std::cout<<ret<<" code"<<std::endl;

    command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
    //Check Concept of memory
    std::cout<<ret<<" code"<<std::endl;



    program = clCreateProgramWithSource(context,1,(const char**)&source_str, (const size_t*)&source_size, &ret);
    std::cout<<ret<<" code"<<std::endl;

    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    std::cout<<ret<<" code"<<std::endl;
    if (ret == CL_BUILD_PROGRAM_FAILURE) {
        // Determine the size of the log
        size_t log_size;
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

        // Allocate memory for the log
        char *log = (char *) malloc(log_size);

        // Get the log
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

        // Print the log
        std::cout<<log<<std::endl;
    }
    std::cout<<"pointer" << input << std::endl;


    resobj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, length * sizeof(int), NULL, &ret);
    std::cout<<ret<<" code"<<std::endl;


    memobj = clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR,length * sizeof(int), input, &ret);
    std::cout<<ret<<" code"<<std::endl;


    kernel = clCreateKernel(program, "hello", &ret);
    std::cout<<ret<<" code"<<std::endl;

    ret = clSetKernelArg(kernel,0, sizeof(memobj),(void *)&memobj);
    std::cout<<ret<<" code"<<std::endl;

    ret = clSetKernelArg(kernel,1, sizeof(resobj),(void *)&resobj);
    std::cout<<ret<<" code"<<std::endl;

    ret = clEnqueueNDRangeKernel(command_queue, kernel,1, NULL,  &work_units,NULL, 0,NULL,NULL);
    std::cout<<ret<<" code"<<std::endl;

    ret = clEnqueueReadBuffer(command_queue, resobj, CL_TRUE, 0, 10 * sizeof(int),&input[0], 0, NULL, NULL);
    std::cout<<ret<<" code"<<std::endl;

    ret = clFlush(command_queue);
    std::cout<<ret<<" code"<<std::endl;

    ret = clFinish(command_queue);
    std::cout<<ret<<" code"<<std::endl;

    ret = clReleaseKernel(kernel);
    std::cout<<ret<<" code"<<std::endl;

    ret = clReleaseProgram(program);
    std::cout<<ret<<" code"<<std::endl;

    ret = clReleaseMemObject(memobj);
    std::cout<<ret<<" code"<<std::endl;

    ret = clReleaseCommandQueue(command_queue);
    std::cout<<ret<<" code"<<std::endl;

    ret = clReleaseContext(context);
    for (int i = 0 ; i <10 ; i++) {
        std::cout<<input[i]<<" "<<std::endl;
    }

    return 0;
}
