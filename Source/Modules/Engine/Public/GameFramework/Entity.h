#pragma once

#include "CoreMinimal.h"
#include "CoreConfig.h"
#include "Functionality/Factory.h"

namespace ishak {
	class World;
}


namespace ishak 
{
	/*** Main class for the Ishak Engine, for now, nothing fancy, for now... */
	class CORE_API Entity 
	{
	public:
		Entity() = default;
		Entity(World* world);	

		void BeginPlay();
		void Tick(float deltaTime);
		void Render();

		std::string GetTexture() const;
		Vector2 GetPosition() const;
		void SetPosition(const Vector2& newPosition );

	protected:
		virtual void DoOnBeginPlay(){ }
		virtual void DoTick(float deltaTime){ }
		virtual void DoRender(){ }

	protected:
		struct RenderContext
		{
			std::string texture;
		} m_renderContext;

		Vector2 m_position{ 0, 0 };
	private:
		World* m_World{ nullptr };		
	};	
} // ishak