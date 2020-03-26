#pragma once

namespace ramvm {
	struct Argument;

	class ExecutionFrame
	{
	private:
		std::vector<DataValue> registers;
		int retIP, retSP;

	public:
		ExecutionFrame();
		ExecutionFrame(int _retIP, int _retSP, int _numRegs);

		ResultType ReadRegister(int _regIndex, DataVariant& _value, ResultInfo& _info);
		ResultType WriteRegister(int _regIndex, DataVariant _value, ResultInfo& _info);
		int GetRetIP() { return retIP; }
		int GetRetSP() { return retSP; }

		void PrintRegisters();
	};
}
