#include "pch.h"
#include "ramvm_vm.h"
#include "ramvm_instruction.h"

namespace ramvm {
	VM::VM(int _numCMDLineArgs, int _maxMemory, std::vector<Instruction*>& _instrs)
	{
		memory = Memory(_maxMemory);
		instructions = _instrs;
		ip = 0;
		topLevelExecFrame = ExecutionFrame(0, _numCMDLineArgs);
	}

	VM::~VM()
	{
		for (auto instr : instructions)
			delete instr;
	}

	ResultType VM::Run(ResultInfo& _info)
	{
		while (ip < (int)instructions.size())
		{
			ExecutionFrame& execFrame = execFrames.empty() ? topLevelExecFrame : execFrames.back();
			Instruction* curInstr = instructions[ip];

			switch (curInstr->GetType())
			{
				case InstructionType::HALT: return ResultType::SUCCESS;
				case InstructionType::MOVE: {
					InstrMove* instr = (InstrMove*)curInstr;
					ResultType resType = ResultType::SUCCESS;
					DataVariant movVal(instr->dataType);

					resType = ReadFromSrcArg(execFrame, instr->src, movVal, _info);
					if (IsErrorResult(resType))
						return resType;

					resType = WriteToDestArg(execFrame, instr->dest, movVal, _info);
					if (IsErrorResult(resType))
						return resType;
				} break;
				case InstructionType::MALLOC: {
					InstrMalloc* instr = (InstrMalloc*)curInstr;
					ResultType resType = ResultType::SUCCESS;

					//Get Size
					DataVariant size(DataType::INT);
					resType = ReadFromSrcArg(execFrame, instr->size, size, _info);
					if (IsErrorResult(resType))
						return resType;

					//Malloc and Get Address
					int memAddr = -1;
					resType = memory.Malloc(size.AsInt(), memAddr, _info);
					if (IsErrorResult(resType))
						return resType;

					//Write Addr to Destination
					resType = WriteToDestArg(execFrame, instr->dest, DataVariant(memAddr), _info);
					if (IsErrorResult(resType))
						return resType;
				} break;
				case InstructionType::FREE: {
					InstrFree* instr = (InstrFree*)curInstr;
					ResultType resType = ResultType::SUCCESS;

					//Get Address
					DataVariant addr(DataType::INT);
					resType = ReadFromSrcArg(execFrame, instr->addr, addr, _info);
					if (IsErrorResult(resType))
						return resType;

					//Free Address
					resType = memory.Free(addr.AsInt(), _info);
					if (IsErrorResult(resType))
						return resType;
				} break;
				case InstructionType::PRINT: {
					InstrPrint* instr = (InstrPrint*)curInstr;
					ResultType resType = ResultType::SUCCESS;

					DataVariant memAddr(DataType::INT);
					resType = ReadFromSrcArg(execFrame, instr->start, memAddr, _info);
					if (IsErrorResult(resType))
						return resType;

					DataVariant length(DataType::INT);
					resType = ReadFromSrcArg(execFrame, instr->length, length, _info);
					if (IsErrorResult(resType))
						return resType;

					//Get Chars
					std::vector<char> chars(length.AsInt());
					resType = memory.ReadBuffer(memAddr.AsInt(), length.AsInt(), (byte*)chars.data(), _info);
					if (IsErrorResult(resType))
						return resType;

					chars.push_back(0);

					//Print
					printf(chars.data());
				} break;
				case InstructionType::JUMP: ip = ((InstrJump*)curInstr)->labelIdx; continue;
				case InstructionType::CJUMP: {
					InstrCJump* instr = (InstrCJump*)curInstr;
					DataVariant condVal(DataType::BYTE);
					ResultType resType = ReadFromSrcArg(execFrame, instr->condSrc, condVal, _info);

					if (IsErrorResult(resType)) { return resType; }
					else if (condVal.AsByte() != 0)
					{
						ip = instr->labelIdx;
						continue;
					}
				} break;
				case InstructionType::RETURN: {
					if (execFrames.empty()) { return ResultType::ERR_RET_TOP_LEVEL; }
					else
					{
						InstrReturn* instr = (InstrReturn*)curInstr;
						ExecutionFrame& parentExecFrame = execFrames.size() == 1 ? topLevelExecFrame : execFrames[execFrames.size() - 2];
						ResultType resType = ResultType::SUCCESS;

						for (int i = 0; i < (int)instr->srcs.size(); i++)
						{
							DataVariant retVal(DataType::INT);

							//Read from current exec frame
							resType = ReadFromSrcArg(execFrame, instr->srcs[i], retVal, _info);
							if (IsErrorResult(resType))
								return resType;

							//Write to old exec frame ret dest
							Argument dest;
							resType = parentExecFrame.GetReturnDest(i, dest, _info);
							if (IsErrorResult(resType))
								return resType;

							resType = WriteToDestArg(parentExecFrame, dest, retVal, _info);
							if (IsErrorResult(resType))
								return resType;
						}

						execFrames.pop_back();
						ip = execFrame.GetRetIP();
						continue;
					}
				} break;
				case InstructionType::CALL: {
					InstrCall* instr = (InstrCall*)curInstr;
					ExecutionFrame newExecFrame = ExecutionFrame(ip + 1, instr->regCnt);
					ResultType resType = ResultType::SUCCESS;

					for (int i = 0; i < (int)instr->argSrcs.size(); i++)
					{
						DataVariant argVal(DataType::INT);

						//Read from current exec frame
						resType = ReadFromSrcArg(execFrame, instr->argSrcs[i], argVal, _info);
						if (IsErrorResult(resType))
							return resType;

						//Write to new exec frame
						resType = WriteToDestArg(newExecFrame, Argument(ArgType::REGISTER, i), argVal, _info);
						if (IsErrorResult(resType))
							return resType;
					}

					execFrame.SetReturnDests(instr->retDests);
					execFrames.push_back(newExecFrame);
					ip = instr->labelIdx;
					continue;
				} break;
				case InstructionType::BINOP: {
					InstrBinop* instr = (InstrBinop*)curInstr;
					ResultType resType = ResultType::SUCCESS;
					DataVariant val1(DataType::INT), val2(DataType::INT);

					//Read
					resType = ReadFromSrcArg(execFrame, instr->src1, val1, _info);
					if (IsErrorResult(resType))
						return resType;

					resType = ReadFromSrcArg(execFrame, instr->src2, val2, _info);
					if (IsErrorResult(resType))
						return resType;

					//Calculate
					DataVariant resVal;
					resType = DoBinop(instr->op, val1, val2, resVal);
					if (IsErrorResult(resType))
						return resType;

					//remove this 
					resVal = DataVariant(DataType::INT, resVal.GetValue());
					//////

					resType = WriteToDestArg(execFrame, instr->dest, resVal, _info);
					if (IsErrorResult(resType))
						return resType;
				} break;
				case InstructionType::UNOP: {
					InstrUnop* instr = (InstrUnop*)curInstr;
					ResultType resType = ResultType::SUCCESS;
					DataVariant val(DataType::INT);

					//Read
					resType = ReadFromSrcArg(execFrame, instr->src, val, _info);
					if (IsErrorResult(resType))
						return resType;

					//Calculate
					DataVariant resVal;
					resType = DoUnop(instr->op, val, resVal);
					if (IsErrorResult(resType))
						return resType;

					//remove this 
					resVal = DataVariant(DataType::INT, resVal.GetValue());
					//////

					if (IsErrorResult(resType))
						return resType;
				} break;
				case InstructionType::PUSH: {
					InstrPush* instr = (InstrPush*)curInstr;
					ResultType resType = ResultType::SUCCESS;

					for (int i = 0; i < (int)instr->srcs.size(); i++)
					{
						DataVariant pushVal(DataType::INT);

						//Read Value
						resType = ReadFromSrcArg(execFrame, instr->srcs[i], pushVal, _info);
						if (IsErrorResult(resType))
							return resType;

						//Push Value
						int writeStart = GetSP() + 1;
						stack.resize(stack.size() + pushVal.GetSize());
						WriteStack(writeStart, pushVal);
					}
				} break;
				case InstructionType::POP: {
					InstrPop* instr = (InstrPop*)curInstr;
					ResultType resType = ResultType::SUCCESS;
					DataVariant amt(DataType::INT);

					//Read Amount
					resType = ReadFromSrcArg(execFrame, instr->amt, amt, _info);
					if (IsErrorResult(resType))
						return resType;

					//Scale Amount
					amt = DataVariant(amt.AsInt() * instr->scale);

					//Pop Amount
					int sp = GetSP();

					if (amt.AsInt() < 0 || sp < amt.AsInt() - 1) { return ResultType::ERR_POP_AMT; }
					else if (amt.AsInt() == 0) { return ResultType::SUCCESS; }
					else if (sp == 0) { stack.clear(); }
					else { stack.erase(stack.begin() + sp - amt.AsInt() + 1, stack.end()); }
				} break;
				default: return ResultType::ERR_UNKNOWNINSTR;
			}

			ip++;
		}

		return ResultType::ERR_NOHALT;
	}

