#pragma once

namespace ramvm
{
	enum class ArgType { VALUE, REGISTER, STACK, MEMORY };
	enum class RegisterArgType { REGULAR, MEMORY, STACK, SP };
	enum class StackArgType { ABSOLUTE, SP_OFFSETED };

	class Argument
	{
		ArgType type;
	protected:
		Argument(ArgType _type) : type(_type) { }
	public:
		bool IsStackTop();
		bool IsStackCur();
		bool IsStackPrev();

		virtual std::string ToString() = 0;
		virtual Argument* GetCopy() = 0;

		ArgType GetType() { return type; }
	};

	class ValueArgument : public Argument {
		DataValue value;
	public:
		ValueArgument(DataValue _value) :
			Argument(ArgType::VALUE), value(_value) { }

		DataValue GetValue() { return value; }
		std::string ToString() override { return ToHexString(value.bytes, LONG_SIZE); }
		Argument* GetCopy() override { return new ValueArgument(value); }
	};

	class RegisterArgument : public Argument {
		RegisterArgType readType;
		int index;

		RegisterArgument(RegisterArgType _readType, int _index)
			: Argument(ArgType::REGISTER), readType(_readType), index(_index) { }
	public:
		int GetIndex() { return index; }
		RegisterArgType GetReadType() { return readType; }
		Argument* GetCopy() override { return new RegisterArgument(readType, index); }

		std::string ToString() override;

		static RegisterArgument* CreateRegular(int _index) { return new RegisterArgument(RegisterArgType::REGULAR, _index); }
		static RegisterArgument* CreateStack(int _index) { return new RegisterArgument(RegisterArgType::STACK, _index); }
		static RegisterArgument* CreateMemory(int _index) { return new RegisterArgument(RegisterArgType::MEMORY, _index); }
		static RegisterArgument* CreateSP() { return new RegisterArgument(RegisterArgType::SP, 0); }
	};

	class StackArgument : public Argument {
		StackArgType readType;
		int value; //Could be the absolute value or offset
	public:
		StackArgument(StackArgType _readType, int _value);

		int GetValue() { return value; }
		StackArgType GetReadType() { return readType; }
		Argument* GetCopy() override { return new StackArgument(readType, value); }
		bool IsStackTop() { return readType == StackArgType::SP_OFFSETED && value == 1; }
		bool IsStackCur() { return readType == StackArgType::SP_OFFSETED && value == 0; }
		bool IsStackPrev() { return readType == StackArgType::SP_OFFSETED && value == -1; }

		std::string ToString() override;

		static StackArgument* GenStackTop() { return new StackArgument(StackArgType::SP_OFFSETED, 1); };
		static StackArgument* GenStackCur() { return new StackArgument(StackArgType::SP_OFFSETED, 0); };
		static StackArgument* GenStackPrev() { return new StackArgument(StackArgType::SP_OFFSETED, -1); };
	};
}