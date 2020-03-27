#include "pch.h"
#include "ramvm_vm.h"
#include "ramvm_instruction.h"

namespace ramvm {
	VM::VM(int _numCMDLineArgs, int _maxMemory, std::vector<Instruction*>& _instrs)
	{
		memory = Memory(_maxMemory);
		instructions = _instrs;
		ip = 0;
		topLevelExecFrame = ExecutionFrame(0, 0, _numCMDLineArgs);
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
					resType = memory.Malloc(size.I(), memAddr, _info);
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
					resType = memory.Free(addr.I(), _info);
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
					std::vector<char> chars(length.I());
					resType = memory.ReadBuffer(memAddr.I(), length.I(), (byte*)chars.data(), _info);
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
					else if (condVal.B() != 0)
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

						//Push return values to the stack in reverse so that the 
						//first return value is on the top of the stack when the caller
						//starts reading
						std::vector<byte> retBuffer;
						for (int i = 0; i < (int)instr->srcs.size(); i++)
						{
							TypedArgument src = instr->srcs[i];
							DataVariant retVal(src.dataType);

							//Read from current exec frame
							resType = ReadFromSrcArg(execFrame, src, retVal, _info);
							if (IsErrorResult(resType)) { return resType; }
							else { retBuffer.insert(retBuffer.begin(), retVal.Bytes(), retVal.Bytes() + retVal.GetSize()); }
						}

						//Revert Stack back to position before this frame was created
						stack.erase(stack.begin() + execFrame.GetRetSP(), stack.end());

						//Push Return Buffer to Stack
						stack.insert(stack.end(), retBuffer.begin(), retBuffer.end());

						//Restore old frame
						execFrames.pop_back();
						ip = execFrame.GetRetIP();
						continue;
					}
				} break;
				case InstructionType::CALL: {
					InstrCall* instr = (InstrCall*)curInstr;
					ExecutionFrame newExecFrame = ExecutionFrame(ip + 1, GetSP(), instr->regCnt);
					ResultType resType = ResultType::SUCCESS;

					//Push argument values to the stack in reverse so that the 
					//first argument is on the top of the stack when the callee
					//starts reading
					for (int i = (int)instr->argSrcs.size() - 1; i >= 0; i--)
					{
						TypedArgument src = instr->argSrcs[i];
						DataVariant argVal(src.dataType);

						//Read from current exec frame
						resType = ReadFromSrcArg(execFrame, src, argVal, _info);
						if (IsErrorResult(resType))
							return resType;

						//Push Value to stack
						stack.insert(stack.end(), argVal.Bytes(), argVal.Bytes() + argVal.GetSize());
					}

					execFrames.push_back(newExecFrame);
					ip = instr->labelIdx;
					continue;
				} break;
				case InstructionType::BINOP: {
					InstrBinop* instr = (InstrBinop*)curInstr;
					ResultType resType = ResultType::SUCCESS;
					DataVariant val1(instr->src1.dataType), val2(instr->src2.dataType);

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
					if (IsErrorResult(resType)) { return resType; }
					else { resVal = resVal.To(instr->dest.dataType); } //Set the type for the result

					resType = WriteToDestArg(execFrame, instr->dest, resVal, _info);
					if (IsErrorResult(resType))
						return resType;
				} break;
				case InstructionType::UNOP: {
					InstrUnop* instr = (InstrUnop*)curInstr;
					ResultType resType = ResultType::SUCCESS;
					DataVariant val(instr->src.dataType);

					//Read
					resType = ReadFromSrcArg(execFrame, instr->src, val, _info);
					if (IsErrorResult(resType))
						return resType;

					//Calculate
					DataVariant resVal;

					resType = DoUnop(instr->op, val, resVal);
					if (IsErrorResult(resType)) { return resType; }
					else { resVal = resVal.To(instr->dest.dataType); }

					if (IsErrorResult(resType))
						return resType;
				} break;
				case InstructionType::STORE: {
					InstrStore* instr = (InstrStore*)curInstr;
					ResultType resType = ResultType::SUCCESS;

					//Create buffer
					std::vector<byte> buffer;
					for (int i = 0; i < (int)instr->srcs.size(); i++)
					{
						TypedArgument src = instr->srcs[i];
						DataVariant storeVal(src.dataType);

						//Read Value
						resType = ReadFromSrcArg(execFrame, src, storeVal, _info);
						if (IsErrorResult(resType)) { return resType; }
						else { buffer.insert(buffer.end(), storeVal.Bytes(), storeVal.Bytes() + storeVal.GetSize()); }
					}

					//Malloc and Get Address
					int memAddr = -1;
					resType = memory.Malloc(buffer.size(), memAddr, _info);
					if (IsErrorResult(resType))
						return resType;

					//Write Buffer
					memory.WriteBuffer(memAddr, buffer.size(), buffer.data(), _info);

					//Write Addr to Destination
					resType = WriteToDestArg(execFrame, instr->dest, DataVariant(memAddr), _info);
					if (IsErrorResult(resType))
						return resType;
				} break;
				case InstructionType::PUSH: {
					InstrPush* instr = (InstrPush*)curInstr;
					ResultType resType = ResultType::SUCCESS;

					for (int i = 0; i < (int)instr->srcs.size(); i++)
					{
						TypedArgument src = instr->srcs[i];
						DataVariant pushVal(src.dataType);

						//Read Value
						resType = ReadFromSrcArg(execFrame, src, pushVal, _info);
						if (IsErrorResult(resType))
							return resType;

						//Push Value
						stack.insert(stack.end(), pushVal.Bytes(), pushVal.Bytes() + pushVal.GetSize());
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

					//Pop Amount
					int sp = GetSP(), byteCnt = amt.I() * instr->scale;

					if (byteCnt < 0 || sp < byteCnt - 1) { return ResultType::ERR_POP_AMT; }
					else if (byteCnt == 0) { return ResultType::SUCCESS; }
					else if (sp == 0) { stack.clear(); }
					else { stack.erase(stack.begin() + (sp - byteCnt + 1), stack.end()); }
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
				_value = DataVariant(_value.GetType(), _arg.value);
				return ResultType::SUCCESS;
			}
			case ArgType::REGISTER: return _execFrame.ReadRegister(_arg.value.i, _value, _info);
			case ArgType::MEM_REG: {
				DataVariant addr(DataType::INT);
				ResultType res = _execFrame.ReadRegister(_arg.value.i, addr, _info);

				if (IsErrorResult(res)) { return res; }
				else { return memory.Read(addr.I(), _value, _info); }
			}
			case ArgType::STACK_REG: {
				DataVariant pos(DataType::INT);
				ResultType res = _execFrame.ReadRegister(_arg.value.i, pos, _info);

				if (IsErrorResult(res)) { return res; }
				else { return ReadStack(pos.I(), _value); }
			}
			case ArgType::STACK_PTR: {
				_value = DataVariant(GetSP());
				return ResultType::SUCCESS;
			} break;
			case ArgType::SP_OFFSET: return ReadStack(GetSP() + _arg.value.i, _value);
			default: return ResultType::ERR_ARGUMENT;
		}
	}

	ResultType VM::WriteToDestArg(ExecutionFrame& _execFrame, Argument _arg, DataVariant _value, ResultInfo& _info)
	{
		switch (_arg.type)
		{
			case ArgType::REGISTER: return _execFrame.WriteRegister(_arg.value.i, _value, _info);
			case ArgType::MEM_REG: {
				DataVariant addr(DataType::INT);
				ResultType res = _execFrame.ReadRegister(_arg.value.i, addr, _info);

				if (IsErrorResult(res)) { return res; }
				else { return memory.Write(addr.I(), _value, _info); }
			}
			case ArgType::STACK_REG: {
				DataVariant pos(DataType::INT);
				ResultType res = _execFrame.ReadRegister(_arg.value.i, pos, _info);

				if (IsErrorResult(res)) { return res; }
				else { return WriteStack(pos.I(), _value); }
			}
			case ArgType::SP_OFFSET: {
				if (_arg.IsStackTop()) { stack.insert(stack.end(), _value.Bytes(), _value.Bytes() + _value.GetSize()); return ResultType::SUCCESS; }
				else { return WriteStack(GetSP() + _arg.value.i, _value); }
			}
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
			DataValueToBuffer(&stack[_pos], _value);
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