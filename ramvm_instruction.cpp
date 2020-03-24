#include "pch.h"
#include "ramvm_instruction.h"

namespace ramvm {
	const std::string UnopToString(Unop _type)
	{
		switch (_type)
		{
			case Unop::NEG: return "NEG";
			case Unop::LOG_NOT: return "LNOT";
			default: return "UnopToString - Unop not handled!";
		}
	}

	const std::string BinopToString(Binop _type)
	{
		switch (_type)
		{
			case ramvm::Binop::ADD: return "ADD";
			case ramvm::Binop::SUB: return "SUB";
			case ramvm::Binop::MUL: return "MUL";
			case ramvm::Binop::DIV: return "DIV";
			case ramvm::Binop::MOD: return "MOD";
			case ramvm::Binop::LSHIFT: return "LSHIFT";
			case ramvm::Binop::RSHIFT: return "RSHIFT";
			case ramvm::Binop::LT: return "LT";
			case ramvm::Binop::GT: return "GT";
			case ramvm::Binop::LTEQ: return "LTEQ";
			case ramvm::Binop::GTEQ: return "GTEQ";
			case ramvm::Binop::EQ: return "EQ";
			case ramvm::Binop::NEQ: return "NEQ";
			case ramvm::Binop::BIT_AND: return "BAND";
			case ramvm::Binop::BIT_OR: return "BOR";
			case ramvm::Binop::BIT_XOR: return "BXOR";
			case ramvm::Binop::LOG_AND: return "LAND";
			case ramvm::Binop::LOG_OR: return "LOR";
			default: return "BinopToString - Binop not handled!";
		}
	}

	Instruction::Instruction(InstructionType _type, std::vector<Argument> _args)
	{
		type = _type;
		args = _args;
	}

	std::string Instruction::ToString()
	{
		switch (type)
		{
			case ramvm::InstructionType::HALT: return "HALT";
			case ramvm::InstructionType::MOVE: return "MOV " + GetMoveSrc().ToString() + " " + GetMoveDest().ToString();
			case ramvm::InstructionType::BINOP: return BinopToString(GetBinopType()) + " " + GetBinopSrc1().ToString() + " " + GetBinopSrc2().ToString() + " " + GetBinopDest().ToString();
			case ramvm::InstructionType::UNOP: return UnopToString(GetUnopType()) + " " + GetUnopSrc().ToString() + " " + GetUnopDest().ToString();
			case ramvm::InstructionType::JUMP: return "JUMP " + GetJumpLabelIdx();
			case ramvm::InstructionType::CJUMP: return "CJUMP " + std::to_string(GetCJumpLabelIdx()) + " " + GetCJumpCondSrc().ToString();
			case ramvm::InstructionType::CALL: return "CALL (NEED TO HANDLE TOSTRING)";
			case ramvm::InstructionType::RETURN: return "RETURN (NEED TO HANDLE TOSTRING)";
			case ramvm::InstructionType::PRINT: return "PRINT " + GetPrintSrc().ToString() + " " + GetPrintLength().ToString();
			case ramvm::InstructionType::MALLOC: return "MALLOC " + GetMallocSize().ToString() + " " + GetMallocDest().ToString();
			case ramvm::InstructionType::FREE: return "FREE " + GetFreeAddr().ToString();
			case ramvm::InstructionType::PUSH: {
				std::stringstream ss;
				ss << "PUSH";

				for (auto arg : GetPushSrcs())
					ss << " " << arg.ToString();

				return ss.str();
			}
			case ramvm::InstructionType::POP: return "POP " + GetPopAmt().ToString();
			default: return "Instruction::ToString() - InstructionType not handled!";
		}
	}

	Argument::Argument(ArgType _type, int _val)
	{
		type = _type;
		value = _val;
	}

	std::string Argument::ToString()
	{
		switch (type)
		{
			case ramvm::ArgType::VALUE: return std::to_string(value);
			case ramvm::ArgType::REGISTER: return "R" + std::to_string(value);
			case ramvm::ArgType::MEM_REG: return "{R" + std::to_string(value) + "}";
			case ramvm::ArgType::STACK_PTR: return "SP";
			case ramvm::ArgType::STACK_REG: return "[R" + std::to_string(value) + "]";
			case ramvm::ArgType::SP_OFFSET: return "[" + std::to_string(value) + "]";
			case ramvm::ArgType::INVALID: return "INVAILD";
			default: return "Argument::ToString() - ArgType not handled!";
		}
	}

