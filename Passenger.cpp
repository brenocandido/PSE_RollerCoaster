#include "Passenger.h"
#include "RollerCoasterCar.h"
#include <iostream>

Passenger::Passenger(int id, std::queue<Passenger *> &queue, std::mutex &mu, std::condition_variable &cv, std::mutex &muTerminal) :
    _ID{id},
    _passengerQueue{queue},
    _muQueue{mu},
    _cvQueue{cv},
    _muTerminal{muTerminal}
{ 
    _carAssigned = false;
}

void Passenger::thread()
{
    while(true)
    {
        _joinPassengerQueue();

        _waitCarAssigned();

        _safePrint("Passenger " + std::to_string(_ID) + " boarding Car " + std::to_string(_assignedCar->id()));
        _assignedCar->board();

        _waitCarUnassigned();

        _safePrint("Passenger " + std::to_string(_ID) + " unboarding Car " + std::to_string(_assignedCar->id()));
        _assignedCar->unboard();

        _assignedCar = nullptr;
    }
}

void Passenger::load(RollerCoasterCar *car)
{
    {
        std::unique_lock<std::mutex> lock(_muLoad);
        _carAssigned = true;
    }

    _assignedCar = car;
    _cvLoad.notify_all();
}

void Passenger::unload()
{
    {
        std::unique_lock<std::mutex> lock(_muLoad);
        _carAssigned = false;
    }

    _cvLoad.notify_all();
}

const int Passenger::id()
{
    return _ID;
}

void Passenger::_joinPassengerQueue()
{
    _safePrint("Passenger " + std::to_string(_ID) + " joined waiting queue!");

    std::unique_lock<std::mutex> lock(_muQueue);
    _passengerQueue.push(this);
    _cvQueue.notify_all();
}

void Passenger::_waitCarAssigned()
{
    std::unique_lock<std::mutex> lock(_muLoad);
    _cvLoad.wait(lock, [&](){return _carAssigned;});
}

void Passenger::_waitCarUnassigned()
{
    std::unique_lock<std::mutex> lock(_muLoad);
    _cvLoad.wait(lock, [&](){return !_carAssigned;});
}

void Passenger::_safePrint(std::string msg)
{
    std::unique_lock<std::mutex> lock(_muTerminal);
    std::cout << msg << std::endl;
}
