#pragma once

namespace ramvm
{
	enum class ArgType { VALUE, REGISTER, STACK, MEMORY };
	enum class RegisterType { SP, FP, GP };

	class Argument
	{
		ArgType type;
	protected:
		Argument(ArgType _type);
	public:
		ArgType GetType();
		std::shared_ptr<Argument> GetSharedCopy();

		bool IsStackTop();
		bool IsStackCur();
		bool IsStackPrev();

		virtual std::string ToString() = 0;
		virtual Argument* GetCopy() = 0;
	};

	class ValueArgument : public Argument {
		DataValue value;
	public:
		ValueArgument(DataValue _value);

		DataValue GetValue();

		std::string ToString() override;
		Argument* GetCopy() override;
	};

	class RegisterArgument : public Argument {
		RegisterType regType;
	public:
		RegisterArgument(RegisterType _regType);
		
		RegisterType GetRegType();

		Argument* GetCopy() override;
		std::string ToString() override;
	};

	class StackArgument : public Argument {
		Argument* posSrc;
		int offset;
	public:
		StackArgument(Argument* _posSrc, int _offset);
		~StackArgument();

		Argument* GetPosSrc();
		int GetOffset();

		bool IsStackTop();
		bool IsStackCur();
		bool IsStackPrev();

		Argument* GetCopy() override;
		std::string ToString() override;

		static StackArgument* GenStackTop();
		static StackArgument* GenStackCur();
		static StackArgument* GenStackPrev();
	};

	class MemoryArgument : public Argument {
		Argument* addrSrc;
		int offset;
	public:
		MemoryArgument(Argument* _addrSrc, int _offset);
		~MemoryArgument();

		Argument* GetAddrSrc();
		int GetOffset();

		Argument* GetCopy() override;
		std::string ToString() override;
	};
}