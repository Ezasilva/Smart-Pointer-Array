#include <iostream>
#include "TPtr.h"

class MyClass {
public:
    MyClass() { std::cout << "MyClass constructed\n"; }
    ~MyClass() { std::cout << "MyClass destroyed\n"; }
    void say() { std::cout << "Hello from MyClass!\n"; }
};

int main() {
    std::cout << "Single object:\n";
    TPtr<MyClass> obj;
    obj.New();
    obj->say();

    std::cout << "\nArray of objects:\n";
    TPtr<int> array(5);
    for (int i = 0; i < array.Size(); ++i)
        array[i] = i * 10;

    for (int i = 0; i < array.Size(); ++i)
        std::cout << "array[" << i << "] = " << array[i] << "\n";

    std::cout << "\nArray of pointers:\n";
    TPtr<MyClass*> ptrArray(3);
    for (int i = 0; i < ptrArray.Size(); ++i)
        ptrArray[i] = new MyClass();

    std::cout << "\nShared pointer behavior:\n";
    TPtr<int> shared(4);
    shared[0] = 99;
    TPtr<int> another = shared;
    std::cout << "Shared value: " << another[0] << "\n";

    std::cout << "\nEnd of program.\n";
    return 0;
}
