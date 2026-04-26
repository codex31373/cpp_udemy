#include <iostream>
#include <string>
#include <memory>

////////////////////////////////////////////////////////////////////////////////
// Protection Proxy: Controls access to an object based on permissions/rules.
// The ResponsiblePerson proxy adds age-based restrictions to Person's actions.
// - drink: allowed only if age >= 18
// - drive: allowed only if age >= 16
// - drink_and_drive: always forbidden (returns "dead")
// The proxy has the same interface as Person but adds protective logic.
////////////////////////////////////////////////////////////////////////////////

//task
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
////////////////////////////////////////////////////////////////////////////////
// Virtual Proxy: Lazy initialization of expensive objects.
// The ImageProxy delays loading the actual image until it's first displayed.
// Useful when object creation is costly and the object might not be used.
////////////////////////////////////////////////////////////////////////////////

struct Image
{
    std::string filename;
    bool loaded = false;

    Image(const std::string& filename) : filename(filename)
    {
        std::cout << "  Loading image: " << filename << " (expensive operation)" << std::endl;
        loaded = true;
    }

    void display() const
    {
        std::cout << "  Displaying: " << filename << std::endl;
    }
};

struct ImageProxy
{
    std::string filename;
    std::unique_ptr<Image> image;

    ImageProxy(const std::string& filename) : filename(filename) {}

    void display()
    {
        if (!image) {
            std::cout << "Lazy loading triggered!" << std::endl;
            image = std::make_unique<Image>(filename);
        }
        image->display();
    }
};

////////////////////////////////////////////////////////////////////////////////
// Smart Reference Proxy: Adds additional behavior when accessing an object.
// This example tracks access count and logs each access to the resource.
////////////////////////////////////////////////////////////////////////////////

struct Resource
{
    std::string data;
    Resource(const std::string& data) : data(data) {}

    void access() const
    {
        std::cout << "  Resource content: " << data << std::endl;
    }
};

struct SmartReferenceProxy
{
    std::shared_ptr<Resource> resource;
    mutable int access_count = 0;

    SmartReferenceProxy(const std::shared_ptr<Resource>& resource)
        : resource(resource) {}

    void access() const
    {
        access_count++;
        std::cout << "  [Access #" << access_count << "] ";
        resource->access();
    }

    int get_access_count() const { return access_count; }
};

////////////////////////////////////////////////////////////////////////////////

int main(const int argc,const char *argv[])
{
    std::cout << "\033[92m" << "\nProxy : 26 April 2026\n"
              << "\033[0m" << std::endl;

    std::cout << "--- Protection Proxy ---" << std::endl;
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

    std::cout << "\n--- Virtual Proxy ---" << std::endl;
    ImageProxy proxy("photo.png");
    std::cout << "Proxy created, image not loaded yet." << std::endl;
    proxy.display(); // First call triggers loading
    proxy.display(); // Second call uses cached image

    std::cout << "\n--- Smart Reference Proxy ---" << std::endl;
    auto resource = std::make_shared<Resource>("Secret Data");
    SmartReferenceProxy smart_proxy(resource);
    smart_proxy.access();
    smart_proxy.access();
    smart_proxy.access();
    std::cout << "Total accesses: " << smart_proxy.get_access_count() << std::endl;

    return 0;
}