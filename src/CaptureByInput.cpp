﻿/************************************************************************************************/
// CaptureByInput.cpp : Defines the entry point for the console application.

// MAGEWELL PROPRIETARY INFORMATION

// The following license only applies to head files and library within Magewell’s SDK 
// and not to Magewell’s SDK as a whole. 

// Copyrights © Nanjing Magewell Electronics Co., Ltd. (“Magewell”) All rights reserved.

// Magewell grands to any person who obtains the copy of Magewell’s head files and library 
// the rights,including without limitation, to use, modify, publish, sublicense, distribute
// the Software on the conditions that all the following terms are met:
// - The above copyright notice shall be retained in any circumstances.
// -The following disclaimer shall be included in the software and documentation and/or 
// other materials provided for the purpose of publish, distribution or sublicense.

// THE SOFTWARE IS PROVIDED BY MAGEWELL “AS IS” AND ANY EXPRESS, INCLUDING BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL MAGEWELL BE LIABLE 

// FOR ANY CLAIM, DIRECT OR INDIRECT DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT,
// TORT OR OTHERWISE, ARISING IN ANY WAY OF USING THE SOFTWARE.

// CONTACT INFORMATION:
// SDK@magewell.net
// http://www.magewell.com/
//
/************************************************************************************************/

#ifdef OPEN_CV_INSTALLED
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#endif

#include <stdio.h>
#include <string.h>
#include <sys/eventfd.h>
#include <sys/select.h>
#include <unistd.h>
#include <iostream>

#include "MWFOURCC.h"
#include "LibMWCapture/MWCapture.h"
#include "LibMWCapture/MWEcoCapture.h"

#include "bitmap.h"

#define NUM_CAPTURE 100

#define MAX_BUFFER_COUNT 4
#define CAPTURE_WIDTH 1920
#define CAPTURE_HEIGHT 1080
#define CAPTURE_FOURCC MWFOURCC_BGR24
typedef int mw_event_t;
int EcoEventWait(mw_event_t event, int timeout/*ms*/)
{
	fd_set rfds;
	struct timeval tv;
	struct timeval *ptv = NULL;
	eventfd_t value = 0;
	int retval;

	FD_ZERO(&rfds);
	FD_SET(event, &rfds);

	if (timeout < 0) {
		ptv = NULL;
	} else if (timeout == 0) {
		tv.tv_sec = timeout / 1000;
		tv.tv_usec = (timeout % 1000) * 1000;
		ptv = &tv;
	} else {
		tv.tv_sec = timeout / 1000;
		tv.tv_usec = (timeout % 1000) * 1000;
		ptv = &tv;
	}

	retval = select(event + 1, &rfds, NULL, NULL, ptv);
	if (retval == -1)
		return retval;
	else if (retval > 0) {
		retval = eventfd_read(event, &value);
		if (value > 0) {
			return value;
		} else {
			return retval < 0 ? retval : -1;
		}
	}

	// timeout
	return 0;
}

