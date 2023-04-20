#ifndef PASSENGER_H_
#define PASSENGER_H_

#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>
#include <random>

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
    const int _BASE_BOARD_TIME = 2500;
    
    bool _carAssigned;

    BoardHandle *_boardHandle;

    std::queue<Passenger *> &_passengerQueue;
    std::mutex &_muQueue;
    std::condition_variable &_cvQueue;
    
    std::mutex _muLoad;
    std::condition_variable _cvLoad;

    std::mutex &_muTerminal;

    std::mt19937_64 _rng;

    void _joinPassengerQueue();
    void _waitCarAssigned();
    void _waitCarUnassigned();
    void _board();
    void _unboard();
    void _safePrint(std::string msg);

    int _generateBoardTime();
};

#endif // PASSENGER_H_