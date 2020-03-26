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
		DataValue value;

		switch (_type)
		{
			case DataType::BYTE: value.b = _buffer[0]; break;
			case DataType::INT: case DataType::FLOAT: {
				value.bytes[0] = _buffer[0];
				value.bytes[1] = _buffer[1];
				value.bytes[2] = _buffer[2];
				value.bytes[3] = _buffer[3];
			} break;
			case DataType::DOUBLE: case DataType::LONG: {
				value.bytes[0] = _buffer[0];
				value.bytes[1] = _buffer[1];
				value.bytes[2] = _buffer[2];
				value.bytes[3] = _buffer[3];
				value.bytes[4] = _buffer[4];
				value.bytes[5] = _buffer[5];
				value.bytes[6] = _buffer[6];
				value.bytes[7] = _buffer[7];
			} break;
		}

		return DataVariant(_type, value);
	}

	inline void DataValueToBuffer(byte* _buffer, DataVariant _value)
	{
		byte* bytes = _value.Bytes();

		switch (_value.GetType())
		{
			case DataType::BYTE: _buffer[0] = bytes[0]; break;
			case DataType::INT: case DataType::FLOAT: {
				_buffer[0] = bytes[0];
				_buffer[1] = bytes[1];
				_buffer[2] = bytes[2];
				_buffer[3] = bytes[3];
			} break;
			case DataType::DOUBLE: case DataType::LONG: {
				_buffer[0] = bytes[0];
				_buffer[1] = bytes[1];
				_buffer[2] = bytes[2];
				_buffer[3] = bytes[3];
				_buffer[4] = bytes[4];
				_buffer[5] = bytes[5];
				_buffer[6] = bytes[6];
				_buffer[7] = bytes[7];
			} break;
		}
	}

	inline constexpr DataType CharToDataType(char _c)
	{
		switch (_c)
		{
			case 'B': return DataType::BYTE;
			case 'I': return DataType::INT;
			case 'F': return DataType::FLOAT;
			case 'D': return DataType::DOUBLE;
			case 'L': return DataType::LONG;
			default: return DataType::UNKNOWN;
		}
	}
	inline constexpr DataTypeDouble CharsToDataTypes(char _c1, char _c2) { return { CharToDataType(_c1), CharToDataType(_c2) }; }
	inline constexpr DataTypeTriple CharsToDataTypes(char _c1, char _c2, char _c3) { return { CharToDataType(_c1), CharToDataType(_c2), CharToDataType(_c3) }; }
	inline std::vector<DataType> CharsToDataTypes(const std::string& _chars)
	{
		std::vector<DataType> result;
		result.reserve(_chars.length());

		for (auto const& c : _chars)
			result.push_back(CharToDataType(c));

		return result;
	}
	inline constexpr char DataTypeToChar(DataType _type)
	{
		switch (_type)
		{
			case DataType::BYTE: return 'B';
			case DataType::INT: return 'I';
			case DataType::FLOAT: return 'F';
			case DataType::DOUBLE: return 'D';
			case DataType::LONG: return 'L';
			default: return 0;
		}
	}
	inline std::string DataTypesToChars(const std::vector<DataType>& _types, bool _includeBrackets)
	{
		std::string result;

		for (auto const& t : _types)
			result.push_back(DataTypeToChar(t));

		return _includeBrackets ? "<" + result + ">" : result;
	}


	void PrintResult(ResultType _type);
}