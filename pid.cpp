#include "pid.h"

#include "util.h"

#include <iostream>
#include <cmath>

using namespace std;

pid::pid(pos dest)
{
    position = pos(0,0,0);
    destination = dest;
    input = errorSum = er = lastEr = model(0,0);
    vp = speed();
    en = enkoders(0,0,0,0,0);
    v.p = 4;
    v.i = 0;
    v.d = 0;
    w.p = 20;
    w.i = 0;
    w.d = 0;
    destination.o = atan2((destination.y - position.y),(destination.x - position.x));
    rv = speed(125, 125);
    for (int i = 0; i < 10; ++i)
    {
        errors[i].v = 0;
        errors[i].w = 0;
    }
    county = 0;
    ensuml = ensumr = 0;
}

bool pid::run(HANDLE& hSerial)
{
    feedback(hSerial);
    er = error();
    errors[county] = er;
    errorSum = model(0,0);
    for (int i = 0; i < 10; ++i)
    {
        errorSum.v += errors[i].v;
        errorSum.w += errors[i].w;
    }
    county = (county+1)%10;
    input.v = v.p * er.v + v.d*(lastEr.v - er.v);
    if (errorSum.v < 400)
        input.v += v.i * errorSum.v;
    input.w = w.p * er.w + w.i*errorSum.w + w.d*(lastEr.w - er.w);
    //cout << vr << ' ' << vl << endl;
    vp.vr = -((2*input.v + input.w*L)/(2*R) + 0.4);
    vp.vl = ((2*input.v - input.w*L)/(2*R) + 0.4);
    if (vp.vr < -75)
    {
        vp.vr = -75;
    }
    else if (vp.vr > 75)
    {
        vp.vr = 75;
    }

    if (vp.vl < -75)
    {
        vp.vl = -75;
    }
    else if (vp.vl > 75)
    {
        vp.vl = 75;
    }
    vin.vr = vp.vr + 125;
    vin.vl = vp.vl + 125;
    //cout << input.v << ' ' << input.w << endl;
    //cout << er.v << ' ' << er.w << endl;
    //cout << vinr << ' ' << vinl << endl;
    cout << position.x << ' ' << position.y << ' ' << position.o << endl;
    cout << endl;
    /*if ((int)position.x == (int)destination.x && (int)position.y == (int)destination.y)
        return true;
    else*/
        return false;
}

model pid::error()
{
    model er;
    er.v = sqrt((position.x - destination.x)*(position.x - destination.x) + (position.y - destination.y)*(position.y - destination.y));
    if (er.v > 110)//120
    {
        er.v = 110;
    }
    er.w = destination.o - position.o;
    er.w = atan2(sin(er.w), cos(er.w));
    return er;
}

void pid::changeSpeed(HANDLE& hSerial)
{
    if (rv.vr < vin.vr)
    {
        ++rv.vr;
        sendread(hSerial, 2);
        sendread(hSerial, rv.vr);
    }
    else if (rv.vr > vin.vr)
    {
        --rv.vr;
        sendread(hSerial, 2);
        sendread(hSerial, rv.vr);
    }

    if (rv.vl < vin.vl)
    {
        ++rv.vl;
        sendread(hSerial, 1);
        sendread(hSerial, rv.vl);
    }
    else if (rv.vl > vin.vl)
    {
        --rv.vl;
        sendread(hSerial, 1);
        sendread(hSerial, rv.vl);
    }
}

void pid::setSpeed(HANDLE& hSerial)
{
    sendread(hSerial, 1);
    sendread(hSerial, vin.vl);
    sendread(hSerial, 2);
    sendread(hSerial, vin.vr);
}

void pid::feedback(HANDLE& hSerial) //1 - L, 2 - R
{
    en.l = sendread(hSerial, 5);
    en.r = sendread(hSerial, 6);
    ensumr += en.r;
    ensuml += en.l;
    en.dl = 2 * PI * R * (en.l/N);
    en.dr = 2 * PI * R * (en.r/N);
    /*en.dl = vl*0.1;
    en.dr = vr*0.1;*/
    en.dc = (en.dl + en.dr)/2;
    lastPosition = position;
    position.x += en.dc*cos(position.o);
    position.y += en.dc*sin(position.o);
    position.o += (en.dr-en.dl)/L;
    position.o = atan2(sin(position.o), cos(position.o));
    destination.o = atan2((destination.y - position.y),(destination.x - position.x));
    destination.o = atan2(sin(destination.o), cos(destination.o));
}

bool pid::isThere()
{
    if (er.v < 4)
        return true;
    else
        return false;
}
