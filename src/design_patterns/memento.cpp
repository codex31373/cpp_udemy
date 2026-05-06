#include <iostream>
#include <vector>
#include <memory>
#include <stack>
#include <stdexcept>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Bank Account Memento Example
// Complete undo/redo system using the Memento pattern.
// Each operation automatically saves state. Undo restores previous,
// redo restores next after an undo.
////////////////////////////////////////////////////////////////////////////////

struct BankAccountMemento
{
    int balance;
    std::string last_action;

    BankAccountMemento(int balance, const std::string& action)
        : balance(balance), last_action(action) {}
};

class BankAccount
{
    int balance = 0;

    //History stacks for undo/redo
    std::stack<BankAccountMemento> undo_stack;
    std::stack<BankAccountMemento> redo_stack;

    void save_state(const std::string& action)
    {
        undo_stack.push(BankAccountMemento(balance, action));
        //Clear redo stack on new action (branching history)
        while (!redo_stack.empty()) redo_stack.pop();
    }

public:
    explicit BankAccount(int balance = 0) : balance(balance)
    {
        //Initial state is the first memento for undo back to origin
        undo_stack.push(BankAccountMemento(balance, "account opened"));
    }

    void deposit(int amount)
    {
        if (amount <= 0)
            throw std::invalid_argument("Deposit amount must be positive");

        save_state("deposit " + std::to_string(amount));
        balance += amount;
        cout << "Deposited " << amount << ", balance = " << balance << endl;
    }

    void withdraw(int amount)
    {
        if (amount <= 0)
            throw std::invalid_argument("Withdraw amount must be positive");
        if (amount > balance)
            throw std::invalid_argument("Insufficient funds");

        save_state("withdraw " + std::to_string(amount));
        balance -= amount;
        cout << "Withdrew " << amount << ", balance = " << balance << endl;
    }

    bool undo()
    {
        if (undo_stack.size() <= 1)  //Can't undo initial state
        {
            cout << "Nothing to undo" << endl;
            return false;
        }

        //Current state goes to redo stack
        redo_stack.push(BankAccountMemento(balance, "current"));

        //Restore previous state
        auto memento = undo_stack.top();
        undo_stack.pop();
        balance = undo_stack.top().balance;

        cout << "Undo: reverted " << memento.last_action
             << ", balance = " << balance << endl;
        return true;
    }

    bool redo()
    {
        if (redo_stack.empty())
        {
            cout << "Nothing to redo" << endl;
            return false;
        }

        //Push current to undo before restoring
        undo_stack.push(BankAccountMemento(balance, "redo"));

        auto memento = redo_stack.top();
        redo_stack.pop();
        balance = memento.balance;

        cout << "Redo: restored to balance = " << balance << endl;
        return true;
    }

    int get_balance() const { return balance; }
};

////////////////////////////////////////////////////////////////////////////////
// Memento Coding Exercise
// TokenMachine keeps tokens (shared_ptr<Token> reference types).
// When adding a token, return a Memento snapshot.
// CRITICAL: Since tokens are shared_ptr, memento must DEEP COPY token values
// so external modifications to the original token don't affect the snapshot.
////////////////////////////////////////////////////////////////////////////////

struct Token
{
  int value;

  Token(int value) : value(value) {}
};

struct Memento
{
  vector<shared_ptr<Token>> tokens;
};

struct TokenMachine
{
  vector<shared_ptr<Token>> tokens;

  Memento add_token(int value)
  {
    return add_token(make_shared<Token>(value));
  }

  // adds the token to the set of tokens and returns the
  // snapshot of the entire system
  Memento add_token(const shared_ptr<Token>& token)
  {
    tokens.push_back(token);

    //Deep copy: create new Token objects with same values
    //This ensures the memento is independent of future changes to the original tokens
    Memento memento;
    for (const auto& t : tokens)
    {
      memento.tokens.push_back(make_shared<Token>(t->value));
    }
    return memento;
  }

  // reverts the system to a state represented by the token
  void revert(const Memento& m)
  {
    tokens.clear();
    //Deep copy from memento back to machine
    for (const auto& t : m.tokens)
    {
      tokens.push_back(make_shared<Token>(t->value));
    }
  }
};

int main(const int argc,const char *argv[])
{
    std::cout << "\033[92m" << "\nMemento | 7 May 2026\n"
              << "\033[0m" << std::endl;

    TokenMachine machine;

    //Add tokens and save snapshots
    auto m1 = machine.add_token(1);
    cout << "After adding token 1: machine has " << machine.tokens.size() << " token(s)" << endl;

    auto m2 = machine.add_token(2);
    cout << "After adding token 2: machine has " << machine.tokens.size() << " token(s)" << endl;

    auto m3 = machine.add_token(3);
    cout << "After adding token 3: machine has " << machine.tokens.size() << " token(s)" << endl;

    //Demonstrate: external modification of shared_ptr token
    cout << "\nToken values before external modification:" << endl;
    for (size_t i = 0; i < machine.tokens.size(); ++i)
    {
        cout << "  Token[" << i << "] = " << machine.tokens[i]->value << endl;
    }

    //Modify the last token externally (simulating someone changing the shared_ptr)
    machine.tokens.back()->value = 999;

    cout << "\nAfter modifying last token to 999:" << endl;
    for (size_t i = 0; i < machine.tokens.size(); ++i)
    {
        cout << "  Token[" << i << "] = " << machine.tokens[i]->value << endl;
    }

    //Verify memento m3 still has the original values (deep copy worked)
    cout << "\nMemento m3 snapshot (should still be 1, 2, 3):" << endl;
    for (size_t i = 0; i < m3.tokens.size(); ++i)
    {
        cout << "  Token[" << i << "] = " << m3.tokens[i]->value << endl;
    }

    //Revert to m2 state (should have tokens 1, 2)
    cout << "\nReverting to m2 state (1, 2):" << endl;
    machine.revert(m2);
    for (size_t i = 0; i < machine.tokens.size(); ++i)
    {
        cout << "  Token[" << i << "] = " << machine.tokens[i]->value << endl;
    }

    //Revert to m1 state (should have token 1)
    cout << "\nReverting to m1 state (1):" << endl;
    machine.revert(m1);
    for (size_t i = 0; i < machine.tokens.size(); ++i)
    {
        cout << "  Token[" << i << "] = " << machine.tokens[i]->value << endl;
    }


    cout << "\n\033[92m" << "--- Bank Account Memento (Undo/Redo) ---" << "\033[0m" << endl;

    BankAccount account(100);  //Start with 100
    cout << "Initial balance: " << account.get_balance() << endl;

    account.deposit(50);
    account.deposit(25);
    account.withdraw(30);
    //Balance should be 145

    cout << "\nCurrent balance: " << account.get_balance() << endl;

    cout << "\n--- Undo operations ---" << endl;
    account.undo();  //Undo withdraw 30
    account.undo();  //Undo deposit 25

    cout << "\nCurrent balance after undo: " << account.get_balance() << endl;

    cout << "\n--- Redo operations ---" << endl;
    account.redo();  //Redo deposit 25

    cout << "\nCurrent balance after redo: " << account.get_balance() << endl;

    cout << "\n--- New operation clears redo history ---" << endl;
    account.deposit(10);  //This clears redo stack
    cout << "Balance: " << account.get_balance() << endl;

    cout << "\n--- Try to redo (should fail) ---" << endl;
    account.redo();  //Nothing to redo

    return 0;
}