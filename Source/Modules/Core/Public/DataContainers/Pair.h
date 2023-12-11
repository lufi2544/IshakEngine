#pragma once

namespace ishak{

	/**
	 * Implementation of the Pair class. Will hold info about a key-value.
	 * */
	template<typename K, typename V>
	struct TPair
	{
		TPair() = default;
		TPair(K const& keyParam, V const& valueParam)
			: key{ keyParam }
			, value{ valueParam }
		{

		}


		K key;
		V value;
	};

	
	template<typename K, typename V>
	TPair<K, V> MakePair(K const& key, V const& value)
	{
		return TPair<K, V>(key, value);
	}

}// ishak
