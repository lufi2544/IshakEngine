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
		Entity(World* world);	
		~Entity();

		void BeginPlay();
		void Tick(float deltaTime);
		void Render();

		String GetTexture() const;
		Vector2 GetPosition() const;
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