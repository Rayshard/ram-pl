#include "pch.h"
#include "ramvm_vm.h"
#include "ramvm_parser.h"
#include "ramc_environment.h"
#include "ramvm_instruction.h"
#include "ramvm_lexer.h"
#include "ramc_lexer.h"
#include "ramc_token.h"
#include "ramc_parser.h"
#include "ramc_ast.h"

bool OptimizeInstructionSet(ramvm::InstructionSet& _set)
{
	ramvm::InstructionSet result;
	bool changed = false;

	/*for (int i = 0; i < (int)_set.size(); i++)
	{
		auto instr = _set[i];

		if (i < (int)_set.size() - 1) //Single Instruction Optimizations
		{
			if (instr.GetType() == ramvm::InstructionType::MOVE &&
				instr.GetMoveDest().IsStackTop())
			{
				result.push_back(ramvm::Instruction::CreatePush(instr.GetMoveSrc()));
				changed = true;
				continue;
			}
		}

		if (i < (int)_set.size() - 2) //Double Instruction Optimizations
		{
			auto instr2 = _set[i + 1];

			if (instr.GetType() == ramvm::InstructionType::PUSH && instr2.GetType() == ramvm::InstructionType::PUSH)
			{
				std::vector<ramvm::Argument> srcs = instr.GetPushSrcs();
				auto sndSrcs = instr2.GetPushSrcs();

				srcs.insert(srcs.end(), sndSrcs.begin(), sndSrcs.end());
				result.push_back(ramvm::Instruction::CreatePush(srcs));
				changed = true;
				i++;
				continue;
			}
		}

		if (i < (int)_set.size() - 3) //Triple Instruction Optimizations
		{
			auto instr2 = _set[i + 1], instr3 = _set[i + 2];

			if (instr.GetType() == ramvm::InstructionType::PUSH &&
				instr2.GetType() == ramvm::InstructionType::BINOP &&
				instr3.IsSinglePop() &&
				instr.GetPushNumSrcs() >= 2 &&
				instr2.GetBinopDest().IsStackPrev())
			{
				if (instr2.GetBinopSrc1().IsStackCur() && instr2.GetBinopSrc2().IsStackPrev() ||
					instr2.GetBinopSrc1().IsStackPrev() && instr2.GetBinopSrc2().IsStackCur())
				{
					auto pushSrcs = instr.GetPushSrcs();
					auto src1 = pushSrcs.back();
					pushSrcs.pop_back();
					auto src2 = pushSrcs.back();
					pushSrcs.pop_back();

					if (instr2.GetBinopSrc1().IsStackPrev()) //If arguments are reversed
					{
						auto temp = src1;
						src1 = src2;
						src2 = temp;
					}

					if (!pushSrcs.empty())
						result.push_back(ramvm::Instruction::CreatePush(pushSrcs));

					result.push_back(ramvm::Instruction::CreateBinop(instr2.GetBinopType(), src1, src2, ramvm::Argument::CreateStackTop()));
					changed = true;
					i+=2;
					continue;
				}
			}

			if (instr.GetType() == ramvm::InstructionType::BINOP &&
				instr2.GetType() == ramvm::InstructionType::MOVE &&
				instr3.IsSinglePop() &&
				instr.GetBinopDest().IsStackTop() &&
				instr2.GetMoveSrc().IsStackCur() &&
				!instr2.GetMoveDest().IsStackTop())
			{
				result.push_back(ramvm::Instruction::CreateBinop(instr.GetBinopType(), instr.GetBinopSrc1(), instr.GetBinopSrc2(), instr2.GetMoveDest()));
				changed = true;
				i += 2;
				continue;
			}
		}

		result.push_back(instr);
	}*/

	if (changed)
		_set = result;

	return changed;
}

int CompilerMain()
{
	using namespace ramc;

	std::ifstream stream("Tests/Compiler/DynamicMemory.ram");
	ParseResult result = ParseFile(&stream, "test.ram", 4);

	if (!result.IsSuccess()) { std::cout << "Parse Error: " << result.ToString() << std::endl; }
	else
	{
		ASTProgram* prog = dynamic_cast<ASTProgram*>(result.GetNode());

		std::cout << "Program AST:" << std::endl << prog->ToString(1) << std::endl;

		Environment env(0);
		TypeResult typeRes = prog->TypeCheck(&env);

		if (!typeRes.IsSuccess())
		{
			std::cout << "Type Error: " << typeRes.ToString(true) << std::endl;
			return 0;
		}

		auto instrs = prog->GenerateCode(&env);
		std::cout << "Program Instructions (Unoptimized): " << std::to_string(instrs.size()) << " Instructions" << std::endl;

		ProgramInfo progInfo = prog->GetInfo();
		auto labelledInstrs = progInfo.GetLabelledInstrs();
		std::unordered_multimap<Instruction*, std::string> instrLabels;

		for (auto it : labelledInstrs)
			instrLabels.insert(std::make_pair(it.second, it.first));

		for (int i = 0; i < (int)instrs.size(); i++)
		{
			Instruction* instr = instrs[i];

			auto labelSearch = instrLabels.equal_range(instr);
			for (auto it = labelSearch.first; it != labelSearch.second; ++it)
				std::cout << it->second << std::endl;

			std::cout << i << "\t" << instr->ToString() << std::endl;
		}

		//return 0;
		//Run VM
		using namespace ramvm;

		try
		{
			VM vm = VM(1024, instrs, progInfo.GetLabels(instrs));

			ResultInfo resInfo;
			ResultType result = vm.Run(resInfo);

			if (IsErrorResult(result)) { PrintResult(result); }
			else
			{
				vm.PrintRegisters();
				vm.PrintMemory();
				vm.PrintStack();
			}
		}
		catch (std::length_error) { PrintResult(ResultType::ERR_MEMORY_LIMIT); }

		delete prog;
	}

	return 0;
}

int VMMain()
{
	using namespace ramvm;

	//Open File
	std::ifstream file("TestProgram.ramc");
	if (!file)
	{
		std::cout << "Unable to read file" << std::endl;
		return 0;
	}

	//std::cout << LexFile(&file, 4) << std::endl;
	//return 0;

	//Parse Program
	ParseResult result = ParseFile(&file, "filename", 4);
	file.close();

	if (!result.IsSuccess())
	{
		std::cout << "Parse Error: " << result.ToString() << std::endl;
		return 0;
	}

	//Run VM
	try
	{
		auto program = result.GetInstructionSet();
		VM vm = VM(1024, program, result.GetLabels());

		ResultInfo resInfo;
		ResultType result = vm.Run(resInfo);

		if (IsErrorResult(result)) { PrintResult(result); }
		else
		{
			vm.PrintRegisters();
			vm.PrintMemory();
			vm.PrintStack();
		}
	}
	catch (...)
	{
		try
		{
			auto ePtr = std::current_exception();
			if (ePtr) { std::rethrow_exception(ePtr); }
		}
		catch (const std::exception& e) { std::cout << e.what() << std::endl; }
	}
}

int main()
{
	//return VMMain();
	return CompilerMain();
}