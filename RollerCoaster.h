#ifndef ROLLERCOASTER_H_
#define ROLLERCOASTER_H_

#include <queue>
#include <vector>

#include "Passenger.h"
#include "RollerCoasterCar.h"

class RollerCoaster
{
public:
    RollerCoaster(int totalCars, int totalPassengers, int carCapacity, int runsPerPassenger);
    ~RollerCoaster();
private:
    std::vector<Passenger> passengers;
    std::vector<RollerCoasterCar> cars;

    std::queue<Passenger *> passengerQueue;
};

#endif // ROLLERCOASTER_H_