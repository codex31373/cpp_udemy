#include <iostream>
#include <sstream>
#include <string>
#include <variant>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Visitor Coding Exercise
// Double-dispatch visitor for printing mathematical expressions.
// Addition gets brackets: (2+3), Multiplication does not: 2*3
////////////////////////////////////////////////////////////////////////////////

//Forward declarations
struct Expression;
struct Value;
struct AdditionExpression;
struct MultiplicationExpression;

struct ExpressionVisitor
{
    virtual void accept(Expression&) = 0;
    virtual void accept(Value&) = 0;
    virtual void accept(AdditionExpression&) = 0;
    virtual void accept(MultiplicationExpression&) = 0;
    virtual ~ExpressionVisitor() = default;
};

struct Expression
{
    virtual void visit(ExpressionVisitor& ev) = 0;
    virtual ~Expression() = default;
};

struct Value : Expression
{
    int value;

    Value(int value) : value(value) {}

    void visit(ExpressionVisitor& ev) override
    {
        ev.accept(*this);
    }
};

struct AdditionExpression : Expression
{
    Expression &lhs, &rhs;

    AdditionExpression(Expression &lhs, Expression &rhs) : lhs(lhs), rhs(rhs) {}

    void visit(ExpressionVisitor& ev) override
    {
        ev.accept(*this);
    }
};

struct MultiplicationExpression : Expression
{
    Expression &lhs, &rhs;

    MultiplicationExpression(Expression &lhs, Expression &rhs)
        : lhs(lhs), rhs(rhs) {}

    void visit(ExpressionVisitor& ev) override
    {
        ev.accept(*this);
    }
};

struct ExpressionPrinter : ExpressionVisitor
{
    ostringstream oss;

    void accept(Expression& e) override
    {
        e.visit(*this);
    }

    void accept(Value& v) override
    {
        oss << v.value;
    }

    void accept(AdditionExpression& ae) override
    {
        oss << "(";
        ae.lhs.visit(*this);
        oss << "+";
        ae.rhs.visit(*this);
        oss << ")";
    }

    void accept(MultiplicationExpression& me) override
    {
        me.lhs.visit(*this);
        oss << "*";
        me.rhs.visit(*this);
    }

    string str() const
    {
        return oss.str();
    }

    void clear()
    {
        oss.str("");
        oss.clear();
    }
};

int main(const int argc,const char* argv[])
{
    std::cout << "\033[92m" << "\nVisitor | 11 May 2026\n"
              << "\033[0m" << std::endl;

    // std::variant + std::visit example
    std::variant<int, std::string> var = 42;
    
    std::visit([](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, int>) {
            std::cout << "Integer: " << arg << '\n';
        } else if constexpr (std::is_same_v<T, std::string>) {
            std::cout << "String: " << arg << '\n';
        }
    }, var);

    //Test 1: Simple addition (2+3) -> (2+3)
    cout << "\n\n--- Test 1: Simple addition ---" << endl;
    Value v2{2};
    Value v3{3};
    AdditionExpression simple{v2, v3};
    ExpressionPrinter ep;
    ep.accept(simple);
    cout << "Output: " << ep.str() << " (expected: (2+3))" << endl;

    //Test 2: Simple multiplication 2*3 -> 2*3 (no brackets)
    cout << "\n--- Test 2: Simple multiplication ---" << endl;
    ep.clear();
    Value v5{5};
    MultiplicationExpression mul{v2, v5};  // 2*5
    ep.accept(mul);
    cout << "Output: " << ep.str() << " (expected: 2*5)" << endl;

    //Test 3: Addition of multiplication (2*3+4) -> (2*3+4)
    cout << "\n--- Test 3: Addition of multiplication ---" << endl;
    ep.clear();
    Value v4{4};
    MultiplicationExpression mul2{v2, v3};  // 2*3
    AdditionExpression add{mul2, v4};        // 2*3+4
    ep.accept(add);
    cout << "Output: " << ep.str() << " (expected: (2*3+4))" << endl;

    //Test 4: Multiplication of addition (2+3)*4 -> (2+3)*4
    cout << "\n--- Test 4: Multiplication of addition ---" << endl;
    ep.clear();
    MultiplicationExpression mul3{simple, v4};  // (2+3)*4
    ep.accept(mul3);
    cout << "Output: " << ep.str() << " (expected: (2+3)*4)" << endl;

    //Test 5: Nested addition ((1+2)+(3+4)) -> ((1+2)+(3+4))
    cout << "\n--- Test 5: Nested addition ---" << endl;
    ep.clear();
    Value v1{1};
    AdditionExpression add1{v1, v2};  // (1+2)
    AdditionExpression add2{v3, v4};  // (3+4)
    AdditionExpression nested{add1, add2};
    ep.accept(nested);
    cout << "Output: " << ep.str() << " (expected: ((1+2)+(3+4)))" << endl;

    return 0;
}