#include "util.h"
#include "pid.h"

#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

int main()
{
    HANDLE hSerial;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts={0};

    init(hSerial, dcbSerialParams, timeouts);

    sendread(hSerial, 1);
    sendread(hSerial, 125);
    sendread(hSerial, 2);
    sendread(hSerial, 125);
    sendread(hSerial, 5);
    sendread(hSerial, 6);

    char ch;
    uint8_t k=0;
    uint16_t adc;
    DWORD dwBytesRead = 0;

    int shiftx = 640, shifty = 480;
    int desx = 100, desy = 100;
    Mat rob = Mat(Size(1280, 960), CV_8U, Scalar(0));
    circle(rob, Point(desx + shiftx, desy + shifty), 5, Scalar(100, 100, 100), -1);
    namedWindow("robot",0);
    resizeWindow("robot", 640, 480);
    pid robot(pos(desx, 0, 0));
    char c;
    int l = 1;
    while (!robot.run(hSerial))
    {
        //for (int i = 0; i < 16; ++i)
        //{
          //  Sleep(10);
          //  robot.changeSpeed(hSerial);
        //}
        if (robot.isThere())
        {
            switch(l)
            {
                case 0:
                    robot.setDestination(pos(desx, 0, 0));
                    ++l;
                    break;
                case 1:
                    robot.setDestination(pos(desx, -desy, 0));
                    ++l;
                    break;
                case 2:
                    robot.setDestination(pos(0, -desy, 0));
                    ++l;
                    break;
                case 3:
                    robot.setDestination(pos(0, 0, 0));
                    l = 0;
                    break;
            }
        }
        robot.setSpeed(hSerial);
        Sleep(100);
        line(rob, Point(robot.getLastPosition().x + shiftx, robot.getLastPosition().y + shifty), Point(robot.getPosition().x + shiftx, robot.getPosition().y + shifty), Scalar(200, 200, 200), 2);
        imshow("robot", rob);
        c = waitKey(20);
        if((char)c==27 ) break;
    }
    /*while (l != 99)
    {
        cin >> l;
        k = l;
        sendread(hSerial, k);
    }*/
    sendread(hSerial, 1);
    sendread(hSerial, 125);
    sendread(hSerial, 2);
    sendread(hSerial, 125);
    CloseHandle(hSerial);
    cout << "enkoder1 " << robot.ensuml << " enkoder2 " << robot.ensumr << endl;
    return 0;
}
