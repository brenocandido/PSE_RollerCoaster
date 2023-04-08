#ifndef ROLLERCOASTERCAR_H_
#define ROLLERCOASTERCAR_H_

class RollerCoasterCar {
public:
    RollerCoasterCar(int capacity);

    void thread();

    void load();
    void unload();

private:
    const int _CAPACITY;

    int _totalPassangers;
};

#endif // ROLLERCOASTERCAR_H_