#include <iostream>
#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
// Broker Chain: A chain of responsibility with a central broker (mediator).
// The broker coordinates queries through a chain of modifiers.
// Each modifier can process, modify, or pass along the query.
// Example: Game creature stats modified by equipment, buffs, etc.
////////////////////////////////////////////////////////////////////////////////

//Query type for different stats
enum class QueryType
{
    attack,
    defense
};

//Query object passed through the chain
struct Query
{
    QueryType type;
    int value;

    Query(QueryType type, int value) : type(type), value(value) {}
};

//Forward declarations
struct Creature;
struct Modifier;

//Broker: coordinates the chain of modifiers
struct Game
{
    std::vector<Modifier*> modifiers;

    void add_modifier(Modifier* modifier);
    void handle_query(Creature& creature, Query& query);
};

//Base Modifier: can be chained
struct Modifier
{
    Game& game;
    Creature& creature;

    Modifier(Game& game, Creature& creature) : game(game), creature(creature)
    {
        game.add_modifier(this);
    }

    virtual void handle(Query& query) {}
};

void Game::add_modifier(Modifier* modifier)
{
    modifiers.push_back(modifier);
}

void Game::handle_query(Creature& creature, Query& query)
{
    for (auto modifier : modifiers)
    {
        if (&modifier->creature == &creature)
        {
            modifier->handle(query);
        }
    }
}

//Creature whose stats can be modified
struct Creature
{
    std::string name;
    int base_attack, base_defense;
    Game& game;

    Creature(Game& game, const std::string& name, int attack, int defense)
        : name(name), base_attack(attack), base_defense(defense), game(game) {}

    int get_attack() const
    {
        Query q{QueryType::attack, base_attack};
        game.handle_query(const_cast<Creature&>(*this), q);
        return q.value;
    }

    int get_defense() const
    {
        Query q{QueryType::defense, base_defense};
        game.handle_query(const_cast<Creature&>(*this), q);
        return q.value;
    }

    friend std::ostream& operator<<(std::ostream& os, const Creature& c)
    {
        return os << c.name << " [attack: " << c.get_attack()
                  << ", defense: " << c.get_defense() << "]";
    }
};

//Concrete modifiers
struct DoubleAttackModifier : Modifier
{
    DoubleAttackModifier(Game& game, Creature& creature)
        : Modifier(game, creature) {}

    void handle(Query& query) override
    {
        if (query.type == QueryType::attack)
        {
            query.value *= 2;
            std::cout << "  [DoubleAttackModifier applied]" << std::endl;
        }
    }
};

struct IncreaseDefenseModifier : Modifier
{
    int bonus;

    IncreaseDefenseModifier(Game& game, Creature& creature, int bonus)
        : Modifier(game, creature), bonus(bonus) {}

    void handle(Query& query) override
    {
        if (query.type == QueryType::defense)
        {
            query.value += bonus;
            std::cout << "  [IncreaseDefenseModifier +" << bonus << " applied]" << std::endl;
        }
    }
};

struct NoBonusesModifier : Modifier
{
    NoBonusesModifier(Game& game, Creature& creature)
        : Modifier(game, creature) {}

    void handle(Query& query) override
    {
        std::cout << "  [NoBonusesModifier: blocking all bonuses]" << std::endl;
        //Resets to base - effectively cancels all previous modifiers
        //This demonstrates how order matters in the chain
    }
};

////////////////////////////////////////////////////////////////////////////////
// Exercise: Goblin Chain of Responsibility
// Rules:
// - Goblin: base 1/1 (attack/defense)
// - GoblinKing: base 3/3
// - When GoblinKing is in play, every other goblin gets +1 Attack
// - Goblins get +1 Defense for every other Goblin in play
////////////////////////////////////////////////////////////////////////////////

struct Creature2;
struct Game2
{
    std::vector<Creature2*> creatures;
};

struct StatQuery
{
    enum Statistic { attack, defense } statistic;
    int result;
};

struct Creature2
{
protected:
    Game2& game;
    int base_attack, base_defense;

public:
    Creature2(Game2 &game, int base_attack, int base_defense)
        : game(game), base_attack(base_attack), base_defense(base_defense)
    {
        game.creatures.push_back(this);
    }

