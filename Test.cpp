#include <iostream>

#include "ECS.h"

int s_componentCounter = 0;

struct TestComponent {
   int value {10937};
};

struct TestComponent2 {
   float value {4.0f};
};

int main(int argc, char const *argv[]) {
   EntityManager ecs;

   EntityHandle testEnt = ecs.CreateEntity();
   ecs.AddComponent<TestComponent>(testEnt);;
   ecs.AddComponent<TestComponent2>(testEnt);


   for (EntityHandle ent : SystemLoop<TestComponent, TestComponent2>(ecs)) {
      std::cout << ecs.GetComponent<TestComponent>(ent)->value << '\n';
   }

   ecs.RemoveComponent<TestComponent>(testEnt);
   ecs.DestroyEntity(testEnt);

   return 0;
}
