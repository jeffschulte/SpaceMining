#include <iostream>
#include <string>
#include <map>
#include <spaceMining.h>

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

// Ive run out of time, so unfortunately I havent been able to fill out the testing like I wanted to.
// Below are two examples of what I would do. Essentially you could instantiate a MiningManager,
// Run events through it, check the return to make sure the numbers are what they should be, then
// check the rsvdTime on the station you expect to make sure the reservation is working properly
// You can pass in functions, defined above, that allow you to control the time of dig, so the tests are
// not random.
// The second test is an example of testing the station, essentially the math of the reservation function
// These are unit tests but you could also just instantiate everything and run a small, deterministic simulation,
// which would allow for integration tests.
// Things to test:
// -Inserting two events with same time, make sure doesnt screw up
// -Test at boundary of simulation length, make sure times and numbers show up correctly
// -Test multiple stations, make sure equal spread of trucks in predicatble fashion
// -Use two trucks one station to make sure reservations are predictable, along with wait times
int main(int argc, char *argv[]) {
    TestName testName = testNameMap.count(argv[1]) ? testNameMap[argv[1]] : BAD_NAME;
    SpaceMining::EventLog log;
    SpaceMining::MiningManager miningManager(2, 1, &log, OneTimeMining);
    SpaceMining::Station testStation(0);
    if (testName == TEST1) {
        printf("\nRunning test 1\n");
        SpaceMining::Event finishMining0 = miningManager.HandleEvent( { SpaceMining::ARRIVE_AT_MINING_SITE, 0, 0 });
        SpaceMining::Event finishMining1 = miningManager.HandleEvent( { SpaceMining::ARRIVE_AT_MINING_SITE, 0, 1 });
        if (finishMining0.eventType != SpaceMining::FINISH_MINING) {
            printf("Handling ARRIVE_AT_MINING_SITE 0 returns incorrect event\n");
            return EXIT_FAILURE;
        }
        if (finishMining1.eventType != SpaceMining::FINISH_MINING) {
            printf("Handling ARRIVE_AT_MINING_SITE 1 returns incorrect event\n");
            return EXIT_FAILURE;
        }
        miningManager.HandleEvent(finishMining0);
        int station = miningManager.GetNextMiningStation();
        if (station != 0) {
            printf("GetNextMiningStation returns incorrectly\n");
            return EXIT_FAILURE;
        }
        int rsvdTime = miningManager.stations[station].GetRsvdTime();
        if (rsvdTime != 36) {
            printf("Did not reserve time properly %d", rsvdTime);
            return EXIT_FAILURE;
        }
        miningManager.HandleEvent(finishMining1);
        rsvdTime = miningManager.stations[station].GetRsvdTime();
        if (rsvdTime != 41) {
            printf("Did not reserve time properly %d", rsvdTime);
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    } else if (testName == TEST2) {
        SpaceMining::Station testStation(0);
        int rsvdTime = testStation.ReserveNextTime(0);
        if (rsvdTime != 35) {
            printf("Incorrect rsvd time %d", rsvdTime);
            return EXIT_FAILURE;
        }
        if (testStation.GetRsvdTime() != 35) {
            printf("GetRsvdTime doesnt match %d", testStation.GetRsvdTime());
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    } else {
        printf("\nBad test name\n");
        return EXIT_FAILURE;
    }
    return 0;
}

