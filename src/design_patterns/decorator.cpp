#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <functional>

//Dynamic decorator
struct Shape
{
    virtual std::string str() const = 0;
};

struct Circle : Shape
{
    float radius{};

    Circle() {}

    Circle(const float radius) : radius(radius) {}

    void resize(float factor) {
        radius *= factor;
    }

    std::string str() const override {
        return "Circle with radius " + std::to_string(radius);
    }
};

struct ColoredShape : Shape
{
    Shape& shape;
    std::string color{};

    ColoredShape(Shape& shape, const std::string color) : shape(shape), color(color) {}

    std::string str() const override {
        return shape.str() + " has the color " + color;
    }
};

struct TransformedShape : Shape
{
    Shape& shape;
    std::string transformation;

    TransformedShape(Shape& shape, const std::string transformation) : shape(shape), transformation(transformation) {}

    std::string str() const override {
        return shape.str() + " has the transformation " + transformation;
    }
};


//Static decorator
//mixin inheritance
//perfect forwarding
template<typename T>
concept isAShape = std::is_base_of<Shape, T>::value;

template<isAShape T> struct ColoredShape2 : T
{
    std::string color{};

    ColoredShape2() {}

    template<typename ... Args>
    ColoredShape2(const std::string color, Args&&... args) : T(std::forward<Args>(args)...), color(color) {}

    std::string str() const override {
        return T::str() + " has the color " + color;
    }
};

//Functional decorator
struct Logger
{
    std::function<void()> func;

    Logger(std::function<void()> func) : func(func) {}

    void operator()() {
        std::cout << "Calling function: ";
        func();
    }
};

//Task
using namespace std;

struct Flower
{
  virtual string str() = 0;
  virtual ~Flower() = default;
};

struct Rose : Flower
{
  string str() override {
    return "A rose";
  }
};

struct RedFlower : Flower
{
  Flower& flower;

  RedFlower(Flower& flower) : flower(flower) {}

  string str() override {
    string base = flower.str();
    if (base.find("red") != string::npos) {
      return base;
    }
    if (base.find("blue") != string::npos) {
      return base + " and red";
    }
    return base + " that is red";
  }
};

struct BlueFlower : Flower
{
  Flower& flower;

  BlueFlower(Flower& flower) : flower(flower) {}

  string str() override {
    string base = flower.str();
    if (base.find("blue") != string::npos) {
      return base;
    }
    if (base.find("red") != string::npos) {
      return base + " and blue";
    }
    return base + " that is blue";
  }
};
////// end of task

int main(const int argc,const char *argv[])
{
    std::cout << "\033[92m" << "\nDecorator : 20 July 2025\n"
              << "\033[0m" << std::endl;

    //Dynamic decorator
    Circle circle(5.0f);
    ColoredShape coloredCircle(circle, "red");
    TransformedShape transformedCircle(coloredCircle, "rotated");
    //Static decorator
    ColoredShape2<Circle> green_circle{"green",7};
    //Functional decorator
    Logger logger([]() { std::cout << "Hello" << std::endl; });
    logger();

    std::cout << circle.str() << std::endl;
    std::cout << coloredCircle.str() << std::endl;
    std::cout << transformedCircle.str() << std::endl;
    std::cout << green_circle.str() << std::endl;
    ///////////////////

    return 0;
}