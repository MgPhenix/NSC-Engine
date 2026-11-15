#include <iostream>
#include "main.h"
#include "Entity.h"


struct BaseComponent : public IComponent<Entity>
{
    void Update(Entity* owner) override {};
};

int main()
{
    Entity e;
    e.AddComponent<BaseComponent>();

    IComponent<Entity>* comp = e.GetComponent<BaseComponent>();

    if (comp)
        std::cout << "yahou";
    else
        std::cout << "mama mia";

    return 0;
}