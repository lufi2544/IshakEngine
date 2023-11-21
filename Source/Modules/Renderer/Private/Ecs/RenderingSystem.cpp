
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
	RenderingSystem::RenderingSystem()
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
		
		TextureComponent& textureComponent{ m_compManipulator->GetComponent<TextureComponent>(entity) };
		const TransformComponent transform{ m_compManipulator->GetComponent<TransformComponent>(entity) };
		if (!textureComponent.texture.lock())
		{
			String texturePath{ *textureComponent.texturePath };		
			// Texture not found in component, then try to retreive from AssetManager.
			WeakPtr<Texture> wTexture{ AssetManager::Get().GetAsset<Texture>(texturePath) };
			if(!wTexture.lock())
			{
				// Error loading the asset in the manager.
				assert(false);
			}

			textureComponent.texture = wTexture;
		}

		Texture* texture{ textureComponent.texture.lock().get() };

		int scaledWidth = 0;
		int scaleHeight = 0;
		// Evaluate if we have to render from a coordinate in a texture, in this case the rendering target rectangle
		// won't be the texture widht itslf.
		if(textureComponent.renderingCoordinates.x == -1 && textureComponent.renderingCoordinates.y == -1)
		{
			scaledWidth = texture->GetWidth() * (int)transform.scale;
			scaleHeight = texture->GetHeight() * (int)transform.scale;
		}else
		{
			scaledWidth = textureComponent.customWidht;
			scaleHeight = textureComponent.customHeight;
		}
		Vector2 renderingCoordinates{ 0,0 };
		
		// Check if we have to render the texture from certain coordinates.
		if(textureComponent.renderingCoordinates.x != 0 || textureComponent.renderingCoordinates.y != 0)
		{
			renderingCoordinates = textureComponent.renderingCoordinates;
		}

		Renderer::Get().QueueRenderCommand(RendererCommand::FromTexture(
			entity,
			texture, 
			transform.position,
			transform.rotation,
			scaledWidth,
			scaleHeight,
			renderingCoordinates,
			textureComponent.ZOrder));
	}
}// ishak

