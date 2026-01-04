#ifndef BINARY_H
#endif 
#define BINARY_H

#include <stdint.h>
#include <stddef.h>
#include <initializer_list>
#include <string>
#include <cmath>

class Binary {
    private:
        unsigned char* data;
        size_t size;
    public:
        Binary();
        Binary(const unsigned char* data, size_t size);
        Binary(const std::initializer_list<unsigned char> &list);
        Binary(const Binary& other);
        Binary(Binary&& other) noexcept;
        Binary(const std::string &t);
        ~Binary() noexcept;

        Binary& operator=(const Binary& other);
        Binary& operator=(Binary&& other) noexcept;

        bool operator==(const Binary& other) const;
        bool operator!=(const Binary& other) const;
        bool operator<(const Binary& other) const;
        bool operator>(const Binary& other) const;
        Binary operator+(const Binary& other) const;
        Binary operator-(const Binary& other) const;
};
