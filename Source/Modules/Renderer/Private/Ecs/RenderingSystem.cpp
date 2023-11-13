
//ECS
#include "Ecs/RenderingSystem.h"
#include "Ecs/RenderingComponent.h"
#include "Ecs/Components/TransformComponent.h"
#include "Ecs/Components/TextureComponent.h"

//CORE
#include "Log/Logger.h"
#include "Asset/AssetManager.h"
 
//RENDERER
#include "Renderer.h"
#include "Asset/Texture.h"

namespace ishak
{
	RenderingSystem::RenderingSystem(Renderer* renderer)
		: m_renderer(renderer)
	{

	}
	void ishak::RenderingSystem::SetComponentRequirements()
	{
		RequireComponent<TextureComponent>();
		RequireComponent<TransformComponent>();
	}

	void RenderingSystem::Update(float dt, Ecs::EntityId entity)
	{

		if (!m_compManipulator->HasComponent<TextureComponent>(entity) || !m_compManipulator->HasComponent<TransformComponent>(entity))
		{
			return;
		}

		TextureComponent& compo{ m_compManipulator->GetComponent<TextureComponent>(entity) };
		String texturePath{ *compo.textureId };
		const TransformComponent transform{ m_compManipulator->GetComponent<TransformComponent>(entity) };

		if (!compo.texture.lock())
		{
			// Texture not found in component, then try to retreive from AssetManager.
			WeakPtr<Texture> wTexture{ AssetManager::Get().GetAsset<Texture>(texturePath) };
			if (!wTexture.lock())
			{
				// Texture non existant in the AssetManager, then load it into memory				
				SDL_Surface* surface = IMG_Load(texturePath.c_str());
				if (surface)
				{
					SDL_Texture* sdltexture = SDL_CreateTextureFromSurface(m_renderer->GetSDLRenderer(), surface);
					SDL_FreeSurface(surface);

					SharedPtr<Texture> texture{ std::make_shared<Texture>(texturePath, sdltexture) };
					//TODO Refactor, make an specialization of the fuction in the Asset Manger, so the asset manater created the memory for this.
					AssetManager::Get().AddAsset(std::move(texture));
				}
			}

			compo.texture = AssetManager::Get().GetAsset<Texture>(texturePath);
		}

		Texture* texture{ compo.texture.lock().get() };
		m_renderer->QueueRenderCommand(RendererCommand::FromTexture(entity, texture, transform.position));
	}
}// ishak

