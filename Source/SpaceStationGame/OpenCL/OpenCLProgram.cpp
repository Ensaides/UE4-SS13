#include "SpaceStationGame.h"
#include "OpenCLManager.h"
#include "OpenCLProgram.h"

void FOpenCLProgram::AddFile(FString FilePath)
{
	// Load the file to a string
	const TCHAR* TCHARFilePath = *FilePath;
	FString File;

	if (!FFileHelper::LoadFileToString(File, TCHARFilePath))
	{
		UE_LOG(SpaceStationGameLog, Warning, TEXT("WARNING: Failed to open OpenCL file: %s"), *FilePath);
	}
	else
	{
		std::string FileString = Helpers::narrow(Helpers::ConvertToWString(File));

		Files.push_back(FileString);
	}
}

void FOpenCLProgram::CreateProgram(AOpenCLManager* Manager)
{
	cl_int Error = 0;

	auto Context = Manager->GetContext();

	Program = clCreateProgramWithSource(*Context, Files.size(), (const char**)Files.data(), NULL, &Error);

	Manager->CheckError(Error);
}

void FOpenCLProgram::CreateKernels(AOpenCLManager* Manager, int32 NumKernels)
{
	unsigned long UnsignedNumKernels = NumKernels;

	Kernels.resize(UnsignedNumKernels);

	cl_int Error = 0;

	Error = clCreateKernelsInProgram(Program, UnsignedNumKernels, Kernels.data(), NULL);

	Manager->CheckError(Error);
}

int32 FOpenCLProgram::AddKernel(AOpenCLManager* Manager, FString KernelName)
{
	int32 KernelIndex = -1;
	cl_int Error = 0;
	cl_kernel Kernel;

	std::string kernelname = Helpers::narrow(Helpers::ConvertToWString(KernelName));

	// Create the kernel
	Kernel = clCreateKernel(Program, kernelname.c_str(), &Error);

	if (!Manager->CheckError(Error))
	{
		Kernels.push_back(Kernel);

		KernelIndex = Kernels.size() - 1;
	}

	return KernelIndex;
}

void FOpenCLProgram::Compile(AOpenCLManager* Manager)
{
	Compile(Manager, CompileOptions);
}

void FOpenCLProgram::Compile(AOpenCLManager* Manager, FString inCompileOptions)
{
	// If no context is specified, then just use the global context
	auto Context = Manager->GetContext();
	auto DeviceList = Manager->GetDeviceList();

	Compile(Manager, inCompileOptions, Context, DeviceList.data());
}

void FOpenCLProgram::Compile(AOpenCLManager* Manager, FString inCompileOptions, cl_context* context, cl_device_id* device_list)
{
	cl_int Error = 0;
	std::string Options = Helpers::narrow(Helpers::ConvertToWString(inCompileOptions));

	Error = clBuildProgram(Program, 1, device_list, Options.c_str(), NULL, NULL);

	if (!Manager->CheckError(Error))
	{
		UE_LOG(SpaceStationGameLog, Warning, TEXT("Finished compiling OpenCL program"));
	}
}