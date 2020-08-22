#include <iostream>

#include "ECS.h"

unsigned int impl_componentCounter = 0;

struct TestComponent {
   int value {10937};
};

struct TestComponent2 {
   float value {4.0f};
};

int main(int argc, char const *argv[]) {
   ECS ecs;

   EntityHandle testEnt = ecs.CreateEntity();
   ecs.AddComponent<TestComponent>(testEnt);
   ecs.AddComponent<TestComponent2>(testEnt);

   EntityHandle testEnt2 = ecs.CreateEntity();
   ecs.AddComponent<TestComponent>(testEnt2);
   //ecs.GetComponent<TestComponent>(testEnt2);

   //for (EntityHandle ent : ECSView<TestComponent>(ecs)) {
      //std::cout << ecs.GetComponent<TestComponent>(ent)->value << '\n';
   //}


   return 0;
}
