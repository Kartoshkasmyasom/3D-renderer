#pragma once

#include "TriangulatedObject.hpp"

#include <list>

namespace Renderer {


    class World {
    public:
    World() = default;
    ~World() noexcept;
    void add_object(TriangulatedObject&& object);
    
    std::list<TriangulatedObject>::iterator begin();
    std::list<TriangulatedObject>::iterator end();

    private:
        std::list<TriangulatedObject> stored_objects_;
    };
} // namespace Renderer