#pragma once

namespace ramvm {
	enum class ResultType {
		SUCCESS,
		ERR_MALLOC,
		ERR_FREE,
		ERR_MEMREAD,
		ERR_MEMWRITE,
		ERR_NOHALT,
		ERR_UNKNOWNINSTR,
		ERR_REG_IDX_OOB,
		ERR_DIVBYZERO,
		ERR_INVALID_DEST,
		ERR_ARGUMENT,
		ERR_NO_EXEC_FRAMES,
		ERR_RET_TOP_LEVEL,
		ERR_RET_DEST_IDX_OOB,
		ERR_MEMORY_LIMIT,
		ERR_STACK_READ,
		ERR_STACK_WRITE,
		ERR_POP_AMT,
		ERR_REG_READ,
		ERR_REG_WRTIE,
		ERR_DO_BINOP,
		ERR_DO_UNOP,
		PARSE_ERR_UNKNOWN_INSTR,
		PARSE_ERR_WRONG_NUM_ARGS,
		PARSE_ERR_INVALID_ARG,
		PARSE_ERR_DUPLICATE_LABEL,
		PARSE_ERR_UNKNOWN_LABEL
	};

	typedef std::map<std::string, std::string> ResultInfo;

	inline bool IsErrorResult(ResultType _type) { return _type != ResultType::SUCCESS; }
	
	inline DataVariant BufferToDataValue(DataType _type, byte* _buffer)
	{
		DataVariant value;

		switch (_type)
		{
			case DataType::BYTE: value = DataVariant(_buffer[0]); break;
			case DataType::INT: value = DataVariant(int(_buffer[0] << 24 | _buffer[1] << 16 | _buffer[2] << 8 | _buffer[3])); break;
		}

		return value;
	}

	inline void DataValueToBuffer(byte* _buffer, DataVariant _value)
	{
		switch (_value.GetType())
		{
			case DataType::BYTE: _buffer[0] = _value.AsByte(); break;
			case DataType::INT: {
				_buffer[0] = (byte)(_value.AsInt() >> 24);
				_buffer[1] = (byte)(_value.AsInt() >> 16);
				_buffer[2] = (byte)(_value.AsInt() >> 8);
				_buffer[3] = (byte)_value.AsInt();
			} break;
		}
	}

	void PrintResult(ResultType _type);
}