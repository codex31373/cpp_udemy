#include <iostream>

//task
/*
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class ContainsIntegers {
public:
    virtual int sum() const = 0;
    virtual ~ContainsIntegers() = default;
};

struct SingleValue : public ContainsIntegers {
    int value{ 0 };

    SingleValue() = default;

    explicit SingleValue(const int value)
        : value{value}
    {
    }

    int sum() const override {
        return value;
    }
};

struct ManyValues : public ContainsIntegers, public vector<int> {
    void add(const int value) {
        push_back(value);
    }

    int sum() const override {
        int sum = 0;
        for (const auto& value : *this) {
            sum += value;
        }
        return sum;
    }
};

int sum(const vector<ContainsIntegers*>& items) {
    int total_sum = 0;
    for (const auto& item : items) {
        total_sum += item->sum();
    }
    return total_sum;
}

int main() {
    SingleValue single_value{ 1 };
    ManyValues other_values;
    other_values.add(2);
    other_values.add(3);
    vector<ContainsIntegers*> items = {&single_value, &other_values};
    cout << sum(items) << endl; // returns 6
    return 0;
}
 */

int main(const int argc,const char *argv[])
{
    std::cout << "\033[92m" << "\n20 July 2025\n"
              << "\033[0m" << std::endl;

    return 0;
}