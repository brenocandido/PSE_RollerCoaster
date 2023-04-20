#ifndef ROLLERCOASTERCAR_H_
#define ROLLERCOASTERCAR_H_

#include <queue>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <random>

#include "Passenger.h"
#include "BoardHandle.h"

class RollerCoasterCar {
public:
    RollerCoasterCar(   int id, int capacity, std::queue<Passenger *> &queue, 
                        std::mutex &mu, std::condition_variable &cv, std::mutex &muTerminal);

    void thread();

    const int id();

private:
    const int _ID;
    const int _CAPACITY;
    const int _BASE_RUN_TIME = 5000;
    const int _BASE_LOAD_TIME = 1000;

    bool _riding;
    int _totalPassengers;
    int _totalBoarded;

    std::vector<Passenger *> _carPassengers;

    std::queue<Passenger *> &_passengerQueue;
    std::condition_variable &_cvQueue;
    std::mutex &_muQueue;

    std::mutex _muTotalBoarded;
    std::condition_variable _cvTotalBoarded;

    std::mutex &_muTerminal;

    std::mt19937_64 _rng;

    BoardHandle _boardHandle;

    void _loadPassengers();
    void _unloadPassengers();
    Passenger *_waitPassengerAvailable();
    void _run();

    void _safePrint(std::string msg);

    int _generateRandomTime(const int baseTime);
};

#endif // ROLLERCOASTERCAR_H_