    virtual int get_attack() = 0;
    virtual int get_defense() = 0;

    virtual void handle_query(StatQuery& query) = 0;

    virtual ~Creature2() = default;
};

class Goblin : public Creature2
{
public:
    Goblin(Game2 &game, int base_attack, int base_defense)
        : Creature2(game, base_attack, base_defense) {}

    Goblin(Game2 &game) : Creature2(game, 1, 1) {}

    int get_attack() override
    {
        StatQuery q{StatQuery::attack, base_attack};
        for (auto creature : game.creatures)
        {
            creature->handle_query(q);
        }
        return q.result;
    }

    int get_defense() override
    {
        StatQuery q{StatQuery::defense, base_defense};
        for (auto creature : game.creatures)
        {
            creature->handle_query(q);
        }
        return q.result;
    }

    void handle_query(StatQuery& query) override
    {
        //Each other goblin gives +1 defense
        if (query.statistic == StatQuery::defense)
        {
            query.result++;
        }
    }
};

class GoblinKing : public Goblin
{
public:
    GoblinKing(Game2 &game) : Goblin(game, 3, 3) {}

    void handle_query(StatQuery& query) override
    {
        //GoblinKing gives +1 attack to other goblins (not itself)
        if (query.statistic == StatQuery::attack)
        {
            query.result++;
        }
        //Also gives +1 defense like regular goblins
        if (query.statistic == StatQuery::defense)
        {
            query.result++;
        }
    }
};

int main(const int argc,const char *argv[])
{
    std::cout << "\033[92m" << "\nChain of Responsibility (Broker Chain) : 26 April 2026\n"
              << "\033[0m" << std::endl;

    Game game;

    Creature goblin(game, "Goblin", 10, 5);
    std::cout << "Base creature: " << goblin << std::endl;

    std::cout << "\nAdding DoubleAttackModifier..." << std::endl;
    DoubleAttackModifier double_attack(game, goblin);
    std::cout << goblin << std::endl;

    std::cout << "\nAdding IncreaseDefenseModifier (+3)..." << std::endl;
    IncreaseDefenseModifier extra_defense(game, goblin, 3);
    std::cout << goblin << std::endl;

    Creature dragon(game, "Dragon", 50, 30);
    std::cout << "\n--- New creature: " << dragon << std::endl;

    std::cout << "\nAdding DoubleAttackModifier to Dragon..." << std::endl;
    DoubleAttackModifier dragon_double(game, dragon);
    std::cout << dragon << std::endl;

    std::cout << "\n--- Multiple modifiers stack ---" << std::endl;
    std::cout << "Goblin: " << goblin << std::endl;
    std::cout << "Dragon: " << dragon << std::endl;


    std::cout << "\033[92m" << "\nGoblin Chain of Responsibility Exercise\n"
              << "\033[0m" << std::endl;

    //Test 1: Single goblin
    Game2 game1;
    Goblin goblin1(game1);
    std::cout << "Test 1 - Single goblin:" << std::endl;
    std::cout << "  Attack: " << goblin1.get_attack() << " (expected: 1)" << std::endl;
    std::cout << "  Defense: " << goblin1.get_defense() << " (expected: 1)" << std::endl;

    //Test 2: Three goblins
    Game2 game2;
    Goblin g2(game2), g3(game2), g4(game2);
    std::cout << "\nTest 2 - Three goblins:" << std::endl;
    std::cout << "  Each goblin: " << g2.get_attack() << "/" << g2.get_defense()
              << " (expected: 1/3)" << std::endl;

    //Test 3: GoblinKing with goblins
    Game2 game3;
    Goblin g5(game3), g6(game3), g7(game3);
    GoblinKing king(game3);
    std::cout << "\nTest 3 - 3 Goblins + GoblinKing:" << std::endl;
    std::cout << "  Ordinary goblin: " << g5.get_attack() << "/" << g5.get_defense()
              << " (expected: 2/4)" << std::endl;
    std::cout << "  GoblinKing: " << king.get_attack() << "/" << king.get_defense()
              << " (expected: 3/6)" << std::endl;

    return 0;
}