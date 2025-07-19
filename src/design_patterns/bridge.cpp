#include <iostream>
#include <string>
#include <memory>

//pimpl idiom

//this is in the .hpp
class Person
{
    class PersonImpl;
    PersonImpl* mImpl;

    std::string mName{}; 

public:
    Person();
    ~Person();

    void great();
    void setName(const std::string& aName);
};

//end of .hpp


//this in in the .cpp file,

class Person::PersonImpl{
public:
    void greet(Person*);
    void setName(const std::string& aName, Person*);
};

void Person::PersonImpl::greet(Person * aP)
{
    std::cout << "name: " << aP->mName << std::endl;
};

void Person::PersonImpl::setName(const std::string& aName, Person * aP)
{
    aP->mName = aName;
};

Person::Person() : mImpl{new PersonImpl} {};
Person::~Person() { delete mImpl; };

void Person::great() { mImpl->greet(this); };

void Person::setName(const std::string& aName) { mImpl->setName(aName,this); };

////end of cpp
///////////////end of pimpl


////shrink-wrapped pimpl

template <typename T> class pimpl
{
private:
    std::unique_ptr<T> mImpl{};
public:
    pimpl();
    ~pimpl();

    template <typename ... Args> pimpl(Args&& ...args) : mImpl{ std::make_unique<T>(std::forward(args)...) } {};

    T* operator->();
    T& operator*();
};

template <typename T>
pimpl<T>::pimpl() : mImpl{ std::make_unique<T>() } {};

template <typename T>
pimpl<T>::~pimpl() = default;

template <typename T>
T* pimpl<T>::operator->()
{
    std::cout << "operator-> engaged" << std::endl;
    return mImpl.get();
}

template <typename T>
T& pimpl<T>::operator*()
{
    std::cout << "operator* engaged" << std::endl;
    return *mImpl.get();
}

class Foo{
    class impl;
    pimpl<impl> mImplF;
public: 
    void fecho();

};

class Foo::impl{
public:
    void echoo() { std::cout << "impl screaming echoo" << std::endl; }
};

void Foo::fecho() { mImplF->echoo(); };

///////////////////////////// end of shrink-wrapped pimpl

//task and example
#include <string>
using namespace std;

struct Renderer {
    virtual string what_to_render_as() const = 0;
};

struct VectorRenderer : Renderer {
    string what_to_render_as() const override {
        return "lines";
    }
};

struct RasterRenderer : Renderer {
    string what_to_render_as() const override {
        return "pixels";
    }
};

struct Shape {
    string name;
    Renderer& renderer;

    Shape(string name, Renderer& renderer) : name(name), renderer(renderer) {}

    string str() const {
        return "Drawing " + name + " as " + renderer.what_to_render_as();
    }
};

struct Triangle : Shape {
    Triangle(Renderer& renderer) : Shape("Triangle", renderer) {}
};

struct Square : Shape {
    Square(Renderer& renderer) : Shape("Square", renderer) {}
};

/*
int main() {
    RasterRenderer raster;
    VectorRenderer vector;

    Triangle triangle(raster);
    Square square(vector);

    cout << triangle.str() << endl; // Output: Drawing Triangle as pixels
    cout << square.str() << endl; // Output: Drawing Square as lines

    return 0;
}
*/
///////////






int main(const int argc,const char *argv[])
{
    std::cout << "\033[92m" << "\n19 July 2025\n"
              << "\033[0m" << std::endl;
    
    //pimpl

    Person a{};
    a.setName("Chochko");

    a.great();

    Foo ff;
    ff.fecho();

    
    //bridge

    RasterRenderer raster;
    VectorRenderer vector;

    Triangle triangle(raster);
    Square square(vector);

    cout << triangle.str() << endl; // Output: Drawing Triangle as pixels
    cout << square.str() << endl; // Output: Drawing Square as lines

    return 0;
}