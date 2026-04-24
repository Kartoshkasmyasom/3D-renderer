#pragma once

#include "TriangulatedObject.hpp"

#include <vector>

namespace Renderer {


    class World {
    public:
    private:
        std::vector<TriangulatedObject> stored_objects_;
    }
} // namespace Renderer