	ResultType VM::ReadFromSrcArg(ExecutionFrame& _execFrame, Argument _arg, DataVariant& _value, ResultInfo& _info)
	{
		switch (_arg.type)
		{
			case ArgType::VALUE: {
				_value = DataVariant(_arg.value);
				return ResultType::SUCCESS;
			}
			case ArgType::REGISTER: return _execFrame.ReadRegister(_arg.value.AsInt(), _value, _info);
			case ArgType::MEM_REG: {
				DataVariant addr(DataType::INT);
				ResultType res = _execFrame.ReadRegister(_arg.value.AsInt(), addr, _info);

				if (IsErrorResult(res)) { return res; }
				else { return memory.Read(addr.AsInt(), _value, _info); }
			}
			case ArgType::STACK_REG: {
				DataVariant pos(DataType::INT);
				ResultType res = _execFrame.ReadRegister(_arg.value.AsInt(), pos, _info);

				if (IsErrorResult(res)) { return res; }
				else { return ReadStack(pos.AsInt(), _value); }
			}
			case ArgType::STACK_PTR: {
				_value = GetSP();
				return ResultType::SUCCESS;
			} break;
			case ArgType::SP_OFFSET: return ReadStack(GetSP() + _arg.value.AsInt(), _value);
			default: return ResultType::ERR_ARGUMENT;
		}
	}

