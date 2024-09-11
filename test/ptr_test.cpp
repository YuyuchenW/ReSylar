#include <cstdio>
#include <vector>
#include <iostream>
#include "../ptr/cunique_ptr.hpp"

struct MyClass
{
    int a, b, c;
};

struct Animal
{
    virtual void speak() = 0;
    virtual ~Animal() = default;
};

struct Dog : Animal
{
    int age;

    Dog(int age_) : age(age_)
    {
    }

    virtual void speak()
    {
        printf("Bark! I'm %d Year Old!\n", age);
    }
};

struct Cat : Animal
{
    int &age;

    Cat(int &age_) : age(age_)
    {
    }

    virtual void speak()
    {
        printf("Meow! I'm %d Year Old!\n", age);
    }
};

int main()
{
    std::vector<CUniquePtr<Animal>> zoo;
    int age = 3;

    CUniquePtr<Animal> dog(new Dog(age));

    zoo.push_back(std::move(dog));
    zoo.push_back(CUniquePtr<Animal>(new Cat(age)));
    ++age;
    for (auto &animal : zoo)
    {
        animal->speak();
    }

    return 0;
}