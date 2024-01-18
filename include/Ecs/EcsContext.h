#pragma once
#include "System.h"
#include "ComponentManipulator.h"
#include "EntityManager.h"

#include "ECSConfig.h"


namespace ishak{ namespace Ecs{


	enum ContextID : uint8
	{
		ENGINE,
		RENDERER,
		CUSTOM, // Game
		/*....*/
		NUM
	};

	/**
	* Global class that will hold all the info related to the ECS( systems, entities, containers... )
	* We will have containers that are unique for every ecscontext( only certain component containers for the Engine, other for the renderer,
	* and others for the Custom( Game )). So in this case, we share the containers across the different contexts.
	*/
	class ECS_API EcsContext
	{
	public:
		EcsContext() = default;
		EcsContext(SharedPtr<EntityManager> manager, UniquePtr<ComponentManipulator>&& compManipulator);

		ComponentManipulator* GetComponentManipulator();
		EntityManager* GetEntityManager();

		void RegisterSystem(SharedPtr<System>&& system);
		Ecs::EntityId RegisterEntity(void* obj);

		void UnregisterEntity(void* obj);
		void UpdateContext(float dt);

	private:				
		SharedPtr<EntityManager> m_entityManager;
		UniquePtr<ComponentManipulator> m_componentManipulator;
		TArray<SharedPtr<System>> m_systems;
	};

	/** Container in charge of storing the different Ecs Container. It frees it when destroyed. */
	struct ECS_API EcsContextContainer
	{		
		EcsContextContainer(SharedPtr<EntityManager> entityManagerParam)
			: entityManger{ entityManagerParam }
		{
			contextCollection.Reserve(ContextID::NUM);			
		}

		~EcsContextContainer()
		{
			for(EcsContext* context : contextCollection)
			{
				delete context;

				context = nullptr;
			}
		}

		EcsContext* GetEcsContext(ContextID contextId)
		{
			if(!contextCollection.CheckSizeAt(contextId))
			{
				return nullptr;
			}

			return contextCollection[contextId];
		}
		
		 void AddContext(EcsContext* context)
		 {
			 contextCollection.Add(context);
		 }

		SharedPtr<EntityManager> entityManger;
		TArray<EcsContext*> contextCollection;
	};

}} // ishak::Ecs