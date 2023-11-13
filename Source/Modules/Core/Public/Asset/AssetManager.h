#pragma once

#include "CoreMinimal.h"
#include "Asset.h"

#include "CoreConfig.h"

namespace ishak
{
	class CORE_API AssetManager	
	{
	private:
		AssetManager() = default;

	public:
		static AssetManager& Get()
		{
			static AssetManager lazySingleton;
			return lazySingleton;
		}

		void AddAsset(SharedPtr<Asset>&& asset);

		template<typename T>
		WeakPtr<T> GetAsset(const String& id)
		{
			auto fountIt{ m_assets.find(id) };
			if(fountIt == std::end(m_assets))
			{
				return { };
			}

			return std::static_pointer_cast<T>(fountIt->second);
		}

	private:
		std::unordered_map<String, SharedPtr<Asset>> m_assets;		
	};
}//ishak



