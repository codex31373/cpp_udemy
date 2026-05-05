#include <iostream>
#include <vector>
#include <string>

int main(const int argc,const char *argv[])
{
    std::vector<std::string> names = {"Alice", "Bob", "Charlie"};
    std::vector<std::string>::iterator it{names.begin()};

    for ( ; it != names.end(); ++it) {
        std::cout << *it << " ";
    }

    for (auto&& name : names) {
        std::cout << name << " ";
    }

    std::cout << "\033[92m" << "\nIterator : 5 May 2026\n"
              << "\033[0m" << std::endl;

    return 0;
}