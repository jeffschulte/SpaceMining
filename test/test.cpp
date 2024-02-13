#include <iostream>
#include <string>
#include <map>
#include <spaceMining.h>

enum TestName {TEST1, TEST2, BAD_NAME};

std::map<std::string, TestName> testNameMap = {
    {"test1", TEST1},
    {"test2", TEST2}

};

int main(int argc, char *argv[]) {
    int value = spaceMiningFunction();
    printf("Testing! %d", value);
    TestName testName = testNameMap.count(argv[1]) ? testNameMap[argv[1]] : BAD_NAME;
    switch (testName) {
        case TEST1:
            printf("\nRunning test 1\n");
            return 0;
        case TEST2:
            printf("\nRunning test 2\n");
            return 0;
        case BAD_NAME:
        default:
            printf("\nBad test name\n");
            return 1;
    }
}