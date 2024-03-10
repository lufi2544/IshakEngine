#pragma once
#include "Core/CoreMinimal.h"
#include "Core/CoreConfig.h"

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

	template<typename DataT, typename AllocatorT = memory::DefaultAllocator>
	class CORE_API TList
	{
		struct CORE_API Node
		{

			Node() = default;

			Node(DataT&& dataParam)
				: data(std::move(dataParam))
			{

			}
			
			Node(DataT const& dataParam)
				: data(dataParam)
			{

			}

			DataT data;
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

			DataT& operator*() const
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
				m_head = CreateNode(other.m_head->data);
			}
			else
			{
				return;
			}

			m_size = other.m_size;

			Node* otherCurrentNode = { other.m_head->next };
			Node* itNode = { m_head };

			while(otherCurrentNode != nullptr)
			{
				Node* addedNode = CreateNode(otherCurrentNode->data);
				itNode->next = addedNode;
				itNode = addedNode; 
				
				 otherCurrentNode = otherCurrentNode->next;	
			}
		
		}

		TList(TList&& other)
		{
			m_head = other.m_head;
			m_size = other.m_size;

			other.Invalidate();
		}

		~TList()
		{
			Clear();	
		}

		TList& operator = (TList&& other)
		{
			m_head = other.m_head;
			m_size = other.m_size;

			other.Invalidate();
		}

		TList& operator = (TList const& other)
		{

			// if copied, we have to reallocate all the memory again
			if(other.m_head)
			{
				m_head = CreateNode(other.m_head->data);

			}else
			{
				return *this;
			}

			m_size = other.m_size;

			Node* otherCurrentNode = { other.m_head->next };
			Node* itNode = { m_head };

			while(otherCurrentNode != nullptr)
			{
				Node* addedNode = CreateNode(otherCurrentNode->data);
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

		bool CheckSizeAt(size_t idx)
		{
			return m_size > idx;
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

		void Add(const DataT& dataToAdd)
		{
			// N(d, n) --- N(d, n) --- N(d,n)
			
			if(m_head == nullptr)
			{
				m_head = CreateNode(dataToAdd);
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

			lastNode->next = CreateNode(dataToAdd);
			++m_size;
		}

		void Add(DataT&& dataToAdd)
		{
			// N(d, n) --- N(d, n) --- N(d,n)
		
			if(m_head == nullptr)
			{
				// TODO ISHMEMORY
				m_head = CreateNode(std::move(dataToAdd));
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
			lastNode->next = CreateNode(std::move(dataToAdd));

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

		size_t Find(DataT const& toFind)
		{
			size_t idx = 0;
			for(DataT& it : *this)
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
		void Invalidate()
		{
			m_size = 0;
			m_head = nullptr;
		}

		Node* CreateNode(const DataT& data)
		{
			Node* node{ (Node*)allocator.Allocate(sizeof(Node)) };
			new (node) Node;
			node->data = data;

			return node;
		}

		Node* CreateNode(DataT&& data)
		{
			Node* node{ (Node*)allocator.Allocate(sizeof(Node)) };
			new (node) Node;
			node->data = std::move(data);			

			return node;
		}

	private:			
		size_t m_size{ 0 };
		Node* m_head{ nullptr };
		AllocatorT allocator;
	};

}// ishak
