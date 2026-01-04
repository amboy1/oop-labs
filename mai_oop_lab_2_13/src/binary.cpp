#include "binary.h"
#include <stdexcept> 
#include <algorithm> 


Binary::Binary() : data(nullptr), size(0) {}


Binary::Binary(const unsigned char* data, size_t size) : data(nullptr), size(size) {
	for (size_t i = 0; i < size; ++i) {
		if (data[i] != 0 && data[i] != 1) {
			throw std::invalid_argument("Binary data must contain only 0 or 1.");
		}
	}
	this->data = new unsigned char[size];
	std::copy(data, data + size, this->data);
}


Binary::Binary(const std::initializer_list<unsigned char> &list) : data(nullptr), size(list.size()) {
	for (auto value : list) {
		if (value != 0 && value != 1) {
			throw std::invalid_argument("Binary data must contain only 0 or 1.");
		}
	}
	this->data = new unsigned char[size];
	std::copy(list.begin(), list.end(), this->data);
}


Binary::Binary(const Binary& other) : data(nullptr), size(other.size) {
    if (other.data) {
        this->data = new unsigned char[size];
        std::copy(other.data, other.data + size, this->data);
    }
}


Binary::Binary(Binary&& other) noexcept {
    this->data = other.data;
    this->size = other.size;

    other.data = nullptr;
    other.size = 0;
}


Binary::Binary(const std::string &t) : data(nullptr), size(t.size()) {
    for (size_t i = 0; i < size; ++i) {
        if (t[i] != '0' && t[i] != '1') {
            throw std::invalid_argument("String must contain only '0' or '1'.");
        }
    }
    this->data = new unsigned char[size];
    for (size_t i = 0; i < size; ++i) {
        this->data[i] = t[i] - '0';
    }
}


Binary::~Binary() noexcept {
    delete[] data;
    data = nullptr;
}


Binary& Binary::operator=(const Binary& other) {
    if (this == &other) {
        return *this; 
    }

    delete[] this->data;

    this->size = other.size;
    this->data = new unsigned char[other.size];
    std::copy(other.data, other.data + other.size, this->data);

    return *this; 
}


Binary& Binary::operator=(Binary&& other) noexcept {
    if (this == &other) {
        return *this; 
    }

    delete[] this->data;

    this->data = other.data;
    this->size = other.size;

    other.data = nullptr;
    other.size = 0;

    return *this; 
}


bool Binary::operator==(const Binary& other) const {
    if (this->size != other.size) {
        return false;
    }
    for (size_t i = 0; i < this->size; ++i) {
        if (this->data[i] != other.data[i]) {
            return false;
        }
    }
    return true;
}


bool Binary::operator!=(const Binary& other) const {
    return !(*this == other);
}


bool Binary::operator<(const Binary& other) const {
    size_t minSize = std::min(this->size, other.size);
    for (size_t i = 0; i < minSize; ++i) {
        if (this->data[i] < other.data[i]) {
            return true;
        } else if (this->data[i] > other.data[i]) {
            return false;
        }
    }
    return this->size < other.size;
}

bool Binary::operator>(const Binary& other) const {
    return other < *this;
}


Binary Binary::operator+(const Binary& other) const {
    size_t maxSize = std::max(this->size, other.size);
    size_t newSize = maxSize + 1; 
    unsigned char* resultData = new unsigned char[newSize];
    std::fill(resultData, resultData + newSize, 0);
    
    int carry = 0;
    for (size_t i = 0; i < maxSize; ++i) {
        int bit1 = (i < this->size) ? this->data[this->size - 1 - i] : 0;
        int bit2 = (i < other.size) ? other.data[other.size - 1 - i] : 0;
        int sum = bit1 + bit2 + carry;
        resultData[newSize - 1 - i] = sum % 2;
        carry = sum / 2;
    }
    resultData[0] = carry;
    
    size_t startIndex = 0;
    while (startIndex < newSize - 1 && resultData[startIndex] == 0) {
        startIndex++;
    }
    
    Binary result(resultData + startIndex, newSize - startIndex);
    delete[] resultData;
    return result;
}



Binary Binary::operator-(const Binary& other) const {
    if (*this < other) {
        throw std::invalid_argument("Negative result in subtraction");
    }

    size_t newSize = this->size;
    unsigned char* resultData = new unsigned char[newSize];
    std::fill(resultData, resultData + newSize, 0);
    
    int borrow = 0;
    for (size_t i = 0; i < newSize; ++i) {
        int bit1 = this->data[this->size - 1 - i];
        int bit2 = (i < other.size) ? other.data[other.size - 1 - i] : 0;
        
        int diff = bit1 - bit2 - borrow;
        if (diff < 0) {
            diff += 2;
            borrow = 1;
        } else {
            borrow = 0;
        }
        resultData[newSize - 1 - i] = diff;
    }

    size_t startIndex = 0;
    while (startIndex < newSize - 1 && resultData[startIndex] == 0) {
        startIndex++;
    }

    Binary result(resultData + startIndex, newSize - startIndex);
    delete[] resultData;
    return result;
}

