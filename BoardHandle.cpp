#include "BoardHandle.h"

BoardHandle::BoardHandle(int carId) :
    _CAR_ID{carId}
{
    _totalBoarded = 0;
}

void BoardHandle::board()
{
    {
        std::unique_lock<std::mutex> lock(_muTotalBoarded);
        _totalBoarded++;
    }

    _cvTotalBoarded.notify_all();
}

void BoardHandle::unboard()
{
    {
        std::unique_lock<std::mutex> lock(_muTotalBoarded);
        _totalBoarded--;
    }
    
    _cvTotalBoarded.notify_all();
}

int BoardHandle::carId()
{
    return _CAR_ID;
}

void BoardHandle::waitAllBoarded(int totalPassengers)
{
    std::unique_lock<std::mutex> lock(_muTotalBoarded);
    _cvTotalBoarded.wait(lock, [&](){return (_totalBoarded == totalPassengers);});
}

void BoardHandle::waitAllUnboarded()
{
    std::unique_lock<std::mutex> lock(_muTotalBoarded);
    _cvTotalBoarded.wait(lock, [&](){return (_totalBoarded == 0);});
}