#include <iostream>
#include <spaceMining.h>

class MiningTimeRandomizer {
    std::random_device rd;
    std::mt19937 gen;
public:
    MiningTimeRandomizer() : gen(rd()){}
    int GetMiningTime() {
        std::uniform_int_distribution<int> distrib(60, 300);
        return int(std::floor(distrib(gen)));
    }
};

MiningTimeRandomizer* gRandomizer = nullptr;

int GetMiningTimeGlobalWrapper() {
    return gRandomizer->GetMiningTime();
}


int main(int argc, char* argv[]) {
    SpaceMining::PrintArt();
    int numTrucks, numStations;
    bool printLog = false;
    SpaceMining::ParseArgs(argc, argv, &numTrucks, &numStations, &printLog);
    printf("\nNumber of Trucks: %d, Number of Stations: %d\n", numTrucks, numStations);
    MiningTimeRandomizer randomizer;
    gRandomizer = &randomizer;
    SpaceMining::EventLog log;
    SpaceMining::MiningManager miningManager(numTrucks, numStations, &log, GetMiningTimeGlobalWrapper);
    miningManager.RunSimulation();
    if (printLog) log.PrintLog();
    for(int i=0;i<numTrucks;i++) {
        log.PrintTruckStats(i);
    }
    for(int i=0;i<numStations;i++) {
        log.PrintStationStats(i);
    }
    return 0;
}