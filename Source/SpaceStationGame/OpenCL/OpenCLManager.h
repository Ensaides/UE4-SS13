#pragma once

#include "Manager.h"
#include "OpenCLProgram.h"
#include <vector>
#include <thread>
#include "OpenCLManager.generated.h"

UCLASS()
class AOpenCLManager : public AManager
{
	GENERATED_UCLASS_BODY()

	bool bCompletedInitialization;

	virtual void Initialize() override;

private:
	virtual void CompleteInitialization() { bCompletedInitialization = true; };

public:
	// Program members
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Programs")
		TArray<FOpenCLProgram> Programs;

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Add a source file to a program. Call this before you compile the program, of course.", DisplayName = "Add File To Program"), Category = "Program")
		void ProgramAddFile(int32 ProgramIndex, FString Filepath);

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Create the program from the source files. Does not compile the program. Call this after you add all of your source files to the program.", DisplayName = "Create Program"), Category = "Program")
		void CreateProgram(int32 ProgramIndex);

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Compile and link a program.", DisplayName = "Compile Program"), Category = "Program")
		void CompileProgram(int32 ProgramIndex);

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Automatically create kernels from a compiled program. This will fill up the program's kernels array.", DisplayName = "Create Program Kernels"), Category = "Program")
		void CreateKernels(int32 ProgramIndex, int32 NumKernels);

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Add a kernel to a compiled program. This will add the kernel to the kernels array and it will return the index of the new kernel.", DisplayName = "Add Program Kernel"), Category = "Program")
		int32 AddKernel(int32 ProgramIndex, FString KernelName);


public:
	bool CheckError(cl_int Error);

	bool CheckError(cl_int Error, cl_program* Program, cl_device_id* Device);

private:
	std::thread OpenCLThread;

	std::vector<cl_platform_id> Platforms;

	std::vector<cl_device_id> Devices;

	// Global command queue
	cl_command_queue CommandQueue;

	// Global context
	cl_context Context;

public:
	cl_context* GetContext() { return &Context; };

	cl_command_queue* GetCommandQueue() { return &CommandQueue; };
	
	std::vector<cl_device_id> GetDeviceList() { return Devices; };
};