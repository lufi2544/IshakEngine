#include "doctest.h"

#include "ComponentContainer.h"
#include "ComponentManipulator.h"
#include "Component.h"
#include "EcsContext.h"

namespace ishak {
	namespace Ecs {

		struct HealthComponent
		{
			HealthComponent() = default;

			int current;
			int max;

			friend bool operator == (HealthComponent const& lhs, HealthComponent const& rhs)
			{
				return lhs.current == rhs.current && lhs.max == rhs.max;
			}
		};


		namespace ComponentContainerTests {

			TEST_CASE("Registering a ContainerType, OK")
			{
				UniquePtr<EntityManager> manager = std::make_unique<EntityManager>();
				UniquePtr<ComponentManipulator> manip = std::make_unique<ComponentManipulator>();
				EcsContext context{ std::move(manager), std::move(manip) };				

				EntityId entity{ 22 };						
				SharedPtr<IComponentContainer> container = std::make_shared<Ecs::ComponentContainer<HealthComponent>>();

				// Register the component container
				ComponentManipulator compManipulator;
				compManipulator.RegisterComponentContainer(std::move(container));

				// Adding a simple component
				HealthComponent healthComp{ 1, 2, };
				compManipulator.AddComponentDeferred(entity, healthComp);

				compManipulator.UpdateComponentsStates();

				// Checking if the component has been stored
				const bool bHasComp = compManipulator.HasComponent<HealthComponent>(entity);
				const bool bIsSameComp = compManipulator.GetComponent<HealthComponent>(entity) == healthComp;
				
				CHECK((bHasComp && bIsSameComp) == true);
			}

			TEST_CASE("Adding a component and then removing it, OK")
			{
				EntityId entity{ 22 };
				SharedPtr<IComponentContainer> container = std::make_shared<Ecs::ComponentContainer<HealthComponent>>();

				// Register the component container
				ComponentManipulator compManipulator;
				compManipulator.RegisterComponentContainer(std::move(container));

				// Adding a simple component
				HealthComponent healthComp{ 1, 2, };
				compManipulator.AddComponentDeferred(entity, healthComp);

				compManipulator.RemoveComponentDeferred<HealthComponent>(entity);

				compManipulator.UpdateComponentsStates();

				// Checking if the component has been stored
				const bool bHasComp = compManipulator.HasComponent<HealthComponent>(entity);		
				const uint8 componentSignature{ compManipulator.GetComponentContainerSignatureId<HealthComponent>() };

				bool bFoundEntity{ false };
				const bool bHasSignature{ (compManipulator.GetEntitySignature(entity, bFoundEntity).test(componentSignature)) };


				CHECK(bHasComp == false);
				CHECK(bFoundEntity == true);
				CHECK(bHasSignature == false);
			}

			TEST_CASE("Adding a component and then removing it, Adding another one, it is in the free space, OK")
			{				
				SharedPtr<IComponentContainer> container = std::make_shared<Ecs::ComponentContainer<HealthComponent>>(22);

				// Register the component container
				ComponentManipulator compManipulator;
				compManipulator.RegisterComponentContainer(std::move(container));

				HealthComponent healthComp{ 1, 2, };
				for(int ientity = 0; ientity < 22; ++ientity)
				{
					// Adding a simple component
					compManipulator.AddComponentDeferred(ientity, healthComp);					
				}

				compManipulator.UpdateComponentsStates();

				EntityId entityToDelete1{ 4 };
				EntityId entityToDelete2{ 13 };

				// Indexes for the entities components in the container.
				bool bFound1;
				const uint16 entity1ComponentIdx{ compManipulator.Debug_GetComponentIdxForEntity<HealthComponent>(entityToDelete1, bFound1) };

				bool bFound2;
				const uint16 entity2ComponentIdx{ compManipulator.Debug_GetComponentIdxForEntity<HealthComponent>(entityToDelete2, bFound2) };

				compManipulator.RemoveComponentDeferred<HealthComponent>(entityToDelete1);
				compManipulator.RemoveComponentDeferred<HealthComponent>(entityToDelete2);

				compManipulator.UpdateComponentsStates();

				const TArray<uint16> freeSpaces{ compManipulator.Debug_GetFreeSpacesIndexes<HealthComponent>() };

				CHECK(freeSpaces.Size() == 2);
				CHECK(bFound1 == true);
				CHECK(bFound2 == true);
				CHECK(freeSpaces.Contains(entity1ComponentIdx) == true);
				CHECK(freeSpaces.Contains(entity2ComponentIdx) == true);
			}


