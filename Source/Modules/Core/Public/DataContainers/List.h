#pragma once
#include "CoreMinimal.h"
#include "CoreConfig.h"

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
	class CORE_API TList
	{
		struct Node
		{
			Node(T&& dataParam)
				: data(std::move(dataParam))
			{

			}
			
			Node(T const& dataParam)
				: data(dataParam)
			{

			}

			T data;
			Node* next{ nullptr };	
		};

	public:
		struct CORE_API Iterator
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

		TList (TList const& other)
		{
			// if copied, we have to reallocate all the memory again
			if(other.m_head)
			{
				m_head = new Node(other.m_head->data);

			}else
			{
				return;
			}

			m_size = other.m_size;

			Node* otherCurrentNode = { other.m_head->next };
			Node* itNode = { m_head };

			while(otherCurrentNode != nullptr)
			{
				Node* addedNode = new Node(otherCurrentNode->data);
				itNode->next = addedNode;
				itNode = addedNode; 
				
				 otherCurrentNode = otherCurrentNode->next;	
			}
		
		}


		~TList()
		{
			Clear();	
		}


		TList& operator = (TList&& other)
		{
			m_head = other.m_head;
			m_size = other.m_size;

			other.m_head = nullptr;
			other.m_size = 0;

		}

		TList& operator = (TList const& other)
		{

			// if copied, we have to reallocate all the memory again
			if(other.m_head)
			{
				m_head = new Node(other.m_head->data);

			}else
			{
				return *this;
			}

			m_size = other.m_size;

			Node* otherCurrentNode = { other.m_head->next };
			Node* itNode = { m_head };

			while(otherCurrentNode != nullptr)
			{
				Node* addedNode = new Node(otherCurrentNode->data);
				itNode->next = addedNode;
				itNode = addedNode; 
				
				 otherCurrentNode = otherCurrentNode->next;	
			}

			return *this;		
		}


		friend bool operator == (TList const& lhs, TList const& rhs)
		{	

			if(rhs.m_size != lhs.m_size)
			{
				return false;
			}

			Node* lhsCurrent = lhs.m_head;
			Node* rhsCurrent = rhs.m_head;

			while(lhsCurrent != nullptr)
			{

				if(lhsCurrent->data != rhsCurrent->data)
				{
					return false;
				}
					
				rhsCurrent = rhsCurrent->next;	
				lhsCurrent = lhsCurrent->next;
			}

			return true;				
		}

		size_t Size() const
		{
			return m_size;
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
					m_head = currentNode->next;
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

		size_t Find(T const& toFind)
		{
			size_t idx = 0;
			for(T& it : *this)
			{
				if(it  == toFind)
				{
					return idx;
				}

				++idx;
			}

			return -1;
		}

		void Clear()
		{
			if(m_size == 0)
			{
				return;
			}

			Node* currentNode = m_head;
			while(currentNode != nullptr)
			{
				
				Node* nodeToDelete = currentNode;
				currentNode = currentNode->next;				

				delete nodeToDelete;
				
			}

			m_head = nullptr;
			m_size = 0;
		}

	private:			
		size_t m_size{ 0 };
		Node* m_head{ nullptr };
	};

}// ishak
