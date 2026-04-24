#include <iostream>
#include <string>

class Person
{
    friend class ResponsiblePerson;
    int age;
public:
    Person(int age) : age(age) {}

    int get_age() const { return age; }
    void set_age(int age) { this->age = age; }

    std::string drink() const { return "drinking"; }
    std::string drive() const { return "driving"; }
    std::string drink_and_drive() const { return "driving while drunk"; }
};

class ResponsiblePerson
{
public:
    ResponsiblePerson(const Person &person) : person(person) {}

    int get_age() const { return person.get_age(); }
    void set_age(int age) { person.set_age(age); }

    std::string drink() const
    {
        if (person.get_age() < 18) return "too young";
        return person.drink();
    }

    std::string drive() const
    {
        if (person.get_age() < 16) return "too young";
        return person.drive();
    }

    std::string drink_and_drive() const
    {
        return "dead";
    }

private:
    Person person;
};

int main(const int argc,const char *argv[])
{
    std::cout << "\033[92m" << "\nProxy : 24 April 2026\n"
              << "\033[0m" << std::endl;

    Person p1(10);
    ResponsiblePerson rp1(p1);
    std::cout << "Age 10 - drink: " << rp1.drink() << std::endl;
    std::cout << "Age 10 - drive: " << rp1.drive() << std::endl;
    std::cout << "Age 10 - drink_and_drive: " << rp1.drink_and_drive() << std::endl;

    Person p2(17);
    ResponsiblePerson rp2(p2);
    std::cout << "\nAge 17 - drink: " << rp2.drink() << std::endl;
    std::cout << "Age 17 - drive: " << rp2.drive() << std::endl;

    Person p3(20);
    ResponsiblePerson rp3(p3);
    std::cout << "\nAge 20 - drink: " << rp3.drink() << std::endl;
    std::cout << "Age 20 - drive: " << rp3.drive() << std::endl;

    return 0;
}