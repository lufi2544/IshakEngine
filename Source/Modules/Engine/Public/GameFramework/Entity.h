#pragma once

#include "CoreMinimal.h"
#include "CoreConfig.h"
#include "Functionality/Factory.h"


#include "EntityId.h"


namespace ishak 
{
	class World;

	/*** Main class for the Ishak Engine, for now, nothing fancy, for now... */
	class CORE_API Entity 
	{
	public:
		Entity() = default;
		// TODO Create the Actor Class
		Entity(World* world);	
		Entity(World* world, Vector2 position);
		~Entity();

		void BeginPlay();
		void Tick(float deltaTime);
		void Render();

		World* GetWorld();

		String GetTexture() const;
		Vector2 GetPosition();
		inline Ecs::EntityId GetEntityId() { return entityId; }
		void SetPosition(const Vector2& newPosition );

	protected:
		virtual void DoOnBeginPlay(){ }
		virtual void DoTick(float deltaTime){ }
		virtual void DoRender(){ }

	protected:
		struct RenderContext
		{
			String texturePath;
		} m_renderContext;

		Vector2 m_position{ 0, 0 };
	private:
		World* m_World{ nullptr };		

		Ecs::EntityId entityId{ Ecs::kNullId };

		bool bVisible{ true };
	};	
} // ishak