#include <iostream>
#include <spaceMining.h>


void PrintUsage() {
    std::cout << "Usage: ./spaceMining -n <number of trucks> -m <number of unload stations>\n";
}

void ParseArgs(int argc, char* argv[], int* numTrucks, int* numStations) {
    if (argc != 5) {
        PrintUsage();
        std::exit(1);
    }

    bool mFound = false, nFound = false;

    for (int i = 1; i < argc; i += 2) {
        std::string arg = argv[i];
        if (arg == "-m") {
            *numStations = std::atoi(argv[i + 1]);
            mFound = true;
        } else if (arg == "-n") {
            *numTrucks = std::atoi(argv[i + 1]);
            nFound = true;
        } else {
            PrintUsage();
            std::exit(1);
        }
    }

    if (!mFound || !nFound) {
        PrintUsage();
        std::exit(1);
    }
}

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
    int value = spaceMiningFunction();
    printf("Hello World! %d", value);
    int numTrucks, numStations;
    ParseArgs(argc, argv, &numTrucks, &numStations);
    printf("\n%d %d\n", numTrucks, numStations);
    MiningTimeRandomizer randomizer;
    gRandomizer = &randomizer;
    EventLog log;
    MiningManager miningManager(numTrucks, numStations, &log, GetMiningTimeGlobalWrapper);
    miningManager.RunSimulation();
    //log.PrintLog();
    for(int i=0;i<numTrucks;i++) {
        log.PrintTruckStats(i);
    }
    for(int i=0;i<numStations;i++) {
        log.PrintStationStats(i);
    }
    return 0;
}