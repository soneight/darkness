#ifdef notdef
#pragma once
#endif
#include <exception>
#include <iostream>
#include <vector>

namespace son8::example {
    class Print {
    public:
        static void example() {
            std::cout << "example\n";
        }
    };
} // namespace son8::example

int main(int argc, char *argv[]) try
{
    std::cout << "tokenColors " << argv[0] << std::endl;
    son8::example::Print::example();

    std::vector< int > throw_exception;
    std::cout << throw_exception.at(8) << '\n';
    return 8;
} catch(std::exception const &e) {
    std::cerr << "std::exception: " << e.what() << std::endl;
} catch(...) {
    std::cerr << "...: unknown exception\n";
}
