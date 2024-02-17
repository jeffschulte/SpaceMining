#include <vector>
#include <random>
#include <set>

int spaceMiningFunction();

#define TRUCK_TRAVEL_TIME 30 // thirty minutes to travel from site to station
#define TRUCK_UNLOAD_TIME 5 // 5 minutes to unload load at station
#define TOTAL_SIMULATION_TIME 72*60 // 72 hours of total simulation

enum EventType {
    ARRIVE_AT_MINING_SITE,
    FINISH_MINING,
    ARRIVE_AT_STATION,
    FINISH_UNLOADING,
};

struct Event {
    EventType eventType;
    int time;
    int truckNumber;
    int stationNumber = 0;
    int truckReservationTime = 0;
};

class EventLog {
    std::vector<Event> eventLog;
public:
    void AddEvent(Event event);
};

class Station {
    int rsvdUntil = 0;
public:
    int GetRsvdTime();
    int ReserveNextTime(int currentTime);
    Station(int initRsvdUntil) : rsvdUntil(initRsvdUntil) {};
};

// maybe do singleton pattern on the Mining Manager as a whole, since there must be only one
class MiningManager {
    struct EventCompare {
        bool operator()(const Event& lhs, const Event& rhs) const {
            return lhs.time < rhs.time; // Order elements in descending order
        };
    };
    std::set<Event, EventCompare> eventList;
    std::vector<Station> stations;
    std::random_device rd; // JEFF not doing random correctly
    std::mt19937 gen;

    int GetMiningTime();
    Event HandleEvent(Event event);
    int GetAndReserveNextMiningStation();
    EventLog* log;
public:
    MiningManager(int numTrucks, int numStations, EventLog* log);
    void RunSimulation();
};

