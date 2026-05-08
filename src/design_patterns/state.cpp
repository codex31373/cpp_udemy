#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Classic State Machine Example
// Turnstile with explicit states, events, transition table, and entry/exit actions.
////////////////////////////////////////////////////////////////////////////////

enum class TurnstileState
{
    locked,
    unlocked
};

enum class TurnstileEvent
{
    coin,
    push
};

//Transition table: (current_state, event) -> next_state
using TransitionKey = pair<TurnstileState, TurnstileEvent>;
using TransitionTable = map<TransitionKey, TurnstileState>;

struct Turnstile
{
    TurnstileState state = TurnstileState::locked;
    int coins = 0;

    TransitionTable transitions = {
        {{TurnstileState::locked,   TurnstileEvent::coin}, TurnstileState::unlocked},
        {{TurnstileState::locked,   TurnstileEvent::push}, TurnstileState::locked},
        {{TurnstileState::unlocked, TurnstileEvent::coin}, TurnstileState::unlocked},
        {{TurnstileState::unlocked, TurnstileEvent::push}, TurnstileState::locked},
    };

    void on_enter(TurnstileState new_state)
    {
        switch (new_state)
        {
        case TurnstileState::locked:
            cout << "  [Enter] Turnstile is now LOCKED" << endl;
            break;
        case TurnstileState::unlocked:
            cout << "  [Enter] Turnstile is now UNLOCKED" << endl;
            break;
        }
    }

    void on_exit(TurnstileState old_state)
    {
        switch (old_state)
        {
        case TurnstileState::locked:
            cout << "  [Exit]  Leaving LOCKED state" << endl;
            break;
        case TurnstileState::unlocked:
            cout << "  [Exit]  Leaving UNLOCKED state" << endl;
            break;
        }
    }

    void handle(TurnstileEvent event)
    {
        auto key = make_pair(state, event);
        auto it = transitions.find(key);

        if (it == transitions.end())
        {
            cout << "  [Error] No transition from current state for this event" << endl;
            return;
        }

        auto next_state = it->second;
        if (next_state == state)
        {
            //Self-transition or no change
            if (event == TurnstileEvent::push && state == TurnstileState::locked)
            {
                cout << "  [Action] Push blocked, turnstile is locked" << endl;
            }
            else if (event == TurnstileEvent::coin && state == TurnstileState::unlocked)
            {
                cout << "  [Action] Coin accepted (already unlocked, extra credit)" << endl;
                ++coins;
            }
            return;
        }

        //State change
        on_exit(state);
        state = next_state;
        on_enter(state);

        //Entry actions
        if (state == TurnstileState::unlocked && event == TurnstileEvent::coin)
        {
            cout << "  [Action] Coin accepted, unlocking turnstile" << endl;
            ++coins;
        }
        else if (state == TurnstileState::locked && event == TurnstileEvent::push)
        {
            cout << "  [Action] Person passed through, locking turnstile" << endl;
        }
    }

    string state_name() const
    {
        return state == TurnstileState::locked ? "LOCKED" : "UNLOCKED";
    }
};




////////////////////////////////////////////////////////////////////////////////
// State Coding Exercise
// Combination lock that transitions between LOCKED, digit display, OPEN, ERROR.
////////////////////////////////////////////////////////////////////////////////

class CombinationLock
{
    vector<int> combination;
    vector<int> entered;

public:
    string status;

    CombinationLock(const vector<int> &combination) : combination(combination)
    {
        status = "LOCKED";
    }

    void enter_digit(int digit)
    {
        //If already in a final state, ignore further input
        if (status == "OPEN" || status == "ERROR")
            return;

        entered.push_back(digit);

        //Check if entered sequence matches the combination prefix
        bool prefix_match = true;
        for (size_t i = 0; i < entered.size(); ++i)
        {
            if (i >= combination.size() || entered[i] != combination[i])
            {
                prefix_match = false;
                break;
            }
        }

        if (!prefix_match)
        {
            status = "ERROR";
        }
        else if (entered.size() == combination.size())
        {
            status = "OPEN";
        }
        else
        {
            //Build status string from entered digits
            status.clear();
            for (int d : entered)
            {
                status += to_string(d);
            }
        }
    }
};

int main(const int argc,const char *argv[])
{
    std::cout << "\033[92m" << "\nState | 8 May 2026\n"
              << "\033[0m" << std::endl;

    //Test 1: Correct combination
    cout << "--- Test 1: Correct combination (1,2,3) ---" << endl;
    CombinationLock cl1({1, 2, 3});
    cout << "Initial: " << cl1.status << " (expected: LOCKED)" << endl;

    cl1.enter_digit(1);
    cout << "After 1: " << cl1.status << " (expected: 1)" << endl;

    cl1.enter_digit(2);
    cout << "After 12: " << cl1.status << " (expected: 12)" << endl;

    cl1.enter_digit(3);
    cout << "After 123: " << cl1.status << " (expected: OPEN)" << endl;

    //Test 2: Incorrect combination
    cout << "\n--- Test 2: Incorrect combination (1,2,3) ---" << endl;
    CombinationLock cl2({1, 2, 3});
    cout << "Initial: " << cl2.status << " (expected: LOCKED)" << endl;

    cl2.enter_digit(1);
    cout << "After 1: " << cl2.status << " (expected: 1)" << endl;

    cl2.enter_digit(4);
    cout << "After 14: " << cl2.status << " (expected: ERROR)" << endl;

    cl2.enter_digit(3);  //Should be ignored in ERROR state
    cout << "After 143: " << cl2.status << " (expected: ERROR)" << endl;

    //Test 3: Too many digits
    cout << "\n--- Test 3: Too many digits (1,2) ---" << endl;
    CombinationLock cl3({1, 2});
    cl3.enter_digit(1);
    cl3.enter_digit(2);
    cout << "After 12: " << cl3.status << " (expected: OPEN)" << endl;

    cl3.enter_digit(3);  //Should be ignored in OPEN state
    cout << "After 123: " << cl3.status << " (expected: OPEN)" << endl;

    //Test 4: Wrong first digit
    cout << "\n--- Test 4: Wrong first digit (5,5,5) ---" << endl;
    CombinationLock cl4({5, 5, 5});
    cl4.enter_digit(1);
    cout << "After 1: " << cl4.status << " (expected: ERROR)" << endl;

    
    cout << "\n\033[92m" << "--- Classic State Machine (Turnstile) ---" << "\033[0m" << endl;

    Turnstile turnstile;
    cout << "Initial state: " << turnstile.state_name() << endl;

    cout << "\n1. Push turnstile (locked):" << endl;
    turnstile.handle(TurnstileEvent::push);

    cout << "\n2. Insert coin:" << endl;
    turnstile.handle(TurnstileEvent::coin);

    cout << "\n3. Push turnstile (unlocked):" << endl;
    turnstile.handle(TurnstileEvent::push);

    cout << "\n4. Insert coin (already locked - self-loop):" << endl;
    turnstile.handle(TurnstileEvent::coin);

    cout << "\n5. Insert another coin (already unlocked):" << endl;
    turnstile.handle(TurnstileEvent::coin);

    cout << "\n6. Push to exit:" << endl;
    turnstile.handle(TurnstileEvent::push);

    cout << "\nTotal coins collected: " << turnstile.coins << endl;
    cout << "Final state: " << turnstile.state_name() << endl;

    return 0;
}