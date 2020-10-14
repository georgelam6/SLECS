#include "SystemManager.h"

#include <iostream>

void SystemManager::EntityDestroyed(EntityHandle ent) {
   for (const auto& pair : m_systems) {
		pair.second->m_entities.erase(ent);
   }
}

void SystemManager::EntitySignatureChanged(EntityHandle ent, Signature entitySignature) {
   for (const auto& pair : m_systems) {
      const auto& type = pair.first;
      const auto& system = pair.second;
      const auto& systemSignature = m_signatures[type];

      if ((entitySignature & systemSignature) == systemSignature) {
         system->m_entities.insert(ent);
      } else {
         system->m_entities.erase(ent);
      }
   }
}
