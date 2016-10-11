#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// 멤버 변수 정의
class Histogram1D {
private:
 int histSize[1]; // 빈도수
 float hranges[2]; // 최소/최대 화소값
 const float* ranges[1];
 int channels[1]; // 여기서 1채널만 사용
public :
 Histogram1D() { // 1차원 히스토그램을 위한 인자 준비
  histSize[0]= 256;
  hranges[0]= 0.0;
  hranges[1]= 255.0;
  ranges[0]= hranges; 
  channels[0]= 0; // 기본적으로 채널을 0으로 보기
 }

 // 정의한 멤버 변수로 그레이레벨 영상의 히스토그램을 계산할 때는 다음 메소드를 사용해 수행
 cv::MatND getHistogram(const cv::Mat &image) { 
  // 1차원(1D) 히스토그램 계산.
  cv::MatND hist;
  cv::calcHist(&image, // 히스토그램 계산 
   1,   // 단일 영상의 히스토그램만
   channels, // 대상 채널               
   cv::Mat(), // 마스크 사용하지 않음     
   hist,  // 결과 히스토그램         
   1,   // 1차원(1D) 히스토그램           
   histSize, // 빈도수                  
   ranges  // 화소값 범위             
  );
  return hist;
 }

 // 값의 순서만으로 의미를 파악하기 어려우므로 바 그래프를 사용
 // 그래프를 생성하는 메소드
 cv::Mat getHistogramImage(const cv::Mat &image){ 
  // 1차원(1D) 히스토그램을 계산하고, 영상으로 반환

  cv::MatND hist= getHistogram(image); // 먼저 히스토그램 계산

  double maxVal=0; // 최대 빈도수 가져오기
  double minVal=0; // 최소 빈도수 가져오기
  cv::minMaxLoc(hist, &minVal, &maxVal, 0, 0);

  cv::Mat histImg(histSize[0], histSize[0], CV_8U,cv::Scalar(255));
   // 히스토그램을 출력하기 위한 영상

  int hpt = static_cast<int>(0.9*histSize[0]);
   // nbins의 90%를 최대점으로 설정

  
  for(int h=0; h<histSize[0]; h++){ // 각 빈도에 대한 수직선을 그리기 
   float binVal = hist.at<float>(h);
   int intensity = static_cast<int>(binVal*hpt/maxVal);
   cv::line(histImg,cv::Point(h,histSize[0]),cv::Point(h,histSize[0]-intensity),cv::Scalar::all(0));
    // 두 점 간의 거리를 그리는 함수
  }
  return histImg;
 }
};

int main()
{
 cv::Mat image = cv::imread("out.jpg", 0);
  // 입력 영상 읽기
  // 이진 영상으로 간주해 열기
 if(!image.data)
  return 0;

 cv::namedWindow("Image");
 cv::imshow("Image", image);

 Histogram1D h; // 히스토그램 객체
 cv::MatND histo = h.getHistogram(image); // 히스토그램 계산

 for(int i=0 ; i<256 ; i++) // 각 빈도 조회
  std::cout << "Value " << i << " = " << histo.at<float>(i) << std::endl;

 cv::namedWindow("Histogram");
 cv::imshow("Histogram", h.getHistogramImage(image));
  // 히스토그램을 영상으로 띄우기
  // 가운데를 중심으로 왼쪽이 검정색, 오른쪽이 흰색값
  // 가운데 봉우리 부분은 중간 명암도 값
  // 왼쪽이 영상의 전경, 오른쪽이 배경
 
 // 영상을 두 그룹으로 나누는 부분을 경계값으로 처리해 확인
 cv::Mat thresholded; // 경계값으로 이진 영상 생성
 cv::threshold(image,thresholded,100,255,cv::THRESH_BINARY);
  // 영상을 경계화 하기 위해 히스토그램의 
  // 높은 봉우리(명암값 60) 방향으로 증가하기 직전인 최소값으로 정함.
 cv::namedWindow("Binary Image"); // 경계화된 영상 띄워 보기
 cv::imshow("Binary Image",thresholded); // 배경과 전경이 분할됨
 
 cv::Mat eroded;
 
 cv::Mat dilated;

 cv::dilate(thresholded, dilated, cv::Mat());
 cv::erode(dilated, eroded, cv::Mat());
 imwrite("./dilerod.jpg", eroded);
 
 cv::erode(thresholded, eroded, cv::Mat());
 cv::dilate(thresholded, dilated, cv::Mat());
 imwrite("./eroddila.jpg", dilated);
 imwrite("./thresholded.jpg", thresholded);

 cv::waitKey(0);

 return 0;
}
