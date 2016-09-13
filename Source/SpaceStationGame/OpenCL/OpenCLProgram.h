#pragma once

#include "OpenCL.h"
#include <vector>
#include <string>
#include "StringHelpers.h"
#include "OpenCLProgram.generated.h"

// Internal wrapper for OpenCL programs. The OpenCL manager works with this
class AOpenCLManager;
class FOpenCLKernel;

USTRUCT(BlueprintType)
struct SPACESTATIONGAME_API FOpenCLProgram
{
	GENERATED_BODY()

	// Add files to the program source
	void AddFile(FString FilePath);

	// Create the program from the source code files
	void CreateProgram(AOpenCLManager* Manager);

	// Build (compile and link) a program
	void Compile(AOpenCLManager* Manager);

	void Compile(AOpenCLManager* Manager, FString inCompileOptions);

	void Compile(AOpenCLManager* Manager, FString inCompileOptions, cl_context* context, cl_device_id* device_list);

	// Create the kernels from a built program
	void CreateKernels(AOpenCLManager* Manager, int32 NumKernels);

	// Manually add kernels to a built program
	// Returns the index of the new kernel
	int32 AddKernel(AOpenCLManager* Manager, FString KernelName);

public:
	cl_program Program;
	std::vector<cl_kernel> Kernels;
	std::vector<std::string> Files;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Compile")
		FString CompileOptions;

	FOpenCLProgram()
	{
		CompileOptions = "";
		Program = cl_program();
	}
};