#include <iostream>
#include "main.h"
#include "Entity.h"


struct BaseComponent : public IComponent<Entity>
{
    void Update(Entity* owner) override {};
    using Type = BaseComponent;
};

int main()
{
    Entity e;
    e.AddComponent<BaseComponent>();

   auto comp = e.GetComponent<BaseComponent>();

    if (comp)
        std::cout << "yahou";
    else
        std::cout << "mama mia";

    return 0;
}