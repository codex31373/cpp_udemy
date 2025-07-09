#include <iostream>
#include <functional>
using namespace std;

// task to implement
struct SingletonTester
{
  template <typename T>
  static bool is_singleton(function<T*()> factory)
  {
        T* instance1 = factory();
        T* instance2 = factory();
        return instance1 == instance2;
  }
};
//----------------------


class Logger {
public:
    virtual void log(const std::string& message) = 0;
};

class ConsoleLogger : public Logger {
public:
    void log(const std::string& message) override {
        std::cout << message << std::endl;
    }
};

class Singleton {
public:
    static Singleton& getInstance() {
        static Singleton instance;  //"Meyers' Singleton" pattern, which is a modern and thread-safe way
        return instance;
    }

    void setLogger(Logger* logger) {
        this->logger = logger;
    }

    void doSomething() {
        logger->log("Doing something...");
    }

private:
    Logger* logger = nullptr;

    Singleton() {} // Private constructor to prevent instantiation

    // Delete copy constructor and assignment operator to prevent copying
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};


int main(const int argc,const char *argv[])
{
    Singleton& singleton = Singleton::getInstance();
    Singleton& singleton1 = Singleton::getInstance();

    std::cout << &singleton << " = " << &singleton1 << std::endl;

    ConsoleLogger consoleLogger;
    singleton.setLogger(&consoleLogger); //dependency injection with type of loger, can be defined another one
    singleton.doSomething();

    std::cout << "\033[92m" << "\nSingleton : 9 July 2025\n"
              << "\033[0m" << std::endl;

    return 0;
}