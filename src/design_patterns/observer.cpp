#include <iostream>
#include <vector>
#include <algorithm>

////////////////////////////////////////////////////////////////////////////////
// Observer Coding Exercise
// Rats attack as a swarm: each rat's attack equals the total number of rats.
// When a rat enters (constructor) or dies (destructor), all rats are notified.
////////////////////////////////////////////////////////////////////////////////

struct IRat
{
    virtual void set_attack(int attack) = 0;
    virtual ~IRat() = default;
};

struct Game
{
    std::vector<IRat*> rats;

    void notify_rats()
    {
        int swarm_size = static_cast<int>(rats.size());
        for (auto* rat : rats)
        {
            rat->set_attack(swarm_size);
        }
    }
};

struct Rat : IRat
{
    Game& game;
    int attack{1};

    Rat(Game &game) : game(game)
    {
        game.rats.push_back(this);
        game.notify_rats();
    }

    ~Rat()
    {
        auto it = std::find(game.rats.begin(), game.rats.end(), this);
        if (it != game.rats.end())
        {
            game.rats.erase(it);
        }
        game.notify_rats();
    }

    void set_attack(int attack) override
    {
        this->attack = attack;
    }
};

////////////////////////////////////////////////////////////////////////////////
// Observer/Observable Example
// Classic push-based observer pattern with explicit subscribe/unsubscribe.
// Observable (Subject) pushes updates to all subscribed Observers.
////////////////////////////////////////////////////////////////////////////////

//Forward declarations
struct Observable;

struct Observer
{
    virtual void update(Observable& subject) = 0;
    virtual ~Observer() = default;
};

struct Observable
{
    std::vector<Observer*> observers;

    void subscribe(Observer* observer)
    {
        observers.push_back(observer);
    }

    void unsubscribe(Observer* observer)
    {
        auto it = std::find(observers.begin(), observers.end(), observer);
        if (it != observers.end())
        {
            observers.erase(it);
        }
    }

    void notify()
    {
        for (auto* observer : observers)
        {
            observer->update(*this);
        }
    }
};

//Concrete subject: Temperature sensor
struct TemperatureSensor : Observable
{
    float temperature{0.0f};

    void set_temperature(float temp)
    {
        temperature = temp;
        notify();  //Push notification to all observers
    }

    float get_temperature() const { return temperature; }
};

//Concrete observer: Display shows current temperature
struct TemperatureDisplay : Observer
{
    std::string name;
    float last_temperature{0.0f};

    explicit TemperatureDisplay(std::string name) : name(std::move(name)) {}

    void update(Observable& subject) override
    {
        auto& sensor = static_cast<TemperatureSensor&>(subject);
        last_temperature = sensor.get_temperature();
        std::cout << "  [Display " << name << "] Temperature updated: "
                  << last_temperature << " C" << std::endl;
    }
};

//Concrete observer: Logger records all changes
struct TemperatureLogger : Observer
{
    int change_count{0};

    void update(Observable& subject) override
    {
        auto& sensor = static_cast<TemperatureSensor&>(subject);
        ++change_count;
        std::cout << "  [Logger] Change #" << change_count
                  << ": " << sensor.get_temperature() << " C" << std::endl;
    }
};

//Concrete observer: Alert triggers on threshold
struct TemperatureAlert : Observer
{
    float threshold;

    explicit TemperatureAlert(float threshold) : threshold(threshold) {}

    void update(Observable& subject) override
    {
        auto& sensor = static_cast<TemperatureSensor&>(subject);
        if (sensor.get_temperature() > threshold)
        {
            std::cout << "  [ALERT] Temperature " << sensor.get_temperature()
                      << " C exceeds threshold " << threshold << " C!" << std::endl;
        }
    }
};


int main(const int argc,const char *argv[])
{
    std::cout << "\033[92m" << "\nObserver | 7 May 2026\n"
              << "\033[0m" << std::endl;

    Game game;

    //Create first rat
    Rat* rat1 = new Rat(game);
    std::cout << "After 1st rat: attack = " << rat1->attack << " (expected: 1)" << std::endl;

    //Create second rat
    Rat* rat2 = new Rat(game);
    std::cout << "After 2nd rat: rat1 attack = " << rat1->attack
              << ", rat2 attack = " << rat2->attack << " (expected: 2, 2)" << std::endl;

    //Create third rat
    Rat* rat3 = new Rat(game);
    std::cout << "After 3rd rat: all attacks = " << rat1->attack << ", "
              << rat2->attack << ", " << rat3->attack
              << " (expected: 3, 3, 3)" << std::endl;

    //Kill second rat
    delete rat2;
    std::cout << "After 2nd rat dies: attacks = " << rat1->attack << ", "
              << rat3->attack << " (expected: 2, 2)" << std::endl;

    //Kill first rat
    delete rat1;
    std::cout << "After 1st rat dies: attack = " << rat3->attack
              << " (expected: 1)" << std::endl;

    //Kill last rat
    delete rat3;
    std::cout << "All rats dead, game has " << game.rats.size() << " rats" << std::endl;

    
    
    std::cout << "\n\033[92m" << "--- Observer/Observable Example ---" << "\033[0m" << std::endl;

    TemperatureSensor sensor;
    TemperatureDisplay display1("Living Room");
    TemperatureDisplay display2("Bedroom");
    TemperatureLogger logger;
    TemperatureAlert alert(30.0f);  //Alert above 30C

    //Subscribe observers
    sensor.subscribe(&display1);
    sensor.subscribe(&display2);
    sensor.subscribe(&logger);
    sensor.subscribe(&alert);

    std::cout << "\nSetting temperature to 22.5:" << std::endl;
    sensor.set_temperature(22.5f);

    std::cout << "\nSetting temperature to 25.0:" << std::endl;
    sensor.set_temperature(25.0f);

    std::cout << "\nUnsubscribing Bedroom display..." << std::endl;
    sensor.unsubscribe(&display2);

    std::cout << "\nSetting temperature to 32.0 (triggers alert):" << std::endl;
    sensor.set_temperature(32.0f);

    std::cout << "\nTotal changes logged: " << logger.change_count << std::endl;

    return 0;
}