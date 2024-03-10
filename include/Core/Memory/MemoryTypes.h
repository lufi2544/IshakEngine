#pragma once

#include <memory>
#include "Core/CoreConfig.h"

namespace ishak
{
	template<typename T>
	using WeakPtr = std::weak_ptr<T>;

	template <typename T>
	using SharedPtr = std::shared_ptr<T>;

	template <typename T>
	using UniquePtr = std::unique_ptr<T>;
}


struct CORE_API FMemory
{
	static void* SystemMalloc(size_t size)
	{
		return ::malloc(size);
	}

	static void SystemFree(void* ptr)
	{
		::free(ptr);
	}
};