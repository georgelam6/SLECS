#include <iostream>
#include <string>


#define MAX_ENTITIES 100
#define MAX_COMPONENTS 32

#include "SLECS.h"



struct TestComponent {
   int value = 500;
};

struct TestComponent2 {
   float x = 4.0f;
   float y = 5.0f;
};

int main(int argc, char const *argv[]) {
   ECS ecsManager;

   EntityHandle testEnt = ecsManager.CreateEntity();
   ecsManager.AddComponent<TestComponent>(testEnt);
   ecsManager.AddComponent<TestComponent2>(testEnt);

   EntityHandle testEnt2 = ecsManager.CreateEntity();
   ecsManager.AddComponent<TestComponent>(testEnt2)->value = 10;
   ecsManager.AddComponent<TestComponent2>(testEnt2)->x = 100;
   ecsManager.GetComponent<TestComponent2>(testEnt2)->y = 2;


   for (EntityHandle ent : System<TestComponent, TestComponent2>(ecsManager)) {
      std::cout << "\n" << "Entity " << GetEntityIndex(ent) << '\n';
      std::cout << "TestComponent value: " << ecsManager.GetComponent<TestComponent>(ent)->value << '\n';
      std::cout << "TestComponent2 x: " << ecsManager.GetComponent<TestComponent2>(ent)->x << '\n';
      std::cout << "TestComponent2 y: " << ecsManager.GetComponent<TestComponent2>(ent)->y << '\n';
   }

   return 0;
}
