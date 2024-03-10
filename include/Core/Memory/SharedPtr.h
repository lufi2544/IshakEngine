#pragma once
#include "MemoryManager.h"
#include "WeakPtr.h"

namespace ishak { namespace Memory {

	// TODO TESTCASE When we want a sharedptr to handle another object, but is already handling one.


	/** Smart pointer that handles object like memory. Behaves the same as std::shared_ptr from STL. */
	template <class T>
	class SharedPtr
	{

		/** Class that handles the memory count and dictates destruction. */
		struct MemoryBlock
		{
			MemoryBlock() = default;

			size_t weakRefs{ 0 };
			size_t strongRefs{ 0 };
		};

	public:
		typedef T ObjectT;
		typedef T* ObjectPtr;

		SharedPtr() = default;
		SharedPtr(ObjectPtr ptr)
		{
			m_ptr = ptr;
			m_memBlock = static_cast<MemoryBlock*>(MemoryManager::Get().GetEngineAllocator()->Malloc(sizeof(MemoryBlock)));
			++m_memBlock->strongRefs;
		}
		~SharedPtr()
		{
			--m_memBlock->strongRefs;

			if (m_memBlock->strongRefs == 0)
			{
				// No more strong refs, then we are pend to be free the memory we hold.
				MemoryManager::Get().GetEngineAllocator()->Free(m_ptr);
			}
		}

		SharedPtr(SharedPtr const& other)
		{
			m_ptr = other.m_ptr;
			other.m_memBlock->strongRefs++;
			m_memBlock = other.m_memBlock;
		}

		SharedPtr(SharedPtr&& other) noexcept
		{
			m_ptr = other.m_ptr;
			m_memBlock = other.m_memBlock;

			other.m_memBlock = nullptr;
			other.m_ptr = nullptr;
		}

		SharedPtr& operator = (SharedPtr const& other)
		{
			// Check if not already handling an object.
			// smrt a = new Smart(dog);
			// a = new Smart(anotherDog) --> Exception

			m_ptr = other.m_ptr;
			m_memBlock = other.m_memBlock;

			m_memBlock->strongRefs++;

			return &this;
		}

		// TODO Repeat with WeakPtr



		MemoryBlock* m_memBlock{ nullptr };
		ObjectPtr m_ptr{ nullptr };
	};

}}//ishak::Memory