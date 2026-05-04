#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>

////////////////////////////////////////////////////////////////////////////////
// Interpreter Pattern: Lexing and Parsing
// Example: Parsing numeric expressions like "1+2+3" and evaluating them
////////////////////////////////////////////////////////////////////////////////

//Token types from lexing
struct Token
{
    enum Type { integer, plus, minus, lparen, rparen } type;
    std::string text;

    Token(Type type, const std::string& text) : type(type), text(text) {}

    friend std::ostream& operator<<(std::ostream& os, const Token& token)
    {
        return os << "`" << token.text << "`";
    }
};

//Lexer: converts string into tokens
std::vector<Token> lex(const std::string& input)
{
    std::vector<Token> result;
    for (size_t i = 0; i < input.size(); ++i)
    {
        char c = input[i];
        switch (c)
        {
        case '+':
            result.emplace_back(Token::plus, "+");
            break;
        case '-':
            result.emplace_back(Token::minus, "-");
            break;
        case '(':
            result.emplace_back(Token::lparen, "(");
            break;
        case ')':
            result.emplace_back(Token::rparen, ")");
            break;
        default:
            //Parse multi-digit integer
            if (isdigit(c))
            {
                std::string number;
                number += c;
                while (i + 1 < input.size() && isdigit(input[i + 1]))
                {
                    number += input[++i];
                }
                result.emplace_back(Token::integer, number);
            }
            break;
        }
    }
    return result;
}

//Abstract Syntax Tree element
struct Element
{
    virtual int eval() const = 0;
    virtual ~Element() = default;
};

//Integer literal
struct Integer : Element
{
    int value;
    Integer(int value) : value(value) {}
    int eval() const override { return value; }
};

//Binary operation
struct BinaryOperation : Element
{
    enum Type { addition, subtraction } type;
    std::shared_ptr<Element> lhs, rhs;

    int eval() const override
    {
        if (type == addition)
            return lhs->eval() + rhs->eval();
        else
            return lhs->eval() - rhs->eval();
    }
};

