#include "pch.h"
#include "ramvm_memory.h"

namespace ramvm {
	Memory::Memory()
	{
		capacity = 0;
	}

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

	ResultType Memory::ReadBuffer(int _start, int _length, byte* _buffer, ResultInfo& _info)
	{
		int endAddr = _start + _length - 1;

		if (IsAddress(_start) && IsAddress(endAddr))
		{
			auto startBlock = blocks.end();

			//Check is block range is allocated
			for (auto itBlock = blocks.begin(); itBlock != blocks.end(); itBlock++)
			{
				if (startBlock != blocks.end())
				{
					if (itBlock->second.free)
						return ResultType::ERR_MEMREAD; //Free Block between start and end

					if (endAddr >= itBlock->first)
						break;
				}
				else if (_start >= itBlock->first)
				{
					if (itBlock->second.free)
						return ResultType::ERR_MEMREAD;

					startBlock = itBlock; //Set Start Block

					//Check if end address is in
					if (endAddr < itBlock->first + itBlock->second.size)
						break;
				}
				else { continue; }
			}

			//Read from buffer to blocks
			int readOff = _start - startBlock->first, iBuffer = 0;

			while (true)
			{
				_buffer[iBuffer++] = startBlock->second.data[readOff++];

				if (iBuffer == _length) { break; }
				else if (readOff == startBlock->second.size)
				{
					readOff = 0;
					startBlock++;
				}
			}

			return ResultType::SUCCESS;
		}
		else { return ResultType::ERR_MEMREAD; }
	}

	ResultType Memory::WriteBuffer(int _start, int _length, byte* _buffer, ResultInfo& _info)
	{
		int endAddr = _start + _length - 1;

		if (IsAddress(_start) && IsAddress(endAddr))
		{
			auto startBlock = blocks.end();

			//Check is block range is allocated
			for (auto itBlock = blocks.begin(); itBlock != blocks.end(); itBlock++)
			{
				if (startBlock != blocks.end())
				{
					if (itBlock->second.free)
						return ResultType::ERR_MEMWRITE; //Free Block between start and end

					if (endAddr >= itBlock->first)
						break;
				}
				else if (_start >= itBlock->first)
				{
					if (itBlock->second.free)
						return ResultType::ERR_MEMWRITE;

					startBlock = itBlock; //Set Start Block

					//Check if end address is in
					if (endAddr < itBlock->first + itBlock->second.size)
						break;
				}
				else { continue; }
			}

			//Write from buffer to blocks
			int writeOff = _start - startBlock->first, iBuffer = 0;

			while (true)
			{
				startBlock->second.data[writeOff++] = _buffer[iBuffer++];

				if (iBuffer == _length) { break; }
				else if (writeOff == startBlock->second.size)
				{
					writeOff = 0;
					startBlock++;
				}
			}

			return ResultType::SUCCESS;
		}
		else { return ResultType::ERR_MEMWRITE; }
	}

	ResultType Memory::Read(int _addr, DataVariant& _value, ResultInfo& _info)
	{
		if (_value.GetType() == DataType::BYTE)
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
							_value.AsBytes()[0] = itBlock.second.data[_addr - blockAddr];
							return ResultType::SUCCESS;
						}
						else { return ResultType::ERR_MEMREAD; }
					}
				}

				return ResultType::ERR_MEMREAD;
			}
			else { return ResultType::ERR_MEMREAD; }
		}
		else { return ReadBuffer(_addr, _value.GetSize(), _value.AsBytes(), _info); }
	}

	ResultType Memory::Write(int _addr, DataVariant _value, ResultInfo& _info)
	{
		if (_value.GetType() == DataType::BYTE)
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
							itBlock->second.data[_addr - blockAddr] = _value.AsByte();
							return ResultType::SUCCESS;
						}
						else { return ResultType::ERR_MEMWRITE; }
					}
				}

				return ResultType::ERR_MEMWRITE;
			}
			else { return ResultType::ERR_MEMWRITE; }
		}
		else { return WriteBuffer(_addr, _value.GetSize(), _value.AsBytes(), _info); }
	}

	Memory::Block::Block()
	{
		size = 0;
		free = true;
	}

	Memory::Block::Block(int _size, bool _free)
	{
		size = _size;
		free = _free;

		if (!free)
			data = std::vector<byte>(_size);
	}
}