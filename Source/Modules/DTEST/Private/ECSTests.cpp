#include "IshakTest.h"

#include "ComponentContainer.h"
#include "ComponentManipulator.h"
#include "Component.h"

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
				EntityId entity{ 22 };						
				SharedPtr<IComponentContainer> container = std::make_shared<Ecs::ComponentContainer<HealthComponent>>();

				// Register the component container
				ComponentManipulator compManipulator;
				compManipulator.RegisterComponentContainer(std::move(container));

				// Adding a simple component
				HealthComponent healthComp{ 1, 2, };
				compManipulator.AddComponent(entity, healthComp);

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
				compManipulator.AddComponent(entity, healthComp);
				compManipulator.RemoveComponent<HealthComponent>(entity);

				// Checking if the component has been stored
				const bool bHasComp = compManipulator.HasComponent<HealthComponent>(entity);				

				CHECK(bHasComp == false);
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
					compManipulator.AddComponent(ientity, healthComp);					
				}

				EntityId entityToDelete1{ 4 };
				EntityId entityToDelete2{ 13 };

				// Indexes for the entities components in the container.
				bool bFound1;
				const uint16 entity1ComponentIdx{ compManipulator.Debug_GetComponentIdxForEntity<HealthComponent>(entityToDelete1, bFound1) };

				bool bFound2;
				const uint16 entity2ComponentIdx{ compManipulator.Debug_GetComponentIdxForEntity<HealthComponent>(entityToDelete2, bFound2) };

				compManipulator.RemoveComponent<HealthComponent>(entityToDelete1);
				compManipulator.RemoveComponent<HealthComponent>(entityToDelete2);

				const TArray<uint16> freeSpaces{ compManipulator.Debug_GetFreeSpacesIndexes<HealthComponent>() };

				CHECK(freeSpaces.Size() == 2);
				CHECK(bFound1 == true);
				CHECK(bFound2 == true);
				CHECK(freeSpaces.Contains(entity1ComponentIdx) == true);
				CHECK(freeSpaces.Contains(entity2ComponentIdx) == true);
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
					compManipulator.AddComponent(ientity, healthComp);
				}

				EntityId entityToTest1{ 4 };
				EntityId entityToTest2{ 13 };
				EntityId entityToTest3{ 2 };
						

				// As the spaces are gonig to be taken from the last deleted to the first deleted, the entity3 space is the first taken when adding a
				// new component to the container, so in this case the space that is going to be still free when we add 2 components later, is the 
				// space for entity 1.
				bool bFound;
				const uint16 entity1ComponentIdx{ compManipulator.Debug_GetComponentIdxForEntity<HealthComponent>(entityToTest1, bFound) };


				compManipulator.RemoveComponent<HealthComponent>(entityToTest1);
				compManipulator.RemoveComponent<HealthComponent>(entityToTest2);
				compManipulator.RemoveComponent<HealthComponent>(entityToTest3);


				compManipulator.AddComponent(entityToTest1, healthComp);
				compManipulator.AddComponent(entityToTest2, healthComp);

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
					compManipulator.AddComponent(ientity, healthComp);
				}

				for (int ientity = 0; ientity < compToRemove; ++ientity)
				{
					// Removing some components
					compManipulator.RemoveComponent<HealthComponent>(ientity);
				}
																	
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
					compManipulator.AddComponent(ientity, healthComp);
				}

				for (int ientity = 0; ientity < compToRemove; ++ientity)
				{
					// Removing some components
					compManipulator.RemoveComponent<HealthComponent>(ientity);
				}

				compManipulator.FlushComponenContainerAllocation<HealthComponent>();
				

				for (int ientity = 0; ientity < compToAdd; ++ientity)
				{
					// Adding a simple component
					compManipulator.AddComponent(ientity, healthComp);
				}

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
						compManipulator.AddComponent(ientity, custom);
						continue;
					}
					// Adding a simple component
					compManipulator.AddComponent(ientity, healthComp);
				}

				for (int ientity = 0; ientity < compToRemove; ++ientity)
				{
					// Removing some components
					compManipulator.RemoveComponent<HealthComponent>(ientity);
				}

				compManipulator.FlushComponenContainerAllocation<HealthComponent>();
  
				auto comp{ compManipulator.GetComponent<HealthComponent>(entityToTest) };

				CHECK(comp.current == custom.current);
				CHECK(comp.max == custom.max);
			}

		}

		TEST_CASE("System Signature Test")
		{
			struct HealthComponent
			{
				int now;
				int max;
			};

			EntityId entityToTest{ 0 };
			SharedPtr<IComponentContainer> container = std::make_shared<Ecs::ComponentContainer<TransformComponent>>();
			SharedPtr<IComponentContainer> container2 = std::make_shared<Ecs::ComponentContainer<HealthComponent>>();

			// Register the component container
			ComponentManipulator compManipulator;
			compManipulator.RegisterEntity(entityToTest);
			compManipulator.RegisterComponentContainer(std::move(container));
			compManipulator.RegisterComponentContainer(std::move(container2));

			SharedPtr<HealthSystem> system{ std::make_shared<HealthSystem>() };
			compManipulator.RegisterSystem(std::move(system));


			TransformComponent transform{ 1, 2, 3 };
			HealthComponent health{ 1, 2 };
			compManipulator.AddComponent(entityToTest, transform);
			compManipulator.AddComponent(entityToTest, health);

			compManipulator.UpdateSystems(0.016f);

			HealthComponent h{ compManipulator.GetComponent<HealthComponent>(entityToTest) };
			TransformComponent t{ compManipulator.GetComponent<TransformComponent>(entityToTest) };

			CHECK(h.max == health.max);
			CHECK(h.max == health.max);

			CHECK(t.x == t.x);
			CHECK(t.y == t.y);
			CHECK(t.z == t.z);
		}

}} //ishak::Ecs