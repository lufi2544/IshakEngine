#pragma once

// General includes for the ECS
// TODO Add description about the ECS.

#include "EntityId.h"
#include "ComponentContainer.h"
#include "ComponentManipulator.h"
#include "EcsContext.h"
#include "EntityManager.h"
#include "IEntityCreator.h"
#include "System.h"
#include "SystemConfig.h"

/* Heder file that will include all the ECS relevant file
* 
* 
* DOCUMENTATION: 
* The Ecs system architecture is a programming pattern that looks to take advantage of the cpu cache (data, instruction)
* for executing logic along the engine by dividing the execution between components which hold data and systems which execute logic from the components data.
* 
* The main point of this is that we have different Component Classes( will be only data ) and a collection of systems. 
* In this case the systems will execute all the logic, the data from the Components will be used for this execution flow.
* EXPLAINATION: 
* 
*	Entity: 

	An entity will be just understood as a single GUID, like an integer.
* 
*   ComponentsContainer:
* 
*	I have opted for having a data structure called ComponentContainer, that will store a collection of components of the same type in memory. 
*   When we create this container, a X amount of components will be created with defaulted values in memory, so whenever we want to add a component, we have already 
*   the component created and it will be just copying it to the new value. Whenever an entity removes a component, that will be marked as free, so next time we want to 
*   add a new component, this free component will be taken with the new component value.
*	
*	**IMPORTANT**: ComponentContainers can be shared between ComponentManipulators, so we can access and modify those components from different parts in the code.
*		The TransfomComponent will be shared, because we want to access its state in the RenderingEcsContexts and the GameEcsContext for example.
* 
*   ComponentManipulator:
*	
*	This class as it says, it's in charge of manipulating( deleting, adding...) new components to the different entities. We have to register the different componentContainers here.
*	 
*	EcsContext:
*	
*	Holds the different systems and the component manipulator for this EcsContext. Each and every context will have their own componentManipulator. We have to Register
*   the systems that we want this context to update.
* 
*   EntityManager:
* 
*	This manager is in charge of assigning and registering the different entities along the Engine.
*/
