#include "Object.hpp"
#include <iostream>

namespace Renderer{
    void Object::clear() noexcept {
        if(VAO_ != 0) {
            glDeleteVertexArrays(1, &VAO_);
            VAO_ = 0;
        }

        unsigned int buffer_size = buffers_.size();
        if(buffers_[0] != 0) {
            glDeleteBuffers(buffer_size, buffers_.data());
        }

        mesh_data_.clear();
    }

    Object::Object(MeshData&& data) : mesh_data_(std::move(data)) {
        upload_to_gpu(mesh_data_);
    }

    Object::~Object() {
        clear();
    }

    Object::Object(Object&& obj) noexcept {
        VAO_ = obj.VAO_;
        buffers_ = obj.buffers_;
        obj.VAO_ = 0;
        obj.buffers_.fill(0);
    }

    Object& Object::operator=(Object&& obj) noexcept {
        if(this != &obj) {
            clear();
            VAO_ = obj.VAO_;
            buffers_ = obj.buffers_;
            obj.VAO_ = 0;
            obj.buffers_.fill(0);
        }
        return *this;
    }


    void Object::upload_to_gpu(const MeshData& data) {
        if(data.positions.empty() || data.indices.empty()) {
            throw std::runtime_error("Could not upload empty mesh on GPU!");
        }

        glGenVertexArrays(1, &VAO_);
        glBindVertexArray(VAO_);

        glGenBuffers(static_cast<GLsizei>(buffers_.size()), buffers_.data());

        glBindBuffer(GL_ARRAY_BUFFER, buffers_[POS_VB]);
        glBufferData(
            GL_ARRAY_BUFFER, 
            static_cast<GLsizeiptr>(sizeof(data.positions[0]) * data.positions.size()),
            &data.positions[0], 
            GL_STATIC_DRAW
        );
        glEnableVertexAttribArray(POSITION_LOCATION);
        glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);

        if(!data.normals.empty()) {
            glBindBuffer(GL_ARRAY_BUFFER, buffers_[NORMAL_VB]);
            glBufferData(
                GL_ARRAY_BUFFER, 
                static_cast<GLsizeiptr>(sizeof(data.normals[0]) * data.normals.size()),
                &data.normals[0], 
                GL_STATIC_DRAW
            );
            glEnableVertexAttribArray(NORMAL_LOCATION);
            glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers_[INDEX_BUFFER]);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(sizeof(data.indices[0]) * data.indices.size()),
            &data.indices[0], 
            GL_STATIC_DRAW
        );

        glBindVertexArray(0);
    }
} // namespace Renderer