// Build me from the CMakeLists.txt using cmake like:
//
#include <iostream>

using namespace std;

int main(int argc, const char* argv[]) {
    for(int i = 1; i < argc; ++i)
        std::cout << argv[i];
    std::cout << std::endl;
    return 0;
}
