#pragma once

#include <initializer_list>

#include "Core/CoreConfig.h"
#include "Core/Memory/Allocator.h"
#include "Core/TypeTraits/TypeTraits.h"
#include <assert.h>

namespace ishak {

	/** 
	  * Dynamically allocated container for storing Data.
	  * NOTE: Custom Implementation of std::vector.
	  * // TODO MEMORY Add the allocator 
	  */

	// TODO ARRAY for now the array capacity grows with a coefficient of x2 every time an 
	// addition fails due to capacity problems.
	template<typename DataT, typename AllocatorT = memory::DefaultAllocator> 
	class CORE_API TArray 
	{	
		static size_t constexpr DEFAULT_CAPACITY = 2;
		static size_t constexpr MIN_SIZE_DELTA_TO_NEWALLOC_WHEN_RESIZED = 5;

	public:
		struct CORE_API Iterator
		{
			Iterator(DataT* ptr, size_t idxParam = 0)
				: dataPtr{ ptr  }
				, idx{ idxParam } 
			{

			}


			/** This is the definition for the ++iterattor. */
			Iterator& operator ++()
			{
				++idx;
				return *this;
			}

			/** This is a definition for the iterator++.*/
			Iterator operator ++(int)
			{
				Iterator temp = *this;
				++(*this);
				return temp;
			}


			DataT& operator * () 
			{
				return dataPtr[idx];
			}

			const DataT& operator* () const
			{
				return dataPtr[idx];	
			}

			bool operator == (Iterator const& other ) const
			{
				return idx == other.idx;
			}

			bool operator != (Iterator const& other) const
			{
				return !(*this == other);
			}

			DataT* dataPtr { nullptr };
			size_t idx{ 0 };
		};

	public:
		TArray(uint32_t capacity, AllocatorT const& allocator) 
			: m_capacity{ capacity }
			, m_allocator{ allocator }
		{
			
		}

		TArray(AllocatorT const& alloc)
			: m_allocator{ alloc }
		{
			m_capacity = DEFAULT_CAPACITY;
		}

		TArray() 
		{
			m_capacity = DEFAULT_CAPACITY;
		}

		TArray(std::initializer_list<DataT>&& initList)
		{
			m_capacity = initList.size();
			m_size = initList.size();
			AllocateCapacity();
			InitializeArrayElements();
			
			size_t idx = 0;
			for(auto& element : initList)
			{
				m_data[idx] = std::move(element);
				++idx;
			}
		}

		~TArray() 
		{
			m_allocator.Free(m_data);
		}

		TArray(const TArray<DataT, AllocatorT>& other)
		{
			m_capacity = other.m_capacity;
			m_size = other.m_size;
			AllocateCapacity();			
			for(uint32 idx = 0; idx < m_size; ++idx)
			{
				m_data[idx] = other.m_data[idx];
			}			
		}

		TArray(TArray<DataT>&& other) noexcept
		{
			m_capacity = other.m_capacity;
			m_size = other.m_size;
			m_data = other.m_data;

			// Invalidates the other container once we have moved all the data.
			other.Invalidate();
		}

		DataT* GetData()
		{
			return m_data;
		}

		DataT* GetData() const
		{
			return m_data;
		}

		//---- Iterator Functions ----
		Iterator begin()
		{
			// TODO Add TEST
			if(this == nullptr)
			{
				return Iterator(nullptr);
			}

			return Iterator(m_data);
		}

		Iterator begin() const
		{
			return Iterator(m_data);
		}

		Iterator end() const
		{
			return Iterator(m_data, m_size);		
		}

		Iterator end()
		{
			if(this == nullptr)
			{
				return Iterator(nullptr);
			}

			return Iterator(m_data,  m_size);
		}
		//----  ----


#pragma region --Operators--
		
		DataT& operator[](int32 idx) 
		{
			// TODO ASSERT Custom Assert
			// Maybe Capacity is Ok, if not, then access violation.
			assert(CheckSizeAt(idx) == true);

			return m_data[idx];
		}

		const DataT& operator[](int32 idx) const
		{
			assert(CheckSizeAt(idx) == true);

			return m_data[idx];
		}

		TArray<DataT, AllocatorT>& operator = (const TArray<DataT, AllocatorT>& other)
		{
			if(m_data)
			{
				m_allocator.Free(m_data);
			}

			m_capacity = other.m_capacity;
			m_size = other.m_size;
			AllocateCapacity();
			InitializeArrayElements();
			// TODO Add Test;
			for (uint32 idx = 0; idx < m_size; ++idx)
			{
				m_data[idx] = other.m_data[idx];
			}

			return *this;
		}