			TEST_CASE("Adding a component Container needs to resize, try to get the last entity added component, OK")
			{
				SharedPtr<IComponentContainer> container = std::make_shared<Ecs::ComponentContainer<HealthComponent>>();

				// Register the component container
				ComponentManipulator compManipulator;
				compManipulator.RegisterComponentContainer(std::move(container));

				HealthComponent healthComp{ 1, 2, };
				int toAdd{ 8 };
				for (int ientity = 0; ientity < toAdd; ++ientity)
				{
					// Adding a simple component
					compManipulator.AddComponentDeferred(ientity, healthComp);
				}

				compManipulator.UpdateComponentsStates();

				EntityId lastEntity{ 3 };

				HealthComponent lastComponent{ 99, 99 };
				compManipulator.AddComponentDeferred(lastEntity, lastComponent);
				compManipulator.UpdateComponentsStates();

				HealthComponent component{ compManipulator.GetComponent<HealthComponent>(lastEntity) };

				CHECK(component.current == lastComponent.current);
				CHECK(component.max == lastComponent.max);
			}

			TEST_CASE("Adding some components, removing them, freed space. Adding it Again, spaces taken, OK")
			{
				SharedPtr<IComponentContainer> container = std::make_shared<Ecs::ComponentContainer<HealthComponent>>(22);

				// Register the component container
				ComponentManipulator compManipulator;
				compManipulator.RegisterComponentContainer(std::move(container));

				HealthComponent healthComp{ 1, 2, };
				for (int ientity = 0; ientity < 22; ++ientity)
				{
					// Adding a simple component
					compManipulator.AddComponentDeferred(ientity, healthComp);
				}

				compManipulator.UpdateComponentsStates();

				EntityId entityToTest1{ 4 };
				EntityId entityToTest2{ 13 };
				EntityId entityToTest3{ 2 };
						

				// As the spaces are gonig to be taken from the last deleted to the first deleted, the entity3 space is the first taken when adding a
				// new component to the container, so in this case the space that is going to be still free when we add 2 components later, is the 
				// space for entity 1.
				bool bFound;
				const uint16 entity1ComponentIdx{ compManipulator.Debug_GetComponentIdxForEntity<HealthComponent>(entityToTest1, bFound) };


				compManipulator.RemoveComponentDeferred<HealthComponent>(entityToTest1);
				compManipulator.RemoveComponentDeferred<HealthComponent>(entityToTest2);
				compManipulator.RemoveComponentDeferred<HealthComponent>(entityToTest3);

				compManipulator.UpdateComponentsStates();


				compManipulator.AddComponentDeferred(entityToTest1, healthComp);
				compManipulator.AddComponentDeferred(entityToTest2, healthComp);

				compManipulator.UpdateComponentsStates();

				const TArray<uint16> freeSpaces{ compManipulator.Debug_GetFreeSpacesIndexes<HealthComponent>() };

				CHECK(freeSpaces.Size() == 1);				
				CHECK(freeSpaces.Contains(entity1ComponentIdx) == true);
			}


			TEST_CASE("Adding some components, removing them.Flush Collection Allocation. OK")
			{
				SharedPtr<IComponentContainer> container = std::make_shared<Ecs::ComponentContainer<HealthComponent>>(2);

				// Register the component container
				ComponentManipulator compManipulator;
				compManipulator.RegisterComponentContainer(std::move(container));

				const uint16 compToAdd{ 200 };
				const uint16 compToRemove{ 100 };

				const HealthComponent healthComp{ 1, 2, };
				for (int ientity = 0; ientity < compToAdd; ++ientity)
				{
					// Adding a simple component
					compManipulator.AddComponentDeferred(ientity, healthComp);
				}

				compManipulator.UpdateComponentsStates();

				for (int ientity = 0; ientity < compToRemove; ++ientity)
				{
					// Removing some components
					compManipulator.RemoveComponentDeferred<HealthComponent>(ientity);
				}

				compManipulator.UpdateComponentsStates();
																	
				compManipulator.FlushComponenContainerAllocation<HealthComponent>();

				const TArray<uint16> freeSpacesAfterFlush{ compManipulator.Debug_GetFreeSpacesIndexes<HealthComponent>() };
				CHECK(freeSpacesAfterFlush.Size() == 0);				
				CHECK(compManipulator.Debug_GetComponentsCollection<HealthComponent>()->Capacity() == compToAdd - compToRemove);
			}

