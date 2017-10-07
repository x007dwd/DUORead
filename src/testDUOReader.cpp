//
// Created by bobin on 17-10-7.
//

#include "DUOReader.h"

void CALLBACK DUOCallback(const PDUOFrame pFrameData, void *pUserData) {
    PDUOFrame _pFrameData = pFrameData;
//    SetEvent(_evFrame);
    cv::Mat left, right;
    left.create(_pFrameData->height, _pFrameData->width, CV_8U);
    right.create(_pFrameData->height, _pFrameData->width, CV_8U);
    left.data = _pFrameData->leftData;
    right.data = _pFrameData->rightData;
    std::stringstream ss, ss1;
    ss << "left" << _pFrameData->timeStamp << ".png";
    std::string left_name = ss.str();
    ss1 << "right" << _pFrameData->timeStamp << ".png";
    std::string right_name = ss1.str();
    cv::imwrite(left_name, left);
    cv::imwrite(right_name, right);


    printf("DUO Frame Timestamp: %10.1f ms\n", pFrameData->timeStamp / 10.0f);
    if (pFrameData->IMUPresent) {
        for (int i = 0; i < pFrameData->IMUSamples; i++) {
            printf(" Sample #%d\n", i + 1);
            printf("  Accelerometer: [%8.5f, %8.5f, %8.5f]\n", pFrameData->IMUData[i].accelData[0],
                   pFrameData->IMUData[i].accelData[1],
                   pFrameData->IMUData[i].accelData[2]);
            printf("  Gyro:          [%8.5f, %8.5f, %8.5f]\n", pFrameData->IMUData[i].gyroData[0],
                   pFrameData->IMUData[i].gyroData[1],
                   pFrameData->IMUData[i].gyroData[2]);
            printf("  Temperature:   %8.6f C\n", pFrameData->IMUData[i].tempData);
        }
    }
    printf("------------------------------------------------------\n");
}

#define WIDTH    752
#define HEIGHT    480
#define FPS        30

int main(int argc, char**argv){
    DUOReader duo_reader;
    duo_reader.OpenDUOCamera(WIDTH, HEIGHT, FPS);
    duo_reader.SetGain(0);
//	duo_reader.SetExposure(50);
    duo_reader.SetAutoExpose(true);
    duo_reader.SetLed(25);
    duo_reader.StartDUOFrame(DUOCallback);

}
