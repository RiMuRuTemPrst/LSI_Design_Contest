#ifndef MYCLASS_H
#define MYCLASS_H

#include <string>

class MyClass {
public:
    MyClass(const std::string& name);
    void sayHello();
private:
    std::string name;
};

#endif
