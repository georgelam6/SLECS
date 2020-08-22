#pragma once

#include <vector>
#include <bitset>
#include <tuple>

extern unsigned int impl_componentCounter;
template <typename T>
unsigned int GetComponentID() {
   static unsigned int s_componentID = impl_componentCounter++;
   return s_componentID;
}

typedef unsigned int EntityIndex;
typedef unsigned int EntityVersion;
typedef unsigned long long EntityHandle;

const int MAX_COMPONENTS = 32;
const int MAX_ENTITIES = 100;
typedef std::bitset<MAX_COMPONENTS> ComponentMask;


inline EntityHandle CreateHandleToEntity(EntityIndex index, EntityVersion version) {
  return ((EntityHandle)index << 32) | ((EntityHandle)version);
}

inline EntityIndex GetEntityIndex(EntityHandle entity) {
  return entity >> 32;
}

inline EntityVersion GetEntityVersion(EntityHandle entity) {
  return (EntityVersion)entity;
}

inline bool IsEntityValid(EntityHandle entity) {
  return (entity >> 32) != EntityIndex(-1);
}

#define INVALID_ENTITY CreateHandleToEntity(EntityIndex(-1), 0)


/* This class is a pool of memory for components
   It is stored in a char array, since we cannot know the
   size of the container at compile time*/
class ComponentContainer {
private:
   char* m_data {nullptr};
   size_t m_elementSize {0};
public:
   ComponentContainer(size_t elementSize) {
      m_elementSize = elementSize;
      m_data = new char[m_elementSize * MAX_ENTITIES];
   }

   ~ComponentContainer() {
      delete[] m_data;
   }

   inline void* Get(size_t index) {
      // Look up the component
      return m_data + index * m_elementSize;
   }
};


struct ECS {
private:
   std::vector<std::pair<EntityHandle, ComponentMask>> m_entities;
   std::vector<ComponentContainer*> m_components;
   std::vector<EntityIndex> m_freeEntities; /* A vector of entities that can be overwritton */

public:
   ~ECS() {
      for (size_t i = 0; i < m_components.size(); i++) {
         delete m_components[i];
      }
   }

   inline const std::vector<std::pair<EntityHandle, ComponentMask>>& Internal_GetEntities() const {
      return m_entities;
   }

   template <typename T>
   T* AddComponent(EntityHandle entity) {
      unsigned int componentID = GetComponentID<T>();

      if (m_entities[GetEntityIndex(entity)].first != entity) {
         std::cerr << __FUNCTION__ << ": Attempt to access a deleted entity." << '\n';
         return nullptr;
      }

      if (m_components.size() <= componentID) {
         m_components.resize(componentID + 1, nullptr);
      }

      if (m_components[componentID] == nullptr) {
         m_components[componentID] = new ComponentContainer(sizeof(T));
      }

      T* newComponent = new(m_components[componentID]->Get(entity)) T();

      m_entities[GetEntityIndex(entity)].second.set(componentID);
      return newComponent;
   }

   template <typename T>
   T* GetComponent(EntityHandle entity) {
      int componentID = GetComponentID<T>();

      if (m_entities[GetEntityIndex(entity)].first != entity) {
         std::cerr << __FUNCTION__ << ": Attempt to access a deleted entity." << '\n';
         return nullptr;
      }

      if (!m_entities[GetEntityIndex(entity)].second.test(componentID)) {
         std::cerr << __FUNCTION__ << ": Component doesn't exist on the requested entity. Returning nullptr instead." << '\n';
         return nullptr;
      }

      T* gotComponent = static_cast<T*>(m_components[componentID]->Get(entity));
      return gotComponent;
   }

   template <typename T>
   void RemoveComponent(EntityHandle entity) {
      if (m_entities[GetEntityIndex(entity)].first != entity) {
         std::cerr << __FUNCTION__ << ": Attempt to access a deleted entity." << '\n';
         return;
      }

      m_entities[GetEntityIndex(entity)].second.reset(GetComponentID<T>());
   }


   void DestroyEntity(EntityHandle entity) {
      EntityHandle newEntity = CreateHandleToEntity(EntityIndex(-1), GetEntityVersion(entity) + 1);

      EntityIndex index = GetEntityIndex(entity);

      m_entities[index].first = newEntity;
      m_entities[index].second.reset();
      m_freeEntities.push_back(index);
   }

   EntityHandle CreateEntity() {
      if (!m_freeEntities.empty()) {
         EntityIndex newIndex = m_freeEntities.back();
         m_freeEntities.pop_back();

         EntityHandle newEntity = CreateHandleToEntity(newIndex, GetEntityVersion(m_entities[newIndex].first));

         m_entities[newIndex].first = newEntity;
         return m_entities[newIndex].first;
      }

      m_entities.push_back({m_entities.size(), ComponentMask()});
      return m_entities.back().first;
   }
};

template <typename... ComponentTypes>
struct ECSView {
private:
   ECS* m_entityManager {nullptr};
   bool m_all {false};
   ComponentMask m_componentMask;
public:
   ECSView(ECS& manager) : m_entityManager(&manager) {
      if (sizeof...(ComponentTypes) == 0) {
         m_all = true;
      } else {
         /* Basically loop through all the components and add them to the mask */
         unsigned int componentIDs[] = {0, GetComponentID<ComponentTypes>() ... };
         for (unsigned int i = 1; i < (sizeof...(ComponentTypes) + 1); i++) {
            m_componentMask.set(componentIDs[i]);
         }
      }
   }

   struct Iterator {
      EntityIndex index;
      ECS* manager;
      ComponentMask mask;
      bool all {false};

      Iterator(ECS* manager, EntityIndex index, ComponentMask mask, bool all) :
         manager(manager), index(index), all(all) {}

      EntityHandle operator*() const {
         return manager->Internal_GetEntities()[index].first;
      }

      bool operator==(const Iterator& other) const {
         return index == other.index || index == manager->Internal_GetEntities().size();
      }

      bool operator!=(const Iterator& other) const {
         return index != other.index || index != manager->Internal_GetEntities().size();
      }

      Iterator& operator++() {
         while (index < manager->Internal_GetEntities().size() && !ValidIndex()) {
            index++;
         }
         return *this;
      }

      bool ValidIndex() {
         return
            /* The handle is valid */
            IsEntityValid(manager->Internal_GetEntities()[index].first) &&
            /* ...And it has the correct mask */
            (all || mask == (mask & manager->Internal_GetEntities()[index].second));
      }

   };

   const Iterator begin() const {
      int firstIndex = 0;
      while (firstIndex < m_entityManager->Internal_GetEntities().size() &&
      (m_componentMask != (m_componentMask & m_entityManager->Internal_GetEntities()[firstIndex].second)
      || !IsEntityValid(m_entityManager->Internal_GetEntities()[firstIndex].first))) {
         firstIndex++;
      }
      return Iterator(m_entityManager, firstIndex, m_componentMask, m_all);
   }

   const Iterator end() const {
      return Iterator(m_entityManager, EntityIndex(m_entityManager->Internal_GetEntities().size()), m_componentMask, m_all);
   }
};
