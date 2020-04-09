#pragma once

namespace ramvm {
	class Memory
	{
	private:
		struct Block
		{
			int size;
			bool free;
			std::vector<byte> data;

			Block();
			Block(int _size, bool _free);
		};

		std::map<int, Block> blocks;
		int capacity;

		bool IsAddress(int _addr) { return _addr >= 0 && _addr < GetCapacity(); }
	public:
		Memory();
		Memory(int _capacity);

		ResultType Malloc(int _size, int& _addr, ResultInfo& _info);
		ResultType Free(int _addr, ResultInfo& _info);
		ResultType ReadBuffer(int _start, int _length, byte* _buffer, ResultInfo& _info);
		ResultType WriteBuffer(int _start, int _length, byte* _buffer, ResultInfo& _info);
		ResultType Read(int _addr, DataVariant& _value, ResultInfo& _info);
		ResultType Write(int _addr, DataVariant _value, ResultInfo& _info);
		void PrintAllocatedBlocks(int _bytesPerRow);

		int GetCapacity() { return capacity; }
	};
}