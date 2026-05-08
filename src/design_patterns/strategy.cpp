#include <iostream>
#include <complex>
#include <tuple>
#include <cmath>
#include <vector>
#include <sstream>
#include <memory>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Dynamic Strategy Example
// TextProcessor that can switch formatting strategies at runtime.
// Key difference from previous example: strategy is set via setter, not constructor.
////////////////////////////////////////////////////////////////////////////////



struct ListStrategy
{
    virtual void start(std::ostringstream& oss) {}
    virtual void add_list_item(std::ostringstream& oss, const std::string& item) = 0;
    virtual void end(std::ostringstream& oss) {}
    virtual ~ListStrategy() = default;
};

//HTML list strategy
struct HtmlListStrategy : ListStrategy
{
    void start(std::ostringstream& oss) override
    {
        oss << "<ul>\n";
    }

    void add_list_item(std::ostringstream& oss, const std::string& item) override
    {
        oss << "  <li>" << item << "</li>\n";
    }

    void end(std::ostringstream& oss) override
    {
        oss << "</ul>\n";
    }
};

//Markdown list strategy
struct MarkdownListStrategy : ListStrategy
{
    void add_list_item(std::ostringstream& oss, const std::string& item) override
    {
        oss << "- " << item << "\n";
    }
};

//Text processor with dynamic strategy
class TextProcessor
{
    std::ostringstream oss;
    std::unique_ptr<ListStrategy> list_strategy;

public:
    void set_strategy(std::unique_ptr<ListStrategy> strategy)
    {
        list_strategy = std::move(strategy);
    }

    void append_list(const std::vector<std::string>& items)
    {
        if (!list_strategy) return;

        list_strategy->start(oss);
        for (const auto& item : items)
        {
            list_strategy->add_list_item(oss, item);
        }
        list_strategy->end(oss);
    }

    void clear()
    {
        oss.str("");
        oss.clear();
    }

    std::string str() const
    {
        return oss.str();
    }
};



////////////////////////////////////////////////////////////////////////////////
// Strategy Coding Exercise
// Quadratic equation solver with interchangeable discriminant strategies.
// Ordinary: returns negative discriminant as-is (complex roots possible).
// Real: returns NaN for negative discriminant (no real roots).
////////////////////////////////////////////////////////////////////////////////

struct DiscriminantStrategy
{
    virtual double calculate_discriminant(double a, double b, double c) = 0;
    virtual ~DiscriminantStrategy() = default;
};

struct OrdinaryDiscriminantStrategy : DiscriminantStrategy
{
    double calculate_discriminant(double a, double b, double c) override
    {
        return b * b - 4 * a * c;
    }
};

struct RealDiscriminantStrategy : DiscriminantStrategy
{
    double calculate_discriminant(double a, double b, double c) override
    {
        double discriminant = b * b - 4 * a * c;
        return discriminant >= 0 ? discriminant : numeric_limits<double>::quiet_NaN();
    }
};

class QuadraticEquationSolver
{
    DiscriminantStrategy& strategy;
public:
    QuadraticEquationSolver(DiscriminantStrategy &strategy) : strategy(strategy) {}

    tuple<complex<double>, complex<double>> solve(double a, double b, double c)
    {
        complex<double> discriminant(strategy.calculate_discriminant(a, b, c), 0);
        complex<double> root_discriminant = sqrt(discriminant);
        complex<double> minus_b(-b, 0);
        complex<double> two_a(2 * a, 0);

        complex<double> root1 = (minus_b + root_discriminant) / two_a;
        complex<double> root2 = (minus_b - root_discriminant) / two_a;

        return {root1, root2};
    }
};

int main(const int argc,const char *argv[])
{
    std::cout << "\033[92m" << "\nStrategy | 8 May 2026\n"
              << "\033[0m" << std::endl;

    //Test 1: Two real roots with Ordinary strategy
    //x^2 - 5x + 6 = 0 -> roots 2, 3
    cout << "--- Test 1: Two real roots (x^2 - 5x + 6) ---" << endl;
    OrdinaryDiscriminantStrategy ordinary;
    QuadraticEquationSolver solver1(ordinary);
    auto [r1a, r1b] = solver1.solve(1, -5, 6);
    cout << "Ordinary strategy: root1=" << r1a << ", root2=" << r1b << endl;

    //Test 2: Complex roots with Ordinary strategy
    //x^2 + 1 = 0 -> roots i, -i
    cout << "\n--- Test 2: Complex roots (x^2 + 1) ---" << endl;
    auto [r2a, r2b] = solver1.solve(1, 0, 1);
    cout << "Ordinary strategy: root1=" << r2a << ", root2=" << r2b << endl;

    //Test 3: Two real roots with Real strategy
    cout << "\n--- Test 3: Real roots with Real strategy ---" << endl;
    RealDiscriminantStrategy real;
    QuadraticEquationSolver solver2(real);
    auto [r3a, r3b] = solver2.solve(1, -5, 6);
    cout << "Real strategy: root1=" << r3a << ", root2=" << r3b << endl;

    //Test 4: Negative discriminant with Real strategy -> NaN
    cout << "\n--- Test 4: Negative discriminant with Real strategy ---" << endl;
    auto [r4a, r4b] = solver2.solve(1, 0, 1);
    cout << "Real strategy: root1=" << r4a << ", root2=" << r4b << endl;


    cout << "\n\033[92m" << "--- Dynamic Strategy (Text Processor) ---" << "\033[0m" << endl;

    TextProcessor processor;
    vector<string> items = {"foo", "bar", "baz"};

    //Switch to HTML strategy
    cout << "\nHTML output:" << endl;
    processor.set_strategy(std::make_unique<HtmlListStrategy>());
    processor.append_list(items);
    cout << processor.str() << endl;

    processor.clear();

    //Switch to Markdown strategy at runtime
    cout << "Markdown output:" << endl;
    processor.set_strategy(std::make_unique<MarkdownListStrategy>());
    processor.append_list(items);
    cout << processor.str() << endl;

    processor.clear();

    //Switch back to HTML
    cout << "HTML output again:" << endl;
    processor.set_strategy(std::make_unique<HtmlListStrategy>());
    processor.append_list({"dynamic", "strategy", "switch"});
    cout << processor.str() << endl;

    return 0;
}