#include <iostream>
#include <stdio.h>
#include <sys/time.h>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <vector>
#include "opencv2/opencv.hpp"

#define LOW_BOUND       0.7
#define HIGH_BOUND      1.3

struct timeval start_time;
struct timeval end_time;
double elapsed_time;
float fps;
double total_fps = 0.0, average_fps = 0.0;
long cnt=1;
char buffer[25];
CvFont font;
char getFlag = 0;
long getx, gety;
int getRed, getGreen, getBlue;
int Bval, Rval, Gval;

using namespace std;
using namespace cv;

void on_eventhandle(int event, int x, int y, int flag, void* param) {
	switch(event) {
	        case CV_EVENT_LBUTTONDOWN:
	        if(getFlag==0) {
	                getx=x;
	                gety=y;
	                getFlag=1;
	        }else if(getFlag==1){
	                getFlag==2;
	        }
	        break;
	        
	        case CV_EVENT_RBUTTONDOWN:
	        getFlag==0;
	        break;
	        
	}    
}

int main(int argc,char** argv) {
        IplImage* img;
        IplImage* GrayImg = 0;
        IplImage* SmoothImg = 0;
        IplImage* Edge = 0;
        IplImage* HSVImg;
        IplImage* HImg = 0;
        IplImage* SImg = 0;
        IplImage* VImg = 0;
        CvMat* mask;
        int key;
        int k;
        double dp = 1;
        double min_dist = 300;
        int H,S,V;
        int lowH,lowS,lowV,highH,highS,highV;
        CvSize size;
        int cx,cy,radius,ccnt;
        cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX,0.5,0.5);
        
	if( argc != 2) {
		cout << "hi" <<endl;
		return -1;
	}

	Mat image;
	image = imread(argv[1], CV_LOAD_IMAGE_COLOR); // read file

	if(! image.data ) {
		cout << "Could not open or find the image"<<endl;
		return -1;
	}

	namedWindow("My Window", WINDOW_AUTOSIZE);
	imshow("My Window", image);
        
        CvCapture* capture = cvCaptureFromCAM(0);
        cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 640);
        cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 480);
        cvSetCaptureProperty(capture, CV_CAP_PROP_FPS, 65);
        
        
        while(1)
        {
                gettimeofday(&start_time, NULL);
                
                cvGrabFrame(capture);
                img = cvRetrieveFrame(capture);
                
                cvSetMouseCallback("PreviewImage", on_eventhandle, (void*)img);
                
                if(getFlag==0)
                {
                        sprintf(buffer, "Picking up color by left click");
                        cvPutText(img, buffer, cvPoint(200,20), &font,CV_RGB(255,255,255));
                }else if(getFlag==1)
                {
                        sprintf(buffer,"If below color is right you chosen, start detect by left click again");
                        cvPutText(img,buffer, cvPoint(20,20),&font,CV_RGB(255,255,255));
                        size=cvGetSize(img);
                        CvScalar v = cvGet2D(img, gety,getx);
                        getBlue = (int)v.val[0];
                        getGreen = (int)v.val[1];
                        getRed = (int)v.val[2];
                        
                        SmoothImg = cvCreateImage(size, IPL_DEPTH_8U,3);
                        cvSmooth(img, SmoothImg, CV_GAUSSIAN,9,9,2,2);
                        HSVImg = cvCreateImage(size,IPL_DEPTH_8U,3);
                        cvCvtColor(SmoothImg,HSVImg,CV_BGR2HSV);
                        CvScalar s = cvGet2D(HSVImg, gety,getx);
                        H=(int)s.val[0];
                        S=(int)s.val[1];
                        V=(int)s.val[2];
                        
                        sprintf(buffer,"X=%d,Y=%d,R=%d,G=%d,B=%d,H=%d,S=%d,V=%d",getx,gety,getRed,getGreen,getBlue, H,S,V);
                        cvPutText(img, buffer, cvPoint(getx,gety),&font,CV_RGB(255,255,255));
                 }else
                 {
                        sprintf(buffer,"You can picking up again by right click");
                        cvPutText(img, buffer, cvPoint(100,20), &font, CV_RGB(255,255,255));
                        cvSmooth(img, SmoothImg,CV_GAUSSIAN,9,9,2,2);
                        cvCvtColor(SmoothImg,HSVImg,CV_BGR2HSV);
                        
                        lowH= H * LOW_BOUND;
                        highH= H * HIGH_BOUND;
                        if(highH >=180) highH= 179;
                        
                        lowS= S * LOW_BOUND;
                        highS= S * HIGH_BOUND;
                        if(highS >=256) highS= 255;
                        
                        lowV= V * LOW_BOUND;
                        highV= V * HIGH_BOUND;
                        if(highH >=256) highH= 255;
                        
                        mask = cvCreateMat(size.height, size.width, CV_8UC1);
                        cvInRangeS(HSVImg, cvScalar(lowH,lowS,lowV),cvScalar(highH,highS,highV),mask);
                        
                        cvErode(mask,mask,NULL);
                        cvDilate(mask,mask,NULL);
                        
                        GrayImg = cvCreateImage(size,8,1);
                        cvCopy(mask, GrayImg, NULL);
                        cvSmooth(GrayImg,GrayImg,CV_GAUSSIAN,15,15,2,2);
                        
                        CvMemStorage* storage = cvCreateMemStorage(0);
                        
                        CvSeq* Circles = cvHoughCircles(GrayImg, storage, CV_HOUGH_GRADIENT,2,size.height/10,100,40,5,60);
                        
                        for(k=0; k<Circles->total;k++)
                        {
                                float* circle;
                                circle = (float*)cvGetSeqElem(Circles,k);
                                
                                ccnt = Circles->total;
                                
                                cx=cvRound(circle[0]);
                                cy=cvRound(circle[1]);
                                radius = cvRound(circle[2]);
                                cvCircle(img,cvPoint(cx,cy),radius,CV_RGB(0,255,0),3,8,0);
                                
                                sprintf(buffer,"X=%d, Y=%d, R=%d",cx,cy,radius);
                                cvPutText(img, buffer,cvPoint(cx-80,cy+radius+15),&font,CV_RGB(255,255,255));
                        }
                        
                        cvReleaseMemStorage(&storage);
                        
                        cvNamedWindow("HSVImage",1);
                        cvShowImage("HSVImage",HSVImg);
                        cvMoveWindow("HSVImage",850,200);
                        
                        cvNamedWindow("Mask",1);
                        cvShowImage("Mask",mask);
                        cvMoveWindow("Mask",850,700);
                }
                
                cvNamedWindow("PreviewImage", 1);
                cvShowImage("PreviewImage", img);
                cvMoveWindow("PreviewImage", 200, 200);
                
                key = cvWaitKey(1);
                if(key > 10) break;
                
                gettimeofday(&end_time, NULL);
                
                elapsed_time = (double)(end_time.tv_sec) + (double)(end_time.tv_usec)/1000000.0 - (double)(start_time.tv_sec) - (double)(start_time.tv_usec)/1000000.0;
                fps = 1.0 /elapsed_time;
                total_fps += fps;
                average_fps = total_fps / (double)(cnt);
                cnt++;
                if(cnt==100)
                {
                        cnt=1;
                        total_fps=0.0;
                }
                
                elapsed_time *= 1000.0;
        } //end of while(1)
        
        
        cvReleaseImage(&img);
        cvReleaseCapture(&capture);
        cvReleaseMat(&mask);
        cvReleaseImage(&SmoothImg);
        cvReleaseImage(&GrayImg);
        cvReleaseImage(&HSVImg);
        
        return 0;
                 
                
        
}
