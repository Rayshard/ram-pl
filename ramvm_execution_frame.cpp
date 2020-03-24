#include "pch.h"
#include "ramvm_execution_frame.h"
#include "ramvm_instruction.h"

namespace ramvm {
	ExecutionFrame::ExecutionFrame()
	{
		retIP = -2;
		registers = std::vector<int>();
	}

	ExecutionFrame::ExecutionFrame(int _retIP, int _numRegs)
	{
		retIP = _retIP;
		registers = std::vector<int>(_numRegs);
	}

	ResultType ExecutionFrame::ReadRegister(int _regIndex, int& _value, ResultInfo& _info)
	{
		if (_regIndex < (int)registers.size())
		{
			_value = registers[_regIndex];
			return ResultType::SUCCESS;
		}
		else { return ResultType::ERR_REG_IDX_OOB; }
	}

	ResultType ExecutionFrame::WriteRegister(int _regIndex, int _value, ResultInfo& _info)
	{
		if (_regIndex < (int)registers.size())
		{
			registers[_regIndex] = _value;
			return ResultType::SUCCESS;
		}
		else { return ResultType::ERR_REG_IDX_OOB; }
	}

	ResultType ExecutionFrame::GetReturnDest(int _idx, Argument& _dest, ResultInfo& _info)
	{
		if (_idx < (int)retDests.size())
		{
			_dest = retDests[_idx];
			return ResultType::SUCCESS;
		}
		else { return ResultType::ERR_RET_DEST_IDX_OOB; }
	}

	void ExecutionFrame::SetReturnDests(std::vector<Argument>& _dests) { retDests = _dests; }
	int ExecutionFrame::GetRetIP() { return retIP; }


	void ExecutionFrame::PrintRegisters()
	{
		std::cout << "---------------------REGISTERS---------------------" << std::endl;

		for (int i = 0; i < (int)registers.size(); i++)
			std::cout << "R" << i << ": " << registers[i] << std::endl;

		std::cout << "---------------------------------------------------" << std::endl;
	}
}
