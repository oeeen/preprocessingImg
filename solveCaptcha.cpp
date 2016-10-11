#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int main(int argc, char **argv) {
   if( argc != 2) {
      cout << "No input file" << endl;
      return -1;
   }

   Mat img;
   img = imread(argv[1], CV_LOAD_IMAGE_COLOR); // read file
   
   if( !img.data) {
      cout << "Could not open or find the image" << endl;
      return -1;
   }
   
   namedWindow("Original Image", WINDOW_AUTOSIZE);
   imshow("Original Image", img);
   
   Mat imgGray;
   
   cvtColor(img, imgGray, COLOR_BGR2GRAY);
   
   imshow("Gray Image", imgGray);
   imshow("~Gray Image", ~imgGray);
   
   //imwrite("./out.jpg", ~imgGray);
   waitKey(0);
   return 0;
   
}
