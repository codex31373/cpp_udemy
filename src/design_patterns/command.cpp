#include <iostream>
#include <vector>
#include <memory>

//Bank Account
struct BankAccount
{
    int balance = 0;
    int overdraft_limit = -500;

    bool deposit(int amount)
    {
        balance += amount;
        std::cout << "Deposited " << amount << ", balance is now " << balance << std::endl;
        return true;
    }

    bool withdraw(int amount)
    {
        if (balance - amount >= overdraft_limit)
        {
            balance -= amount;
            std::cout << "Withdrew " << amount << ", balance is now " << balance << std::endl;
            return true;
        }
        std::cout << "Cannot withdraw " << amount << " (would exceed overdraft limit)" << std::endl;
        return false;
    }
};

//Command interface
struct Command
{
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual ~Command() = default;
};

//Concrete commands
struct BankAccountCommand : Command
{
    enum class Action { deposit, withdraw } action;
    BankAccount& account;
    int amount;
    bool succeeded = false;

    BankAccountCommand(BankAccount& account, Action action, int amount)
        : action(action), account(account), amount(amount) {}

    void execute() override
    {
        if (action == Action::deposit)
        {
            succeeded = account.deposit(amount);
        }
        else if (action == Action::withdraw)
        {
            succeeded = account.withdraw(amount);
        }
    }

    void undo() override
    {
        if (!succeeded) return;

        if (action == Action::deposit)
        {
            account.withdraw(amount);
        }
        else if (action == Action::withdraw)
        {
            account.deposit(amount);
        }
    }
};

//Command processor (invoker)
struct CompositeBankAccountCommand : Command
{
    std::vector<std::unique_ptr<BankAccountCommand>> commands;

    void add_command(std::unique_ptr<BankAccountCommand> cmd)
    {
        commands.push_back(std::move(cmd));
    }

    void execute() override
    {
        for (auto& cmd : commands)
        {
            cmd->execute();
        }
    }

    void undo() override
    {
        for (auto it = commands.rbegin(); it != commands.rend(); ++it)
        {
            (*it)->undo();
        }
    }
};

//Dependent Composite: undoes previous commands if any command fails
//Ensures atomicity - either all commands succeed, or none do
struct DependentCompositeCommand : Command
{
    std::vector<std::unique_ptr<BankAccountCommand>> commands;

    void add_command(std::unique_ptr<BankAccountCommand> cmd)
    {
        commands.push_back(std::move(cmd));
    }

    void execute() override
    {
        bool success = true;
        for (auto& cmd : commands)
        {
            cmd->execute();
            if (!cmd->succeeded)
            {
                success = false;
                break;
            }
        }

        if (!success)
        {
            //Undo all successful commands in reverse order
            for (auto it = commands.rbegin(); it != commands.rend(); ++it)
            {
                if ((*it)->succeeded)
                {
                    (*it)->undo();
                }
            }
        }
    }

    void undo() override
    {
        for (auto it = commands.rbegin(); it != commands.rend(); ++it)
        {
            if ((*it)->succeeded)
            {
                (*it)->undo();
            }
        }
    }
};

//Money transfer between accounts (uses DependentComposite for atomicity)
struct MoneyTransferCommand : DependentCompositeCommand
{
    BankAccount& from;
    BankAccount& to;
    int amount;

    MoneyTransferCommand(BankAccount& from, BankAccount& to, int amount)
        : from(from), to(to), amount(amount)
    {
        add_command(std::make_unique<BankAccountCommand>(from, BankAccountCommand::Action::withdraw, amount));
        add_command(std::make_unique<BankAccountCommand>(to, BankAccountCommand::Action::deposit, amount));
    }
};

////////////////////////////////////////////////////////////////////////////////
// Command Coding Exercise
// Simple command pattern with Account processing Command objects
////////////////////////////////////////////////////////////////////////////////

struct Command2
{
    enum Action { deposit, withdraw } action;
    int amount{0};
    bool success{false};
};

struct Account
{
    int balance{0};

    void process(Command2& cmd)
    {
        switch (cmd.action)
        {
        case Command2::deposit:
            balance += cmd.amount;
            cmd.success = true;
            break;
        case Command2::withdraw:
            if (balance >= cmd.amount)
            {
                balance -= cmd.amount;
                cmd.success = true;
            }
            else
            {
                cmd.success = false;
            }
            break;
        }
    }
};

int main(const int argc,const char *argv[])
{
    std::cout << "\033[92m" << "\nCommand : 30 April 2026\n"
              << "\033[0m" << std::endl;

    //Simple commands
    std::cout << "--- Simple Commands ---" << std::endl;
    BankAccount account;
    BankAccountCommand deposit(account, BankAccountCommand::Action::deposit, 100);
    BankAccountCommand withdraw(account, BankAccountCommand::Action::withdraw, 50);

    deposit.execute();
    withdraw.execute();

    std::cout << "\nUndoing withdraw..." << std::endl;
    withdraw.undo();
    std::cout << "Final balance: " << account.balance << std::endl;

    //Money transfer (composite command)
    std::cout << "\n--- Money Transfer (Composite) ---" << std::endl;
    BankAccount alice, bob;

    MoneyTransferCommand transfer(alice, bob, 100);

    std::cout << "Before transfer:" << std::endl;
    std::cout << "  Alice: " << alice.balance << ", Bob: " << bob.balance << std::endl;

    //Alice deposits first
    alice.deposit(500);
    std::cout << "Alice deposited 500, balance: " << alice.balance << std::endl;

    transfer.execute();
    std::cout << "After transfer:" << std::endl;
    std::cout << "  Alice: " << alice.balance << ", Bob: " << bob.balance << std::endl;

    std::cout << "\nUndoing transfer..." << std::endl;
    transfer.undo();
    std::cout << "After undo:" << std::endl;
    std::cout << "  Alice: " << alice.balance << ", Bob: " << bob.balance << std::endl;

    //Dependent composite - transaction fails atomically
    std::cout << "\n--- Dependent Composite (Atomic Transaction) ---" << std::endl;
    BankAccount charlie, dave;
    charlie.deposit(50);  //Only 50, not enough for 100 withdrawal

    MoneyTransferCommand failed_transfer(charlie, dave, 100);
    std::cout << "Before failed transfer (Charlie has only 50, needs 100):" << std::endl;
    std::cout << "  Charlie: " << charlie.balance << ", Dave: " << dave.balance << std::endl;

    failed_transfer.execute();  //Should fail and rollback
    std::cout << "After failed transfer:" << std::endl;
    std::cout << "  Charlie: " << charlie.balance << " (should be 50 - rollback occurred), Dave: " << dave.balance << std::endl;


    std::cout << "\033[92m" << "\nCommand Exercise\n"
              << "\033[0m" << std::endl;

    Account account2;
    std::cout << "Initial balance: " << account.balance << std::endl;

    Command2 deposit2{Command2::deposit, 100, false};
    account2.process(deposit2);
    std::cout << "After deposit 100: balance=" << account.balance
              << ", success=" << (deposit2.success ? "true" : "false") << std::endl;

    Command2 withdraw2{Command2::withdraw, 50, false};
    account2.process(withdraw2);
    std::cout << "After withdraw 50: balance=" << account.balance
              << ", success=" << (withdraw2.success ? "true" : "false") << std::endl;

    Command2 failed_withdraw{Command2::withdraw, 200, false};
    account2.process(failed_withdraw);
    std::cout << "After withdraw 200 (insufficient funds): balance=" << account.balance
              << ", success=" << (failed_withdraw.success ? "true" : "false") << std::endl;

    return 0;
}