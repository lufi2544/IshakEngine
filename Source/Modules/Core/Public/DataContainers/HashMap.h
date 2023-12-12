#pragma once

#include "DataContainers/Array.h"
#include "DataContainers/List.h"
#include "DataContainers/Pair.h"
#include "CoreMinimal.h"
#include "Hashing.h"

namespace ishak
{

	/** Implementation of a HashMap.
	 * Basically a hash map is a data container that stored the data in a key-value type, whithin a 
	 * collection of buckets that contain the key-value pairs, which are hashed when added, so we 
	 * can have different buckets in an array entry as they have the same hash.
	 * 
	 * We have to have in mind that when creating a hash map, we are allocating enough buckets that 
	 * when hashing a new key to a value, then we can have enough space to position it at first.
	 * */
	template<typename K, typename V>
	class CORE_API THashMap
	{
		typedef TPair<K, V> BucketElementT;
		typedef TList<BucketElementT> BucketT;
		typedef TArray<BucketT> BucketCollectionT;

	public:
		static constexpr size_t INITIAL_BUCKETS = 10;
		static constexpr float REHASHING_LOAD_FACTOR = 0.9f;
		static constexpr size_t REHASHING_COEFFICIENT = 2;


		THashMap()			
		{
			m_bucketsCollection.AddDefaulted(INITIAL_BUCKETS);	
		}

		size_t Size() const
		{	
			return m_size;
		}

		size_t GetBucketsNum() const
		{
			return m_bucketsCollection.Size();
		}

		bool Contains(K const& key)
		{	
			for(BucketT const& bucket : m_bucketsCollection)
			{
				for(BucketElementT const& bucketPair : bucket)
				{
					if(bucketPair.key == key)
					{
						return true;
					}
				}
			}

			return false;
		}

		BucketCollectionT  GetBuckets(){ return m_bucketsCollection; } 

		void Add(BucketElementT const& toAdd)
		{	
			ishak::Hash<K> hasher;
			size_t const hashVal{ hasher(toAdd.key) };
			size_t const moduledHash{ hashVal % m_bucketsCollection.Size() };
			m_bucketsCollection.CheckAssertSizeAt(moduledHash);

			BucketT& hashedBucket{ m_bucketsCollection[moduledHash] };	
			if(IsElementAlreadyInBucket(hashedBucket, toAdd.key))
			{
				ReplaceValueForKeyInBucket(hashedBucket, toAdd.key, toAdd.value);
				return;
			}

			hashedBucket.Add(toAdd);
			m_size++;

				
			float const loadFactor = (float)m_bucketsCollection.Size() / (float)m_size;
			if(loadFactor <=  REHASHING_LOAD_FACTOR)
			{
				Rehash(m_bucketsCollection.Size() * REHASHING_COEFFICIENT);	
			}
		}

	
		void Remove(K const& keyToRemove)
		{
			for(BucketT& bucket : m_bucketsCollection)
			{
				uint8 idx = 0;
				for(BucketElementT& bucketPair : bucket)
				{
					if(bucketPair.key == keyToRemove)
					{
						bucket.Remove(idx);
						return;
					}
					++idx;
				}
			}	
		}

		V& operator [](K const& key)
		{
			ishak::Hash<K> hasher;
			const size_t hashVal{ hasher(key) };
			const size_t moduledHash { hashVal % m_bucketsCollection.Size() };
			m_bucketsCollection.CheckAssertSizeAt(moduledHash);
				

			BucketT& bucket{ m_bucketsCollection[moduledHash] };

			for(TPair<K, V>& bucketPair : bucket)
			{
				if(bucketPair.key == key)
				{
					return bucketPair.value;
				}
			}

				// If not fount, then we add the value and then return it

				// TODO Function
				// TODO Add doubly linked list

			bucket.Add(MakePair<K, V>(key, V{}));

				// By having a doubly linked list we would avoid iterating all the ptrs by iterating from the tail
			for(TPair<K, V>& bucketPair : bucket)
			{
				if(bucketPair.key == key)
				{
					return bucketPair.value;
				}
			}


			assert(false);
		}

	private:
		bool IsElementAlreadyInBucket(BucketT const& bucket, K const& keyToCheck) const
		{
			for(BucketElementT const& bucketPair : bucket)
			{
				if(bucketPair.key == keyToCheck)
				{
					return true;
				}
			}

			return false;
		}

		void ReplaceValueForKeyInBucket(BucketT& bucket, K const& key, V const& newValue) 
		{

			for(BucketElementT& element : bucket)
			{
				if(element.key == key)
				{
					element.value = newValue;
					return;
				}
			}
		}

		void Rehash(size_t newBucketNum)
		{
			BucketCollectionT rehashedBucketCollection;	
			rehashedBucketCollection.AddDefaulted(newBucketNum);

			for(BucketT& bucket: m_bucketsCollection)
			{
				for(BucketElementT& element : bucket)
				{
				
					ishak::Hash<K> hasher;
					size_t const hashVal{ hasher(element.key) };
					size_t const moduledHash{ hashVal % rehashedBucketCollection.Size() };
					rehashedBucketCollection.CheckAssertSizeAt(moduledHash);

					rehashedBucketCollection[moduledHash].Add(element);
				}
			}


			m_bucketsCollection = rehashedBucketCollection;	
		}


	private:
		BucketCollectionT m_bucketsCollection;

		/* Number of current keys in the map. */
		size_t m_size{ 0 };
	};


}
