#include <iostream>
#include <memory>
#include <vector>

#include <ECS.h>

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
		Signature signature;
		signature.set(g_ecs->GetComponentType<TestComponent>());
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
