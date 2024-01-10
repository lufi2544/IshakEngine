#pragma once 
#include "CoreConfig.h"
#include "CoreMinimal.h"

namespace ishak{ namespace Memory {

	/* Base class for allocators, needed? */
	class CORE_API Allocator
	{
	public:
		Allocator() = default;
		Allocator(Allocator const&) = delete;
		Allocator& operator = (Allocator const&) = delete;


		virtual void* Allocate(size_t size, size_t alignment) = 0;	
		virtual void Free(void* ptr) = 0;

		
	};


	class CORE_API StackAllocator : public Allocator
	{
		public:
			StackAllocator() = default;


			void* Allocate(size_t size, size_t alignment) override
			{
				return nullptr;
			}

			void Free(void* ptr) override
			{

			}
	};

	
}}// ishak::memory
