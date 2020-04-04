#include "pch.h"
#include "ramvm_argument.h"

namespace ramvm {
	bool Argument::IsStackTop() { return type == ArgType::STACK && ((StackArgument*)this)->IsStackTop(); }
	bool Argument::IsStackCur() { return type == ArgType::STACK && ((StackArgument*)this)->IsStackCur(); }
	bool Argument::IsStackPrev() { return type == ArgType::STACK && ((StackArgument*)this)->IsStackPrev(); }

	StackArgument::StackArgument(StackArgType _readType, int _value)
		: Argument(ArgType::STACK)
	{
		readType = _readType;
		value = _value;

		switch (readType)
		{
			case StackArgType::ABSOLUTE: {
				if (value < 0)
					throw std::runtime_error("StackArgument() - An abosulte stack argument can only have a nonnegative position!");
			} break;
			case StackArgType::SP_OFFSETED: {
				if (value > 1)
					throw std::runtime_error("StackArgument() - An SP offseted stack argument can only have a offset less than or equal to 1!");
			} break;
		}
	}

	std::string StackArgument::ToString()
	{
		switch (readType)
		{
			case StackArgType::ABSOLUTE: return "[" + std::to_string(value) + "]";
			case StackArgType::SP_OFFSETED: {
				if (value == 0) { return "[SP]"; }
				else if (value == 1) { return "[SP+1]"; }
				else { return "[SP" + std::to_string(value) + "]"; }
			}
			default: return "StackArgument::ToString() - StackArgType not handled!";
		}
	}
	
	std::string RegisterArgument::ToString()
	{
		switch (readType)
		{
			case RegisterArgType::REGULAR: return "R" + std::to_string(index);
			case RegisterArgType::STACK: return "[R" + std::to_string(index) + "]";
			case RegisterArgType::MEMORY: return "{R" + std::to_string(index) + "}";
			case RegisterArgType::SP: return "SP";
			default: return "RegisterArgument::ToString() - RegisterArgType not handled!";
		}
	}
}