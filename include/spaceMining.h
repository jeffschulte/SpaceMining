#include <vector>
#include <random>
#include <set>

namespace SpaceMining {
    
#define TRUCK_TRAVEL_TIME 30 // thirty minutes to travel from site to station
#define TRUCK_UNLOAD_TIME 5 // 5 minutes to unload load at station
//#define TOTAL_SIMULATION_TIME 72*60 // 72 hours of total simulation
#define TOTAL_SIMULATION_TIME 1500 // 72 hours of total simulation

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
    int timeSpentWaiting = 0;
};

// This is the sorted list (sorted in time) of all the events.
class EventLog {
    std::vector<Event> eventLog;
public:
    void AddEvent(Event event);
    void PrintLog();
    void PrintTruckStats(int truck);
    void PrintStationStats(int station);
};

// The stations keep track of their latest reservation time, which can be quaried and updated
class Station {
    int rsvdUntil = 0;
public:
    int GetRsvdTime();
    int ReserveNextTime(int currentTime);
    Station(int initRsvdUntil) : rsvdUntil(initRsvdUntil) {};
};

// The conducting class is the MiningManager, and we use singleton pattern to ensure that there is only one of these.
// It holds the event list, which the core loop of the program, RunSimulation(), will loop through.
// It also holds a list of the stations
// Also holds the event log, which has copies of all the events that occured along the way. 
typedef int (*GetMiningTimeFunc)();
class MiningManager {
    static MiningManager* singleInstance;
    struct EventCompare {
        bool operator()(const Event& lhs, const Event& rhs) const {
            return lhs.time < rhs.time; // Order elements in descending order
        };
    };
    std::multiset<Event, EventCompare> eventList;
    std::vector<Station> stations;
    GetMiningTimeFunc GetMiningTime;

    Event HandleEvent(Event event);
    int GetNextMiningStation();
    EventLog* log;
public:
    MiningManager(int numTrucks, int numStations, EventLog* log, GetMiningTimeFunc func);
    // Singleton design pattern - disallow copying by deleting copy and assignment constructors
    MiningManager(const MiningManager&) = delete;
    MiningManager& operator=(const MiningManager&) = delete;
    static MiningManager* GetInstance(int numTrucks, int numStations, EventLog* log, GetMiningTimeFunc func);
    void RunSimulation();
};

// Util functions
void PrintArt();
void PrintUsage();
void ParseArgs(int argc, char* argv[], int* numTrucks, int* numStations, bool* printLog);

} // namespace SpaceMining