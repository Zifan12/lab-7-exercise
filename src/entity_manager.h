#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <array>
#include <cstddef>  // size_t
#include <limits>
#include <stack>
#include <tuple>
#include <vector>
#include <cassert>
#include <utility>
#include <type_traits>

#include "pack.h"  // Pack from prep
#include "registry.h"

using EntityID = size_t;

template <typename... ComponentTypes>
class EntityManager {
public:
    EntityID createEntity() {
        std::array<size_t, sizeof...(ComponentTypes)> row;
        row.fill(std::numeric_limits<size_t>::max());
        if (freeIDS.empty()) {
            tickets.push_back(row);
            aliveEntities.push_back(true);
            return tickets.size() - 1;
        } else {
            auto id = freeIDS.top();
            freeIDS.pop();
            aliveEntities[id] = true;
            tickets[id] = row;
            return id;
        }
    }

    bool validEntity(EntityID id) const {
        return (id < aliveEntities.size() && aliveEntities[id]);
    }

    template <typename ComponentType>
    bool hasComponent(EntityID id) const {
        assert(validEntity(id));
        constexpr int col = Pack<ComponentTypes...>::template index<ComponentType>;
        return tickets[id][col] != std::numeric_limits<size_t>::max();
    }

    template <typename ComponentType> ComponentType& getComponent(EntityID id) {
        assert(validEntity(id));
        assert(hasComponent<ComponentType>(id));
        constexpr int col = Pack<ComponentTypes...>::template index<ComponentType>;

        size_t ticket = tickets[id][col];
        return std::get<col>(registries)[ticket];
    }

    template <typename ComponentType>
    void addComponent(EntityID id, ComponentType&& component) {
        assert(validEntity(id));
        using C = std::remove_reference_t<ComponentType>;
        assert(!hasComponent<C>(id));
        constexpr int col = Pack<ComponentTypes...>::template index<C>;
        tickets[id][col] = std::get<col>(registries).store(std::forward<ComponentType>(component));

    }

private:
    std::vector<std::array<size_t, sizeof...(ComponentTypes)>> tickets;
    std::tuple<Registry<ComponentTypes>...> registries;
    std::vector<bool> aliveEntities;
    std::stack<size_t> freeIDS;
};

#endif  // ENTITY_MANAGER_H