		TArray<DataT, AllocatorT>& operator = (TArray<DataT, AllocatorT>&& other)
		{
			if (m_data)
			{
				m_allocator.Free(m_data);
			}

			m_capacity = other.m_capacity;
			m_size = other.m_size;
			m_data = other.m_data;

			other.Invalidate();

			return *this;
		}

		friend bool operator == (TArray<DataT, AllocatorT> const& lhs, TArray<DataT, AllocatorT> const& rhs)
		{			
			// Check sizes first
			if(lhs.Size() != rhs.Size())
			{
				return false;
			}

			DataT* rhsData = rhs.GetData();
			DataT* lhsData = lhs.GetData();

			// Check contained data.		
			for(int i = 0; i < lhs.Size(); ++i)
			{
				if(!(rhsData[i] == lhsData[i]))
				{
					return false;
				}
			}


			return true;
		}


		friend bool operator != (TArray<DataT, AllocatorT> const& lhs, TArray<DataT, AllocatorT> const& rhs)
		{
			// Check sizes first
			if (lhs.Size() != rhs.Size())
			{
				return true;
			}

			DataT* rhsData = rhs.GetData();
			DataT* lhsData = lhs.GetData();

			// Check contained data.		
			for (int i = 0; i < lhs.Size(); ++i)
			{
				if (!(rhsData[i] == lhsData[i]))
				{
					return true;
				}
			}


			return false;
		}

#pragma endregion --Operators--


		bool IsEmpty() const
		{
			return Size() == 0;
		}

		
		DataT& Last()
		{
			return m_data[m_size - 1];
		} 

		DataT& Front()
		{			
			assert(IsEmpty() == false);

			return m_data[0];
		}
		
		void PopBack()
		{
			Remove(Size() - 1);
		}

		// TODO remve by value

		/** Deep clear. I think there is no point on leaving this with memory allocated. */
		void Clear()
		{			
			m_size = 0;
			m_capacity = DEFAULT_CAPACITY;

			if(m_data != nullptr)
			{
				m_allocator.Free(m_data);
			}

			AllocateCapacity();		
		}

		/** Adds X amount of defaulted objects in the container. */
		void AddDefaulted(uint32 count)
		{
			TryAllocateCapacity();

			const uint32 oldSize{ m_size };
			const uint32 newSize{ oldSize + count };
			if(m_capacity < newSize)
			{
				// MEMORY Maybe adding more space to the allocation? 
				m_capacity = newSize;
				AllocateCapacityAndMoveData();
			}

			m_size = newSize;

			// have in mind that if the array grows, then we just init the passed size data, thus
			// if the array was 4 size, and then once called this function we have 100 elemets and the buffer has capacity for 105, 
			// there will be only 4 elements constructed in memory, so through this way, we are 
			// initializing the new elements in memory that are new, in this case 96.
			InitializeArrayElementsFromIdx(oldSize > 0
				? newSize - oldSize - 1 
				: oldSize);
		}

		/** Adds a new value to the Array(r-value, move operator). */
		void Add(DataT&& dataToAdd)
		{			
			TryAllocateCapacity();

			// Check if we have to reallocate memory
			const bool bNeedsToReallocateMemory{ m_size == m_capacity };
			if (bNeedsToReallocateMemory) 
			{				
				m_capacity *= 2;

				AllocateCapacityAndMoveData();
			}

			// Internally Construct the Memory for copyng the new value to the Array.
			new (m_data + m_size) DataT;

			m_data[m_size++] = std::move(dataToAdd);
		}

		/** Adds a new value to the Array. */
		void Add(const DataT& dataToAdd)
		{
			TryAllocateCapacity();

			// Check if we have to reallocate memory
			const bool bNeedsToReallocateMemory{ m_size == m_capacity };
			if (bNeedsToReallocateMemory)
			{				
				m_capacity *= 2;

				AllocateCapacityAndMoveData();
			}

			// Internally Construct the Memory for copyng the new value to the Array.
			new (m_data + m_size) DataT;

			m_data[m_size++] = dataToAdd;
		}

		void AddUnique(const DataT& toAdd)
		{
			if(!Contains(toAdd))
			{
				Add(toAdd);
			}
		}

		void AddUnique(DataT&& toAdd)
		{
			if(!Contains(toAdd))
			{
				Add(toAdd);
			}
		}

