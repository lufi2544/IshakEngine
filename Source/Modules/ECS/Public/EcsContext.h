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
			if(contextCollection.Size() < contextId)
			{
				// TODO Exception
				assert(false);
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