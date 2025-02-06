#include <iostream>

#include "cpp/Tests/Test.h"

int main() {
    try {
        Test tester;
        tester.runTests();
        tester.testMutationRates();
        tester.testCrossoverRates();
        std::cout << "Tests completed successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error during testing: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
