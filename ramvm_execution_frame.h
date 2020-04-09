#pragma once

namespace ramvm {
	class ExecutionFrame
	{
		int retIP, framePtr;
	public:
		ExecutionFrame() = default;
		ExecutionFrame(int _retIP, int _framePtr);

		int GetRetIP();
		int GetFramePtr();
	};
}
