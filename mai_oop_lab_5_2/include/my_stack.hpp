#ifndef MY_STACK_HPP
#define MY_STACK_HPP

#include "my_memory_resource.hpp"
#include <memory_resource>
#include <stdexcept>
#include <utility>

template<typename T>
class MyStack {
private:
    struct Node {
        T data;
        Node* next;

        template<typename... Args>
        Node(Args&&... args) 
            : data(std::forward<Args>(args)...), next(nullptr) {}
    };

    using allocator_type = std::pmr::polymorphic_allocator<Node>;
    allocator_type allocator_;
    Node* top_;
    size_t size_;

public: 
    explicit MyStack(std::pmr::memory_resource* resource = std::pmr::get_default_resource())
        : allocator_(resource), top_(nullptr), size_(0) {}
    
    ~MyStack() {
        while (!empty()) {
            pop();
        }
    }
    
    MyStack(const MyStack& other, std::pmr::memory_resource* resource = std::pmr::get_default_resource())
        : allocator_(resource), top_(nullptr), size_(0) {
        if (!other.empty()) {
            MyStack temp;
            Node* current = other.top_;
            while (current) {
                temp.push(current->data);
                current = current->next;
            }
            while (!temp.empty()) {
                push(temp.top());
                temp.pop();
            }
        }
    }
    
    MyStack& operator=(const MyStack& other) {
        if (this != &other) {
            MyStack temp(other);
            swap(temp);
        }
        return *this;
    }
    
    void swap(MyStack& other) noexcept {
        std::swap(top_, other.top_);
        std::swap(size_, other.size_);
        std::swap(allocator_, other.allocator_);
    }
    
    template<typename U>
    void push(U&& value) {
        Node* new_node = allocator_.allocate(1);
        allocator_.construct(new_node, std::forward<U>(value));
        
        new_node->next = top_;
        top_ = new_node;
        ++size_;
    }
    
    void pop() {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        
        Node* old_top = top_;
        top_ = top_->next;
        
        allocator_.destroy(old_top);
        allocator_.deallocate(old_top, 1);
        --size_;
    }
    
    T& top() {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return top_->data;
    }
    
    const T& top() const {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return top_->data;
    }
    
    bool empty() const {
        return top_ == nullptr;
    }
    
    size_t size() const {
        return size_;
    }

    class Iterator {
    private: 
        Node* current_;
    public: 
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Iterator(Node* node = nullptr) : current_(node) {}

        Iterator& operator++() {
            if (current_) current_ = current_->next;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        T& operator*() const {
            return current_->data;
        }

        T* operator->() const {
            return &current_->data;
        }

        bool operator==(const Iterator& other) const {
            return current_ == other.current_;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
    };

    Iterator begin() { return Iterator(top_); }
    Iterator end() { return Iterator(nullptr); }
};

#endif