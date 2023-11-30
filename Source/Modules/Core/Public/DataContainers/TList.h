#pragma once
#include "CoreMinimal.h"

namespace ishak{

	/** Custom implementation of a linked list.
	 *
	 *  This data continer will own a list of Data Nodes that are created upon addition.
	 *  The advantage of the Linked list againsts the TArray for example is that we do not have 
	 *  to reserve the memory for adding more elements at runtime, we basically allocate memory
	 *  for the new Nodes as we go. This is good in some scenarios where we do not have to iterate
	 *  through a list of elements constantly and just want to have some data stored, or maybe 
	 *  we do not want to access by idx, in those cases where performance is not a constraint or 
	 *  we do not know the size of the final contained data set, linked list could be a nice choice.
	 *
	 *
	 * */

	template<typename T>
	class TList
	{
		struct Node
		{
			Node(T const& dataParam)
				: data{ dataParam }
			{

			}

			Node(T&& mData)
				: data{ mData }
			{

			}

			T data;
			Node* next{ nullptr };	
		};

	public:
		struct Iterator
		{
			Iterator(Node* node)
				: current{ node }
			{

			}

			Iterator& operator ++()
			{
				current = current->next;

				return *this;
			}

			T& operator*() const
			{
				return current->data;
			}	

			bool operator != (const Iterator& other) const
			{
				return this->current != other.current;
			}

			Node* current { nullptr };
		};

		TList() = default;
		~TList()
		{
			Node* currentNode = m_head;
			while(currentNode != nullptr)
			{	
				Node* nodeToDelete = currentNode;
				currentNode = currentNode->next;

				delete nodeToDelete;
			}
		}

		bool IsValid() const
		{
			return m_head != nullptr;
		}

		Iterator begin() const
		{
			return Iterator(m_head);
		}

		Iterator end() const
		{
			return Iterator(nullptr);
		}

		void Add(const T& dataToAdd)
		{
			// N(d, n) --- N(d, n) --- N(d,n)
			
			if(m_head == nullptr)
			{
				m_head = new Node(dataToAdd);
				++m_size;
				return;
			}
		
			// iterate through all the nodes recursively
			Node* lastNode = m_head;
			while(lastNode->next != nullptr)
			{
				// update the last node and the iterator
				lastNode = lastNode->next;
			}

			lastNode->next = new Node(dataToAdd);
			++m_size;
		}

		void Add(T&& dataToAdd)
		{
			// N(d, n) --- N(d, n) --- N(d,n)
		
			if(m_head == nullptr)
			{
				// TODO ISHMEMORY
				m_head = new Node(std::move(dataToAdd));
				++m_size;
				return;
			}


			// iterate through all the nodes recursively
			Node* lastNode = m_head;
			while(lastNode->next != nullptr)
			{
				// update the last node and the iterator
				lastNode = lastNode->next;
			}

			// TODO ISHMEMORY
			lastNode->next = new Node(std::move(dataToAdd));

			++m_size;
		}


		void Remove(size_t indexToRemove)
		{
			Node* currentNode = m_head;
			Node* nodeToRemove = nullptr;
			size_t idx = 0;
			while(currentNode != nullptr)
			{
				if(indexToRemove == 0 && idx == 0)
				{
					nodeToRemove = currentNode;
					break;
				}

				// Find the node we want to Remove
				if(indexToRemove == idx + 1)
				{		
					nodeToRemove = currentNode->next;			
					currentNode->next = nodeToRemove->next;		
					break;
				}

				currentNode = currentNode->next;
				++idx;
			}

			if(nodeToRemove != nullptr)
			{
				delete nodeToRemove;
				--m_size;
			}

		}

	private:			
		size_t m_size{ 0 };
		Node* m_head{ nullptr };
	};

}// ishak
