#pragma once

namespace ramvm {
	struct Argument;

	class ExecutionFrame
	{
	private:
		std::vector<int> registers;
		std::vector<Argument> retDests;
		int retIP;

	public:
		ExecutionFrame();
		ExecutionFrame(int _retIP, int _numRegs);

		ResultType ReadRegister(int _regIndex, int& _value, ResultInfo& _info);
		ResultType WriteRegister(int _regIndex, int _value, ResultInfo& _info);
		ResultType GetReturnDest(int _idx, Argument& _dest, ResultInfo& _info);
		void SetReturnDests(std::vector<Argument>& _dests);
		int GetRetIP();

		void PrintRegisters();
	};
}
