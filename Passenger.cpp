#include "Passenger.h"
#include <iostream>
#include <thread>

Passenger::Passenger(int id, std::queue<Passenger *> &queue, std::mutex &mu, std::condition_variable &cv, std::mutex &muTerminal) :
    _ID{id},
    _boardHandle{nullptr},
    _passengerQueue{queue},
    _muQueue{mu},
    _cvQueue{cv},
    _muTerminal{muTerminal}
{ 
    _carAssigned = false;

    std::random_device rd;
    auto seed = rd();
    _rng.seed(seed);
}

void Passenger::thread()
{
    while(true)
    {
        _joinPassengerQueue();

        _waitCarAssigned();
        _board();

        _waitCarUnassigned();
        _unboard();
    }
}

void Passenger::load(BoardHandle *boardHandle)
{
    {
        std::unique_lock<std::mutex> lock(_muLoad);
        _carAssigned = true;
        _boardHandle = boardHandle;
    }

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

void Passenger::_board()
{
    int randTime = _generateBoardTime();

    std::this_thread::sleep_for(std::chrono::milliseconds(randTime));
    _safePrint("Passenger " + std::to_string(_ID) + " boarding Car " + std::to_string(_boardHandle->carId()) +
                " after " + std::to_string(randTime) + " milliseconds!");

    _boardHandle->board();
}

void Passenger::_unboard()
{
    int randTime = _generateBoardTime();

    std::this_thread::sleep_for(std::chrono::milliseconds(randTime));
    _safePrint("Passenger " + std::to_string(_ID) + " unboarding Car " + std::to_string(_boardHandle->carId()) +
                " after " + std::to_string(randTime) + " milliseconds!");

    _boardHandle->unboard();
    _boardHandle = nullptr;
}

void Passenger::_safePrint(std::string msg)
{
    std::unique_lock<std::mutex> lock(_muTerminal);
    std::cout << msg << std::endl;
}

int Passenger::_generateBoardTime()
{
    std::uniform_real_distribution<double> uFunc(0, 1);
    int randTime = uFunc(_rng) * _BASE_BOARD_TIME/2.0 + _BASE_BOARD_TIME/2;

    return randTime;
}
