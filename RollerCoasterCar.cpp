#include "RollerCoasterCar.h"
#include "Passenger.h"
#include <chrono>
#include <thread>
#include <iostream>

RollerCoasterCar::RollerCoasterCar( int id, int capacity, std::queue<Passenger *> &queue, 
                                    std::mutex &mu, std::condition_variable &cv, std::mutex &muTerminal) :
    _ID{id},
    _CAPACITY{capacity},
    _passengerQueue{queue},
    _cvQueue{cv},
    _muQueue{mu},
    _muTerminal{muTerminal}
{ 
    _riding = false;
    _totalPassengers = 0;
    _totalBoarded = 0;

    std::random_device rd;
    auto seed = rd();
    _rng.seed(seed);
}

void RollerCoasterCar::thread()
{
    while(true)
    {
        _loadPassengers();
        _waitAllBoarded();

        _run();

        _unloadPassengers();
        _waitAllUnboarded();

        _safePrint("All passengers from Car " + std::to_string(_ID) + " unboarded!.");
        _totalPassengers = 0;
    }
    
}

void RollerCoasterCar::board()
{
    {
        std::unique_lock<std::mutex> lock(_muTotalBoarded);
        _totalBoarded++;
    }

    _cvTotalBoarded.notify_all();
}

void RollerCoasterCar::unboard()
{
    {
        std::unique_lock<std::mutex> lock(_muTotalBoarded);
        _totalBoarded--;
    }
    
    _cvTotalBoarded.notify_all();
}

const int RollerCoasterCar::id()
{
    return _ID;
}

void RollerCoasterCar::_loadPassengers()
{
    while(_totalPassengers < _CAPACITY)
    {
        Passenger *pPass = _waitPassengerAvailable();
        _carPassengers.push_back(pPass);
        pPass->load(this);
        _totalPassengers++;
    }
}

void RollerCoasterCar::_unloadPassengers()
{
    for (auto &pPass : _carPassengers)
    {
        _safePrint("Car " + std::to_string(_ID) + " dropping Passenger " + std::to_string(pPass->id()));
        pPass->unload();
    }

    _carPassengers.clear();
}

Passenger *RollerCoasterCar::_waitPassengerAvailable()
{
    Passenger *ret = nullptr;

    _safePrint("Car " + std::to_string(_ID) + " waiting for passengers!");

    {
        std::unique_lock<std::mutex> lock(_muQueue);
        _cvQueue.wait(lock, [&](){return !_passengerQueue.empty();});

        ret = _passengerQueue.front();
        _passengerQueue.pop();
    }

    _safePrint("Car " + std::to_string(_ID) + " picking up Passenger " + std::to_string(ret->id()));

    return ret;
}

void RollerCoasterCar::_run()
{
    std::uniform_real_distribution<double> uFunc(0, 1);
    int randTime = uFunc(_rng) * _BASE_RUN_TIME/2.0 + _BASE_RUN_TIME/2;

    _safePrint("Car " + std::to_string(_ID) + " running!");
    std::this_thread::sleep_for(std::chrono::milliseconds(randTime));
    _safePrint("Car " + std::to_string(_ID) + " run finished!");
} 

void RollerCoasterCar::_waitAllBoarded()
{
    std::unique_lock<std::mutex> lock(_muTotalBoarded);
    _cvTotalBoarded.wait(lock, [&](){return (_totalBoarded == _totalPassengers);});
}

void RollerCoasterCar::_waitAllUnboarded()
{
    std::unique_lock<std::mutex> lock(_muTotalBoarded);
    _cvTotalBoarded.wait(lock, [&](){return (_totalBoarded == 0);});
}

void RollerCoasterCar::_safePrint(std::string msg)
{
    std::unique_lock<std::mutex> lock(_muTerminal);
    std::cout << msg << std::endl;
}
