#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "MeshData.hpp"

#include <array>
#include <stdexcept>
#include <utility>

namespace Renderer{

    class Object {
    public:
        Object() = default;
        explicit Object(MeshData&& data);

        ~Object() noexcept;

        Object(const Object& obj) = delete;
        Object& operator=(const Object& obj) = delete;
        Object(Object&& obj) noexcept;
        Object& operator=(Object&& obj) noexcept;


    private:
        void clear() noexcept;
        void upload_to_gpu(const MeshData& data);
    
    private:
        enum BUFFER_TYPE {
            INDEX_BUFFER =  0,
            POS_VB = 1,
            NORMAL_VB = 2,
            NUM_BUFFERS = 3
        };

        const char POSITION_LOCATION = 0;
        const char NORMAL_LOCATION = 1;

        GLuint VAO_ = 0;
        std::array<GLuint, NUM_BUFFERS> buffers_{};

        MeshData mesh_data_;
    };
} // namespace Renderer