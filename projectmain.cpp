#include "hotel.h"
#include "hotel.cpp"
#include <iostream>

using namespace std;

int main() {
    try {
        Hotel California("Hotel California", "California, US", "619-671-6022");
        California.hotelinfoprint();

        login();
    } catch (const exception& e) {
        cerr << "Exception occurred: " << e.what() << endl;
        return 1;
    }

    return 0;
}
