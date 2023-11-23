#pragma once

#include "CoreConfig.h"

namespace ishak {

	/** Data container that holds a key-value type data. */
	template<typename K, typename V>
	class CORE_API TPair
	{
	public:
		TPair() = default;
	

	public:
		K key;
		V value;
	};

}//ishak