#include "World.hpp"

namespace Renderer{
    void World::add_object(TriangulatedObject&& object) {
        stored_objects_.emplace_back(std::move(object));
    }

    std::list<TriangulatedObject>::iterator World::begin() {
        return stored_objects_.begin();
    }

    std::list<TriangulatedObject>::iterator World::end() {
        return stored_objects_.end();
    }

    World::~World() noexcept {
        for(auto it = stored_objects_.begin(); it != stored_objects_.end(); ++it) {
            it->~TriangulatedObject();
        }
    }
}