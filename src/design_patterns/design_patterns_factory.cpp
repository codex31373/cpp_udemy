#pragma once

#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <memory>
#include <map>
#include <functional>

//------------ factory ---------

class Point {
    Point(const double& aX, const double& aY) : mX(aX), mY(aY) {};

    double mX;
    double mY;

public:
    void printCoordinates() {
        std::cout << "x: " << mX << " y: " << mY << std::endl;
    }

    //factory methods
    static Point newCartesian(const float& aX, const float& aY){
        return {aX, aY};
    }

    static Point newPolar(const double& aR, const double& aTheta){
        return {aR * cos(aTheta), aR * sin(aTheta)};
    }

    //can be moved here Pointfactory to be Inner factory method, it will have access to private member in that way
};

//concrete factory
struct PointFactory{
    static Point createCartesian(const double& aX, const double aY) {
        return Point::newCartesian(aX, aY); //ocp is preserved, constructor remains private
    }

    static Point createPolar(const double& aR, const double aTheta) {
        return Point::newPolar(aR, aTheta); //ocp is preserved, constructor remains private
    }
};

//-----------------------------------------------
//Abstract factories

class HotDrink{
public:
    virtual ~HotDrink() = default;
    virtual void prepare(const int& aVolume) = 0;
};

class Tea : public HotDrink{
public:
    void prepare(const int& aVolume) override{
        std::cout << "preparing tea with water: " << aVolume << " ml" << std::endl;
    }
};

class Coffee : public HotDrink{
public:
    void prepare(const int& aVolume) override{
        std::cout << "preparing coffee with water: " << aVolume << " ml" << std::endl;
    }
};


class HotDrinkFactory{ //this is the abstract factory
public:
    virtual std::unique_ptr<HotDrink> make() const = 0;
};

class TeaFactory : public HotDrinkFactory{
public:
    std::unique_ptr<HotDrink> make() const override{
        return std::make_unique<Tea>();
    }
};

class CoffeeFactory : public HotDrinkFactory{
public:
    std::unique_ptr<HotDrink> make() const override{
        return std::make_unique<Coffee>();
    }
};

class DrinkFactory{
    std::map<std::string, std::unique_ptr<HotDrinkFactory>> mHotDrinkFactories{};
public:
    DrinkFactory()
    {
        mHotDrinkFactories["tea"] = std::make_unique<TeaFactory>();
        mHotDrinkFactories["coffee"] = std::make_unique<CoffeeFactory>();
    }

    std::unique_ptr<HotDrink> prepareDrink(const std::string& aName)
    {
        auto drink = mHotDrinkFactories[aName]->make();
        drink->prepare(200);
        return drink;
    }
};

class DrinkWithVolumeFactory{
    std::map<std::string, std::function<std::unique_ptr<HotDrink>()>> mFactories{};
public:
    DrinkWithVolumeFactory()
    {
        mFactories["tea"] = [] {
            auto tea = std::make_unique<Tea>(); 
            tea->prepare(150); 
            return tea;
        };

        mFactories["coffee"] = [] {
            auto coffee = std::make_unique<Coffee>(); 
            coffee->prepare(50); 
            return coffee;
        };
    }

    std::unique_ptr<HotDrink> prepareDrinkEasy(const std::string& aName)
    {
        return mFactories[aName]();
    }
};


//-----------------------------------------------

/* -- test task --
#include <string>
using namespace std;

struct Person {
  int id;
  string name;
};

class PersonFactory {
private:
  int idCounter = 0; // initialize id counter to 0

public:
  Person create_person(const string& name) {
    Person person;
    person.id = idCounter; // set id to current counter value
    person.name = name; // set name to input parameter
    idCounter++; // increment counter for next person
    return person;
  }
};
*/

int main(const int argc,const char *argv[])
{
    auto p = Point::newPolar(3, M_PI_4);
    p.printCoordinates();

    auto p2 = PointFactory::createPolar(3, M_PI_4);
    p2.printCoordinates();

    DrinkFactory df{};

    auto d = df.prepareDrink("tea");

    DrinkWithVolumeFactory dvf{};

    auto d2 = dvf.prepareDrinkEasy("coffee");

    std::cout << "6 July 2025\n";
    return 0;
}