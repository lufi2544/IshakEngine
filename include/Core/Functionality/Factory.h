#pragma once 

// ISHAK
#include "Core/CoreMinimal.h"
#include "Core/CoreConfig.h"

// STD
#include <typeindex>
#include <functional>

namespace ishak {

	enum class EClassMultiplicity
	{
		MultipleObjects,
		Singleton
	};

	class CORE_API Factory
	{
		using BuilderFn = std::function<std::shared_ptr<void>(Factory&)>;
	
		struct FactoryData
		{
			EClassMultiplicity classMultiplicity;
			BuilderFn builder;
			std::shared_ptr<void> createdObj;
		};

	public:
		Factory();

		template<typename T>
		void RegisterBuilder(const BuilderFn& builder, EClassMultiplicity multiplicity = EClassMultiplicity::MultipleObjects)
		{
			m_builderMap[typeid(T)] = FactoryData{ multiplicity, builder };
		}

		template<typename T>
		std::shared_ptr<T> GetOrBuild()
		{
			if (m_builderMap.find(typeid(T)) != std::end(m_builderMap))
			{
				FactoryData& objectData{ m_builderMap[typeid(T)] };				

				if(objectData.classMultiplicity == EClassMultiplicity::Singleton)
				{
					if(!objectData.createdObj)
					{
						objectData.createdObj = std::invoke(objectData.builder, *this);
					}

					return std::reinterpret_pointer_cast<T>(objectData.createdObj);
				}
				
				return std::reinterpret_pointer_cast<T>(std::invoke(objectData.builder, *this));				
			}
			else 
			{
				// Builder not found.
				
			}

			return { };
		}

	private:
		std::map<std::type_index, FactoryData> m_builderMap;
	};
	
}// ishak