#pragma once 
#include <algorithm>

#include "Core/CoreConfig.h"
#include "Core/CoreMinimal.h"

#include <memory_resource>

namespace ishak{ namespace Memory {

	/* Base class for allocators, needed? */
	class CORE_API Allocator
	{
	public:
		virtual ~Allocator();

		virtual void* do_allocate(size_t size, size_t alignment) = 0;	
		virtual void do_deallocate(void* ptr) = 0;
	};


	class CORE_API NewDeleteAllocator : public std::pmr::memory_resource 
	{

	public:
		NewDeleteAllocator() = default;

		~NewDeleteAllocator() override =default;
		NewDeleteAllocator(const  NewDeleteAllocator& other) = delete;
		void operator = (const NewDeleteAllocator& other) = delete;


		void* do_allocate(size_t size, size_t alignment) override
		{
			return ::operator new (size);
		}

		void do_deallocate(void* _Ptr, size_t _Bytes, size_t _Align) override
		{
			::operator delete(_Ptr);
		}

		bool do_is_equal(const memory_resource& _That) const noexcept override
		{
			return this == &_That;
		}
		
	};



    class BufferedSequentialAllocator : public std::pmr::memory_resource
	{
    public:
        BufferedSequentialAllocator(char* buffer, size_t bufferSize)
            : buffer_(buffer), bufferSize_(bufferSize)
    	{
            // Initially, the entire buffer is a free block
            freeBlocks_.push_back({ bufferSize_, buffer});
        }
    
        ~BufferedSequentialAllocator() = default;

        // Allocate memory with alignment
		void* do_allocate(size_t size, size_t alignment) override
		{
            // Find a suitable free block
            Block* block = findFreeBlock(size, alignment);

            if (block) {
                // Allocate from the free block
                size_t totalSize = size + alignment - 1;
                char* alignedAddress = reinterpret_cast<char*>(alignAddress(block->start, alignment));

                // Check if the remaining space is enough to split the block
                if (block->size > totalSize) {
                    splitBlock(*block, totalSize);
                }
                else {
                    // Remove the block from the free list if it's fully allocated
                    freeBlocks_.erase(std::remove(freeBlocks_.begin(), freeBlocks_.end(), *block), freeBlocks_.end());
                }

                return alignedAddress;
            }
            else {
                // Out of memory
                return nullptr;
            }
		}

        // Deallocate memory
		void do_deallocate(void*  ptr, size_t _Bytes, size_t _Align) override
		{
            // Add the deallocated block to the free list
            Block deallocatedBlock = { 0, static_cast<char*>(ptr) };
            freeBlocks_.push_back(deallocatedBlock);

            // Merge adjacent free blocks
            mergeFreeBlocks();
		}

        bool do_is_equal(const memory_resource& _That) const noexcept override
		{
            return this == &_That;
		}

    private:
        struct Block {
            size_t size{ 0 };
            char* start{ nullptr };

            bool operator == (Block const& other)
            {
                return other.size == size && other.start == start;
            }
        };

        char* buffer_;
        size_t bufferSize_;

        // List of free blocks
        std::vector<Block> freeBlocks_;

        // Helper function to align memory
        void* alignAddress(void* address, size_t alignment)
        {
            return reinterpret_cast<void*>((reinterpret_cast<std::uintptr_t>(address) + alignment - 1) & ~(alignment - 1));
        }

        // Helper function to find a suitable free block
        Block* findFreeBlock(size_t size, size_t alignment)
        {
            for (Block& block : freeBlocks_) {
                char* alignedAddress = reinterpret_cast<char*>(alignAddress(block.start, alignment));
                size_t totalSize = size + (alignedAddress - block.start);

                if (totalSize <= block.size) {
                    return &block;
                }
            }

            return nullptr;
        }

        // Helper function to split a block
        void splitBlock(Block& block, size_t size)
        {
            // Update the free block's start address and size
            block.start += size;
            block.size -= size;

            // Merge adjacent free blocks after splitting
            mergeFreeBlocks();
        }

        // Helper function to merge adjacent free blocks
        void mergeFreeBlocks()
        {
            // Sort the free blocks by their start addresses
            std::sort(freeBlocks_.begin(), freeBlocks_.end(), [](const Block& a, const Block& b) {
                return a.start < b.start;
                });

            // Merge adjacent free blocks
            auto it = freeBlocks_.begin();
            while (it != freeBlocks_.end()) {
                auto nextIt = std::next(it);
                if (nextIt != freeBlocks_.end() && it->start + it->size == nextIt->start) {
                    // Merge adjacent blocks
                    it->size += nextIt->size;
                    it = freeBlocks_.erase(nextIt);
                }
                else {
                    ++it;
                }
            }
        }
    };

}}// ishak::memory
