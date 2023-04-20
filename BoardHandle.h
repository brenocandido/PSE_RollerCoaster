#ifndef BOARDHANDLE_H_
#define BOARDHANDLE_H_

#include <mutex>
#include <condition_variable>

class BoardHandle
{
public:
    BoardHandle(int carId);

    void board();
    void unboard();

    int carId();

    void waitAllBoarded(int totalPassengers);
    void waitAllUnboarded();

private:
    const int _CAR_ID;
    int _totalBoarded;

    std::mutex _muTotalBoarded;
    std::condition_variable _cvTotalBoarded;
};

#endif // BOARDHANDLE_H_