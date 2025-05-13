# TPtr – Lightweight Smart Pointer and Smart Array for C++ (No STL)

`TPtr` is a lightweight and safe smart pointer implementation for C++ that automatically manages memory allocation and deallocation for both single objects and arrays — all without using the STL. It supports:

- Single objects (`new T`)
- Arrays (`new T[size]`)
- Arrays of pointers (`new T*[size]` with elements allocated via `new T`)
- Reference counting for safe pointer sharing
- Automatic deletion based on the type of object
- Compatible with embedded systems, SDKs, game engines, or legacy environments

## 🚀 Features

- ✅ Replaces raw `new` / `delete` with automatic memory management
- ✅ Manual reference counting (like `shared_ptr`)
- ✅ Type-safe deletion for objects, arrays, and pointer arrays
- ✅ Zero STL usage – ideal for environments with restricted C++ features
- ✅ Lightweight custom exception system (`TPtrOutOfRangeException`)
- ✅ Header-only design (`TPtr.h`) – easy to include and use

## 📦 Usage Examples

### Single Object

```cpp
TPtr<MyClass> obj;
obj.New(); // Allocates with new MyClass
obj->doSomething();
