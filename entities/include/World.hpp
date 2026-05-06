#pragma once

#include "TriangulatedObject.hpp"

#include <cstddef>
#include <iterator>
#include <memory>
#include <vector>

namespace Renderer {


    class World {
    private:
    struct Node;

    public:
    class iterator {
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = TriangulatedObject;
        using pointer = TriangulatedObject*;
        using reference = TriangulatedObject&;
        using iterator_category = std::forward_iterator_tag;

        iterator() = default;

        reference operator*() const;
        pointer operator->() const;
        iterator& operator++();
        iterator operator++(int);
        bool operator==(const iterator& other) const;
        bool operator!=(const iterator& other) const;

    private:
        friend class World;

        iterator(Node* current, Node* head);

        Node* current_ = nullptr;
        Node* head_ = nullptr;
    };

    World() = default;
    ~World() noexcept;
    void add_object(TriangulatedObject&& object);
    bool empty() const;
    TriangulatedObject* selected_object();
    void select_next_object();
    void select_previous_object();
    
    iterator begin();
    iterator end();

    private:
        struct Node {
            explicit Node(TriangulatedObject&& object);

            TriangulatedObject object;
            Node* next = nullptr;
            Node* prev = nullptr;
        };

        Node* head_ = nullptr;
        Node* selected_ = nullptr;
        std::vector<std::unique_ptr<Node>> stored_objects_;
    };
} // namespace Renderer
