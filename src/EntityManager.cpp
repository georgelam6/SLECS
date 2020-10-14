#include "EntityManager.h"

#include <cassert>

EntityManager::EntityManager() {
   for (EntityHandle ent = 0; ent < MAX_ENTITIES; ent++) {
      m_availableEntities.push(ent);
   }
}

EntityHandle EntityManager::CreateEntity() {
   assert(m_livingEntityCount < MAX_ENTITIES && "Too many entities in existance");

   EntityHandle newEnt = m_availableEntities.front();
   m_availableEntities.pop();
   ++m_livingEntityCount;

   return newEnt;
}

void EntityManager::DestroyEntity(EntityHandle ent) {
   assert(ent < MAX_ENTITIES && "Entity out of range.");

   m_signatures[ent].reset();

   m_availableEntities.push(ent);
   --m_livingEntityCount;
}

void EntityManager::SetSignature(EntityHandle ent, Signature signature) {
   assert(ent < MAX_ENTITIES && "Entity out of range.");

   m_signatures[ent] = signature;
}

Signature EntityManager::GetSignature(EntityHandle ent) {
   assert(ent < MAX_ENTITIES && "Entity our of range.");

   return m_signatures[ent];
}
