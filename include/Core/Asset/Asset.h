#pragma once

#include "Core/CoreConfig.h"

#include "Core/CoreMinimal.h"

namespace ishak
{
	class CORE_API Asset : public std::enable_shared_from_this<Asset>
	{
	public:
		Asset(const String& idParam);
		virtual ~Asset() = default;

		virtual SharedPtr<Asset> Load() = 0;

		String GetId() { return m_path; }

	protected:
		/** This id is the absolute path for the asset. */
		String m_path;
	};
}//ishak