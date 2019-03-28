#ifndef PID_H
#define PID_H

#include "types.h"
#include <windows.h>

class pid
{
    private:
        const double R = 5.9, L = 39.6, N = 36;
        model input;
        pidreg v, w;
        pos position, lastPosition, destination;
        model errorSum;
        model er;
        model lastEr;
        enkoders en;
        speed vp, vin;
        speed rv; // aktualna predkosc robota
        model error();
        void feedback(HANDLE& hSerial);
        model errors[10];
        int county;
    public:
        int ensuml, ensumr;

        pid(pos dest);
        bool run(HANDLE& hSerial);
        void changeSpeed(HANDLE& hSerial);
        pos getPosition(){return position;};
        pos getLastPosition(){return lastPosition;}
        void setSpeed(HANDLE& hSerial);
        void setDestination(pos newDestination){destination = newDestination;};
        speed getSpeed(){return rv;};
        bool isThere();
};

#endif // PID_H
