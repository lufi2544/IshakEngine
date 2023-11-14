#include "Asset/AssetManager.h"


namespace ishak
{
	AssetManager& AssetManager::Get()
	{		
		static AssetManager lazySingleton;
		return lazySingleton;		
	}
	void AssetManager::AddAsset(SharedPtr<Asset>&& asset)
	{
		auto fountIt{ m_assets.find(asset->GetId()) };
		if (fountIt != std::end(m_assets))
		{
			// found asset, already added
			return;
		}


		m_assets.insert(std::make_pair(asset->GetId(), std::move(asset)));
	}

	void AssetManager::AddAsset(const SharedPtr<Asset>& asset)
	{
		auto fountIt{ m_assets.find(asset->GetId()) };
		if (fountIt != std::end(m_assets))
		{
			// found asset, already added
			return;
		}


		m_assets.insert(std::make_pair(asset->GetId(), std::move(asset)));
	}

}//ishak