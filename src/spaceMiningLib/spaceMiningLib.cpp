#include <spaceMining.h>
#include <cmath>

int spaceMiningFunction() {
    return 5;
}

void EventLog::AddEvent(Event event) { 
    eventLog.push_back(event);
}

void EventLog::PrintTruckStats(int truck) {
    std::vector<Event> truckLog;
    for (int i=0;i<eventLog.size();i++) {
        if (eventLog[i].truckNumber == truck) {
            truckLog.push_back(eventLog[i]);
        }
    }
    int numberLoadsMined = 0;
    int timeSpentWaitingToUnload = 0;
    for (int i=0;i<truckLog.size();i++) {
        //printf("\nin %d %d", truckLog[i].time, truckLog[i-1].time);
        switch (truckLog[i].eventType)
        {
            case ARRIVE_AT_MINING_SITE:
                break;
            case FINISH_MINING:
                numberLoadsMined++;
                break;
            case ARRIVE_AT_STATION:
                break;
            case FINISH_UNLOADING:
                //printf("\n%d %d %d %d\n", truckLog[i-1].eventType, truckLog[i].time, truckLog[i-1].time, TRUCK_UNLOAD_TIME);
                timeSpentWaitingToUnload += (truckLog[i].time - truckLog[i-1].time - TRUCK_UNLOAD_TIME);
                break;
            default:
                printf("\nBadTruckLogEntry\n");
                exit(1);
        }
    }
    printf("\nTruck Number %d: NumberLoadsMined = %d, TimeSpentWaitingToUnload = %d\n", truck, numberLoadsMined, timeSpentWaitingToUnload);
}

void EventLog::PrintStationStats(int station) {
    std::vector<Event> stationLog;
    for (int i=0;i<eventLog.size();i++) {
        if (eventLog[i].truckNumber == station) {
            stationLog.push_back(eventLog[i]);
        }
    }
    int totalWaitTime = 0;
    int totalNumberTrucksUnloaded = 0;
    int j;
    for (int i=0;i<stationLog.size();i++) {
        if (stationLog[i].eventType == FINISH_UNLOADING) {
            totalNumberTrucksUnloaded++;
            j = i - 1;
            while (stationLog[j].truckNumber != stationLog[i].truckNumber) j--;
            totalWaitTime += stationLog[i].time - stationLog[j].time - TRUCK_UNLOAD_TIME;
        }
    }
    printf("\nStation Number %d: TotalNumberTrucksUnloaded = %d, AverageWaitTime = %d\n", station, totalNumberTrucksUnloaded, totalWaitTime/totalNumberTrucksUnloaded);
}

void EventLog::PrintLog() {
    printf("\n\neventType,time,truckNumber,stationNumber,truckReservationTime\n");
    for (int i=0;i<eventLog.size();i++) {
        if (eventLog[i].truckNumber == 0) {
            printf("%d,%d,%d,%d,%d\n", eventLog[i].eventType, eventLog[i].time, eventLog[i].truckNumber, eventLog[i].stationNumber, eventLog[i].truckReservationTime);
        }
    }
}

int Station::GetRsvdTime() {
    return rsvdUntil;
}

int Station::ReserveNextTime(int currentTime) {
    if (rsvdUntil > currentTime + TRUCK_TRAVEL_TIME) {
        rsvdUntil += TRUCK_UNLOAD_TIME;
        return rsvdUntil;
    } else {
        printf("current %d, rsvdUntil %d\n", currentTime, rsvdUntil);
        rsvdUntil = currentTime + TRUCK_TRAVEL_TIME + TRUCK_UNLOAD_TIME;
        return rsvdUntil;
    }
}

MiningManager::MiningManager(int numTrucks, int numStations, EventLog* log, GetMiningTimeFunc func) : stations(numTrucks, Station(0)), log(log), GetMiningTime(func) {
    for (int i=0;i<numTrucks;i++) {
        eventList.insert({ ARRIVE_AT_MINING_SITE, 0, i });
    }
}

void MiningManager::RunSimulation() {
    int currentEventTime = 0;
    //int debug = 0;
    while (eventList.size() > 0) {
        //debug++;
        //if(debug > 10) exit(1);
        auto it = eventList.begin();
        Event nextEvent = HandleEvent(*it);
        //printf("\n DEBUG %d %d, %d %d,\n", (*it).eventType, (*it).time, nextEvent.eventType, nextEvent.time);
        if (nextEvent.time < TOTAL_SIMULATION_TIME) {
            //printf("\n DEBUG isnide\n");
            eventList.insert(nextEvent);
        }
        eventList.erase(it);
    }
}


int MiningManager::GetNextMiningStation() {
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
    int newRsvdTime;
    switch(event.eventType) {
        case ARRIVE_AT_MINING_SITE:
            // the truck will mine for miningTime and then FINISH_MINING at current + miningTime.
            miningTime = GetMiningTime();
            return { FINISH_MINING, event.time + miningTime, event.truckNumber };
        case FINISH_MINING:
            // the truck will get a reservation time at a specific station, then depart for that station.
            stationToTravelTo = GetNextMiningStation();
            newRsvdTime = stations[stationToTravelTo].ReserveNextTime(event.time);
            printf("RSVD TIME %d\n", stations[stationToTravelTo].GetRsvdTime());
            return { ARRIVE_AT_STATION, event.time + TRUCK_TRAVEL_TIME, event.truckNumber, stationToTravelTo, newRsvdTime };
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