	ResultType VM::WriteToDestArg(ExecutionFrame& _execFrame, Argument _arg, DataVariant _value, ResultInfo& _info)
	{
		switch (_arg.type)
		{
			case ArgType::REGISTER: return _execFrame.WriteRegister(_arg.value.AsInt(), _value, _info);
			case ArgType::MEM_REG: {
				DataVariant addr(DataType::INT);
				ResultType res = _execFrame.ReadRegister(_arg.value.AsInt(), addr, _info);

				if (IsErrorResult(res)) { return res; }
				else { return memory.Write(addr.AsInt(), _value, _info); }
			}
			case ArgType::STACK_REG: {
				DataVariant pos(DataType::INT);
				ResultType res = _execFrame.ReadRegister(_arg.value.AsInt(), pos, _info);

				if (IsErrorResult(res)) { return res; }
				else { return WriteStack(pos.AsInt(), _value); }
			}
			case ArgType::SP_OFFSET: return WriteStack(GetSP() + _arg.value.AsInt(), _value);
			default: return ResultType::ERR_INVALID_DEST;
		}
	}

	ResultType VM::ReadStack(int _pos, DataVariant& _value)
	{
		if (_pos >= 0 && _pos + _value.GetSize() <= (int)stack.size())
		{
			_value = BufferToDataValue(_value.GetType(), &stack[_pos]);
			return ResultType::SUCCESS;
		}
		else { return ResultType::ERR_STACK_READ; }
	}

	ResultType VM::WriteStack(int _pos, DataVariant _value)
	{
		if (_pos >= 0 && _pos + _value.GetSize() <= (int)stack.size())
		{
			if (_value.GetType() == DataType::BYTE) { stack[_pos] = _value.AsByte(); }
			else { DataValueToBuffer(&stack[_pos], _value); }

			return ResultType::SUCCESS;
		}
		else { return ResultType::ERR_STACK_WRITE; }
	}

	void VM::PrintCurRegisters()
	{
		if (execFrames.empty()) { topLevelExecFrame.PrintRegisters(); }
		else { return execFrames.back().PrintRegisters(); }
	}

	void VM::PrintMemory(int _startAddr, int _length)
	{
		std::cout << "---------------------MEMORY---------------------" << std::endl;

		ResultInfo resInfo;
		std::vector<byte> buffer(_length);
		memory.ReadBuffer(_startAddr, _length, buffer.data(), resInfo);

		for (int i = 0; i < _length; i++)
			std::cout << _startAddr + i << ": " << std::to_string(buffer[i]) << std::endl;

		std::cout << "---------------------------------------------------" << std::endl;
	}

	void VM::PrintStack()
	{
		std::cout << "---------------------STACK---------------------" << std::endl;

		int sp = GetSP();

		for (int i = 0; i < (int)stack.size(); i++)
			std::cout << i << ": " << ToHexString(stack[i]) << ", " << std::to_string(stack[i]) << (i == sp ? "  <- SP" : "") << std::endl;

		std::cout << "--------------------------------------------------" << std::endl;
	}
}