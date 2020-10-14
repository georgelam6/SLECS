#pragma once

#include "ECSCommon.h"

#include <queue>
#include <array>

class EntityManager {
private:
   std::queue<EntityHandle> m_availableEntities;

   std::array<Signature, MAX_ENTITIES> m_signatures;

   uint32_t m_livingEntityCount{0};
public:
   EntityManager();

   EntityHandle CreateEntity();
   void DestroyEntity(EntityHandle ent);

   void SetSignature(EntityHandle ent, Signature signature);
   Signature GetSignature(EntityHandle ent);
};
