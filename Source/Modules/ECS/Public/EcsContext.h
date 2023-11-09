#pragma once
#include "System.h"
#include "ComponentManipulator.h"
#include "EntityManager.h"

#include "ECSConfig.h"


namespace ishak{ namespace Ecs{

	class ECS_API EcsContext
	{
	public:
		EcsContext() = default;
		EcsContext(UniquePtr<EntityManager>&& manager, UniquePtr<ComponentManipulator>&& compManipulator);

		ComponentManipulator* GetComponentManipulator();
		EntityManager* GetEntityManager();

		void RegisterSystem(SharedPtr<System>&& system);
		Ecs::EntityId RegisterEntity(void* obj);

		void UnregisterEntity(void* obj);
		void UpdateContext(float dt);

	private:				
		UniquePtr<EntityManager> m_entityManager;
		UniquePtr<ComponentManipulator> m_componentManipulator;
		TArray<SharedPtr<System>> m_systems;
	};

}} // ishak::Ecs