		/** Removes by index a certain value in the container. */
		void Remove(int32 idxToRemove)
		{		

			CheckSizeAt(idxToRemove);

			// Find the element and swap the element to remove with the last element in the container and then
			// remove the last element in the container.
										
			// cache the elements Data.
			DataT lastData = m_data[m_size - 1];				
			
			// Move the current value to Remove to the end of the container.
			m_data[m_size - 1] = std::move(m_data[idxToRemove]);

			// Copy the previus value of the previus last element to the idxToRemove.
			m_data[idxToRemove] = lastData;
			
			--m_size;										
		}

		/** Resizes the vector to a certain newSize, cases:
		* 
		* Size greater than the old size:
		*	- new memory will be allocated for the new size and the vector moved to a new memory location.
		* 
		* Size is smaller than the new size .
		*	- last elements in the vector( delta size elements ) will be ignored in some cases, and in others, new memory block will be allocated.
		* 
		* Size greater than size but smaller to capacity.
		* 
		*	- size will be greater only, no memory allocation needed here.
		* 		
		*/
		void Resize(int32 newSize)
		{
			if(newSize < m_size)
			{

				// Remove the elements from the delta size positions.
				int deltaSizes = m_size - newSize;

				// In this case size need to be set before deep copying the data.
				m_size = newSize;

				// In some cases we should leave space free for more memory needed to be allocated.
				if(deltaSizes > MIN_SIZE_DELTA_TO_NEWALLOC_WHEN_RESIZED)
				{
					// New Alloc the Array
					m_capacity = newSize;
					AllocateCapacityAndMoveData();					
				}
				
								
			}else if(newSize > m_size)
			{
				if(newSize > m_capacity)
				{
					m_capacity = newSize;
					AllocateCapacityAndMoveData();
				}

				// Set the size after deep copy
				m_size = newSize;
			}			
		}

		/** Resevers the memory for the newCapacity. Size is not changes. */
		void Reserve(int32 newCapacity)
		{
			m_capacity = newCapacity;
			AllocateCapacityAndMoveData();
		}

		bool Contains(const DataT& toCompare) const
		{			
			for(uint32 idx = 0; idx < m_size; ++idx)
			{
				if(m_data[idx] == toCompare)
				{
					return true;
				}
			}

			return false;
		}
		
		/** Array size. */
		uint32 Size() const
		{
			return m_size;
		}

		uint32 Capacity() const
		{
			return m_capacity;
		}

		/** Returns true if we can access to the passed idx. */
		bool CheckSizeAt(std::uint32_t idx) const
		{			
			return idx < m_size;
		}

		/** Returns true if we can access to the passed idx. */
		bool CheckAssertSizeAt(std::uint32_t idx)
		{
			const bool bCond{ idx < m_size };
			assert(bCond == true);

			return bCond;
		}

	private:

		void TryAllocateCapacity()
		{
			if(m_data == nullptr)
			{
				AllocateCapacity();
			}
		}

		void InitializeArrayElements()
		{
			for(int i = 0; i < m_size; ++i)
			{
				new (m_data + i) DataT;
			}
		}

		void InitializeArrayElementsFromIdx(int idx)
		{
			for(int i = idx; i < m_size; ++i)
			{
				new (m_data + i) DataT;
			}
		}

		void InitializeArrayElementsInBuffer(DataT* buffer, size_t size)
		{
			for(int i = 0; i < size; ++i)
			{
				new (buffer + i) DataT;
			}
		}

		void AllocateCapacityAndMoveData()
		{
			DataT* newDataPtr{ static_cast<DataT*>(m_allocator.Allocate(sizeof(DataT) * m_capacity))};
			InitializeArrayElementsInBuffer(newDataPtr, m_size);

			// copy all the data from the current dinamic ptr to the new one.
			for (uint32 idx = 0; idx < m_size; ++idx)
			{
				newDataPtr[idx] = m_data[idx];
			}

			m_allocator.Free(m_data);

			m_data = newDataPtr;
		}

		/** Allocates the m_capacity in memory. Used for initializing the container. */
		void AllocateCapacity()
		{		
			// Check if we have the Capacity OK! before allocating.
			assert(m_capacity > 0);
			m_data = (DataT*)(m_allocator.Allocate(sizeof(DataT) * m_capacity));
		}

		/** 
		* Called when we move this container.
		* 
		* NOTE: This is only meant for moving the data, if we call this and the contained data is
		* not moved or freed, we are gonna have a memory leak.
		*/
		void Invalidate()
		{
			m_size = 0;
			m_capacity = DEFAULT_CAPACITY;
			m_data = nullptr;
		}
	
		public:
		uint32 m_size{ 0 };
		uint32 m_capacity{ 0 };
		DataT* m_data{ nullptr };
		AllocatorT m_allocator;
	};

}// ishak
