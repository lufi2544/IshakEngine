#pragma once

#include "CoreMinimal.h"
#include "Component.h"

#include "ECSConfig.h"

namespace ishak { namespace Ecs {

		

		struct ECS_API IComponentContainer
		{
			virtual ~IComponentContainer() = default;

			virtual ComponentId GetId() = 0;
		};

		template<typename T>
		struct ECS_API ComponentContainer : public IComponentContainer
		{
			ComponentContainer() = default;

			ComponentId GetId()
			{
				
				return T::GetId();
			}

			TArray<Component<T>> components;
		};


} }// ishak::Ecs