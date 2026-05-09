#include <iostream>
#include <vector>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Template Method Coding Exercise
// Card game combat with temporary vs permanent damage mechanics.
// combat() is the template method; hit() is the primitive operation.
////////////////////////////////////////////////////////////////////////////////

struct Creature
{
    int attack, health;

    Creature(int attack, int health) : attack(attack), health(health) {}
};

struct CardGame
{
    vector<Creature> creatures;

    CardGame(const vector<Creature> &creatures) : creatures(creatures) {}

    // Template method: defines the combat algorithm skeleton
    int combat(int creature1, int creature2)
    {
        Creature& first = creatures[creature1];
        Creature& second = creatures[creature2];

        // Remember original health for potential restoration
        int first_health = first.health;
        int second_health = second.health;

        // Both creatures hit each other simultaneously
        hit(first, second);
        hit(second, first);

        // Determine the winner
        int result = -1;
        if (first.health > 0 && second.health <= 0)
            result = creature1;
        else if (first.health <= 0 && second.health > 0)
            result = creature2;

        // Cleanup hook: restore or persist damage
        cleanup(first, second, first_health, second_health);

        return result;
    }

    virtual void hit(Creature& attacker, Creature& other) = 0;

    // Hook method for post-combat cleanup (restores health by default)
    virtual void cleanup(Creature& first, Creature& second, int first_health, int second_health)
    {
        first.health = first_health;
        second.health = second_health;
    }
};

// Temporary damage: health resets after each combat round
struct TemporaryCardDamageGame : CardGame
{
    TemporaryCardDamageGame(const vector<Creature> &creatures) : CardGame(creatures) {}

    void hit(Creature &attacker, Creature &other) override
    {
        other.health -= attacker.attack;
    }

    // cleanup() inherited: restores health after combat
};

// Permanent damage: health persists between combat rounds
struct PermanentCardDamageGame : CardGame
{
    PermanentCardDamageGame(const vector<Creature> &creatures) : CardGame(creatures) {}

    void hit(Creature &attacker, Creature &other) override
    {
        other.health -= attacker.attack;
    }

    // Override cleanup to keep damage (do nothing)
    void cleanup(Creature&, Creature&, int, int) override {}
};

int main(const int argc,const char *argv[])
{
    std::cout << "\033[92m" << "\nTemplate Method | 10 May 2026\n"
              << "\033[0m" << std::endl;

    //Test 1: Temporary damage - 1/2 vs 1/3 (never kill each other)
    cout << "--- Test 1: Temporary damage (1/2 vs 1/3) ---" << endl;
    TemporaryCardDamageGame temp_game({{1, 2}, {1, 3}});
    int result = temp_game.combat(0, 1);
    cout << "Round 1 result: " << result << " (expected: -1, no winner)" << endl;
    cout << "Creature 0 health: " << temp_game.creatures[0].health << " (expected: 2)" << endl;
    cout << "Creature 1 health: " << temp_game.creatures[1].health << " (expected: 3)" << endl;

    //Test 2: Temporary damage - 2/2 vs 2/2 (both die)
    cout << "\n--- Test 2: Temporary damage (2/2 vs 2/2) ---" << endl;
    TemporaryCardDamageGame temp_game2({{2, 2}, {2, 2}});
    result = temp_game2.combat(0, 1);
    cout << "Result: " << result << " (expected: -1, both die)" << endl;

    //Test 3: Permanent damage - 1/2 vs 1/3 (creature 1 wins after 2 rounds)
    cout << "\n--- Test 3: Permanent damage (1/2 vs 1/3) ---" << endl;
    PermanentCardDamageGame perm_game({{1, 2}, {1, 3}});
    result = perm_game.combat(0, 1);
    cout << "Round 1 result: " << result << " (expected: -1)" << endl;
    cout << "C0 health: " << perm_game.creatures[0].health << ", C1 health: "
         << perm_game.creatures[1].health << endl;

    result = perm_game.combat(0, 1);
    cout << "Round 2 result: " << result << " (expected: 1, creature 1 wins)" << endl;
    cout << "C0 health: " << perm_game.creatures[0].health
         << ", C1 health: " << perm_game.creatures[1].health << endl;

    //Test 4: Permanent damage - 2/2 vs 2/2 (both die in one round)
    cout << "\n--- Test 4: Permanent damage (2/2 vs 2/2) ---" << endl;
    PermanentCardDamageGame perm_game2({{2, 2}, {2, 2}});
    result = perm_game2.combat(0, 1);
    cout << "Result: " << result << " (expected: -1, both die)" << endl;

    return 0;
}