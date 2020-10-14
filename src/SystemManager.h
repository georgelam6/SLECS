#pragma once

#include <unordered_map>
#include <memory>
#include <cassert>

#include "System.h"

class SystemManager {
private:
   std::unordered_map<const char*, Signature> m_signatures;

   std::unordered_map<const char*, std::shared_ptr<System>> m_systems;
public:
   void EntityDestroyed(EntityHandle ent);
   void EntitySignatureChanged(EntityHandle ent, Signature entitySignature);

   template<typename T, typename... Args>
   std::shared_ptr<T> RegisterSystem(Args... constructorArgs) {
      const char* typeName = typeid(T).name();

      assert(m_systems.find(typeName) == m_systems.end() && "Registering a system more than once.");

      auto system = std::make_shared<T>(constructorArgs...);
      m_systems.insert({typeName, system});
      return system;
   }

   template<typename T>
   void SetSignature(Signature signature) {
      const char* typeName = typeid(T).name();

      assert(m_systems.find(typeName) != m_systems.end() && "System used before registration.");

      m_signatures.insert({typeName, signature});
   }
};
