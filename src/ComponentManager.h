#pragma once

#include <unordered_map>
#include <memory>

#include "ECSCommon.h"

#include "ComponentArray.h"

class ComponentManager {
private:
   std::unordered_map<const char*, ComponentType> m_componentTypes;
   std::unordered_map<const char*, std::shared_ptr<IComponentArray>> m_componentArrays;

   ComponentType m_nextComponentType;

   template <typename T>
   std::shared_ptr<ComponentArray<T>> GetComponentArray() {
      const char* typeName = typeid(T).name();

      assert(m_componentTypes.find(typeName) != m_componentTypes.end() && "Component used before registration.");

      return std::static_pointer_cast<ComponentArray<T>>(m_componentArrays[typeName]);
   }
public:
   template <typename T>
   void RegisterComponent() {
      const char* typeName = typeid(T).name();

      assert(m_componentTypes.find(typeName) == m_componentTypes.end() && "Registering component type more than once.");

      m_componentTypes.insert({typeName, m_nextComponentType});
      m_componentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

      ++m_nextComponentType;
   }

   template <typename T>
   ComponentType GetComponentType() {
      const char* typeName = typeid(T).name();

      assert(m_componentTypes.find(typeName) != m_componentTypes.end() && "Component used before registration.");

      return m_componentTypes[typeName];
   }

   template <typename T>
   void AddComponent(EntityHandle ent, T component) {
      GetComponentArray<T>()->InsertData(ent, component);
   }

   template <typename T>
   void RemoveComponent(EntityHandle ent) {
      GetComponentArray<T>()->RemoveData(ent);
   }

   template <typename T>
   T& GetComponent(EntityHandle ent) {
      return GetComponentArray<T>()->GetData(ent);
   }

   inline void EntityDestroyed(EntityHandle ent) {
      for (const auto& pair : m_componentArrays) {
         const auto& component = pair.second;

         component->EntityDestroyed(ent);
      }
   }
};
