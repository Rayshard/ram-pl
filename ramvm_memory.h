#pragma once

namespace ramvm {
	class Memory
	{
	private:
		struct Block
		{
			int size;
			bool free;
			std::vector<int> data;

			Block() = default;
			Block(int _size, bool _free);
		};

		std::map<int, Block> blocks;
		int capacity;

		bool IsAddress(int _addr) { return _addr >= 0 && _addr < GetCapacity(); }
	public:
		Memory() = default;
		Memory(int _capacity);

		ResultType Malloc(int _size, int& _addr, ResultInfo& _info);
		ResultType Free(int _addr, ResultInfo& _info);
		ResultType Read(int _addr, int& _value, ResultInfo& _info);
		ResultType Read(int _start, int _length, int* buffer, ResultInfo& _info);
		ResultType Write(int _addr, int _value, ResultInfo& _info);

		int GetCapacity() { return capacity; }
	};
}