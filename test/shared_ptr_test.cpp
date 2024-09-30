#include "../ptr/shared_ptr.hpp"
#include <iostream>

struct Student
{
    const char *name;
    int age;

    explicit Student(const char *name_, int age_) : name(name_), age(age_)
    {
        std::cout << "Student 构造\n";
    }

    Student(Student &&) = delete;

    void func()
    {
        std::cout << "name is " << name << '\n';
    }

    ~Student()
    {
        std::cout << "Student 析构\n";
    }
};

int main()
{
    shared_ptr<Student> p1(new Student("zhangsan", 18));
    p1->func();
    {
        shared_ptr<Student> p2(p1);
        p2->func();
    }
    shared_ptr<Student> p3 = p1;
    p3->func();

    shared_ptr<Student> p4 = make_shared<Student>("cxs",55);
    p4.get()->func();
}