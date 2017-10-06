//
// Created by bobin on 17-10-7.
//

#ifndef DUOREAD_DUOREADER_H
#define DUOREAD_DUOREADER_H


#include <DUOLib.h>
#include <opencv2/opencv.hpp>

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#define WAIT_OBJECT_0    0

struct event_flag {
    pthread_mutex_t mutex;
    pthread_cond_t condition;
    unsigned int flag;
};


class DUOReader {
public:
    // member functionss
    DUOReader();

    ~DUOReader();

    bool OpenDUOCamera(int width, int height, float fps);

    // Waits until the new DUO frame is ready and returns it
    PDUOFrame GetDUOFrame();

    // Stops capture and closes the camera
    void CloseDUOCamera();

    void StartDUOFrame(DUOFrameCallback frameCallback);

    void SetAutoExpose(bool value);

    void SetExposure(float value);

    void SetGain(float value);

    void SetLed(float value);

private:

    void initTermios(int echo);

    /* Restore old terminal i/o settings */
    void resetTermios(void);

    /* Read 1 character - echo defines echo mode */
    char getch_(int echo);

    /* Read 1 character without echo */
    char _getch(void);

    int _kbhit(void);



    event_flag *CreateEvent(void *lpEventAttributes, bool bManualReset, bool bInitialState, char *name);

    void SetEvent(struct event_flag *ev);

    int WaitForSingleObject(struct event_flag *ev, int timeout);

    event_flag *_evFrame;
    // Some member variables

    DUOInstance _duo = NULL;
    PDUOFrame _pFrameData = NULL;

    struct termios _old, _new;
};


#endif //DUOREAD_DUOREADER_H