void EcoCaptureTest(HCHANNEL hChannel)
{
    int iRet, i;
    mw_event_t event;
    LONGLONG llStartTm = 0;
    LONGLONG llCurrentTm = 0;
    MWCAP_VIDEO_ECO_CAPTURE_OPEN stEcoPpen;
    MWCAP_VIDEO_ECO_CAPTURE_FRAME stEcoFrame[MAX_BUFFER_COUNT];
    MWCAP_VIDEO_ECO_CAPTURE_STATUS stEcoStatus;
    memset(stEcoFrame, 0, sizeof(MWCAP_VIDEO_ECO_CAPTURE_FRAME) * MAX_BUFFER_COUNT);
    event = eventfd(0, EFD_NONBLOCK);
    if (0 == event) {
        printf("ERROR:alloc event failed\n");
        return;
    }

    DWORD dwMinStride=FOURCC_CalcMinStride(CAPTURE_FOURCC, CAPTURE_WIDTH, 4);
    DWORD dwImageSize=FOURCC_CalcImageSize(CAPTURE_FOURCC, CAPTURE_WIDTH, CAPTURE_HEIGHT, dwMinStride);
    stEcoPpen.cx = CAPTURE_WIDTH;
    stEcoPpen.cy = CAPTURE_HEIGHT;
    stEcoPpen.dwFOURCC = CAPTURE_FOURCC;
    stEcoPpen.hEvent = event;
    stEcoPpen.llFrameDuration = 400000;//25p/s; -1 for input frame rate 
    iRet = MWStartVideoEcoCapture(hChannel, &stEcoPpen);
    MWGetDeviceTime(hChannel, &llStartTm);
    if(stEcoPpen.llFrameDuration == -1){
        printf("fps by input\n");
    }else if(stEcoPpen.llFrameDuration > 0){
        printf("fps %f\n",10000000.0/stEcoPpen.llFrameDuration);
    }
    if (0 != iRet) {
        printf("ERROR:MWStartVideoEcoCapture failed! iRet=%d\n", iRet);
        goto end;
    }
    for (i = 0; i < MAX_BUFFER_COUNT; i++) {
        stEcoFrame[i].deinterlaceMode = MWCAP_VIDEO_DEINTERLACE_BLEND;
        stEcoFrame[i].cbFrame  = dwImageSize;
        stEcoFrame[i].pvFrame  = (unsigned long)malloc(stEcoFrame[i].cbFrame);
        stEcoFrame[i].cbStride = dwMinStride;
        stEcoFrame[i].bBottomUp = FALSE;
        if (stEcoFrame[i].pvFrame == 0) {
            printf("ERROR:alloc memory error\n");
            MWStopVideoEcoCapture(hChannel);
            goto end;
        }
        stEcoFrame[i].pvContext = (MWCAP_PTR)&stEcoFrame[i];
        memset((void *)stEcoFrame[i].pvFrame, 0, stEcoFrame[i].cbFrame);
        iRet = MWCaptureSetVideoEcoFrame(hChannel, &stEcoFrame[i]);
        if (0 != iRet) {
            printf("ERROR:MWCaptureSetVideoEcoFrame failed! iRet=%d\n", iRet);
            MWStopVideoEcoCapture(hChannel);
            goto end;
        }
    }
    for(i = 0; i < NUM_CAPTURE; i++){
        iRet = EcoEventWait(event, 1000);
        if(iRet <= 0){
            printf("event timeout or error! iRet=%d\n", iRet);
            continue;
        }
        memset(&stEcoStatus, 0, sizeof(stEcoStatus));
        iRet = MWGetVideoEcoCaptureStatus(hChannel, &stEcoStatus);
        if(0 != iRet){
            printf("ERROR:MWGetVideoEcoCaptureStatus failed! iRet=%d\n", iRet);
            break;
        }
        MWGetDeviceTime(hChannel, &llCurrentTm);
        char szBitmapName[64];
        sprintf(szBitmapName,"./video_%d.BMP",NUM_CAPTURE);
        if(i==NUM_CAPTURE-1){
            create_bitmap(szBitmapName, stEcoStatus.pvFrame, dwImageSize, CAPTURE_WIDTH, 0-CAPTURE_HEIGHT);
        }
        iRet = MWCaptureSetVideoEcoFrame(hChannel, (MWCAP_VIDEO_ECO_CAPTURE_FRAME *)stEcoStatus.pvContext);
        if(0 != iRet){
            printf("ERROR:MWCaptureSetVideoEcoFrame failed! iRet=%d\n", iRet);
            break;
        }
    }
    if(i >= NUM_CAPTURE-1){
        printf("\nStop capture\n");
        printf("\nAverage frame duration is %lld ms.\n", (LONGLONG)((llCurrentTm-llStartTm) / (NUM_CAPTURE * 10000)));
        printf("Write 100th frame to file Video_%d\n", NUM_CAPTURE);
    }

    MWStopVideoEcoCapture(hChannel);
    eventfd_write(event, 1);
end:
    for (i = 0; i < MAX_BUFFER_COUNT; i++) {
        if(stEcoFrame[i].pvFrame){
            free((void*)stEcoFrame[i].pvFrame);
        }
    }
    close(event);
}

int get_id(char c)
{
    if(c >= '0' && c <= '9')
        return (int)(c - '0');
    if(c >= 'a' && c <= 'f')
        return (int)(c - 'a' + 10);
    if(c >= 'A' && c <= 'F')
        return (int)(c - 'F' + 10);
    return 0;
}

