#include "SpaceStationGame.h"
#include "OpenCLError.h"
#include "OpenCLManager.h"

AOpenCLManager::AOpenCLManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void AOpenCLManager::Initialize()
{

}

bool AOpenCLManager::CheckError(cl_int Error)
{
	return CheckError(Error, NULL, NULL);
}

bool AOpenCLManager::CheckError(cl_int Error, cl_program* Program, cl_device_id* Device)
{
	if (Error != CL_SUCCESS)
	{
		UE_LOG(SpaceStationGameLog, Warning, TEXT("OpenCL Error!:\t\t	%s"), *OpenCLError::GetError(Error));

		if (Program && Device && Error == CL_BUILD_PROGRAM_FAILURE)
		{
			size_t log_size;
			clGetProgramBuildInfo(*Program, *Device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

			// Allocate memory for the log
			char* log = new char[log_size];

			// Get the log
			clGetProgramBuildInfo(*Program, *Device, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

			std::string logstring(log);

			UE_LOG(SpaceStationGameLog, Warning, TEXT("OpenCL Build Error!:\t\t	%s"), *Helpers::ConvertToFString(logstring));

			delete log;
		}

		return true;
	}

	return false;
}

void AOpenCLManager::ProgramAddFile(int32 ProgramIndex, FString Filepath)
{
	Programs[ProgramIndex].AddFile(Filepath);
}

void AOpenCLManager::CreateProgram(int32 ProgramIndex)
{
	Programs[ProgramIndex].CreateProgram(this);
}

void AOpenCLManager::CompileProgram(int32 ProgramIndex)
{
	Programs[ProgramIndex].Compile(this);
}	

void AOpenCLManager::CreateKernels(int32 ProgramIndex, int32 NumKernels)
{
	Programs[ProgramIndex].CreateKernels(this, NumKernels);
}

int32 AOpenCLManager::AddKernel(int32 ProgramIndex, FString KernelName)
{
	return Programs[ProgramIndex].AddKernel(this, KernelName);
}