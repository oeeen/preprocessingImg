#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <vector>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
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

        Mat imgHSV;
        cvtColor(image, imgHSV, COLOR_BGR2HSV);

        imshow("HSVimg", imgHSV);

        int iLowH = 0;
        int iHighH = 179;

        int iLowS = 0;
        int iHighS = 255;

        int iLowV = 0;
        int iHighV = 255;

        Mat imgThresholded;

        inRange(imgHSV, Scalar(0, 0, 0), Scalar(179, 255, 255), imgThresholded);

        Mat mask;

        imshow("Thresholded Image", imgThresholded);





        waitKey(0);
        return 0;
}