	Instruction Instruction::CreateMove(Argument _src, Argument _dest)
	{
		Instruction instr;
		instr.type = InstructionType::MOVE;
		instr.args.push_back(_src);
		instr.args.push_back(_dest);
		return instr;
	}

	Instruction Instruction::CreateBinop(Binop _op, Argument _src1, Argument _src2, Argument _dest)
	{
		Instruction instr;
		instr.type = InstructionType::BINOP;
		instr.args.push_back(Argument(ArgType::VALUE, (int)_op));
		instr.args.push_back(_src1);
		instr.args.push_back(_src2);
		instr.args.push_back(_dest);
		return instr;
	}

	Instruction Instruction::CreateUnop(Unop _op, Argument _src, Argument _dest)
	{
		Instruction instr;
		instr.type = InstructionType::UNOP;
		instr.args.push_back(Argument(ArgType::VALUE, (int)_op));
		instr.args.push_back(_src);
		instr.args.push_back(_dest);
		return instr;
	}

	Instruction Instruction::CreateCall(int _labelIdx, int _regCnt, std::vector<Argument>& _srcsArgs, std::vector<Argument>& _retDests)
	{
		Instruction instr;
		instr.type = InstructionType::CALL;
		instr.args.push_back(Argument(ArgType::VALUE, _labelIdx));
		instr.args.push_back(Argument(ArgType::VALUE, _regCnt));
		instr.args.push_back(Argument(ArgType::VALUE, _srcsArgs.size()));
		instr.args.push_back(Argument(ArgType::VALUE, _retDests.size()));
		instr.args.insert(instr.args.end(), _srcsArgs.begin(), _srcsArgs.end());
		instr.args.insert(instr.args.end(), _retDests.begin(), _retDests.end());
		return instr;
	}

	std::vector<Argument> Instruction::GetCallArgSrcs()
	{
		std::vector<Argument> srcs;
		int offset = 4, end = offset + GetCallNumSrcs();

		for (int i = offset; i < end; i++)
			srcs.push_back(args[i]);

		return srcs;
	}

	std::vector<Argument> Instruction::GetCallRetDests()
	{
		std::vector<Argument> dests;
		int offset = 4 + GetCallNumSrcs(), end = offset + GetCallNumDests();

		for (int i = offset; i < end; i++)
			dests.push_back(args[i]);

		return dests;
	}

	Instruction Instruction::CreateReturn(std::vector<Argument>& _srcs)
	{
		Instruction instr;
		instr.type = InstructionType::RETURN;
		instr.args = _srcs;
		return instr;
	}

	Instruction Instruction::CreateJump(int _labelIdx)
	{
		Instruction instr;
		instr.type = InstructionType::JUMP;
		instr.args.push_back(Argument(ArgType::VALUE, _labelIdx));
		return instr;
	}

	Instruction Instruction::CreateCJump(int _labelIdx, Argument _condSrc)
	{
		Instruction instr;
		instr.type = InstructionType::CJUMP;
		instr.args.push_back(Argument(ArgType::VALUE, _labelIdx));
		instr.args.push_back(_condSrc);
		return instr;
	}

	Instruction Instruction::CreatePrint(Argument _src, Argument _len)
	{
		Instruction instr;
		instr.type = InstructionType::PRINT;
		instr.args.push_back(_src);
		instr.args.push_back(_len);
		return instr;
	}

	Instruction Instruction::CreateMalloc(Argument _size, Argument _dest)
	{
		Instruction instr;
		instr.type = InstructionType::MALLOC;
		instr.args.push_back(_size);
		instr.args.push_back(_dest);
		return instr;
	}

	Instruction Instruction::CreateFree(Argument _addr)
	{
		Instruction instr;
		instr.type = InstructionType::FREE;
		instr.args.push_back(_addr);
		return instr;
	}

	Instruction Instruction::CreatePush(std::vector<Argument>& _srcs)
	{
		Instruction instr;
		instr.type = InstructionType::PUSH;
		instr.args = _srcs;
		return instr;
	}

	Instruction Instruction::CreatePush(Argument _src)
	{
		Instruction instr;
		instr.type = InstructionType::PUSH;
		instr.args = { _src };
		return instr;
	}

	Instruction Instruction::CreatePop(Argument _amt)
	{
		Instruction instr;
		instr.type = InstructionType::POP;
		instr.args.push_back(_amt);
		return instr;
	}
}
