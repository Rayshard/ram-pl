#include "pch.h"
#include "ramvm_execution_frame.h"
#include "ramvm_instruction.h"

namespace ramvm {
	ExecutionFrame::ExecutionFrame()
	{
		retIP = -1;
		retSP = 0;
		registers = std::vector<DataValue>();
	}

	ExecutionFrame::ExecutionFrame(int _retIP, int _retSP, int _numRegs)
	{
		retIP = _retIP;
		registers = std::vector<DataValue>(_numRegs);
	}

	ResultType ExecutionFrame::ReadRegister(int _regIndex, DataVariant& _value, ResultInfo& _info)
	{
		if (_regIndex < (int)registers.size())
		{
			switch (_value.GetType())
			{
				case DataType::BYTE: _value = DataVariant(registers[_regIndex].b); break;
				case DataType::INT: _value = DataVariant(registers[_regIndex].i); break;
				default: return ResultType::ERR_REG_READ;
			}
			
			return ResultType::SUCCESS;
		}
		else { return ResultType::ERR_REG_IDX_OOB; }
	}

	ResultType ExecutionFrame::WriteRegister(int _regIndex, DataVariant _value, ResultInfo& _info)
	{
		if (_regIndex < (int)registers.size())
		{
			switch (_value.GetType())
			{
				case DataType::BYTE: registers[_regIndex].b = _value.B(); break;
				case DataType::INT: registers[_regIndex].i = _value.I(); break;
				default: return ResultType::ERR_REG_WRTIE;
			}
			
			return ResultType::SUCCESS;
		}
		else { return ResultType::ERR_REG_IDX_OOB; }
	}

	void ExecutionFrame::PrintRegisters()
	{
		std::cout << "---------------------REGISTERS---------------------" << std::endl;

		for (int i = 0; i < (int)registers.size(); i++)
			std::cout << "R" << i << ": " << ToHexString(registers[i].i) << ", " << registers[i].i << std::endl;

		std::cout << "---------------------------------------------------" << std::endl;
	}
}
