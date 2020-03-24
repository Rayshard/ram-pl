#include "pch.h"
#include "ramvm_vm.h"
#include "ramvm_instruction.h"

namespace ramvm {
	VM::VM(int _numCMDLineArgs, int _maxMemory, std::vector<Instruction>& _instrs)
	{
		memory = Memory(_maxMemory);
		instructions = _instrs;
		ip = -1;
		topLevelExecFrame = ExecutionFrame(0, _numCMDLineArgs);
	}

	ResultType VM::Run(ResultInfo& _info)
	{
		while (++ip < (int)instructions.size())
		{
			ExecutionFrame& execFrame = execFrames.empty() ? topLevelExecFrame : execFrames.back();
			Instruction instr = instructions[ip];

			switch (instr.GetType())
			{
				case InstructionType::HALT: return ResultType::SUCCESS;
				case InstructionType::MOVE: {
					ResultType resType = ResultType::SUCCESS;
					int movVal = 0;

					resType = ReadFromSrcArg(execFrame, instr.GetMoveSrc(), movVal, _info);
					if (IsErrorResult(resType))
						return resType;

					resType = WriteToDestArg(execFrame, instr.GetMoveDest(), movVal, _info);
					if (IsErrorResult(resType))
						return resType;
				} break;
				case InstructionType::MALLOC: {
					ResultType resType = ResultType::SUCCESS;

					//Get Size
					int size = 0;
					resType = ReadFromSrcArg(execFrame, instr.GetMallocSize(), size, _info);
					if (IsErrorResult(resType))
						return resType;

					//Malloc and Get Address
					int memAddr = -1;
					resType = memory.Malloc(size, memAddr, _info);
					if (IsErrorResult(resType))
						return resType;

					//Write Addr to Destination
					resType = WriteToDestArg(execFrame, instr.GetMallocDest(), memAddr, _info);
					if (IsErrorResult(resType))
						return resType;
				} break;
				case InstructionType::FREE: {
					ResultType resType = ResultType::SUCCESS;

					//Get Address
					int addr = 0;
					resType = ReadFromSrcArg(execFrame, instr.GetFreeAddr(), addr, _info);
					if (IsErrorResult(resType))
						return resType;

					//Free Address
					resType = memory.Free(addr, _info);
					if (IsErrorResult(resType))
						return resType;
				} break;
				case InstructionType::PRINT: {
					ResultType resType = ResultType::SUCCESS;

					int memAddr = 0;
					resType = ReadFromSrcArg(execFrame, instr.GetPrintSrc(), memAddr, _info);
					if (IsErrorResult(resType))
						return resType;

					int length = 0;
					resType = ReadFromSrcArg(execFrame, instr.GetPrintLength(), length, _info);
					if (IsErrorResult(resType))
						return resType;

					//Get Chars
					std::vector<int> chars(length);
					resType = memory.Read(memAddr, length, chars.data(), _info);
					if (IsErrorResult(resType))
						return resType;

					//Build String
					std::stringstream stream;
					for (int c : chars)
						stream << (char)c;

					//Print
					printf(stream.str().c_str());
				} break;
				case InstructionType::JUMP: ip = instr.GetJumpLabelIdx() - 1; break;
				case InstructionType::CJUMP: {
					int condVal = 0;
					ResultType resType = ReadFromSrcArg(execFrame, instr.GetCJumpCondSrc(), condVal, _info);

					if (IsErrorResult(resType)) { return resType; }
					else if (condVal != 0) { ip = instr.GetCJumpLabelIdx() - 1; }
				} break;
				case InstructionType::RETURN: {
					if (execFrames.empty()) { return ResultType::ERR_RET_TOP_LEVEL; }
					else
					{
						ExecutionFrame& parentExecFrame = execFrames.size() == 1 ? topLevelExecFrame : execFrames[execFrames.size() - 2];
						ResultType resType = ResultType::SUCCESS;

						auto retSrcs = instr.GetReturnSrcs();
						for (int i = 0; i < instr.GetReturnNumSrcs(); i++)
						{
							int retVal = 0;

							//Read from current exec frame
							resType = ReadFromSrcArg(execFrame, retSrcs[i], retVal, _info);
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
					}
				} break;
				case InstructionType::CALL: {
					ExecutionFrame newExecFrame = ExecutionFrame(ip, instr.GetCallRegCount());
					ResultType resType = ResultType::SUCCESS;

					auto argSrcs = instr.GetCallArgSrcs();
					for (int i = 0; i < instr.GetCallNumSrcs(); i++)
					{
						int argVal = 0;

						//Read from current exec frame
						resType = ReadFromSrcArg(execFrame, argSrcs[i], argVal, _info);
						if (IsErrorResult(resType))
							return resType;

						//Write to new exec frame
						resType = WriteToDestArg(newExecFrame, Argument(ArgType::REGISTER, i), argVal, _info);
						if (IsErrorResult(resType))
							return resType;
					}

					auto retDests = instr.GetCallRetDests();
					execFrame.SetReturnDests(retDests);

					execFrames.push_back(newExecFrame);
					ip = instr.GetCallLabelIdx() - 1;
				} break;
				case InstructionType::BINOP: {
					ResultType resType = ResultType::SUCCESS;
					int val1 = 0, val2 = 0;

					//Read
					resType = ReadFromSrcArg(execFrame, instr.GetBinopSrc1(), val1, _info);
					if (IsErrorResult(resType))
						return resType;

					resType = ReadFromSrcArg(execFrame, instr.GetBinopSrc2(), val2, _info);
					if (IsErrorResult(resType))
						return resType;

					//Calculate
					int resVal = 0;

					switch (instr.GetBinopType()) //Switch on Operation Type
					{
						case Binop::ADD: resVal = val1 + val2; break;
						case Binop::SUB: resVal = val1 - val2; break;
						case Binop::MUL: resVal = val1 * val2; break;
						case Binop::DIV: {
							if (val2 != 0) { resVal = val1 / val2; }
							else { resType = ResultType::ERR_DIVBYZERO; }
						} break;
						case Binop::MOD: {
							if (val2 != 0) { resVal = val1 % val2; }
							else { resType = ResultType::ERR_DIVBYZERO; }
						} break;
						case Binop::LSHIFT: resVal = val1 << val2; break;
						case Binop::RSHIFT: resVal = val1 >> val2; break;
						case Binop::LT: resVal = val1 < val2 ? 1 : 0; break;
						case Binop::GT: resVal = val1 > val2 ? 1 : 0; break;
						case Binop::LTEQ: resVal = val1 <= val2 ? 1 : 0; break;
						case Binop::GTEQ: resVal = val1 >= val2 ? 1 : 0; break;
						case Binop::EQ: resVal = val1 == val2 ? 1 : 0; break;
						case Binop::NEQ: resVal = val1 != val2 ? 1 : 0; break;
						case Binop::BIT_AND: resVal = val1 & val2; break;
						case Binop::BIT_OR: resVal = val1 | val2; break;
						case Binop::BIT_XOR: resVal = val1 ^ val2; break;
						case Binop::LOG_AND: resVal = (bool)val1 && (bool)val2 ? 1 : 0; break;
						case Binop::LOG_OR: resVal = (bool)val1 || (bool)val2 ? 1 : 0; break;
					}

					resType = WriteToDestArg(execFrame, instr.GetBinopDest(), resVal, _info);
					if (IsErrorResult(resType))
						return resType;
				} break;
				case InstructionType::UNOP: {
					ResultType resType = ResultType::SUCCESS;
					int val = 0;

					//Read
					resType = ReadFromSrcArg(execFrame, instr.GetUnopSrc(), val, _info);
					if (IsErrorResult(resType))
						return resType;

					//Calculate
					switch (instr.GetUnopType()) //Switch on Operation Type
					{
						case Unop::LOG_NOT: resType = WriteToDestArg(execFrame, instr.GetUnopDest(), !((bool)val), _info); break;
						case Unop::NEG: resType = WriteToDestArg(execFrame, instr.GetUnopDest(), -val, _info); break;
					}

					if (IsErrorResult(resType))
						return resType;
				} break;
				case InstructionType::PUSH: {
					ResultType resType = ResultType::SUCCESS;

					auto pushSrcs = instr.GetPushSrcs();
					for (int i = 0; i < instr.GetPushNumSrcs(); i++)
					{
						int pushVal = 0;

						//Read Value
						resType = ReadFromSrcArg(execFrame, pushSrcs[i], pushVal, _info);
						if (IsErrorResult(resType))
							return resType;

						//Push Value
						stack.push_back(pushVal);
					}
				} break;
				case InstructionType::POP: {
					ResultType resType = ResultType::SUCCESS;
					int amt = 0;

					//Read Amount
					resType = ReadFromSrcArg(execFrame, instr.GetPopAmt(), amt, _info);
					if (IsErrorResult(resType))
						return resType;

					//Pop Amount
					int sp = GetSP();

					if (amt < 0 || sp < amt - 1) { return ResultType::ERR_POP_AMT; }
					else if (amt == 0) { return ResultType::SUCCESS; }
					else if (sp == 0) { stack.clear(); }
					else { stack.erase(stack.begin() + sp - amt + 1, stack.end()); }
				} break;
				default: return ResultType::ERR_UNKNOWNINSTR;
			}
		}

		return ResultType::ERR_NOHALT;
	}

	ResultType VM::ReadFromSrcArg(ExecutionFrame& _execFrame, Argument _arg, int& _value, ResultInfo& _info)
	{
		switch (_arg.type)
		{
			case ArgType::VALUE: {
				_value = _arg.value;
				return ResultType::SUCCESS;
			}
			case ArgType::REGISTER: return _execFrame.ReadRegister(_arg.value, _value, _info);
			case ArgType::MEM_REG: {
				int addr = 0;
				ResultType res = _execFrame.ReadRegister(_arg.value, addr, _info);

				if (IsErrorResult(res)) { return res; }
				else { return memory.Read(addr, _value, _info); }
			}
			case ArgType::STACK_REG: {
				int pos = 0;
				ResultType res = _execFrame.ReadRegister(_arg.value, pos, _info);

				if (IsErrorResult(res)) { return res; }
				else { return ReadStack(pos, _value); }
			}
			case ArgType::STACK_PTR: {
				_value = GetSP();
				return ResultType::SUCCESS;
			} break;
			case ArgType::SP_OFFSET: return ReadStack(GetSP() + _arg.value, _value);
			default: return ResultType::ERR_ARGUMENT;
		}
	}

	ResultType VM::WriteToDestArg(ExecutionFrame& _execFrame, Argument _arg, int _value, ResultInfo& _info)
	{
		switch (_arg.type)
		{
			case ArgType::REGISTER: return _execFrame.WriteRegister(_arg.value, _value, _info);
			case ArgType::MEM_REG: {
				int addr = 0;
				ResultType res = _execFrame.ReadRegister(_arg.value, addr, _info);

				if (IsErrorResult(res)) { return res; }
				else { return memory.Write(addr, _value, _info); }
			}
			case ArgType::STACK_REG: {
				int pos = 0;
				ResultType res = _execFrame.ReadRegister(_arg.value, pos, _info);

				if (IsErrorResult(res)) { return res; }
				else { return WriteStack(pos, _value); }
			}
			case ArgType::SP_OFFSET: {
				if (_arg.value == 1)
				{
					stack.push_back(_value);
					return ResultType::SUCCESS;
				}
				else { return WriteStack(GetSP() + _arg.value, _value); }
			}
			default: return ResultType::ERR_INVALID_DEST;
		}
	}

	ResultType VM::ReadStack(int _pos, int& _value)
	{
		if (_pos >= 0 && _pos < (int)stack.size())
		{
			_value = stack[_pos];
			return ResultType::SUCCESS;
		}
		else { return ResultType::ERR_STACK_READ; }
	}

	ResultType VM::WriteStack(int _pos, int _value)
	{
		if (_pos >= 0 && _pos < (int)stack.size())
		{
			stack[_pos] = _value;
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
		std::vector<int> buffer(_length);
		memory.Read(_startAddr, _length, buffer.data(), resInfo);

		for (int i = 0; i < _length; i++)
			std::cout << _startAddr + i << ": " << buffer[i] << std::endl;

		std::cout << "---------------------------------------------------" << std::endl;
	}

	void VM::PrintStack()
	{
		std::cout << "---------------------STACK---------------------" << std::endl;

		int sp = GetSP();

		for (int i = 0; i < (int)stack.size(); i++)
			std::cout << i << ": " << stack[i] << (i == sp ? "  <- SP" : "") << std::endl;

		std::cout << "--------------------------------------------------" << std::endl;
	}
}