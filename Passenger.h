#ifndef PASSENGER_H_
#define PASSENGER_H_

#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>

// Forward declaration
// RollerCoasterCar file is included in .cpp only
class RollerCoasterCar;

class Passenger
{
public:
    Passenger(int id, std::queue<Passenger *> &queue, std::mutex &mu, std::condition_variable &cv, std::mutex &muTerminal);

    void thread();

    void load(RollerCoasterCar *car);
    void unload();

    const int id();

private:
    const int _ID;
    
    bool _carAssigned;

    RollerCoasterCar *_assignedCar;

    std::queue<Passenger *> &_passengerQueue;
    std::mutex &_muQueue;
    std::condition_variable &_cvQueue;
    
    std::mutex _muLoad;
    std::condition_variable _cvLoad;

    std::mutex &_muTerminal;

    void _joinPassengerQueue();
    void _waitCarAssigned();
    void _waitCarUnassigned();
    void _safePrint(std::string msg);
};

#endif // PASSENGER_H_