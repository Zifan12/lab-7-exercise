#ifndef REGISTRY_H
#define REGISTRY_H

#include <cassert>
#include <cstddef>
#include <stack>
#include <vector>
#include <utility>

template <typename T>
class Registry {
public:

    template <typename U>
    size_t store(U&& item) {
        if (freeSlots.empty()) {
            items.push_back(std::forward<U>(item));
            alive.push_back(true);

            return items.size() - 1;

        } else {
            auto index = freeSlots.top();
            freeSlots.pop();
            items[index] = std::forward<U>(item);
            alive[index] = true;
            return index;
        }
    }

    void remove(size_t ticket) {
        assert(valid(ticket));
        alive[ticket] = false;
        freeSlots.push(ticket);
    }

    bool valid(size_t ticket) const {
        return (ticket < alive.size() && alive[ticket]);
    }

    size_t size() const {
        return items.size() - freeSlots.size();
    }

    T& operator[](size_t ticket) {
        assert(valid(ticket));
        return items[ticket];
    }

    const T& operator[](size_t ticket) const {
        assert(valid(ticket));
        return items[ticket];
    }



private:
    std::vector<T> items;
    std::vector<bool> alive;
    std::stack<size_t> freeSlots;
};

#endif  // REGISTRY_H
