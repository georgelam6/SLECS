#pragma once

#include <unordered_map>
#include <array>
#include <cassert>

#include "ECSCommon.h"

class IComponentArray {
public:
   virtual ~IComponentArray() = default;
   virtual void EntityDestroyed(EntityHandle ent) = 0;
};

template <typename T>
class ComponentArray : public IComponentArray {
private:
   std::array<T, MAX_COMPONENTS> m_componentArray;

   std::unordered_map<EntityHandle, size_t> m_entityToIndexMap;
   std::unordered_map<size_t, EntityHandle> m_indexToEntityMap;

   size_t m_size;
public:
   inline void InsertData(EntityHandle ent, T component) {
      assert(m_entityToIndexMap.find(ent) == m_entityToIndexMap.end() && "Component added to same entity more than once.");

      size_t newIndex = m_size;
      m_entityToIndexMap[ent] = newIndex;
      m_indexToEntityMap[newIndex] = ent;
      m_componentArray[newIndex] = component;
      ++m_size;
   }

   inline void RemoveData(EntityHandle ent) {
      assert(m_entityToIndexMap.find(ent) != m_entityToIndexMap.end() && "Removing non-existent component.");

      size_t indexOfRemovedEntity = m_entityToIndexMap[ent];
      size_t indexOfLastElement = m_size - 1;
      m_componentArray[indexOfRemovedEntity] = m_componentArray[indexOfLastElement];

      EntityHandle entityOfLastElement = m_indexToEntityMap[indexOfLastElement];
      m_entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
      m_indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

      m_entityToIndexMap.erase(ent);
      m_indexToEntityMap.erase(indexOfLastElement);

      --m_size;
   }


   inline T& GetData(EntityHandle ent) {
      assert(m_entityToIndexMap.find(ent) != m_entityToIndexMap.end() && "Retrieving non-existent component.");

      return m_componentArray[m_entityToIndexMap[ent]];
   }

   inline void EntityDestroyed(EntityHandle ent) override {
      if (m_entityToIndexMap.find(ent) != m_entityToIndexMap.end()) {
         RemoveData(ent);
      }
   }
};
