#pragma once 

#include <functional>
#include "Core/String/FString.h"
#include <string>

namespace ishak
{

	template<typename T>
	struct Hash
	{
		Hash() = default;
		size_t operator() (T const& val) const
 		{
			return 0;
		}
	};

	// TODO Hash all the data types. 

	template<>
	struct Hash<int>
	{
		size_t operator() (int const& val) const
		{
			return std::hash<int>()(val);
		}
	};



	template<>
	struct Hash<String>
	{
		size_t operator() (String const& val) const
		{
			return std::hash<std::string>()(std::string(val.c_str()));
		}
	};


	template<>
	struct Hash<float>
	{
		size_t operator() (float const& val) const
		{
			return std::hash<float>()(val);
		}
	};
	
	template<>
	struct Hash<bool>
	{
		size_t operator() (bool const& val) const
		{
			return std::hash<bool>()(val);
		}
	};

}//ishak