HCHANNEL open_channel(int argc, char* argv[]){
    HCHANNEL hChannel = NULL;
    int nChannelCount = MWGetChannelCount();

    if (0 == nChannelCount) {
        printf("ERROR: Can't find channels!\n");
        return NULL;
    }
    printf("Find %d channels!\n",nChannelCount);
    int nProDevCount = 0;
    int nProDevChannel[32] = {-1};
    for (int i = 0; i < nChannelCount; i++){
        MWCAP_CHANNEL_INFO info;
        MW_RESULT mr = MWGetChannelInfoByIndex(i, &info);
        if ((strcmp(info.szFamilyName, "Pro Capture") == 0) || (strcmp(info.szFamilyName, "Eco Capture") == 0)){
            printf("find %s\n",info.szFamilyName);
            nProDevChannel[nProDevCount] = i;
            nProDevCount++;
        }
    }
    if (nProDevCount <= 0){
        printf("\nERROR: Can't find pro or eco channels!\n");
        return NULL;
    }

    printf("Find %d pro or eco channels.\n", nProDevCount);

    // Get <board id > <channel id> or <channel index>
    int byBoardId = -1;
    int byChannelId = -1;
    int nDevIndex = -1;
    BOOL bIndex = FALSE;

    MWCAP_CHANNEL_INFO videoInfo = { 0 };
    if (argc == 1) {
        if (MW_SUCCEEDED != MWGetChannelInfoByIndex(nProDevChannel[0], &videoInfo)) {
            printf("ERROR: Can't get channel info!\n");
            return NULL;
        }

        bIndex = TRUE;
        nDevIndex = 0;
    }
    else if (NULL == strstr(argv[1], ":")){
        bIndex = TRUE;
        if ((strlen(argv[1]) > 2) || (argv[1][0] > '9') || argv[1][0] < '0') {
            printf("\nERROR: Invalid params!\n");
            return NULL;
        }
        nDevIndex = atoi(argv[1]);
        if(nDevIndex >= nProDevCount){
            printf("ERROR: just have %d channel!\n",nProDevCount);
            return NULL;
        }
    }
    else{
        bIndex = FALSE;
        if (strlen(argv[1]) == 3){
            if ((argv[1][0] >= '0' && argv[1][0] <= '9') || (argv[1][0] >= 'a' && argv[1][0] <= 'f') || (argv[1][0] >= 'A' && argv[1][0] <= 'F')){
                byBoardId = get_id(argv[1][0]);//atoi(argv[1]);
            }

            if ((argv[1][2] >= '0' && argv[1][2] <= '3')){
                byChannelId = get_id(argv[1][2]);//atoi(&argv[1][2]);
            }
        }

        if (-1 == byBoardId || -1 == byChannelId) {
            printf("\nERROR: Invalid params!\n");
            return NULL;
        }
    }
    // Open channel
    if (bIndex == TRUE){
        char path[128] = {0};
        MWGetDevicePath(nProDevChannel[nDevIndex], path);
        hChannel = MWOpenChannelByPath(path);
        if (hChannel == NULL) {
            printf("ERROR: Open channel %d error!\n", nDevIndex);
            return NULL;
        }
    }
    else{
        hChannel = MWOpenChannel(byBoardId, byChannelId);
        if (hChannel == NULL) {
            printf("ERROR: Open channel %X:%d error!\n", byBoardId, byChannelId);
            return NULL;
        }
    }
    return hChannel;
}
void print_version_and_useage()
{
    
    BYTE byMaj, byMin;
    WORD wBuild;
    MWGetVersion(&byMaj, &byMin, &wBuild);
    printf("Magewell MWCapture SDK V%d.%d.%d - CaptureByInput\n",byMaj,byMin,wBuild);
    printf("USB Devices are not supported\n");
    printf("Usage:\n");
    printf("CaptureByInput <channel index>\n");
    printf("CaptureByInput <board id>:<channel id>\n\n");

}
bool check_file()
{
    FILE* wavFile = NULL;
    wavFile=fopen("temp.bmp","wb");
    if (NULL == wavFile){
        printf("ERROR: can't create file on now dir!\n");
        printf("\nPress ENTER to exit...\n");
        getchar();
        return false;
    }
    else{
        fclose(wavFile);
        wavFile = NULL;
        remove("temp.bmp");
    }
    return true;
}

