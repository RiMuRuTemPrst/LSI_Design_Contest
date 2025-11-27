#include <iostream>
#include "myclass.h"

MyClass::MyClass(const std::string& name) : name(name) {}

void MyClass::sayHello() {
    std::cout << "Hello, I'm " << name << "!" << std::endl;
}
