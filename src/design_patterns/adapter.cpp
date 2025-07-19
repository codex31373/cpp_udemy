#include <iostream>

// Target interface
class Duck {
public:
    virtual ~Duck() = default;
    virtual void quack() = 0;
    virtual void fly() = 0;
};

// Adaptee interface
class Turkey {
public:
    virtual ~Turkey() = default;
    virtual void gobble() = 0;
    virtual void fly() = 0;
};

// Concrete implementation of a Duck
class MallardDuck : public Duck {
public:
    void quack() override {
        std::cout << "Quack" << std::endl;
    }

    void fly() override {
        std::cout << "I'm flying" << std::endl;
    }
};

// Concrete implementation of a Turkey
class WildTurkey : public Turkey {
public:
    void gobble() override {
        std::cout << "Gobble gobble" << std::endl;
    }

    void fly() override {
        std::cout << "I'm flying a short distance" << std::endl;
    }
};

// Adapter that adapts a Turkey to a Duck
class TurkeyAdapter : public Duck {
private:
    Turkey* turkey;

public:
    TurkeyAdapter(Turkey* turkey) : turkey(turkey) {}

    void quack() override {
        turkey->gobble();
    }

    void fly() override {
        for (int i = 0; i < 5; i++) {
            turkey->fly();
        }
    }
};

// task
/*
struct SquareToRectangleAdapter : Rectangle
{
  SquareToRectangleAdapter(const Square& square)
    : square_(square)
  {
  }

  int width() const override
  {
    return square_.side;
  }

  int height() const override
  {
    return square_.side;
  }

private:
  const Square& square_;
};
*/

int main(const int argc,const char *argv[])
{
    MallardDuck* mallardDuck = new MallardDuck();
    WildTurkey* wildTurkey = new WildTurkey();
    TurkeyAdapter* turkeyAdapter = new TurkeyAdapter(wildTurkey);

    std::cout << "The Turkey says..." << std::endl;
    wildTurkey->gobble();
    wildTurkey->fly();

    std::cout << "\nThe Duck says..." << std::endl;
    mallardDuck->quack();
    mallardDuck->fly();

    std::cout << "\nThe TurkeyAdapter says..." << std::endl;
    turkeyAdapter->quack();
    turkeyAdapter->fly();

    delete mallardDuck;
    delete wildTurkey;
    delete turkeyAdapter;

    std::cout << "\033[92m" << "\nAdapter : 9 July 2025\n"
              << "\033[0m" << std::endl;

    return 0;
}