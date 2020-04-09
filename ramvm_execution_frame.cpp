#include "pch.h"
#include "ramvm_execution_frame.h"
#include "ramvm_instruction.h"

namespace ramvm {
	ExecutionFrame::ExecutionFrame(int _retIP, int _framePtr)
	{
		retIP = _retIP;
		framePtr = _framePtr;
	}

	int ExecutionFrame::GetRetIP() { return retIP; }
	int ExecutionFrame::GetFramePtr() { return framePtr; }
}
