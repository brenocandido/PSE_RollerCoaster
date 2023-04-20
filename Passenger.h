#ifndef PASSENGER_H_
#define PASSENGER_H_

#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>

#include "BoardHandle.h"

class Passenger
{
public:
    Passenger(int id, std::queue<Passenger *> &queue, std::mutex &mu, std::condition_variable &cv, std::mutex &muTerminal);

    void thread();

    void load(BoardHandle *boardHandle);
    void unload();

    const int id();

private:
    const int _ID;
    
    bool _carAssigned;

    BoardHandle *_boardHandle;

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