#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <map>
#include <any>

////////////////////////////////////////////////////////////////////////////////
// Event Broker Mediator Example
// Publish-subscribe pattern using an event broker as mediator.
// Components don't know about each other - they only communicate through events.
////////////////////////////////////////////////////////////////////////////////


struct Event
{
    std::string name;
    std::any data;

    Event() = default;
    Event(const std::string& name) : name(name) {}
    Event(const std::string& name, const std::any& data) : name(name), data(data) {}
};

//Event Broker (Mediator) - central hub for event communication
struct EventBroker
{
    using Handler = std::function<void(const Event&)>;
    std::multimap<std::string, Handler> handlers;

    void subscribe(const std::string& event_name, Handler handler)
    {
        handlers.insert({event_name, handler});
    }

    void publish(const Event& event)
    {
        auto range = handlers.equal_range(event.name);
        for (auto it = range.first; it != range.second; ++it)
        {
            it->second(event);
        }
    }
};

//Player component - subscribes to events and publishes health changes
struct Player
{
    std::string name;
    int health{100};
    EventBroker& broker;

    Player(const std::string& name, EventBroker& broker) : name(name), broker(broker)
    {
        broker.subscribe("heal", [this, name](const Event& e) {
            int amount = std::any_cast<int>(e.data);
            health += amount;
            std::cout << "  " << name << " healed for " << amount
                      << " (health: " << health << ")" << std::endl;
        });

        broker.subscribe("damage_report", [name](const Event& e) {
            std::string target = std::any_cast<std::string>(e.data);
            if (target != name)
            {
                std::cout << "  " << name << " noticed " << target << " took damage" << std::endl;
            }
        });
    }

    void take_damage(int amount)
    {
        health -= amount;
        std::cout << "  " << name << " took " << amount << " damage (health: " << health << ")" << std::endl;
        broker.publish(Event("damage_report", name));
    }
};

//Coach component - subscribes to player events and gives commands
struct Coach
{
    std::string name;
    EventBroker& broker;

    Coach(const std::string& name, EventBroker& broker) : name(name), broker(broker)
    {
        broker.subscribe("damage_report", [name, &broker](const Event& e) {
            std::string target = std::any_cast<std::string>(e.data);
            std::cout << "  Coach " << name << ": " << target << " is hurt, sending medkit!" << std::endl;
            broker.publish(Event("heal", 25));
        });
    }
};

////////////////////////////////////////////////////////////////////////////////
// Mediator Coding Exercise
// Participants communicate through a central Mediator.
// When a participant says() a value, all OTHER participants increase their value.
////////////////////////////////////////////////////////////////////////////////

struct IParticipant
{
    virtual void say(int value) = 0;
    virtual void receive(int value) = 0;
    virtual ~IParticipant() = default;
};

struct Mediator
{
    std::vector<IParticipant*> participants;
};

struct Participant : IParticipant
{
    int value{0};
    Mediator& mediator;

    Participant(Mediator &mediator) : mediator(mediator)
    {
        mediator.participants.push_back(this);
    }

    void say(int value) override
    {
        for (auto* p : mediator.participants)
        {
            if (p != this)
            {
                p->receive(value);
            }
        }
    }

    void receive(int value) override
    {
        this->value += value;
    }
};

int main(const int argc,const char *argv[])
{
    std::cout << "\033[92m" << "\nMediator | 6 May 2026\n"
              << "\033[0m" << std::endl;

    Mediator mediator;
    Participant p1(mediator);
    Participant p2(mediator);

    std::cout << "Initial: P1=" << p1.value << ", P2=" << p2.value << std::endl;

    p1.say(3);
    std::cout << "After P1 says 3: P1=" << p1.value << ", P2=" << p2.value
              << " (expected: P1=0, P2=3)" << std::endl;

    p2.say(2);
    std::cout << "After P2 says 2: P1=" << p1.value << ", P2=" << p2.value
              << " (expected: P1=2, P2=3)" << std::endl;

    //Three participants
    Participant p3(mediator);
    p1.say(5);
    std::cout << "After P1 says 5 (3 participants): P1=" << p1.value
              << ", P2=" << p2.value << ", P3=" << p3.value
              << " (expected: P1=2, P2=8, P3=5)" << std::endl;

    
    std::cout << "\n\033[92m" << "--- Event Broker Mediator ---" << "\033[0m" << std::endl;

    EventBroker broker;

    Player player1("Alice", broker);
    Player player2("Bob", broker);
    Coach coach("Sam", broker);

    std::cout << "\nAlice takes 30 damage:" << std::endl;
    player1.take_damage(30);

    std::cout << "\nBob takes 20 damage:" << std::endl;
    player2.take_damage(20);

    std::cout << "\nFinal health - Alice: " << player1.health
              << ", Bob: " << player2.health << std::endl;

    return 0;
}