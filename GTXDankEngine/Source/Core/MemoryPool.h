#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H

#include "pch.h"

#include "engine.h"
#include "Enums.h"
#include "../utils/Log.h"

template<unsigned int blockSize, unsigned int blocksPerPage>
class MemoryPool
{
	
public:
	struct Block
	{
		char padding[blockSize];
	};
	void* GetFreeBlock();
	void FreeUsedBlock(void* blockToFree);
	int GetMemoryGotten() { return memoryGotten; }


private:
	ErrorEnum AquireNewPage();
	std::list<Block*> freeBlocks;
	int memoryGotten = 0;
};
//.cpp file stuff that needs to be in the header for template reasons


template<unsigned int blockSize, unsigned int blocksPerPage>
inline void* MemoryPool<blockSize, blocksPerPage>::GetFreeBlock()
{
	if (freeBlocks.size() <= 1)
	{
		ErrorEnum didAlocate = AquireNewPage();
		if (didAlocate != ErrorEnum::SUCCESS)
		{
			return nullptr;
		}
	}
	void* returnValue = static_cast<void*>(*freeBlocks.begin());
	freeBlocks.pop_front();

	return returnValue;
}

template<unsigned int blockSize, unsigned int blocksPerPage>
inline void MemoryPool<blockSize, blocksPerPage>::FreeUsedBlock(void* blockToFree)
{
	//type unsafe, im trusting you to return what I gave you
	//perhaps have a list of void* i allocated to check if im gettign one back?
	Block* returningBlock = static_cast<Block*>(blockToFree);

	freeBlocks.push_back(returningBlock);
}


template<unsigned int blockSize, unsigned int blocksPerPage>
inline ErrorEnum MemoryPool<blockSize, blocksPerPage>::AquireNewPage()
{
	try
	{
		Block* firstBlock = new Block[blocksPerPage];
		for (unsigned int i = 0; i < blocksPerPage; ++i)
		{
			freeBlocks.push_back(firstBlock + i);
		}
		memoryGotten += blockSize * blocksPerPage;
		return ErrorEnum::SUCCESS;
	}
	catch (std::bad_alloc&)
	{
		LOG_ERROR("MEMORY_POOL ALOCATION ERROR");
		assert(!"in memory pool, failed to alocate memory");
		return ErrorEnum::FAIL_BECAUSE_NO_MORE_MEMORY;
	}
	
}

#endif // !MEMORYPOOL_H
