#pragma once

#include <memory>

#include "../src/ECSCommon.h"

#include "../src/ComponentManager.h"
#include "../src/EntityManager.h"
#include "../src/SystemManager.h"

#include "../src/ComponentArray.h"
#include "../src/System.h"

class ECS {
private:
   std::unique_ptr<ComponentManager> m_componentManager;
   std::unique_ptr<EntityManager> m_entityManager;
   std::unique_ptr<SystemManager> m_systemManager;
public:
	ECS() {
		m_componentManager = std::make_unique<ComponentManager>();
      m_entityManager = std::make_unique<EntityManager>();
      m_systemManager = std::make_unique<SystemManager>();
	}

   /* Entities */
   EntityHandle CreateEntity() {
      return m_entityManager->CreateEntity();
   }

   void DestroyEntity(EntityHandle ent) {
      m_entityManager->DestroyEntity(ent);
      m_componentManager->EntityDestroyed(ent);
      m_systemManager->EntityDestroyed(ent);
   }

   /* Components */
   template <typename T>
   void RegisterComponent() {
      m_componentManager->RegisterComponent<T>();
   }

   template <typename T>
   T& AddComponent(EntityHandle ent, T component) {
      m_componentManager->AddComponent<T>(ent, component);

      auto signature = m_entityManager->GetSignature(ent);
      signature.set(m_componentManager->GetComponentType<T>(), true);
      m_entityManager->SetSignature(ent, signature);

      m_systemManager->EntitySignatureChanged(ent, signature);

      return m_componentManager->GetComponent<T>(ent);
   }

   template <typename T>
   void RemoveComponent(EntityHandle ent) {
      m_componentManager->RemoveComponent<T>(ent);

      auto signature = m_entityManager->GetSignature(ent);
      signature.set(m_componentManager->GetComponentType<T>(), false);
      m_entityManager->SetSignature(ent, signature);

      m_systemManager->EntitySignatureChanged(ent, signature);
   }

   template <typename T>
   T& GetComponent(EntityHandle ent) {
      return m_componentManager->GetComponent<T>(ent);
   }

   template <typename T>
   bool HasComponent(EntityHandle ent) {
      return m_entityManager->GetSignature(ent).test(m_componentManager->GetComponentType<T>());
   }

   template <typename T>
   ComponentType GetComponentType() {
      return m_componentManager->GetComponentType<T>();
   }

   /* Systems */
   template <typename T, typename... Args>
   std::shared_ptr<T> RegisterSystem(Args... constructorArgs) {
      return m_systemManager->RegisterSystem<T>(constructorArgs...);
   }

   template <typename T>
   void SetSystemSignature(Signature signature) {
      m_systemManager->SetSignature<T>(signature);
   }
};
