# SLECS
Super Lightweight Entity Component System - written using C++ 11

## Entities
There isn't any struct for the entities - They are all kept as IDs, which are 32-bit unsigned integers aliased as `EntityHandle`. Keep in mind that `EntityHandle`s can become invalid if the entity is deleted.

## Components
Components are simply C-style structs (ie. without methods)
```cpp
struct ExampleComponent {
	int x = 2;
	int y = 5;
	std::string someData = "Hello, world";
};
```

## Systems
Systems are classes that inherit from the `System` base
```cpp
class TestSystem : public System {
private:
	int m_iterations;
public:
	void Think() {
		std::cout << "System iteration: " << m_iterations << std::endl;
		for (const auto& ent : m_entities) {
			auto component = g_ecs->GetComponent<TestComponent>(ent);

			std::cout << component.text << std::endl;
			component.value++;
		}
		m_iterations++;
	}
};

auto testSystemInstance = g_ecs->RegisterSystem<TestSystem>(/* Constructor arguments can be passed to the system here*/);
{
	// The signature dictates what components the system has access to. Entities
	// that have components in the signature will automatically be added to the
	// system's m_entities vector
	Signature signature;
	signature.set(g_ecs->GetComponentType<TestComponent>());

	g_ecs->SetSystemSignature<TestSystem>(signature);
}
```

## Complete Example
```cpp
std::unique_ptr<ECS> g_ecs;

struct TestComponent {
	std::string text;
	int value;
};

class TestSystem : public System {
private:
	int m_iterations;
public:
	void Think() {
		std::cout << "System iteration: " << m_iterations << std::endl;

		for (const auto& ent : m_entities) {
			auto component = g_ecs->GetComponent<TestComponent>(ent);

			std::cout << component.text << std::endl;
			component.value++;
		}
		m_iterations++;
	}
};


int main(void) {
	g_ecs = std::make_unique<ECS>();

	g_ecs->RegisterComponent<TestComponent>();

	auto testSystemInstance = g_ecs->RegisterSystem<TestSystem>(/* Constructor arguments can be passed to the system here*/);
	{
		// The signature dictates what components the system has access to. Entities
		// that have components in the signature will automatically be added to the
		// system's m_entities vector
		Signature signature;
		signature.set(g_ecs->GetComponentType<TestComponent>());
		g_ecs->SetSystemSignature<TestSystem>(signature);
	}

	std::vector<EntityHandle> entities(10);

	int count = 0;
	for (auto& ent : entities) {
		ent = g_ecs->CreateEntity();

		g_ecs->AddComponent(ent, TestComponent {
			"Entity number: " + std::to_string(count+1),
			count
		});
		count++;
	}

	for (int i = 0; i < 10000; i++) {
		testSystemInstance->Think();
	}
}

```