//Parser: converts tokens into expression tree
std::shared_ptr<Element> parse(const std::vector<Token>& tokens)
{
    auto result = std::make_shared<BinaryOperation>();
    bool have_lhs = false;

    for (size_t i = 0; i < tokens.size(); ++i)
    {
        const auto& token = tokens[i];

        switch (token.type)
        {
        case Token::integer:
        {
            int value = std::stoi(token.text);
            auto integer = std::make_shared<Integer>(value);

            if (!have_lhs)
            {
                result->lhs = integer;
                have_lhs = true;
            }
            else
            {
                result->rhs = integer;
            }
            break;
        }
        case Token::plus:
            result->type = BinaryOperation::addition;
            break;
        case Token::minus:
            result->type = BinaryOperation::subtraction;
            break;
        case Token::lparen:
        {
            //Find matching rparen (simplified - assumes no nesting)
            size_t j = i + 1;
            while (j < tokens.size() && tokens[j].type != Token::rparen)
                ++j;

            //Parse subexpression
            std::vector<Token> subtokens(tokens.begin() + i + 1, tokens.begin() + j);
            auto subexpr = parse(subtokens);

            if (!have_lhs)
            {
                result->lhs = subexpr;
                have_lhs = true;
            }
            else
            {
                result->rhs = subexpr;
            }
            i = j;  //Skip past the subexpression
            break;
        }
        default:
            break;
        }

        //Handle chained operations: create nested BinaryOperation
        if (have_lhs && result->rhs && i + 1 < tokens.size())
        {
            auto new_op = std::make_shared<BinaryOperation>();
            new_op->lhs = result;
            result = new_op;
            have_lhs = true;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
// Interpreter Coding Exercise
// Expression processor for simple numeric expressions with variables
// - Integral values (e.g., "13")
// - Single-letter variables (e.g., "x")
// - + and - operators only
// - Returns 0 if variable not found, multi-letter variable encountered, or parse failure
////////////////////////////////////////////////////////////////////////////////

struct ExpressionProcessor
{
    std::map<char,int> variables;

    int calculate(const std::string& expression)
    {
        if (expression.empty()) return 0;

        int result = 0;
        int current_number = 0;
        bool has_number = false;
        bool is_addition = true;  //true for +, false for -
        bool expect_operand = true;  //start expecting an operand

        for (size_t i = 0; i < expression.size(); ++i)
        {
            char c = expression[i];

            if (isdigit(c))
            {
                //Parse multi-digit number
                current_number = current_number * 10 + (c - '0');
                has_number = true;
                expect_operand = false;
            }
            else if (isalpha(c))
            {
                //Single-letter variable
                //Check if next character is also a letter (multi-letter variable = invalid)
                if (i + 1 < expression.size() && isalpha(expression[i + 1]))
                {
                    return 0;  //Multi-letter variable
                }

                auto it = variables.find(c);
                if (it == variables.end())
                {
                    return 0;  //Variable not found
                }

                current_number = it->second;
                has_number = true;
                expect_operand = false;
            }
            else if (c == '+' || c == '-')
            {
                //Operator - need to process previous operand first
                if (expect_operand)
                {
                    return 0;  //Two operators in a row or leading operator
                }

                //Apply previous operation
                if (is_addition)
                    result += current_number;
                else
                    result -= current_number;

                //Reset for next operand
                current_number = 0;
                has_number = false;
                is_addition = (c == '+');
                expect_operand = true;
            }
            else
            {
                //Invalid character
                return 0;
            }
        }

        //Apply final operand
        if (expect_operand && !has_number)
        {
            return 0;  //Expression ends with operator
        }

        if (is_addition)
            result += current_number;
        else
            result -= current_number;

        return result;
    }
};

int main(const int argc,const char *argv[])
{
    std::cout << "\033[92m" << "\nInterpreter : 4 May 2026\n"
              << "\033[0m" << std::endl;

    //Example 1: Simple expression
    std::string input1 = "(13+4)-(12+1)";
    std::cout << "Expression: " << input1 << std::endl;

    auto tokens = lex(input1);
    std::cout << "Tokens: ";
    for (const auto& t : tokens)
    {
        std::cout << t << " ";
    }
    std::cout << std::endl;

    auto parsed = parse(tokens);
    std::cout << "Result: " << parsed->eval() << " (expected: 4)" << std::endl;

    //Example 2: Another expression
    std::string input2 = "1+2+3";
    std::cout << "\nExpression: " << input2 << std::endl;

    auto tokens2 = lex(input2);
    std::cout << "Tokens: ";
    for (const auto& t : tokens2)
    {
        std::cout << t << " ";
    }
    std::cout << std::endl;

    auto parsed2 = parse(tokens2);
    std::cout << "Result: " << parsed2->eval() << " (expected: 6)" << std::endl;

    //Example 3: Simple subtraction
    std::string input3 = "10-3";
    std::cout << "\nExpression: " << input3 << std::endl;

    auto tokens3 = lex(input3);
    auto parsed3 = parse(tokens3);
    std::cout << "Result: " << parsed3->eval() << " (expected: 7)" << std::endl;

    std::cout << "\033[92m" << "\nInterpreter Exercise\n"
              << "\033[0m" << std::endl;

    //Task: Implement the ExpressionProcessor.calculate() method
    ExpressionProcessor ep;

    //Test 1: Simple expression
    std::cout << "Test 1: 1+2+3 = " << ep.calculate("1+2+3") << " (expected: 6)" << std::endl;

    //Test 2: Invalid variable
    std::cout << "Test 2: 1+2+xy = " << ep.calculate("1+2+xy") << " (expected: 0)" << std::endl;

    //Test 3: Variable substitution
    ep.variables['x'] = 3;
    std::cout << "Test 3: 10-2-x (x=3) = " << ep.calculate("10-2-x") << " (expected: 5)" << std::endl;

    //Test 4: Unknown variable
    std::cout << "Test 4: 5+y (y not defined) = " << ep.calculate("5+y") << " (expected: 0)" << std::endl;

    return 0;
}