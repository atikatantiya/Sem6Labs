#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

#define MAX_SOURCE_SIZE (0x100000)

int L6q3(void) {
    // Create the two input vectors
	int i, size, num;
	printf("Enter size: ");
	scanf("%d", &size);
	//const int LIST_SIZE = 1024;
    int *inp = (int*)malloc(sizeof(int)*size);
    int *sorted = (int*)malloc(sizeof(int)*size);
    printf("Enter %d values: ", size);
    for(i = 0; i < size; i++) {
    	scanf("%d", inp+i);
    }

    // Load the kernel source code into the array source_str
    FILE *fp;
    char *source_str_even, *source_str_odd;
    size_t source_size;

    fp = fopen("even.cl", "r");
    if (!fp) {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }
    source_str_even = (char*)malloc(MAX_SOURCE_SIZE);
    source_size = fread( source_str_even, 1, MAX_SOURCE_SIZE, fp);
    fclose( fp );

    fp = fopen("odd.cl", "r");
	if (!fp) {
		fprintf(stderr, "Failed to load kernel.\n");
		exit(1);
	}
	source_str_odd = (char*)malloc(MAX_SOURCE_SIZE);
	source_size = fread( source_str_odd, 1, MAX_SOURCE_SIZE, fp);
	fclose( fp );

    // Get platform and device information
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs( platform_id, CL_DEVICE_TYPE_DEFAULT, 1,
            &device_id, &ret_num_devices);
    printf("clGetDeviceIDs: %d\n", ret);

    // Create an OpenCL context
    cl_context context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &ret);

    // Create a command queue
    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

    // Create memory buffers on the device for each vector
    cl_mem inp_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE,
            size * sizeof(int), NULL, &ret);

    // Copy the list to its respective memory buffers
    ret = clEnqueueWriteBuffer(command_queue, inp_mem_obj, CL_TRUE, 0,
            size * sizeof(int), inp, 0, NULL, NULL);
    printf("clEnqueueWriteBuffer: %d\n", ret);


    // Create a program from the kernel source
    cl_program program_even = clCreateProgramWithSource(context, 1,
            (const char **)&source_str_even, (const size_t *)&source_size, &ret);
    cl_program program_odd = clCreateProgramWithSource(context, 1,
                (const char **)&source_str_odd, (const size_t *)&source_size, &ret);

    // Build the program
    ret = clBuildProgram(program_even, 1, &device_id, NULL, NULL, NULL);
    printf("clBuildProgram: %d\n", ret);
    ret = clBuildProgram(program_odd, 1, &device_id, NULL, NULL, NULL);
    printf("clBuildProgram: %d\n", ret);

    if (ret == CL_BUILD_PROGRAM_FAILURE) {
        // Determine the size of the log
        size_t log_size;
        clGetProgramBuildInfo(program_even, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

        // Allocate memory for the log
        char *log = (char *) malloc(log_size);

        // Get the log
        clGetProgramBuildInfo(program_even, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

        // Print the log
        printf("%s\n", log);
    }
    // Create the OpenCL kernel
    cl_kernel kernel_even = clCreateKernel(program_even, "even", &ret);
    printf("clCreateKernel: %d\n", ret);
    cl_kernel kernel_odd = clCreateKernel(program_odd, "odd", &ret);
    printf("clCreateKernel: %d\n", ret);

    // Set the arguments of the kernel
    ret = clSetKernelArg(kernel_even, 0, sizeof(cl_mem), (void *)&inp_mem_obj);
    printf("clSetKernelArg even: %d\n", ret);
    ret = clSetKernelArg(kernel_odd, 0, sizeof(cl_mem), (void *)&inp_mem_obj);
    printf("clSetKernelArg odd: %d\n", ret);

    // Execute the OpenCL kernel on the list
    size_t global_item_size = size/2; // Process the entire lists
    size_t local_item_size = 1; // Divide work items into groups of 64
    for(int i=0;i<size/2;++i){
		ret = clEnqueueNDRangeKernel(command_queue, kernel_even, 1, NULL,
				&global_item_size, &local_item_size, 0, NULL, NULL);
		printf("clEnqueueNDRangeKernel even: %d\n", ret);
		ret = clFinish(command_queue);
		ret = clEnqueueNDRangeKernel(command_queue, kernel_odd, 1, NULL,
				&global_item_size, &local_item_size, 0, NULL, NULL);
		printf("clEnqueueNDRangeKernel even: %d\n", ret);
		ret = clFinish(command_queue);
    }

    // Read the memory buffer C on the device to the local variable C
    ret = clEnqueueReadBuffer(command_queue, inp_mem_obj, CL_TRUE, 0,
            size * sizeof(int), sorted, 0, NULL, NULL);
    printf("clEnqueueReadBuffer: %d\n", ret);

    // Display the result to the screen
    for(i = 0; i < size; i++)
        printf("%d ",sorted[i]);

    // Clean up
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel_even);
    ret = clReleaseKernel(kernel_odd);
    ret = clReleaseProgram(program_even);
    ret = clReleaseProgram(program_odd);
    ret = clReleaseMemObject(inp_mem_obj);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);
    free(inp);
    free(sorted);
    return 0;
}
