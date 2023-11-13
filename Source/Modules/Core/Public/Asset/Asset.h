#pragma once

#include "CoreConfig.h"

#include "CoreMinimal.h"

namespace ishak
{
	class CORE_API Asset
	{
	public:
		Asset(const String& idParam);

		String GetId() { return m_id; }

	private:
		/** This id is the absolute path for the asset. */
		String m_id;
	};
}//ishak