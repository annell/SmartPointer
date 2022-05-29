//
// Created by Stefan Annell on 2022-05-16.
//

#pragma once
#include <utility>

template <typename T>
class AutoPointer {
public:
    using value = AutoPointer<T>;
    AutoPointer() : Data(nullptr) {}

    AutoPointer(T* ptr) : Data(ptr) { }

    AutoPointer(const value& src) : AutoPointer(nullptr) {
        CopyImpl(this, const_cast<value*>(&src));
    }

    AutoPointer(value&& src) {
        MoveImpl(this, &src);
    }

    ~AutoPointer() {
        DeleteImpl(this);
    }

    void reset(T* data = nullptr) {
        if (Data) {
            delete Data;
        }
        Data = data;
    }

    AutoPointer& operator=(const value& src) {
        if (this != &src) {
            CopyImpl(this, const_cast<value*>(&src));
        }
        return *this;
    }

    T& operator*() const {
        return *Data;
    }

    T* operator->() const {
        return Data;
    }

    T* get() const {
        return Data;
    }

    operator bool() const {
        return Data != nullptr;
    }

    friend bool operator==(const value& lhs, const value& rhs) {
        return &lhs == &rhs;
    }

private:
    void CopyImpl(value* lhs, value* rhs) {
        lhs->reset(new T(*rhs->Data));
    }

    void MoveImpl(value* lhs, value* rhs) {
        lhs->Data = rhs->Data;
        rhs->Data = nullptr;
    }

    void DeleteImpl(value* src) {
        src->reset(nullptr);
    }

    T* Data;
};

template <typename T>
class SharedPointer {
public:
    using value = SharedPointer<T>;
    SharedPointer() : SharedPointer(nullptr) {}

    SharedPointer(T* ptr) 
    : Data(ptr)
    , previous(nullptr)
    , next(nullptr) {}

    SharedPointer(const value& src) : SharedPointer(nullptr) {
        CopyImpl(this, const_cast<value*>(&src));
    }

    SharedPointer(value&& src) {
        MoveImpl(this, &src);
    }

    ~SharedPointer() {
        DeleteImpl(this);
    }

    void reset(T* data = nullptr) {
        if (Data) {
            delete Data;
        }
        Data = data;
    }

    SharedPointer& operator=(const value& src) {
        if (this != &src) {
            CopyImpl(this, const_cast<value*>(&src));
        }
        return *this;
    }

    friend bool operator==(const value& lhs, const value& rhs) {
        return &lhs == &rhs;
    }

    T& operator*() const {
        return *Data;
    }

    T* operator->() const {
        return Data;
    }

    T* get() const {
        return Data;
    }

    operator bool() const {
        return Data != nullptr;
    }

    int references() {
        auto countRefs = [] (value* ptrStart) {
            int count = 1;
            auto ptr = ptrStart->next;
            while (ptr) {
                count++;
                ptr = ptr->next;
            }

            ptr = ptrStart->previous;
            while (ptr) {
                count++;
                ptr = ptr->previous;
            }

            return count;
        };
        return countRefs(this);
    }

    private:
    void CopyImpl(value* lhs, value* rhs) {
        lhs->reset(rhs->Data);
        rhs->next = lhs;
        lhs->previous = rhs;
    }

    void MoveImpl(value* lhs, value* rhs) {
        lhs->Data = rhs->Data;
        rhs->Data = nullptr;
        lhs->previous = rhs->previous;
        rhs->previous = nullptr;
        lhs->next = rhs->next;
        rhs->next = nullptr;
    }

    void DeleteImpl(value* src) {
        if (references() == 1) {
            src->reset();
        }
        if (previous) {
            previous->next = next;
        }
        if (next) {
            next->previous = previous;
        }
    }

    T* Data;
    value* previous;
    value* next;
};

template <typename T>
class UniquePointer {
public:
    using value = UniquePointer<T>;
    UniquePointer() : Data(nullptr) {}

    UniquePointer(T* ptr) : Data(ptr) { }

    UniquePointer(const value& src) = delete;

    UniquePointer(value&& src) {
        MoveImpl(this, &src);
    }

    ~UniquePointer() {
        DeleteImpl(this);
    }

    void reset(T* data = nullptr) {
        if (Data) {
            delete Data;
        }
        Data = data;
    }

    value& operator=(const value& src) = delete;

    friend bool operator==(const value& lhs, const value& rhs) {
        return &lhs == &rhs;
    }

    T& operator*() const {
        return *Data;
    }

    T* operator->() const {
        return Data;
    }

    T* get() const {
        return Data;
    }

    operator bool() const {
        return Data != nullptr;
    }

private:
    void DeleteImpl(value* src) {
        src->reset(nullptr);
    }

    void MoveImpl(value* lhs, value* rhs) {
        lhs->Data = rhs->Data;
        rhs->Data = nullptr;
    }

    T* Data;
};


namespace smartPtr {

namespace {

template <template <typename> class P, typename T, typename... Ts>
P<T> MakeImpl(Ts... args) {
    return std::move(P<T>(new T(std::forward<Ts>(args) ...)));
}

}

template<typename T>
using Auto = AutoPointer<T>;

template<typename T, typename... Ts>
Auto<T> MakeAuto(Ts... args) {
    return std::move(MakeImpl<Auto, T, Ts...>(args ...));
}

template<typename T>
using Shared = SharedPointer<T>;

template<typename T, typename... Ts>
Shared<T> MakeShared(Ts... args) {
    return std::move(MakeImpl<Shared, T, Ts...>(args ...));
}

template<typename T>
using Unique = UniquePointer<T>;

template<typename T, typename... Ts>
Unique<T> MakeUnique(Ts... args) {
    return std::move(MakeImpl<Unique, T, Ts...>(args ...));
}

}