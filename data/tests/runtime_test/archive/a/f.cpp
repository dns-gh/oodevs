// Build me from the CMakeLists.txt using cmake like:
//
//  $ cmake -G "NMake Makefiles" && nmake && echowin32 hello

#include <iostream>

using namespace std;

int main(int argc, const char* argv[]) {
    for(int i = 1; i < argc; ++i)
        cout << argv[i];
    cout << endl;
    return 0;
}
