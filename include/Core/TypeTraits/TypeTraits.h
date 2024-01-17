#pragma once
#include <cstdint>
#include <cwchar>

namespace ishak{

	// For now as we only have 1 platform, we do not have to
	// do fancy stuff on the typedefs for this.

	using int8 = std::int8_t;
	using int16 = std::int16_t;
	using int32 = std::int32_t;
	using int64 = std::int64_t;
	using uint8 = std::uint8_t;
	using uint16 = std::uint16_t;	
	using uint32 = std::uint32_t;
	using uint64 = std::uint64_t;
	using size_t = std::size_t;

}// ishak