#include "RollerCoaster.h"

RollerCoaster::RollerCoaster(   int totalCars, int totalPassengers, 
                                int carCapacity, int runsPerPassenger)
{ 
    for (int i = 0; i < totalPassengers; i++)
    {
        passengers.emplace_back(Passenger(runsPerPassenger));
    }

    for (int i = 0; i < totalCars; i++)
    {
        cars.emplace_back(RollerCoasterCar(carCapacity));
    }
    
    for (auto &el : passengers)
    {
        passengerQueue.push(&el);
    }
}

RollerCoaster::~RollerCoaster()
{ }
