#ifndef MY_MEMORY_RESOURCE_HPP
#define MY_MEMORY_RESOURCE_HPP

#include <iostream>
#include <memory_resource>
#include <vector>
#include <algorithm>
#include <stdexcept>

class CustomMemoryResource : public std::pmr::memory_resource {
    struct MemoryBlock {
        size_t offset{0};
        size_t size{0};
        MemoryBlock(size_t o, size_t s) : offset(o), size(s) {}
    };

    static constexpr size_t BUFFER_SIZE{1024};  // Увеличил для тестов
    char memory_buffer[BUFFER_SIZE];
    std::vector<MemoryBlock> used_blocks;

public:
    void* do_allocate(size_t bytes, size_t alignment) override {
        if (bytes == 0) return nullptr;
        
        std::sort(used_blocks.begin(), used_blocks.end(), 
            [](const MemoryBlock& a, const MemoryBlock& b) { 
                return a.offset < b.offset; 
            });
            
        size_t allocation_offset = 0;
        
        for (const MemoryBlock& block : used_blocks) {
            if (allocation_offset + bytes <= block.offset) {
                break;
            } else {
                allocation_offset = block.offset + block.size;
            }
        }
        
        if (allocation_offset + bytes > BUFFER_SIZE) {
            throw std::bad_alloc();
        }

        used_blocks.emplace_back(allocation_offset, bytes);
        return memory_buffer + allocation_offset;
    }

    void do_deallocate(void* ptr, size_t bytes, size_t alignment) override {
        for (size_t i = 0; i < used_blocks.size(); ++i) {
            if (ptr == memory_buffer + used_blocks[i].offset) {
                used_blocks.erase(used_blocks.begin() + i);
                return;
            }
        }
        throw std::logic_error("Попытка освобождения не выделенного блока");
    }

    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override {
        return this == &other;
    }
};

#endif