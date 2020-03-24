#pragma once
#include "ramvm_memory.h"
#include "ramvm_execution_frame.h"

namespace ramvm {
	class Instruction;
	struct Argument;

	class VM
	{
	private:
		Memory memory;
		std::vector<int> stack;
		std::vector<ExecutionFrame> execFrames;
		std::vector<Instruction> instructions;
		ExecutionFrame topLevelExecFrame;
		int ip;

	public:
		VM(int _numCMDLineArgs, int _maxMemory, std::vector<Instruction>& _instrs);

		ResultType Run(ResultInfo& _info);

		ResultType ReadFromSrcArg(ExecutionFrame& _execFrame, Argument _src, int& _value, ResultInfo& _info);
		ResultType WriteToDestArg(ExecutionFrame& _execFrame, Argument _dest, int _value, ResultInfo& _info);
		ResultType ReadStack(int _pos, int& _value);
		ResultType WriteStack(int _pos, int _value);

		int GetSP() { return stack.size() - 1; }

		void PrintCurRegisters();
		void PrintMemory(int _localAddr, int _range);
		void PrintStack();
	};

}