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
		std::vector<byte> stack;
		std::vector<ExecutionFrame> execFrames;
		std::vector<Instruction*> instructions;
		std::unordered_map<std::string, int> labels;
		ExecutionFrame topLevelExecFrame;
		int ip;

	public:
		VM(int _numCMDLineArgs, int _maxMemory, const std::vector<Instruction*>& _instrs, const std::unordered_map<std::string, int>& _labels);
		~VM();

		ResultType Run(ResultInfo& _info);
		ResultType ReadFromSrcArg(ExecutionFrame& _execFrame, Argument* _src, DataVariant& _value, ResultInfo& _info);
		ResultType ReadFromSrcArg(ExecutionFrame& _execFrame, Argument* _arg, byte* _buffer, int _length, ResultInfo& _info);
		ResultType WriteToDestArg(ExecutionFrame& _execFrame, Argument* _dest, DataVariant _value, ResultInfo& _info);
		ResultType ReadStack(int _pos, DataVariant& _value);
		ResultType ReadStack(int _pos, int _length, byte* _buffer);
		ResultType WriteStack(int _pos, DataVariant _value);
		int GetLabelInstrIdx(std::string _label);

		int GetSP() { return stack.size() - 1; }

		void PrintCurRegisters();
		void PrintMemory();
		void PrintStack();
	};

}