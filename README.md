# SLECS
Super Lightweight Entity Component System - A < 500 line single-header ECS

## Components
Components are simply C-style structs (ie. without methods)
```
struct ExampleComponent {
   int x = 2;
   int y = 5;
   std::string someData = "Hello, world";
};
```

## Entities
The entity doesn't have a dedicated class, it is simply an id. Entities are stored in a vector by the main ECS class
```
ECS ecsManager;

/* EntityHandle is an ID that points to an entity */
EntityHandle testEnt = ecsManager.CreateEntity();

/* Both AddComponent and GetComponent return a pointer to the component */
ecsManager.AddComponent<ExampleComponent>(testEnt)->x = 5;
ecsManager.GetComponent<ExampleComponent>(testEnt)->someData = "This is an example";
```

## Systems
The system is an iterator that iterates over entities with specific components
```
/* This system loops over all the entities that have ExampleComponent and SomeTestComponent */
for (EntityHandle ent : System<ExampleComponent, SomeTestComponent>(ecsManager)) {

   /* GetEntityIndex returns an ID for the EntityHandle */
   std::cout << "\n" << "Entity " << GetEntityIndex(ent) << '\n';
   std::cout << "ExampleComponent someData: " << ecsManager.GetComponent<ExampleComponent>(ent)->someData << '\n';
   std::cout << "ExampleComponent x: " << ecsManager.GetComponent<ExampleComponent>(ent)->x << '\n';
   std::cout << "ExampleComponent y: " << ecsManager.GetComponent<ExampleComponent>(ent)->y << '\n';
}
```