			TEST_CASE("Components Collection Flush some times. OK")
			{
				SharedPtr<IComponentContainer> container = std::make_shared<Ecs::ComponentContainer<HealthComponent>>(2);

				// Register the component container
				ComponentManipulator compManipulator;
				compManipulator.RegisterComponentContainer(std::move(container));

				const uint16 compToAdd{ 200 };
				const uint16 compToRemove{ 100 };

				const HealthComponent healthComp{ 1, 2, };
				for (int ientity = 0; ientity < compToAdd; ++ientity)
				{
					// Adding a simple component
					compManipulator.AddComponentDeferred(ientity, healthComp);
				}

				compManipulator.UpdateComponentsStates();

				for (int ientity = 0; ientity < compToRemove; ++ientity)
				{
					// Removing some components
					compManipulator.RemoveComponentDeferred<HealthComponent>(ientity);
				}

				compManipulator.UpdateComponentsStates();

				compManipulator.FlushComponenContainerAllocation<HealthComponent>();
				

				for (int ientity = 0; ientity < compToAdd; ++ientity)
				{
					// Adding a simple component
					compManipulator.AddComponentDeferred(ientity, healthComp);
				}

				compManipulator.UpdateComponentsStates();

				compManipulator.FlushComponenContainerAllocation<HealthComponent>();
				
				const TArray<uint16> freeSpacesAfterFlush{ compManipulator.Debug_GetFreeSpacesIndexes<HealthComponent>() };			
				auto cap{ compManipulator.Debug_GetComponentsCollection<HealthComponent>()->Capacity() };

				CHECK(cap == (compToAdd - compToRemove) + (compToAdd));

			}

			TEST_CASE("Get info after Flush. OK")
			{
				SharedPtr<IComponentContainer> container = std::make_shared<Ecs::ComponentContainer<HealthComponent>>(2);

				// Register the component container
				ComponentManipulator compManipulator;
				compManipulator.RegisterComponentContainer(std::move(container));

				const uint16 compToAdd{ 200 };
				const uint16 compToRemove{ 50 };
				EntityId entityToTest{ 88 };

				const HealthComponent custom{ 88, 88 };

				const HealthComponent healthComp{ 1, 2, };
				for (int ientity = 0; ientity < compToAdd; ++ientity)
				{
					if(ientity == entityToTest)
					{
						compManipulator.AddComponentDeferred(ientity, custom);
						continue;
					}
					// Adding a simple component
					compManipulator.AddComponentDeferred(ientity, healthComp);
				}

				compManipulator.UpdateComponentsStates();

				for (int ientity = 0; ientity < compToRemove; ++ientity)
				{
					// Removing some components
					compManipulator.RemoveComponentDeferred<HealthComponent>(ientity);
				}

				compManipulator.UpdateComponentsStates();

				compManipulator.FlushComponenContainerAllocation<HealthComponent>();
  
				auto comp{ compManipulator.GetComponent<HealthComponent>(entityToTest) };

				CHECK(comp.current == custom.current);
				CHECK(comp.max == custom.max);
			}

		}

		struct CustomEntity
		{
			int a, b;
		};


		class GameEntityCreator : public Ecs::IEntityCreator
		{
		public:
			GameEntityCreator() = default;

			Ecs::EntityId GetEntityId(void* object) override
			{
				if (!object)
				{
					return Ecs::kNullId;
				}

				return ++m_entityAssignerIdx;
			}


			uint32 m_entityAssignerIdx{ 0 };
		};

