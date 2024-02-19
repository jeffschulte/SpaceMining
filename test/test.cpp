#include <iostream>
#include <string>
#include <map>
#include <spaceMining.h>

namespace SpaceMining {

enum TestName {TEST1, TEST2, BAD_NAME};

std::map<std::string, TestName> testNameMap = {
    {"test1", TEST1},
    {"test2", TEST2}
};

int OneTimeMining() {
    return 1;
}
int timeInc = 0;
int IncTimeMining() {
    return timeInc * 10;
}

int main(int argc, char *argv[]) {
    TestName testName = testNameMap.count(argv[1]) ? testNameMap[argv[1]] : BAD_NAME;
    SpaceMining::EventLog log;
    SpaceMining::MiningManager miningManager(2, 1, &log, OneTimeMining);
    if (testName == TEST1) {
        printf("\nRunning test 1\n");
        Event finishMining0 = miningManager.HandleEvent( { ARRIVE_AT_MINING_SITE, 0, 0 });
        Event finishMining1 = miningManager.HandleEvent( { ARRIVE_AT_MINING_SITE, 0, 1 });
        if (finishMining0.eventType != FINISH_MINING) {
            printf("Handling ARRIVE_AT_MINING_SITE 0 returns incorrect event\n");
            return EXIT_FAILURE;
        }
        if (finishMining1.eventType != FINISH_MINING) {
            printf("Handling ARRIVE_AT_MINING_SITE 1 returns incorrect event\n");
            return EXIT_FAILURE;
        }
        miningManager.HandleEvent(finishMining0);
        miningManager.HandleEvent(finishMining1);
        int station = miningManager.GetNextMiningStation();
        if (station != 0) {
            printf("GetNextMiningStation returns incorrectly\n");
            return EXIT_FAILURE;
        }
        int rsvdTime = miningManager.stations[station].GetRsvdTime();
        printf("RSVD TIME %d\n", rsvdTime);
        return EXIT_SUCCESS;
    } else if (testName == TEST2) {
        return EXIT_SUCCESS;
    } else {
        printf("\nBad test name\n");
        return EXIT_FAILURE;
    }
    return 0;
}

}