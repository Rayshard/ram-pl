#include "pch.h"
#include "ramvm_memory.h"

namespace ramvm {
	Memory::Memory(int _capacity)
	{
		capacity = _capacity;
		blocks = std::map<int, Block>();

		blocks.insert_or_assign(0, Block(_capacity, true));
	}

	ResultType Memory::Malloc(int _size, int& _addr, ResultInfo& _info)
	{
		if (_size <= 0)
			return ResultType::ERR_MALLOC;

		int freeAddr = -1, freeAddrLen = GetCapacity();

		//Find Smallest Fitting Free Block
		for (auto const& itBlock : blocks)
		{
			if (!itBlock.second.free)
				continue;

			if (itBlock.second.size == _size)
			{
				freeAddr = itBlock.first;
				break;
			}
			else if (itBlock.second.size > _size&& itBlock.second.size <= freeAddrLen)
			{
				freeAddr = itBlock.first;
				freeAddrLen = itBlock.second.size;
			}
		}

		//Remove/Update Old Free Block
		if (freeAddr == -1) { return ResultType::ERR_MALLOC; } //There wasn't a free one available
		else
		{
			blocks[freeAddr] = Block(_size, false);

			if (freeAddrLen != _size)
				blocks.insert_or_assign(freeAddr + _size, Block(freeAddrLen - _size, true));

			_addr = freeAddr;
			return ResultType::SUCCESS;
		}
	}

	ResultType Memory::Free(int _addr, ResultInfo& _info)
	{
		auto search = blocks.find(_addr);
		if (search == blocks.end()) { return ResultType::ERR_FREE; }
		else if (search->second.free) { return ResultType::ERR_FREE; }
		else
		{
			auto startAddr = search->first, size = search->second.size;

			auto prev = std::prev(search); //Look at previous block
			if (prev != blocks.end() && prev->second.free)
			{
				startAddr = prev->first;
				prev->second.size += size;
			}

			auto next = std::next(search); //Look at next block (two blocks after previous)
			if (next != blocks.end() && next->second.free)
			{
				blocks[startAddr].size += next->second.size;
				blocks.erase(next->first);
			}

			if (startAddr != _addr) { blocks.erase(_addr); }
			else
			{
				blocks[startAddr].free = true;
				blocks[startAddr].data.clear();
			}

			return ResultType::SUCCESS;
		}
		return ResultType::ERR_FREE;
	}

	ResultType Memory::Read(int _addr, int& _value, ResultInfo& _info)
	{
		if (IsAddress(_addr))
		{
			for (auto const& itBlock : blocks)
			{
				int blockAddr = itBlock.first;
				if (_addr >= blockAddr && _addr < blockAddr + itBlock.second.size)
				{
					if (!itBlock.second.free)
					{
						_value = itBlock.second.data[_addr - blockAddr];
						return ResultType::SUCCESS;
					}
					else { return ResultType::ERR_MEMREAD; }
				}
			}

			return ResultType::ERR_MEMREAD;
		}
		else { return ResultType::ERR_MEMREAD; }
	}

	ResultType Memory::Read(int _start, int _length, int* _buffer, ResultInfo& _info)
	{
		if (IsAddress(_start) && IsAddress(_start + _length))
		{
			for (auto itBlock = blocks.begin(); itBlock != blocks.end(); itBlock++)
			{
				if (_start >= itBlock->first)
				{
					//Get Readable Blocks
					std::vector<std::vector<int>*> blockHeaps;
					auto curBlock = itBlock;
					int remainingLength = _length;

					while (true)
					{
						if (curBlock->second.free)
							return ResultType::ERR_MEMREAD;

						blockHeaps.push_back(&(curBlock->second.data));
						remainingLength -= curBlock->second.size;

						if (remainingLength <= 0) //Gathered all the blocks
							break;

						curBlock = std::next(curBlock);
					}

					//Read from blocks to buffer
					auto it = blockHeaps.begin();
					int readOff = _start - itBlock->first, iBuffer = 0;

					while (true)
					{
						_buffer[iBuffer++] = (**it)[readOff++];

						if (iBuffer == _length) { break; }
						else if (readOff == (**it).size())
						{
							readOff = 0;
							it++;
						}
					}

					return ResultType::SUCCESS;
				}
			}

			return ResultType::ERR_MEMREAD;
		}
		else { return ResultType::ERR_MEMREAD; }
	}

	ResultType Memory::Write(int _addr, int _value, ResultInfo& _info)
	{
		if (IsAddress(_addr))
		{
			for (auto itBlock = blocks.begin(); itBlock != blocks.end(); itBlock++)
			{
				int blockAddr = itBlock->first;
				if (_addr >= blockAddr && _addr < blockAddr + itBlock->second.size)
				{
					if (!itBlock->second.free)
					{
						itBlock->second.data[_addr - itBlock->first] = _value;
						return ResultType::SUCCESS;
					}
					else { return ResultType::ERR_MEMWRITE; }
				}
			}

			return ResultType::ERR_MEMWRITE;
		}
		else { return ResultType::ERR_MEMWRITE; }
	}

	Memory::Block::Block(int _size, bool _free)
	{
		size = _size;
		free = _free;

		if (!free)
			data = std::vector<int>(_size);
	}
}