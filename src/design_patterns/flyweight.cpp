#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <sstream>
#include <cctype>

//Flyweight: shared intrinsic state
struct SoldierType
{
    std::string name;
    std::string weapon;
    std::string armor;
    int health;

    SoldierType(const std::string& name, const std::string& weapon,
                const std::string& armor, int health)
        : name(name), weapon(weapon), armor(armor), health(health) {}

    void report() const
    {
        std::cout << "  Type: " << name
                  << ", Weapon: " << weapon
                  << ", Armor: " << armor
                  << ", HP: " << health << std::endl;
    }
};

//Flyweight Factory
struct SoldierFactory
{
    std::unordered_map<std::string, std::unique_ptr<SoldierType>> types;

    SoldierType* get_type(const std::string& name, const std::string& weapon,
                          const std::string& armor, int health)
    {
        if (types.find(name) == types.end()) {
            types[name] = std::make_unique<SoldierType>(name, weapon, armor, health);
            std::cout << "Created new soldier type: " << name << std::endl;
        }
        return types[name].get();
    }

    void list_types() const
    {
        std::cout << "\nAvailable soldier types: ";
        for (const auto& pair : types) {
            std::cout << pair.first << " ";
        }
        std::cout << std::endl;
    }
};

//Context: extrinsic state (unique per instance)
struct Soldier
{
    int x, y;
    SoldierType* type;

    Soldier(int x, int y, SoldierType* type) : x(x), y(y), type(type) {}

    void report() const
    {
        std::cout << "Soldier at (" << x << ", " << y << ") - ";
        type->report();
    }
};

//Flyweight Exercise: Sentence with WordTokens
struct Sentence
{
    struct WordToken
    {
        bool capitalize = false;
    };

    std::vector<std::string> words;
    std::vector<WordToken> tokens;

    Sentence(const std::string& text)
    {
        std::istringstream iss(text);
        std::string word;
        while (iss >> word) {
            words.push_back(word);
            tokens.push_back(WordToken{});
        }
    }

    WordToken& operator[](size_t index)
    {
        return tokens[index];
    }

    std::string str() const
    {
        std::ostringstream oss;
        for (size_t i = 0; i < words.size(); ++i) {
            if (i > 0) oss << " ";
            std::string word = words[i];
            if (tokens[i].capitalize) {
                for (char& c : word) c = std::toupper(c);
            }
            oss << word;
        }
        return oss.str();
    }
};

int main(const int argc,const char* argv[])
{
    std::cout << "\033[92m" << "\nFlyweight : 23 April 2026\n"
              << "\033[0m" << std::endl;

    SoldierFactory factory;

    //Create soldiers - types are shared, positions are unique
    std::vector<Soldier> army;

    army.emplace_back(10, 20, factory.get_type("Archer", "Bow", "Leather", 80));
    army.emplace_back(15, 25, factory.get_type("Archer", "Bow", "Leather", 80));
    army.emplace_back(30, 40, factory.get_type("Knight", "Sword", "Plate", 150));
    army.emplace_back(35, 45, factory.get_type("Knight", "Sword", "Plate", 150));
    army.emplace_back(50, 60, factory.get_type("Knight", "Sword", "Plate", 150));
    army.emplace_back(100, 200, factory.get_type("Mage", "Staff", "Robe", 50));

    factory.list_types();

    std::cout << "\nArmy reports:" << std::endl;
    for (const auto& soldier : army) {
        soldier.report();
    }

    //Memory saved: 6 soldiers but only 3 SoldierType objects
    std::cout << "\nMemory: 6 soldiers, 3 shared types" << std::endl;

    //Flyweight Exercise
    std::cout << "\n--- Sentence Exercise ---" << std::endl;
    Sentence sentence("hello world");
    sentence[1].capitalize = true;
    std::cout << sentence.str() << std::endl; // prints "hello WORLD"

    return 0;
}