		TEST_CASE("Create Context, Everything is, OK")
		{
			
			UniquePtr<GameEntityCreator> entityCreator = std::make_unique<GameEntityCreator>();
			UniquePtr<EntityManager> manager = std::make_unique<EntityManager>(std::move(entityCreator));
			UniquePtr<ComponentManipulator> manip = std::make_unique<ComponentManipulator>();
			EcsContext context{ std::move(manager), std::move(manip) };

			struct HealthComponent
			{
				int now;
				int max;
			};

			EntityId entityToTest{ 0 };
			int entitiesToCreate{ 10 };

			TArray<CustomEntity*> entities;
			for (int i = 0; i< entitiesToCreate; ++i) 
			{
				entities.Add(new CustomEntity());
				context.RegisterEntity(entities[i]);
			}

			auto& worldEntities{ context.GetEntityManager()->GetEntitiesCollection() };

			bool bContidion{ worldEntities.Size() == entities.Size() };

		
			for(auto& entity : entities)
			{
				delete entity;
			}

			CHECK(bContidion == true);						
			
		}

		struct TransformComponent
		{
			bool b{ false };
		};

		struct TransformComponent1
		{
			bool b{ false };
		};

		class HealthSystem : public System
		{
		public:
			HealthSystem() = default;

			void Update(float dt, EntityId entity) override
			{
				auto& t0{ m_compManipulator->GetComponent<TransformComponent>(entity) };
				auto& t1{ m_compManipulator->GetComponent<TransformComponent1>(entity) };

				t0.b = true;
				t1.b = true;
			}

		protected:
			void SetComponentRequirements() override 
			{
				RequireComponent<TransformComponent>();
				RequireComponent<TransformComponent1>();
			}
		};

		TEST_CASE("Added Components to entitis through context, System Ticks, OK")
		{
			UniquePtr<GameEntityCreator> entityCreator = std::make_unique<GameEntityCreator>();
			UniquePtr<EntityManager> manager = std::make_unique<EntityManager>(std::move(entityCreator));
			UniquePtr<ComponentManipulator> manip = std::make_unique<ComponentManipulator>();

			EcsContext context{ std::move(manager), std::move(manip) };


			EntityId entityToTest{ 0 };
			int entitiesToCreate{ 10 };

			TArray<CustomEntity*> entities;
			for (int i = 0; i < entitiesToCreate; ++i)
			{
				entities.Add(new CustomEntity());
				context.RegisterEntity(entities[i]);
			}
			
			SharedPtr<IComponentContainer> container1 = std::make_shared<Ecs::ComponentContainer<TransformComponent>>();
			SharedPtr<IComponentContainer> container2 = std::make_shared<Ecs::ComponentContainer<TransformComponent1>>();

			ComponentManipulator* manipulator{ context.GetComponentManipulator() };
			manipulator->RegisterComponentContainer(std::move(container1));
			manipulator->RegisterComponentContainer(std::move(container2));

			
			UniquePtr<HealthSystem> sys{ std::make_unique<HealthSystem>() };
			context.RegisterSystem(std::move(sys));

			for(auto& entity : entities)
			{
					context.GetComponentManipulator()->AddComponentDeferred<TransformComponent>(
						context.GetEntityManager()->GetEntityId(entity),
						TransformComponent{ });

					context.GetComponentManipulator()->AddComponentDeferred<TransformComponent1>(
						context.GetEntityManager()->GetEntityId(entity),
						TransformComponent1{ });
			}
			
			context.UpdateContext(0.016);
			
			bool bSystemDidNotChangeSomeComponent{ false };
			for(auto& entity : entities)
			{
				const EntityId entityId{ context.GetEntityManager()->GetEntityId(entity) };
				auto T0Comp{ context.GetComponentManipulator()->GetComponent<TransformComponent>(entityId) };
				auto T1Comp{ context.GetComponentManipulator()->GetComponent<TransformComponent1>(entityId) };

				if(!T0Comp.b || !T1Comp.b)
				{
					bSystemDidNotChangeSomeComponent = true;
					break;
				}

			}
			
			CHECK(bSystemDidNotChangeSomeComponent == false);
		}


}} //ishak::Ecs
