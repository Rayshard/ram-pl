#include "pch.h"
#include "ramvm_argument.h"

namespace ramvm {
	Argument::Argument(ArgType _type) : type(_type) { }

	ArgType Argument::GetType() { return type; }
	std::shared_ptr<Argument> Argument::GetSharedCopy() { return std::shared_ptr<Argument>(this->GetCopy()); }

	bool Argument::IsStackTop() { return type == ArgType::STACK && ((StackArgument*)this)->IsStackTop(); }
	bool Argument::IsStackCur() { return type == ArgType::STACK && ((StackArgument*)this)->IsStackCur(); }
	bool Argument::IsStackPrev() { return type == ArgType::STACK && ((StackArgument*)this)->IsStackPrev(); }

#pragma region Value Argument
	ValueArgument::ValueArgument(DataValue _value) : Argument(ArgType::VALUE), value(_value) { }

	DataValue ValueArgument::GetValue() { return value; }
	Argument* ValueArgument::GetCopy() { return new ValueArgument(value); }
	std::string ValueArgument::ToString() { return ToHexString(value.bytes, LONG_SIZE); }
#pragma endregion

#pragma region Register Argument
	RegisterArgument::RegisterArgument(RegisterType _regType) : Argument(ArgType::REGISTER), regType(_regType) { }

	RegisterType RegisterArgument::GetRegType() { return regType; }
	Argument* RegisterArgument::GetCopy() { return new RegisterArgument(regType); }

	std::string RegisterArgument::ToString()
	{
		switch (regType)
		{
			case RegisterType::SP: return "$SP";
			case RegisterType::FP: return "$FP";
			case RegisterType::GP: return "$GP";
			default: ASSERT_MSG(false, "RegisterArgument::ToString() - RegisterArgType not handled!");
		}
	}
#pragma endregion

#pragma region Stack Argument
	StackArgument::StackArgument(Argument* _posSrc, int _offset) : Argument(ArgType::STACK), posSrc(_posSrc), offset(_offset) { }
	StackArgument::~StackArgument() { delete posSrc; }

	Argument* StackArgument::GetPosSrc() { return posSrc; }
	int StackArgument::GetOffset() { return offset; }
	Argument* StackArgument::GetCopy() { return new StackArgument(posSrc->GetCopy(), offset); }

	bool StackArgument::IsStackTop() { return posSrc->GetType() == ArgType::REGISTER && ((RegisterArgument*)posSrc)->GetRegType() == RegisterType::SP && offset == 1; }
	bool StackArgument::IsStackCur() { return posSrc->GetType() == ArgType::REGISTER && ((RegisterArgument*)posSrc)->GetRegType() == RegisterType::SP && offset == 0; }
	bool StackArgument::IsStackPrev() { return posSrc->GetType() == ArgType::REGISTER && ((RegisterArgument*)posSrc)->GetRegType() == RegisterType::SP && offset == -1; }

	std::string StackArgument::ToString()
	{
		if (offset == 0) { return "[" + posSrc->ToString() + "]"; }
		else if (offset > 0) { return "[" + posSrc->ToString() + "+" + std::to_string(offset) + "]"; }
		else { return "[" + posSrc->ToString() + std::to_string(offset) + "]"; }
	}

	StackArgument* StackArgument::GenStackTop() { return new StackArgument(new RegisterArgument(RegisterType::SP), 1); };
	StackArgument* StackArgument::GenStackCur() { return new StackArgument(new RegisterArgument(RegisterType::SP), 0); };
	StackArgument* StackArgument::GenStackPrev() { return new StackArgument(new RegisterArgument(RegisterType::SP), -1); };
#pragma endregion

#pragma region Memory Argument
	MemoryArgument::MemoryArgument(Argument* _addrSrc, int _offset) : Argument(ArgType::MEMORY), addrSrc(_addrSrc), offset(_offset) { }
	MemoryArgument::~MemoryArgument() { delete addrSrc; }

	Argument* MemoryArgument::GetAddrSrc() { return addrSrc; }
	int MemoryArgument::GetOffset() { return offset; }
	Argument* MemoryArgument::GetCopy() { return new MemoryArgument(addrSrc->GetCopy(), offset); }

	std::string MemoryArgument::ToString()
	{
		if (offset == 0) { return "{" + addrSrc->ToString() + "}"; }
		else if (offset > 0) { return "{" + addrSrc->ToString() + "+" + std::to_string(offset) + "}"; }
		else { return "{" + addrSrc->ToString() + std::to_string(offset) + "}"; }
	}
#pragma endregion
}