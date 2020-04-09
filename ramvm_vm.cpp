#include "pch.h"
#include "ramvm_vm.h"
#include "ramvm_instruction.h"

namespace ramvm {
	VM::VM(int _maxMemory, const std::vector<Instruction*>& _instrs, const std::unordered_map<std::string, int>& _labels)
	{
		memory = Memory(_maxMemory);
		instructions = _instrs;
		labels = _labels;
		instrPtr = 0;
		globalPtr = -1;

		execFrames.push_back(ExecutionFrame(0, 0));
	}

	VM::~VM()
	{
		for (auto instr : instructions)
			delete instr;
	}

	ResultType VM::Run(ResultInfo& _info)
	{
		while (true)
		{
			if (instrPtr < 0 || instrPtr >= (int)instructions.size())
				return ResultType::ERR_INVALID_IP;

			ExecutionFrame& execFrame = execFrames.back();
			Instruction* curInstr = instructions[instrPtr];

			std::cout << instrPtr << ":" << curInstr->ToString() << std::endl;

			switch (curInstr->GetType())
			{
				case InstructionType::HALT: return ResultType::SUCCESS;
				case InstructionType::NOOP: break;
				case InstructionType::MOVE: {
					InstrMove* instr = (InstrMove*)curInstr;
					ResultType resType = ResultType::SUCCESS;
					DataVariant movVal(instr->dataType);

					resType = ReadFromSrcArg(instr->src, movVal, _info);
					if (IsErrorResult(resType))
						return resType;

					resType = WriteToDestArg(instr->dest, movVal, _info);
					if (IsErrorResult(resType))
						return resType;
				} break;
				case InstructionType::MALLOC: {
					InstrMalloc* instr = (InstrMalloc*)curInstr;
					ResultType resType = ResultType::SUCCESS;

					//Get Size
					DataVariant size(DataType::INT);
					resType = ReadFromSrcArg(instr->size, size, _info);
					if (IsErrorResult(resType))
						return resType;

					//Malloc and Get Address
					int memAddr = -1;
					resType = memory.Malloc(size.I(), memAddr, _info);
					if (IsErrorResult(resType))
						return resType;

					//Write Addr to Destination
					resType = WriteToDestArg(instr->dest, DataVariant(memAddr), _info);
					if (IsErrorResult(resType))
						return resType;
				} break;
				case InstructionType::FREE: {
					InstrFree* instr = (InstrFree*)curInstr;
					ResultType resType = ResultType::SUCCESS;

					//Get Address
					DataVariant addr(DataType::INT);
					resType = ReadFromSrcArg(instr->addr, addr, _info);
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
					resType = ReadFromSrcArg(instr->start, memAddr, _info);
					if (IsErrorResult(resType))
						return resType;

					DataVariant length(DataType::INT);
					resType = ReadFromSrcArg(instr->length, length, _info);
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
				case InstructionType::JUMP: instrPtr = GetLabelInstrIdx(((InstrJump*)curInstr)->label); continue;
				case InstructionType::CJUMP: {
					InstrCJump* instr = (InstrCJump*)curInstr;
					DataVariant condVal(DataType::BYTE);
					ResultType resType = ReadFromSrcArg(instr->cond, condVal, _info);

					if (IsErrorResult(resType)) { return resType; }
					else if ((condVal.B() == 0 && instr->jumpOnFalse) || (condVal.B() != 0 && !instr->jumpOnFalse))
					{
						instrPtr = GetLabelInstrIdx(instr->label);
						continue;
					}
				} break;
				case InstructionType::RETURN: {
					if (execFrames.size() == 1) { return ResultType::ERR_RET_TOP_LEVEL; }
					else
					{
						InstrReturn* instr = (InstrReturn*)curInstr;
						ExecutionFrame& parentExecFrame = execFrames[execFrames.size() - 2];
						ResultType resType = ResultType::SUCCESS;

						//Read return values from the stack so that when the
						//stack pointer is reset, we can push this onto the
						//top of the stack
						DataVariant amtVal(DataType::INT);
						resType = ReadFromSrcArg(instr->amt, amtVal, _info);
						if (IsErrorResult(resType))
							return resType;

						int retBufferSize = amtVal.I();
						byte* retBuffer = new byte[retBufferSize];
						resType = ReadStack(GetSP() - retBufferSize + 1, retBufferSize, retBuffer);
						if (IsErrorResult(resType))
							return resType;
						
						//Revert Stack back to position before this frame was created
						stack.erase(stack.begin() + execFrame.GetFramePtr(), stack.end());

						//Push Return Buffer to Stack
						stack.insert(stack.end(), retBuffer, retBuffer + retBufferSize);
						delete[] retBuffer;

						//Restore old frame
						execFrames.pop_back();
						instrPtr = execFrame.GetRetIP();
						continue;
					}
				} break;
				case InstructionType::CALL: {
					InstrCall* instr = (InstrCall*)curInstr;
					ResultType resType = ResultType::SUCCESS;

					DataVariant argsByteLenVal(DataType::INT);
					resType = ReadFromSrcArg(instr->argsByteLength, argsByteLenVal, _info);
					if (IsErrorResult(resType))
						return resType;

					ExecutionFrame newExecFrame = ExecutionFrame(instrPtr + 1, GetSP() - argsByteLenVal.I() + 1);
					execFrames.push_back(newExecFrame);

					instrPtr = GetLabelInstrIdx(instr->label);
					continue;
				} break;
				case InstructionType::BINOP: {
					InstrBinop* instr = (InstrBinop*)curInstr;
					ResultType resType = ResultType::SUCCESS;
					DataVariant val1(instr->GetSrc1DataType()), val2(instr->GetSrc2DataType());

					//Read
					resType = ReadFromSrcArg(instr->src1, val1, _info);
					if (IsErrorResult(resType))
						return resType;

					resType = ReadFromSrcArg(instr->src2, val2, _info);
					if (IsErrorResult(resType))
						return resType;

					//Calculate
					DataVariant resVal;

					resType = DoBinop(instr->op, val1, val2, resVal);
					if (IsErrorResult(resType)) { return resType; }
					else { resVal = resVal.To(instr->GetDestDataType()); } //Set the type for the result

					//Write
					resType = WriteToDestArg(instr->dest, resVal, _info);
					if (IsErrorResult(resType))
						return resType;
				} break;
				case InstructionType::UNOP: {
					InstrUnop* instr = (InstrUnop*)curInstr;
					ResultType resType = ResultType::SUCCESS;
					DataVariant val(instr->GetSrcDataType());

					//Read
					resType = ReadFromSrcArg(instr->src, val, _info);
					if (IsErrorResult(resType))
						return resType;

					//Calculate
					DataVariant resVal;

					resType = DoUnop(instr->op, val, resVal);
					if (!IsErrorResult(resType)) { return resType; }
					else { resVal = resVal.To(instr->GetDestDataType()); }

					//Write
					resType = WriteToDestArg(instr->dest, resVal, _info);
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
						Argument* src = instr->srcs[i];
						DataVariant storeVal(instr->dataType);

						//Read Value
						resType = ReadFromSrcArg(src, storeVal, _info);
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
					resType = WriteToDestArg(instr->dest, DataVariant(memAddr), _info);
					if (IsErrorResult(resType))
						return resType;
				} break;
				case InstructionType::COMPARE: {
					InstrCompare* instr = (InstrCompare*)curInstr;
					ResultType resType = ResultType::SUCCESS;
					DataVariant cmpAmt(DataType::INT);

					//Get Length
					resType = ReadFromSrcArg(instr->length, cmpAmt, _info);
					if (IsErrorResult(resType))
						return resType;

					int length = cmpAmt.I();
					byte* src1Bytes = new byte[length];
					byte* src2Bytes = new byte[length];

					//Get Src1 Bytes
					resType = ReadFromSrcArg(instr->src1, src1Bytes, length, _info);
					if (IsErrorResult(resType))
						return resType;

					//Get Src1 Bytes
					resType = ReadFromSrcArg(instr->src2, src2Bytes, length, _info);
					if (IsErrorResult(resType))
						return resType;

					//Write Result to Dest
					bool equal = !memcmp(src1Bytes, src2Bytes, length);
					resType = WriteToDestArg(instr->dest, DataVariant(equal ? (byte)1 : (byte)0), _info);
					delete[] src2Bytes;
					delete[] src1Bytes;
					if (IsErrorResult(resType))
						return resType;
				} break;
				case InstructionType::PUSH: {
					InstrPush* instr = (InstrPush*)curInstr;
					ResultType resType = ResultType::SUCCESS;

					for (int i = 0; i < (int)instr->srcs.size(); i++)
					{
						Argument* src = instr->srcs[i];
						DataVariant pushVal(instr->dataType);

						//Read Value
						resType = ReadFromSrcArg(src, pushVal, _info);
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
					resType = ReadFromSrcArg(instr->amt, amt, _info);
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

			instrPtr++;
			PrintStack();
		}
	}

	ResultType VM::ReadFromSrcArg(Argument* _arg, DataVariant& _value, ResultInfo& _info)
	{
		switch (_arg->GetType())
		{
			case ArgType::VALUE: {
				_value = DataVariant(_value.GetType(), ((ValueArgument*)_arg)->GetValue());
				return ResultType::SUCCESS;
			}
			case ArgType::REGISTER: {
				switch (((RegisterArgument*)_arg)->GetRegType())
				{
					case RegisterType::SP: {
						_value = DataVariant(GetSP());
						return ResultType::SUCCESS;
					}
					case RegisterType::FP: {
						_value = DataVariant(execFrames.back().GetFramePtr());
						return ResultType::SUCCESS;
					}
					case RegisterType::GP: {
						_value = DataVariant(globalPtr);
						return ResultType::SUCCESS;
					}
					default: ASSERT_MSG(false, "VM::ReadFromSrcArg - RegisterType not handled!");
				}
			}
			case ArgType::STACK: {
				StackArgument* stackArg = (StackArgument*)_arg;

				DataVariant pos(DataType::INT);
				ResultType res = ReadFromSrcArg(stackArg->GetPosSrc(), pos, _info);

				if (IsErrorResult(res)) { return res; }
				else { return ReadStack(pos.I() + stackArg->GetOffset(), _value); }
			}
			case ArgType::MEMORY: {
				MemoryArgument* memArg = (MemoryArgument*)_arg;

				DataVariant addr(DataType::INT);
				ResultType res = ReadFromSrcArg(memArg->GetAddrSrc(), addr, _info);

				if (IsErrorResult(res)) { return res; }
				else { return memory.Read(addr.I() + memArg->GetOffset(), _value, _info); }
			}
			default: return ResultType::ERR_ARGUMENT;
		}
	}

	ResultType VM::ReadFromSrcArg(Argument* _arg, byte* _buffer, int _length, ResultInfo& _info)
	{
		switch (_arg->GetType())
		{
			case ArgType::STACK: {
				StackArgument* stackArg = (StackArgument*)_arg;

				DataVariant pos(DataType::INT);
				ResultType res = ReadFromSrcArg(stackArg->GetPosSrc(), pos, _info);

				if (IsErrorResult(res)) { return res; }
				else { return ReadStack(pos.I() + stackArg->GetOffset(), _length, _buffer); }
			}
			case ArgType::MEMORY: {
				MemoryArgument* memArg = (MemoryArgument*)_arg;

				DataVariant addr(DataType::INT);
				ResultType res = ReadFromSrcArg(memArg->GetAddrSrc(), addr, _info);

				if (IsErrorResult(res)) { return res; }
				else { return memory.ReadBuffer(addr.I() + memArg->GetOffset(), _length, _buffer, _info); }
			}
			default: return ResultType::ERR_ARGUMENT;
		}
	}

	ResultType VM::WriteToDestArg(Argument* _arg, DataVariant _value, ResultInfo& _info)
	{
		switch (_arg->GetType())
		{
			case ArgType::REGISTER: {
				switch (((RegisterArgument*)_arg)->GetRegType())
				{
					case RegisterType::GP: {
						globalPtr = _value.I();
						return ResultType::SUCCESS;
					}
					default: ASSERT_MSG(false, "VM::WriteToSrcArg - RegisterType not handled!");
				}
			}
			case ArgType::STACK: {
				StackArgument* stackArg = (StackArgument*)_arg;

				if (stackArg->IsStackTop())
				{
					stack.insert(stack.end(), _value.Bytes(), _value.Bytes() + _value.GetSize());
					return ResultType::SUCCESS;
				}
				else
				{
					DataVariant pos(DataType::INT);
					ResultType res = ReadFromSrcArg(stackArg->GetPosSrc(), pos, _info);

					if (IsErrorResult(res)) { return res; }
					else { return WriteStack(pos.I() + stackArg->GetOffset(), _value); }
				}
			}
			case ArgType::MEMORY: {
				MemoryArgument* memArg = (MemoryArgument*)_arg;

				DataVariant addr(DataType::INT);
				ResultType res = ReadFromSrcArg(memArg->GetAddrSrc(), addr, _info);

				if (IsErrorResult(res)) { return res; }
				else { return memory.Write(addr.I() + memArg->GetOffset(), _value, _info); }
			}
			default: return ResultType::ERR_INVALID_DEST;
		}
	}

	ResultType VM::ReadStack(int _pos, DataVariant& _value)
	{
		if (_value.GetSize() < 0) { return ResultType::ERR_STACK_READ; }
		else if (_pos >= 0 && _pos + _value.GetSize() <= (int)stack.size())
		{
			_value = BufferToDataValue(_value.GetType(), &stack[_pos]);
			return ResultType::SUCCESS;
		}
		else { return ResultType::ERR_STACK_READ; }
	}

	ResultType VM::ReadStack(int _pos, int _length, byte* _buffer)
	{
		if (_length < 0) { return ResultType::ERR_STACK_READ; }
		else if (_pos >= 0 && _pos + _length <= (int)stack.size())
		{
			std::copy(stack.begin() + _pos, stack.begin() + (_pos + _length), _buffer);
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

	int VM::GetLabelInstrIdx(std::string _label)
	{
		auto search = labels.find(_label);
		if (search != labels.end()) { return search->second; }
		else { return -1; }
	}

	void VM::PrintRegisters()
	{
		std::cout << "---------------------REGISTERS---------------------" << std::endl;

		for (int i = 0; i < (int)registers.size(); i++)
		{
			std::cout << "R" << i << ": " << ToHexString(registers[i].l) << ", ";
			std::cout << (int)registers[i].b << "b, ";
			std::cout << registers[i].i << "i, ";
			std::cout << registers[i].f << "f, ";
			std::cout << registers[i].d << "d, ";
			std::cout << registers[i].l << "l" << std::endl;
		}

		std::cout << "---------------------------------------------------" << std::endl;
	}

	void VM::PrintMemory()
	{
		std::cout << "---------------------MEMORY---------------------" << std::endl;

		memory.PrintAllocatedBlocks(8);

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