int main2(int argc, char* argv[])
{
   // Version
    print_version_and_useage();
    //permission
    if(!check_file()){
        return 0;
    }

    if (argc > 2){
        printf("ERROR: Invalid params!\n");
        printf("\nPress 'Enter' to exit!\n");
        getchar();
        return 0;
    }

    if(!MWCaptureInitInstance()){
        printf("have InitilizeFailed");
    }

    HCHANNEL hChannel = NULL;
    HNOTIFY hNotify=0;
    MWCAP_PTR hNotifyEvent=0;
    MWCAP_PTR hCaptureEvent=0;

    HANDLE64 pbImage=0;

    LONGLONG llTotalTime=0LL;

    int nRet;

    do {
        MWRefreshDevice();
        hChannel = open_channel(argc, argv);
        if(NULL == hChannel){
            break;
        }
        MWCAP_CHANNEL_INFO videoInfo = { 0 };
        if (MW_SUCCEEDED != MWGetChannelInfo(hChannel, &videoInfo)) {
            printf("ERROR: Can't get channel info!\n");
            break;
        }

        printf("Open channel - BoardIndex = %X, ChannelIndex = %d.\n", videoInfo.byBoardIndex, videoInfo.byChannelIndex);
        printf("Product Name: %s\n", videoInfo.szProductName);
        printf("Board SerialNo: %s\n\n", videoInfo.szBoardSerialNo);
        if(0 == strcmp(videoInfo.szFamilyName, "Eco Capture")){
            EcoCaptureTest(hChannel);
            break;
        }
        // Capture frames on input signal frequency
        hCaptureEvent=MWCreateEvent();
        if(hCaptureEvent==0){
            printf("Create timer event error\n");
            break;
        }

        hNotifyEvent=MWCreateEvent();
        if(hNotifyEvent==0){
            printf("Create notify event error\n");
            break;
        }

        int cx=1920;
        int cy=1080;

        DWORD dwFourcc=MWFOURCC_BGR24;
        DWORD dwMinStride=FOURCC_CalcMinStride(dwFourcc,cx,4);
        DWORD dwImageSize=FOURCC_CalcImageSize(dwFourcc,cx,cy,dwMinStride);
        pbImage=(HANDLE64)(unsigned long)malloc(dwImageSize);

        MW_RESULT xr;
        xr = MWStartVideoCapture(hChannel, hCaptureEvent);
        if (xr != MW_SUCCEEDED) {
            printf("ERROR: Start Video Capture error!\n");
            break;
        }
        MWCAP_VIDEO_BUFFER_INFO videoBufferInfo;
        MWGetVideoBufferInfo(hChannel, &videoBufferInfo);

        MWCAP_VIDEO_FRAME_INFO videoFrameInfo;
        MWGetVideoFrameInfo(hChannel, videoBufferInfo.iNewestBufferedFullFrame, &videoFrameInfo);

        MWCAP_VIDEO_SIGNAL_STATUS videoSignalStatus;
        MWGetVideoSignalStatus(hChannel, &videoSignalStatus);

        switch (videoSignalStatus.state) {
        case MWCAP_VIDEO_SIGNAL_NONE:
            printf("ERRPR: Input signal status: NONE\n");
            break;
        case MWCAP_VIDEO_SIGNAL_UNSUPPORTED:
            printf("ERRPR: Input signal status: Unsupported\n");
            break;
        case MWCAP_VIDEO_SIGNAL_LOCKING:
            printf("ERRPR: Input signal status: Locking\n");
            break;
        case MWCAP_VIDEO_SIGNAL_LOCKED:
            printf("Input signal status: Locked\n");
            break;
        }

        if (videoSignalStatus.state != MWCAP_VIDEO_SIGNAL_LOCKED) {
            MWStopVideoCapture(hChannel);
            break;
        }
        
        printf("Input signal resolution: %d x %d\n", videoSignalStatus.cx, videoSignalStatus.cy);
        double fps = (videoSignalStatus.bInterlaced == TRUE) ? (double)20000000LL / videoSignalStatus.dwFrameDuration : (double)10000000LL / videoSignalStatus.dwFrameDuration;
        printf("Input signal fps: %.2f\n", fps);
        printf("Input signal interlaced: %d\n", videoSignalStatus.bInterlaced);
        printf("Input signal frame segmented: %d\n", videoSignalStatus.bSegmentedFrame);


        HNOTIFY hNotify = MWRegisterNotify(hChannel, hNotifyEvent, MWCAP_NOTIFY_VIDEO_FRAME_BUFFERED);
        if (hNotify == 0) {
            MWStopVideoCapture(hChannel);
            printf("ERROR: Register Notify error.\n");
            break;
        }
        printf("Begin to capture %d frames by %.2f fps...\n", NUM_CAPTURE, fps);
        fflush(stdout);
        int i;
        //for (i = 0; i < NUM_CAPTURE; i++)
        char inputChar;
        std::cin >> inputChar;
        while( inputChar != 'e' )
        {
            nRet=MWWaitEvent(hNotifyEvent,1000);
            if(nRet<=0) {
                printf("Error:wait notify error or timeout\n");
                break;
            }

            ULONGLONG ullStatusBits = 0;
            xr = MWGetNotifyStatus(hChannel, hNotify, &ullStatusBits);
            if (xr != MW_SUCCEEDED)
                continue;

            xr = MWGetVideoBufferInfo(hChannel, &videoBufferInfo);
            if (xr != MW_SUCCEEDED)
                continue;

            xr = MWGetVideoFrameInfo(hChannel, videoBufferInfo.iNewestBufferedFullFrame, &videoFrameInfo);
            if (xr != MW_SUCCEEDED)
                continue;

            if (ullStatusBits & MWCAP_NOTIFY_VIDEO_FRAME_BUFFERED == 0) {
                continue;
            }
            xr = MWCaptureVideoFrameToVirtualAddressEx(hChannel,
                                                       videoBufferInfo.iNewestBufferedFullFrame,(unsigned char *)pbImage,dwImageSize,dwMinStride,
                                                       1,0,dwFourcc,cx,cy,0,0,0,0,0,100,0,100,0,MWCAP_VIDEO_DEINTERLACE_BLEND,
                                                       MWCAP_VIDEO_ASPECT_RATIO_CROPPING,0,0,0,0,MWCAP_VIDEO_COLOR_FORMAT_UNKNOWN,
                                                       MWCAP_VIDEO_QUANTIZATION_UNKNOWN,MWCAP_VIDEO_SATURATION_UNKNOWN);
            if(xr!=MW_SUCCEEDED)
                continue;

            nRet=MWWaitEvent(hCaptureEvent,1000);
            if(nRet<=0) {
                printf("Error:wait capture event error or timeout\n");
                break;
            }

            MWCAP_VIDEO_CAPTURE_STATUS captureStatus;
            xr = MWGetVideoCaptureStatus(hChannel, &captureStatus);

            LONGLONG llCurrent = 0LL;
            xr = MWGetDeviceTime(hChannel, &llCurrent);

            cv::Mat image(cv::Size(1920, 1080), CV_8UC3, reinterpret_cast<void*>(pbImage));
            cv::Mat img_flip;
            cv::flip(image, img_flip, 0);
            cv::namedWindow("Image");
            cv::imshow("Image", img_flip);
            cv::waitKey(1);

            llTotalTime += (llCurrent - (videoSignalStatus.bInterlaced ? videoFrameInfo.allFieldBufferedTimes[1] : videoFrameInfo.allFieldBufferedTimes[0]));
            /*
            char szBitmapName[64];
            sprintf(szBitmapName,"./video_%d.BMP",NUM_CAPTURE);
            if(i==NUM_CAPTURE-1)
                create_bitmap(szBitmapName,pbImage,dwImageSize,cx,cy);
            */
        }
        xr = MWUnregisterNotify(hChannel, hNotify);
        hNotify=0;
        xr = MWStopVideoCapture(hChannel);
    } while (FALSE);

    if (hChannel != NULL){
        MWCloseChannel(hChannel);
        hChannel=NULL;
    }

    if(hNotifyEvent!=0){
        MWCloseEvent(hNotifyEvent);
        hNotifyEvent=0;
    }

    if(hCaptureEvent!=0){
        MWCloseEvent(hCaptureEvent);
        hCaptureEvent=0;
    }

    if(pbImage!=0){
        free((void *)(unsigned long)pbImage);
        pbImage=0;
    }

    MWCaptureExitInstance();

    printf("\nPress 'Enter' to exit!\n");
    getchar();

    return 0;
}

