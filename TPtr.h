#pragma once

// Generic deleter for non-pointer types (e.g., int, Foo)
template<typename T, typename Array>
class Deleter {
public:
    void deleter(Array* arr) {
        arr->DeletePtr();  // Use delete[] uValue
    }
};

// Specialized deleter for pointer types (e.g., Foo*)
template<typename T, typename Array>
class Deleter<T*, Array> {
public:
    void deleter(Array* arr) {
        arr->DeleteArray();  // Loop through and delete each element, then delete[] uValue
    }
};

// Smart pointer and smart array class
template<typename T>
class TPtr {
private:
    // Internal control block for reference counting
    struct ControlBlock {
        int refCount;
        ControlBlock() : refCount(1) {}
    };

public:
    // Lightweight exception for out-of-range access
    class TPtrOutOfRangeException {
    public:
        const char* what() const {
            return "TPtr: Index out of range";
        }
    };

    typedef TPtr<T> selType;  // Used by the Deleter

    // Default constructor
    TPtr() : uValue(nullptr), uSize(0), control(nullptr) {}

    // Constructor for array or single object
    explicit TPtr(int size) {
        if (size > 0) {
            uValue = new T[size];
            uSize = size;
        } else {
            uValue = new T;
            uSize = 0;
        }
        control = new ControlBlock();
    }

    // Copy constructor with reference counting
    TPtr(const TPtr& other) {
        uValue = other.uValue;
        uSize = other.uSize;
        control = other.control;
        if (control) control->refCount++;
    }

    // Assignment operator with reference counting
    TPtr& operator=(const TPtr& other) {
        if (this != &other) {
            release();
            uValue = other.uValue;
            uSize = other.uSize;
            control = other.control;
            if (control) control->refCount++;
        }
        return *this;
    }

    // Destructor releases resources if last reference
    ~TPtr() {
        release();
    }

    // Allocate a new array or object
    void New(int size = 0) {
        release();
        if (size > 0) {
            uValue = new T[size];
            uSize = size;
        } else {
            uValue = new T;
            uSize = 0;
        }
        control = new ControlBlock();
    }

    // Wrap a raw pointer (object)
    void New(T* ptr) {
        release();
        uValue = ptr;
        uSize = 0;
        control = new ControlBlock();
    }

    // Wrap a raw pointer (array or array of pointers)
    void New(T* ptr, int size) {
        release();
        uValue = ptr;
        uSize = size;
        control = new ControlBlock();
    }

    // Resize the array (copy content)
    void Resize(int newSize) {
        if (newSize <= 0) return;

        T* newArray = new T[newSize];
        int minSize = (newSize < uSize) ? newSize : uSize;

        for (int i = 0; i < minSize; ++i)
            newArray[i] = uValue[i];

        release();
        uValue = newArray;
        uSize = newSize;
        control = new ControlBlock();
    }

    // Remove element at index and shift others
    void RemoveAt(int pos) {
        if (!uValue || pos < 0 || pos >= uSize)
            throw TPtrOutOfRangeException();

        for (int i = pos; i < uSize - 1; ++i)
            uValue[i] = uValue[i + 1];

        uSize--;
    }

    // Subscript operator (mutable)
    T& operator[](int pos) {
        if (!uValue || pos < 0 || pos >= uSize)
            throw TPtrOutOfRangeException();
        return uValue[pos];
    }

    // Subscript operator (const)
    const T& operator[](int pos) const {
        if (!uValue || pos < 0 || pos >= uSize)
            throw TPtrOutOfRangeException();
        return uValue[pos];
    }

    // Dereference and pointer access
    T& operator*() { return *uValue; }
    T* operator->() { return uValue; }

    // Access underlying pointer
    T* Get() { return uValue; }
    T*& Ref() { return uValue; }

    // Returns array/object size
    int Size() const { return uSize; }

    // Increases ref count and returns raw pointer
    T* Share() {
        if (control) control->refCount++;
        return uValue;
    }

    // Delete with delete[]
    void DeletePtr() {
        delete[] uValue;
        uValue = nullptr;
        uSize = 0;
    }

    // Delete each element and then delete[]
    void DeleteArray() {
        for (int i = 0; i < uSize; ++i)
            delete uValue[i];
        delete[] uValue;
        uValue = nullptr;
        uSize = 0;
    }

    // Delete based on type using Deleter
    void Delete() {
        if (!uValue) return;

        if (uSize > 0) {
            Deleter<T, selType> deleter;
            deleter.deleter(this);
        } else {
            delete uValue;
        }

        uValue = nullptr;
        uSize = 0;
    }

private:
    // Release ownership and delete if last reference
    void release() {
        if (control) {
            control->refCount--;
            if (control->refCount == 0) {
                Delete();
                delete control;
            }
            control = nullptr;
        } else {
            Delete();
        }
    }

private:
    T* uValue;               // Raw pointer to object or array
    int uSize;               // Size of array, or 0 if single object
    ControlBlock* control;   // Reference count control block
};
