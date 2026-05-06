#include "World.hpp"

namespace Renderer{
    World::Node::Node(TriangulatedObject&& object)
        : object(std::move(object)) {}

    World::iterator::iterator(Node* current, Node* head)
        : current_(current), head_(head) {}

    World::iterator::reference World::iterator::operator*() const {
        return current_->object;
    }

    World::iterator::pointer World::iterator::operator->() const {
        return &current_->object;
    }

    World::iterator& World::iterator::operator++() {
        if (current_ == nullptr) {
            return *this;
        }

        current_ = current_->next == head_ ? nullptr : current_->next;
        return *this;
    }

    World::iterator World::iterator::operator++(int) {
        iterator copy = *this;
        ++(*this);
        return copy;
    }

    bool World::iterator::operator==(const iterator& other) const {
        return current_ == other.current_ && head_ == other.head_;
    }

    bool World::iterator::operator!=(const iterator& other) const {
        return !(*this == other);
    }

    void World::add_object(TriangulatedObject&& object) {
        auto node = std::make_unique<Node>(std::move(object));
        Node* inserted = node.get();

        if (head_ == nullptr) {
            inserted->next = inserted;
            inserted->prev = inserted;
            head_ = inserted;
            selected_ = inserted;
        } else {
            Node* tail = head_->prev;
            inserted->next = head_;
            inserted->prev = tail;
            tail->next = inserted;
            head_->prev = inserted;
        }

        stored_objects_.emplace_back(std::move(node));
    }

    bool World::empty() const {
        return head_ == nullptr;
    }

    TriangulatedObject* World::selected_object() {
        return selected_ == nullptr ? nullptr : &selected_->object;
    }

    void World::select_next_object() {
        if (selected_ != nullptr) {
            selected_ = selected_->next;
        }
    }

    void World::select_previous_object() {
        if (selected_ != nullptr) {
            selected_ = selected_->prev;
        }
    }

    World::iterator World::begin() {
        return iterator(head_, head_);
    }

    World::iterator World::end() {
        return iterator(nullptr, head_);
    }

    World::~World() noexcept = default;
}
