#pragma once

namespace ishak{

	/**
	 * Implementation of the Pair class. Will hold info about a key-value.
	 * */
	template<class K, class V>
	struct TPair
	{
		TPair() = default;


		K key;
		V value;
	};


}// ishak
