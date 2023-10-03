#pragma once

#include "CoreMinimal.h"
#include "CoreConfig.h"
#include <vector>

namespace ishak {

	/** 
	  * Dynamically allocated container for storing Data.
	  * NOTE: Custom Implementation of std::vector.
	  * // TODO MEMORY Add the allocator 
	  */

	// TODO ARRAY for now the array capacity grows with a coefficient of x2 every time an 
	// addition fails due to capacity problems.
	template<typename DataT> 
	class CORE_API TArray 
	{	

		static std::size_t constexpr DEFAULT_CAPACITY = 2;
	public:

		TArray() 
		{
			m_capacity = DEFAULT_CAPACITY;
			AllocateCapacity();
		}

		~TArray() 
		{
			delete[] m_data;
		}

		TArray(const TArray<DataT>& other)
		{
			m_capacity = other.m_capacity;
			m_size = other.m_size;
			AllocateCapacity();			
			for(int32 idx = 0; idx < m_size; ++idx)
			{
				m_data[idx] = other.m_data[idx];
			}			
		}

		TArray(TArray<DataT>&& other) noexcept
		{
			m_capacity = other.m_capacity;
			m_size = other.m_size;
			AllocateCapacity();

			for (int32 idx = 0; idx < m_size; ++idx)
			{
				m_data[idx] = std::move(other.m_data[idx]);
			}

			// Invalidates the other container once we have moved all the data.
			other.Invalidate();
		}

		//---- Iterator Functions ----
		DataT* begin()
		{
			return m_data;
		}

		const DataT* begin() const
		{
			return m_data;
		}

		DataT* end()
		{
			return std::addressof(m_data[m_size]);
		}

		const DataT* end() const
		{
			return std::addressof(m_data[m_size]);
		}
		//----  ----


#pragma region --Operators--
		
		DataT& operator[](int32 idx)
		{
			// TODO ASSERT Custom Assert
			CheckCapacity(idx);

			return m_data[idx];
		}

		const DataT& operator[](int32 idx) const
		{
			CheckCapacity(idx);

			return m_data[idx];
		}

#pragma endregion --Operators--


		/** Adds a new value to the Array(r-value, move operator). */
		void Add(DataT&& dataToAdd)
		{			
			// Check if we have to reallocate memory
			const bool bNeedToReallocateMemory{ m_size == m_capacity };
			if (bNeedToReallocateMemory) 
			{
				// Change the dinamic ptr to the last Data in the Array.
				m_capacity *= 2;

				DataT* newDataPtr{ new DataT[m_capacity] };

				// copy all the data from the current dinamic ptr to the new one.
				for (int idx = 0; idx < m_size; ++idx ) 
				{
					newDataPtr[idx] = m_data[idx];
				}

				delete[] m_data;

				m_data = newDataPtr;
			}

			m_data[m_size++] = std::move(dataToAdd);
		}

		/** Adds a new value to the Array. */
		void Add(const DataT& dataToAdd)
		{
			// Check if we have to reallocate memory
			const bool bNeedToReallocateMemory{ m_size == m_capacity };
			if (bNeedToReallocateMemory)
			{
				// Change the dinamic ptr to the last Data in the Array.
				m_capacity *= 2;

				DataT* newDataPtr{ new DataT[m_capacity] };

				// copy all the data from the current dinamic ptr to the new one.
				for (int idx = 0; idx < m_size; ++idx)
				{
					newDataPtr[idx] = m_data[idx];
				}

				delete[] m_data;

				m_data = newDataPtr;
			}

			m_data[m_size++] = dataToAdd;
		}

		/** Removes by index a certain value in the container. */
		void Remove(int32 idxToRemove)
		{		

			CheckCapacity(idxToRemove);

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
		
		/** Array size. */
		std::size_t Size() 
		{
			return m_size;
		}

		/** Returns true if we can access to the passed idx. */
		bool CheckCapacity(std::uint32_t idx)
		{			
			return idx < m_size;
		}

	private:
		/** Allocates the m_capacity in memory. Used for initializing the container. */
		void AllocateCapacity()
		{		
			// Check if we have the Capacity OK! before allocating.
			assert(m_capacity > 0);
			m_data = new DataT[m_capacity];
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
			m_capacity = 0;
			m_data = nullptr;
		}
	
	private:
		uint32 m_size{ 0 };
		uint32 m_capacity{ 0 };
		DataT* m_data;
	};



	class Dog
	{
	public:
		Dog() = default;
		~Dog()
		{
			int a = 0;
		}

		int32 id;
	};

	void test ()
	{

	}

}// ishak