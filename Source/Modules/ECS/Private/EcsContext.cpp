#include "EcsContext.h"

namespace ishak{ namespace Ecs{

	EcsContext::EcsContext(UniquePtr<EntityManager>&& manager, UniquePtr<ComponentManipulator>&& compManipulator)
		: m_entityManager(std::move(manager))
		, m_componentManipulator(std::move(compManipulator))
	{

	}
	void EcsContext::RegisterSystem(SharedPtr<System>&& system)
	{
		// TODO CheckForAlreadyRegistered
		system->SetComponentManipulator(m_componentManipulator.get());
		m_systems.Add(std::move(system));
	}

	Ecs::EntityId EcsContext::RegisterEntity(void* entityObject)
	{
		return m_entityManager->RegisterEntity(entityObject);
	}

	ComponentManipulator* EcsContext::GetComponentManipulator()
	{
		return m_componentManipulator.get();
	}

	EntityManager* EcsContext::GetEntityManager()
	{
		return m_entityManager.get();
	}

	void EcsContext::UnregisterEntity(void* obj)
	{
		m_entityManager->UnregisterEntity(obj);
	}

	void EcsContext::UpdateContext(float dt)
	{
		m_componentManipulator->UpdateComponentsStates();

		// Maybe this is not that efficient if we have different systems and every frame we are bringin 
		// different systems to the cache.
		for (auto& entity : m_entityManager->GetEntitiesCollection())
		{
			auto& entitiesSignatures{ m_componentManipulator->GetSignaturesMap() };
			auto signatureIt{ entitiesSignatures.find(entity) };
			if (signatureIt == std::end(entitiesSignatures))
			{
				// Entity with no components
				continue;
			}

			const auto entitySignature{ signatureIt->second };
			for (auto& system : m_systems)
			{
				const auto systemSignature{ system->GetSignature() };
				if ((systemSignature & entitySignature).any())
				{
					system->Update(dt, entity);
				}
			}
		}
	}

}
}// ishak::Ecs