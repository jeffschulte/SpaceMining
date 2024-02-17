#include <spaceMining.h>
#include <cmath>

int spaceMiningFunction() {
    return 5;
}

void EventLog::AddEvent(Event event) { 
    eventLog.push_back(event);
}

int Station::GetRsvdTime() {
    return rsvdUntil;
}

int Station::ReserveNextTime(int currentTime) {
    if (rsvdUntil > currentTime + TRUCK_TRAVEL_TIME) {
        rsvdUntil += TRUCK_UNLOAD_TIME;
        return rsvdUntil;
    } else {
        rsvdUntil = currentTime + TRUCK_UNLOAD_TIME;
        return rsvdUntil;
    }
}

MiningManager::MiningManager(int numTrucks, int numStations, EventLog* log) : stations(numTrucks, Station(0)), gen(rd()), log(log) {
    for (int i=0;i<numTrucks;i++) {
        int arriveAtFirstStationTime = GetMiningTime();
        eventList.insert({ ARRIVE_AT_MINING_SITE, arriveAtFirstStationTime, i });
    }
}

void MiningManager::RunSimulation() {
    // int currentEventTime = 0;
    // while (eventList.) {
    //     auto it = eventList.begin();
    //     if ((*it).time < )
    //     currentEventTime
    // }
    //     printf("\nevent time %d\n", (*it).time);
    //     eventList.erase(it);

}


int MiningManager::GetMiningTime() {
    std::uniform_int_distribution<int> distrib(60, 300);
    return int(std::floor(distrib(gen)));
}

int MiningManager::GetAndReserveNextMiningStation() {
    int currentMinRsvdUntil = TOTAL_SIMULATION_TIME;
    int stationNumber = -1;
    for (int i=0;i<stations.size();i++) {
        if (stations[i].GetRsvdTime() < currentMinRsvdUntil) {
            currentMinRsvdUntil = stations[i].GetRsvdTime();
            stationNumber = i;
        }
    }
    if (stationNumber == -1) {
        printf("\nIncorrect station number in GetAndReserveNextMiningStation\n");
        exit(1);
    }
    return stationNumber;
}



Event MiningManager::HandleEvent(Event event) {
    log->AddEvent(event);
    int miningTime;
    int stationToTravelTo;
    switch(event.eventType) {
        case ARRIVE_AT_MINING_SITE:
            // the truck will mine for miningTime and then FINISH_MINING at current + miningTime.
            miningTime = GetMiningTime();
            return { FINISH_MINING, event.time + miningTime, event.truckNumber };
        case FINISH_MINING:
            // the truck will get a reservation time at a specific station, then depart for that station.
            stationToTravelTo = GetAndReserveNextMiningStation();
            return { ARRIVE_AT_STATION, event.time + TRUCK_TRAVEL_TIME, event.truckNumber, stationToTravelTo, stations[stationToTravelTo].GetRsvdTime() };
        case ARRIVE_AT_STATION:
            // the truck will finish unloading at its pre-given reservation time
            return { FINISH_UNLOADING, event.truckReservationTime, event.truckNumber, event.stationNumber };
        case FINISH_UNLOADING:
            // the truck will depart for the mining site
            return { ARRIVE_AT_MINING_SITE, event.time + TRUCK_TRAVEL_TIME, event.truckNumber };
        default:
            printf("\nUnknown event type! %d\n", event.eventType);
            exit(1);
    }
}
