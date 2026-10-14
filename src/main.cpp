#include <iostream>

#include "entity_manager.h"
#include <cassert>
struct Health {
    int hp;
};

struct Position {
    float x, y;
};

int main() {
    EntityManager<Health, Position> em;

    EntityID e = em.createEntity();
    assert(em.validEntity(e));
    assert(!em.hasComponent<Health>(e));

    em.addComponent(e, Health{100});
    assert(em.hasComponent<Health>(e));
    assert(!em.hasComponent<Position>(e));
    assert(em.getComponent<Health>(e).hp == 100);

    Position p{1.0f, 2.0f};  // named variable: copy path
    em.addComponent(e, p);
    assert(em.getComponent<Position>(e).y == 2.0f);

    em.getComponent<Health>(e).hp = 50;  // reference, write through
    assert(em.getComponent<Health>(e).hp == 50);

    EntityID e2 = em.createEntity();
    assert(e2 != e && !em.hasComponent<Health>(e2));

    return 0;
}