#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>
#include <dirent.h>


int windowX[5][2];
int windowY[5][2];
double fileIdx;
cv::Mat first[28];
cv::Mat second[28];
cv::Mat third[28];
cv::Mat fourth[28];
cv::Mat fifth[28];


int calcPixel(cv::Mat crop, int x, int y);
void calcX(cv::Mat origin) {
	int cnt = 0;
	int cnt2;
	int idxX = 0;
	int flag = 0;
	for (int i = 40; i<198; i++) {
		for (int j = 0; j<50; j++) {
			if (origin.at<uchar>(j, i) == 255) cnt++;
		}
		if (cnt>0 && flag == 0 && i>30) {
			flag = 1;
			windowX[idxX][0] = i;
		}
		if (flag == 1 && cnt == 0) {
			for (int j = 0; j<50; j++) {
				if (origin.at<uchar>(j, i + 1) == 255) cnt++;
			}
			if (cnt == 0) {
				for (int j = 0; j<50; j++) {
					if (origin.at<uchar>(j, i + 2) == 255) cnt++;
				}

				if (cnt == 0) {
					for (int j = 0; j<50; j++) {
						if (origin.at<uchar>(j, i + 3) == 255) cnt++;
					}
					if (cnt == 0) {
						flag = 0;
						windowX[idxX][1] = i;
						if (windowX[idxX][1] - windowX[idxX][0] >= 33) {
							for (int k = windowX[idxX][0] + 7; k < windowX[idxX][1]; k++) {
								cnt2 = 0;
								for (int j = 0; j<50; j++) {
									if (origin.at<uchar>(j, k) == 255 && origin.at<uchar>(j, k + 1) == 255) cnt2++;
								}
								if (cnt2 == 0) {

									//std::cout << windowX[idxX][0] << " " << k << " " << windowX[idxX][1] << std::endl;
									windowX[idxX + 1][1] = windowX[idxX][1];
									windowX[idxX][1] = k + 1;
									idxX++;
									windowX[idxX][0] = k + 2;
									k += 7;
									if (windowX[idxX][1] - windowX[idxX][0] < 35)
										break;
								}
							}
						}
						idxX++;
					}
				}
			}
		}
		cnt = 0;
		if (idxX > 4) break;
	}
	for (int i = 0; i<5; i++) {
		//std::cout << windowX[i][0] << " " << windowX[i][1] << std::endl;
	}
}
void calcY(cv::Mat origin) {
   int cnt = 0, temp=0 ;
   for(int idxX = 0 ; idxX <5 ;idxX ++ ) {
      for(int j=7 ; j<48 ; j++) {
         cnt=0;
         for(int i = windowX[idxX][0] ; i < windowX[idxX][1] ; i++) {
            if(origin.at<uchar>(j, i)==255) {cnt++; break;}
         }
         if(cnt==0) {   //검은선
            if((j-temp) >=20) {
               windowY[idxX][0] = temp ;
               windowY[idxX][1] = j ;
               //std::cout <<temp <<" // " << j <<std::endl;
               temp =0 ;
               break;
            }
            else {temp=0;
            }
         }
         else if(temp<7){  //흰선 + 처음 점 미설정
            temp = j;
         }
         else if(j==47) {
            windowY[idxX][0] = temp ;
            windowY[idxX][1] = j ;
            //std::cout <<temp <<" / " << j <<std::endl;
            temp =0 ;
         }
      
      }
   }
}
/*void calcY(cv::Mat origin) {
int idxX=0;
int cnt=0;
int flag=0;
for(int j = 0; j<50; j++) {
for(int i = windowX[idxX][0] ; i < windowX[idxX][1] ; i++) {
if(origin.at<uchar>(j, i)==255) cnt++;
}
if(cnt>0 && flag==0) {
flag=1;
windowY[idxX][0]=j;
}
if(flag==1 && cnt==0) {
for(int i = windowX[idxX][0] ; i < windowX[idxX][1] ; i++) {
if(origin.at<uchar>(j+1, i)==255) cnt++;
}
if(cnt==0) {
for(int i = windowX[idxX][0] ; i < windowX[idxX][1] ; i++) {
if(origin.at<uchar>(j+2, i)==255) cnt++;
}
if(cnt==0) {
for(int i = windowX[idxX][0] ; i < windowX[idxX][1] ; i++) {
if(origin.at<uchar>(j+3, i)==255) cnt++;
}
if(cnt==0) {
flag=0;
windowY[idxX][1]=j;
idxX++;
j=0;
}
}
}
}
cnt=0;
}
for(int i = 0; i<5; i++) {
std::cout<<windowY[i][0]<<" "<<windowY[i][1]<<std::endl;
}
}*/
/*
cv::Mat rmLine(cv::Mat origin) {
cv::Mat result(50, 198, CV_8U);
int startx=0, starty=0;
int cond=0; // 1 대각선 위, 0 옆으로, -1 대각선 아래

for(int i = 0; i<198; i++) {
for(int j = 0; j<50; j++) {
result.at<uchar>(j, i)=origin.at<uchar>(j, i);
}
}

cv::imshow("ABCDEF", origin);

for(int i = 0; i<198; i++) {
for(int j = 0; j<50; j++) {
if(origin.at<uchar>(j, i)==255) {
startx=i;
starty=j;
std::cout<<"start x: "<<startx <<" starty: "<<starty<<std::endl;
break;
}
}
if(startx!=0) break;
}
std::cout<<(int)result.at<uchar>(19,19)<<std::endl;
int presx=startx;
int presy=starty;
int flag=0;
for(int i = startx; i<198; i++) {
presx=i;
flag=0;
int tmpy=presy;
if(cond==1) {
if(origin.at<uchar>(tmpy-1, presx)==255) flag++;
if(origin.at<uchar>(tmpy, presx)==255) flag++;
if(origin.at<uchar>(tmpy+1, presx)==255) flag++;
//         std::cout<<"condition: " << cond <<std::endl;

if(flag==0) {
//          std::cout<<"flag is zero!"<<std::endl;
} else if(flag==1){
if(origin.at<uchar>(tmpy-1, presx)==255) {
presy--;
cond=1;
result.at<uchar>(presy,presx)=0;
} else if(origin.at<uchar>(tmpy+1, presx)==255) {
presy++;
cond=-1;
result.at<uchar>(presy,presx)=0;
} else if(origin.at<uchar>(tmpy, presx)==255) {
cond=0;
result.at<uchar>(presy,presx)=0;
}
} else if(flag>2) {
cond=0;
} else if(flag==2) {
if(origin.at<uchar>(tmpy-1, presx)==255) {
origin.at<uchar>(tmpy-1, presx)==0;
presy--;
cond=1;
} else {
cond=0;
}
if(origin.at<uchar>(tmpy, presx)==255) origin.at<uchar>(tmpy, presx)==0;
if(origin.at<uchar>(tmpy+1, presx)==255) origin.at<uchar>(tmpy+1, presx)==0;
}
} else if(cond==0) {
if(origin.at<uchar>(tmpy-1, presx)==255) flag++;
if(origin.at<uchar>(tmpy, presx)==255) flag++;
if(origin.at<uchar>(tmpy+1, presx)==255) flag++;
//         std::cout<<"condition: " << cond <<std::endl;
//         std::cout<<"flag: " << flag <<std::endl;
if(flag==0) {
//           std::cout<<"flag is zero!"<<std::endl;
} else if(flag==1) {
if(origin.at<uchar>(tmpy-1, presx)==255) {
presy--;
cond=1;
result.at<uchar>(presy,presx)=0;
} else if(origin.at<uchar>(tmpy+1, presx)==255) {
presy++;
cond=-1;
result.at<uchar>(presy,presx)=0;
} else if(origin.at<uchar>(tmpy, presx)==255) {
cond=0;
result.at<uchar>(presy,presx)=0;
}
} else if(flag>2) {
cond=0;
} else if(flag==2) {
if(origin.at<uchar>(tmpy-1, presx)==255) {
origin.at<uchar>(tmpy-1, presx)==0;
presy--;
cond=1;
} else {
cond=0;
}
if(origin.at<uchar>(tmpy, presx)==255) origin.at<uchar>(tmpy, presx)==0;
if(origin.at<uchar>(tmpy+1, presx)==255) origin.at<uchar>(tmpy+1, presx)==0;
}
} else if(cond==-1) {
if(origin.at<uchar>(tmpy-1, presx)==255) flag++;
if(origin.at<uchar>(tmpy, presx)==255) flag++;
if(origin.at<uchar>(tmpy+1, presx)==255) flag++;
//         std::cout<<"condition: " << cond <<std::endl;

if(flag==0) {
//           std::cout<<"flag is zero!"<<std::endl;
} else if(flag==1) {
if(origin.at<uchar>(tmpy-1, presx)==255) {
presy--;
cond=1;
result.at<uchar>(presy,presx)=0;
} else if(origin.at<uchar>(tmpy+1, presx)==255) {
presy++;
cond=-1;
result.at<uchar>(presy,presx)=0;
} else if(origin.at<uchar>(tmpy, presx)==255) {
cond=0;
result.at<uchar>(presy,presx)=0;
}
} else if(flag>2) {
cond=0;
} else if(flag==2) {
if(origin.at<uchar>(tmpy-1, presx)==255) {
origin.at<uchar>(tmpy-1, presx)==0;
presy--;
cond=1;
} else {
cond=0;
}
if(origin.at<uchar>(tmpy, presx)==255) origin.at<uchar>(tmpy, presx)==0;
if(origin.at<uchar>(tmpy+1, presx)==255) origin.at<uchar>(tmpy+1, presx)==0;
}
}
}

return result;
}*/

cv::Mat rmNoise(cv::Mat origin) {
	cv::Mat result(50, 198, CV_8U);
	int cnt = 0;
	int k = 0;
	int flag = 0;
	int tmp = -1;
	for (int i = 0; i<198; i++) {
		for (int j = 0; j<50; j++) {
			result.at<uchar>(j, i) = origin.at<uchar>(j, i);
		}
	}
	for (int i = 0; i<198; i++) {
		for (int j = 0; j<50; j++) {
			if (origin.at<uchar>(j, i) == 255 && tmp == -1) {
				tmp = j;
				cnt++;
			}
			else if (origin.at<uchar>(j, i) == 255 && tmp != -1) {
				cnt++;
			}
		}
		if (cnt <= 4) {
			if (flag == 1) {
				flag = 0;
			}
			for (int j = 0; j<50; j++) {
				if (origin.at<uchar>(j, i) == 255) result.at<uchar>(j, i) = 0;
			}
		}
		cnt = 0;
	}
	return result;
}
/*
cv::Mat removeNoise(cv::Mat origin) {
cv::Mat result(50,198,CV_8U);
for(int i = 0; i<198; i++) {
for(int j = 0; j<50; j++) {
result.at<uchar>(j, i)=origin.at<uchar>(j, i);
}
}
for(int i = 1; i<197; i++) {
for(int j = 1; j<49; j++) {
if(origin.at<uchar>(j, i)==255) {
if(origin.at<uchar>(j-1, i)==0 || origin.at<uchar>(j+1, i)==0 || (origin.at<uchar>(j, i+1)==0 && origin.at<uchar>(j, i-1)) ) {
result.at<uchar>(j,i)=0;
} else {
result.at<uchar>(j,i)=255;
}
}
}
}
for(int i = 1; i<197; i++) {
for(int j = 1; j<49; j++) {
if(result.at<uchar>(j, i)==255) {
if(result.at<uchar>(j-1, i)==0 &&result.at<uchar>(j+1, i)==0 && (result.at<uchar>(j, i+1)==0 && result.at<uchar>(j, i-1)) ) {
result.at<uchar>(j,i)=0;
} else {
result.at<uchar>(j,i)=255;
}
}
}
}
return result;
}
*/
void readFirst() {
   std::string dirName = "/home/seongmo/다운로드/git/1st/";
	DIR *dir;
	dir = opendir(dirName.c_str());
	std::string imgName;
	fileIdx = 0;
	cv::Mat image;
	int cnt = 0;
	struct dirent *ent;
	if (dir != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			imgName = ent->d_name;
			if (imgName == "." || imgName == "..") continue;
			std::string fullPath(dirName + imgName);
			std::cout << fullPath << std::endl;
			first[cnt] = cv::imread(fullPath, 0);
			cnt++;
		}
		closedir(dir);
	}
	else {
		std::cout << "Error!" << std::endl;
	}
	if (!image.data)
		return ;

}

void readSecond() {
   std::string dirName = "/home/seongmo/다운로드/git/2nd/";
	DIR *dir;
	dir = opendir(dirName.c_str());
	std::string imgName;
	fileIdx = 0;
	cv::Mat image;
	int cnt = 0;
	struct dirent *ent;
	if (dir != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			imgName = ent->d_name;
			if (imgName == "." || imgName == "..") continue;
			std::string fullPath(dirName + imgName);
			std::cout << fullPath << std::endl;
			second[cnt] = cv::imread(fullPath, 0);
			cnt++;
		}
		closedir(dir);
	}
	else {
		std::cout << "Error!" << std::endl;
	}
	if (!image.data)
		return ;

}

void readThird() {
   std::string dirName = "/home/seongmo/다운로드/git/3rd/";
	DIR *dir;
	dir = opendir(dirName.c_str());
	std::string imgName;
	fileIdx = 0;
	cv::Mat image;
	int cnt = 0;
	struct dirent *ent;
	if (dir != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			imgName = ent->d_name;
			if (imgName == "." || imgName == "..") continue;
			std::string fullPath(dirName + imgName);
			std::cout << fullPath << std::endl;
			third[cnt] = cv::imread(fullPath, 0);
			cnt++;
		}
		closedir(dir);
	}
	else {
		std::cout << "Error!" << std::endl;
	}
	if (!image.data)
		return ;

}

char compareImgThird(cv::Mat croppedImg) {
   int cols = croppedImg.cols;
   int rows = croppedImg.rows;
   int cnt=0;
   float max=0;
   int maxIdx=100;
   int pixel[28];
   char result='A';
   for(int i=0; i<24; i++) {
      pixel[i]=calcPixel(third[i], third[i].cols, third[i].rows);
      for(int j=0; j<cols; j++) {
         for(int k=0; k<rows; k++) {
            if(third[i].at<uchar>(k, j)==255) {
               if(croppedImg.at<uchar>(k, j)==255) {
                  cnt++;
               }
               else {
                  cnt--;
               }
            }
            if(croppedImg.at<uchar>(k, j)==0) {
               if(third[i].at<uchar>(k, j)==255) {
                  cnt--;
               }
            }
         }
      }
 //     std::cout<<"index: "<<i <<" percent: "<<((float)cnt)/(cols*rows)<<std::endl;
      if(max<((float)cnt)/(cols*rows) ) {
         max=((float)cnt)/(cols*rows);
         maxIdx=i;
      }
      cnt=0;
   }
   std::cout<<"MaxIdx3: "<<maxIdx<<std::endl;
   cv::imshow("answer3", third[maxIdx]);
   
   switch(maxIdx) {
      case 0: result='G';
      break;
      case 1: result='J';
      break;
      case 2: result='D';
      break;
      case 3: result='Y';
      break;
      case 4: result='K';
      break;
      case 5: result='T';
      break;
      case 6: result='S';
      break;
      case 7: result='Z';
      break;
      case 8: result='O';
      break;
      case 9: result='L';
      break;
      case 10: result='X';
      break;
      case 11: result='M';
      break;
      case 12: result='C';
      break;
      case 13: result='V';
      break;
      case 14: result='B';
      break;
      case 15: result='U';
      break;
      case 16: result='I';
      break;
      case 17: result='E';
      break;
      case 18: result='W';
      break;
      case 19: result='P';
      break;
      case 20: result='N';
      break;
      case 21: result='F';
      break;
      case 22: result='H';
      break;
      case 23: result='R';
      break;
   }
   return result;
}

char compareImgSecond(cv::Mat croppedImg) {
   int cols = croppedImg.cols;
   int rows = croppedImg.rows;
   int cnt=0;
   float max=0;
   int maxIdx=100;
   int pixel[28];
   char result='A';
   for(int i=0; i<24; i++) {
      pixel[i]=calcPixel(second[i], second[i].cols, second[i].rows);
      for(int j=0; j<cols; j++) {
         for(int k=0; k<rows; k++) {
            if(second[i].at<uchar>(k, j)==255) {
               if(croppedImg.at<uchar>(k, j)==255) {
                  cnt++;
               }
               else {
                  cnt--;
               }
            }
            if(croppedImg.at<uchar>(k, j)==0) {
               if(second[i].at<uchar>(k, j)==255) {
                  cnt--;
               }
            }
         }
      }
 //     std::cout<<"index: "<<i <<" percent: "<<((float)cnt)/(cols*rows)<<std::endl;
      if(max<((float)cnt)/(cols*rows) ) {
         max=((float)cnt)/(cols*rows);
         maxIdx=i;
      }
      cnt=0;
   }
   std::cout<<"MaxIdx2: "<<maxIdx<<std::endl;
   cv::imshow("answer2", second[maxIdx]);
   
   switch(maxIdx) {
      case 0: result='G';
      break;
      case 1: result='J';
      break;
      case 2: result='D';
      break;
      case 3: result='Y';
      break;
      case 4: result='K';
      break;
      case 5: result='T';
      break;
      case 6: result='S';
      break;
      case 7: result='Z';
      break;
      case 8: result='O';
      break;
      case 9: result='L';
      break;
      case 10: result='X';
      break;
      case 11: result='M';
      break;
      case 12: result='C';
      break;
      case 13: result='V';
      break;
      case 14: result='B';
      break;
      case 15: result='U';
      break;
      case 16: result='I';
      break;
      case 17: result='E';
      break;
      case 18: result='W';
      break;
      case 19: result='P';
      break;
      case 20: result='N';
      break;
      case 21: result='F';
      break;
      case 22: result='H';
      break;
      case 23: result='R';
      break;
   }
   return result;
}
char compareImg(cv::Mat croppedImg) {
   int cols = croppedImg.cols;
   int rows = croppedImg.rows;
   int cnt=0;
   float max=0;
   int maxIdx=100;
   int pixel[28];
   char result='A';
   for(int i=0; i<24; i++) {
      pixel[i]=calcPixel(first[i], first[i].cols, first[i].rows);
      for(int j=0; j<cols; j++) {
         for(int k=0; k<rows; k++) {
            if(first[i].at<uchar>(k, j)==255) {
               if(croppedImg.at<uchar>(k, j)==255) {
                  cnt++;
               }
               else {
                  cnt--;
               }
            }
            if(croppedImg.at<uchar>(k, j)==0) {
               if(first[i].at<uchar>(k, j)==255) {
                  cnt--;
               }
            }
         }
      }
      if(max<((float)cnt)/(cols*rows) ) {

         max=((float)cnt)/(cols*rows);
         maxIdx=i;
      }
      cnt=0;
   }

   
   switch(maxIdx) {
      case 0: result='G';
      break;
      case 1: result='J';
      break;
      case 2: result='D';
      break;
      case 3: result='Y';
      break;
      case 4: result='K';
      break;
      case 5: result='T';
      break;
      case 6: result='S';
      break;
      case 7: result='Z';
      break;
      case 8: result='O';
      break;
      case 9: result='L';
      break;
      case 10: result='X';
      break;
      case 11: result='M';
      break;
      case 12: result='C';
      break;
      case 13: result='V';
      break;
      case 14: result='B';
      break;
      case 15: result='U';
      break;
      case 16: result='I';
      break;
      case 17: result='E';
      break;
      case 18: result='W';
      break;
      case 19: result='P';
      break;
      case 20: result='N';
      break;
      case 21: result='F';
      break;
      case 22: result='H';
      break;
      case 23: result='R';
      break;
   }

   return result;
}

int calcPixel(cv::Mat crop, int x, int y) {
	char result = 'A';
	int cnt = 0;

	for (int i = 0; i<x; i++) {
		for (int j = 0; j<y; j++) {
			if (crop.at<uchar>(j, i) == 255) cnt++;
		}
	}

	//std::cout << "count" << cnt << std::endl;
	return cnt;
}

void readImg(cv::Mat image, std::string imgName) {
	cv::Mat thresholded; // 경계값으로 이진 영상 생성
	cv::threshold(~image, thresholded, 1, 255, cv::THRESH_BINARY);
	/*cv::namedWindow("Binary Image"); // 경계화된 영상 띄워 보기
	cv::imshow("Binary Image",thresholded); // 배경과 전경이 분할됨
	imwrite("./thresholded.jpg", thresholded);
	cv::Mat tmpd;*/
	//cv::Mat proImg = removeNoise(thresholded);
	cv::Mat newImg = rmNoise(thresholded);
	//cv::Mat rmLineImg = rmLine(newImg);
	//cv::Mat lastImg = rmLine(rmLineImg);
	//proImg = removeNoise(proImg);
	//cv::dilate(lastImg, tmpd, cv::Mat());
	//cv::dilate(tmpd, tmpd, cv::Mat());
	/* cv::erode(tmpd, tmpd, cv::Mat());
	cv::imshow("processed image", proImg);
	cv::imshow("removed line image", lastImg);
	cv::imshow("new version", newImg);

	cv::imshow("dilated image", tmpd);
	cv::imwrite("./processed.jpg", proImg);
	cv::imwrite("./dilated.jpg", tmpd);
	cv::imwrite("./process.jpg", proImg);
	cv::imwrite("./last.jpg", lastImg);
	cv::imwrite("./new.jpg", newImg);
	cv::imwrite("./dil.jpg", tmpd);
	cv::Mat eroded;
	cv::Mat dilated;

	cv::imwrite("./reverse.jpg", ~newImg);*/

	char result[5];
	int pixel;



	calcX(newImg);
	calcY(newImg);
	

	(newImg);

	cv::imshow("aaaaa", newImg);
	std::ostringstream filename;
	filename << imgName << 1 << ".jpg";
	//std::cout << "filename: " << filename.str() << std::endl;
	cv::Rect myROI(windowX[0][0], windowY[0][0], windowX[0][1] - windowX[0][0], windowY[0][1] - windowY[0][0]);
	cv::Mat croppedImg = newImg(myROI);
	if(windowX[0][1] - windowX[0][0] < 12) {
	   result[0]='I';
	}
	else if(windowX[0][1] - windowX[0][0] < 14) {
	   result[0]='J';
	}
	else {
	   result[0]=compareImg(croppedImg);
	}

	imwrite(filename.str(), croppedImg);
	pixel = calcPixel(croppedImg, windowX[0][1] - windowX[0][0], windowY[0][1] - windowY[0][0]);

	std::ostringstream filename2;
	filename2 << imgName << 2 << ".jpg";
	//std::cout << "filename: " << filename2.str() << std::endl;
	cv::Rect secROI(windowX[1][0], windowY[1][0], windowX[1][1] - windowX[1][0], windowY[1][1] - windowY[1][0]);
	croppedImg = newImg(secROI);
	imwrite(filename2.str(), croppedImg);
	pixel = calcPixel(croppedImg, windowX[1][1] - windowX[1][0], windowY[1][1] - windowY[1][0]);
	if(windowX[1][1] - windowX[1][0] < 12) {
	   result[1]='I';
	}
	else if(windowX[1][1] - windowX[1][0] < 14) {
	   result[1]='J';
	}
	else {
	   result[1]=compareImgSecond(croppedImg);
	}

	std::ostringstream filename3;
	filename3 << imgName << 3 << ".jpg";
	//std::cout << "filename: " << filename3.str() << std::endl;
	cv::Rect thiROI(windowX[2][0], windowY[2][0], windowX[2][1] - windowX[2][0], windowY[2][1] - windowY[2][0]);
	croppedImg = newImg(thiROI);
	imwrite(filename3.str(), croppedImg);
	pixel = calcPixel(croppedImg, windowX[2][1] - windowX[2][0], windowY[2][1] - windowY[2][0]);
	if(windowX[2][1] - windowX[2][0] < 12) {
	   result[2]='I';
	}
	else if(windowX[2][1] - windowX[2][0] < 14) {
	   result[2]='J';
	}
	else {
	   result[2]=compareImgThird(croppedImg);
	}

	std::ostringstream filename4;
	filename4 << imgName << 4 << ".jpg";
	//std::cout << "filename: " << filename4.str() << std::endl;
	cv::Rect forROI(windowX[3][0], windowY[3][0], windowX[3][1] - windowX[3][0], windowY[3][1] - windowY[3][0]);
	croppedImg = newImg(forROI);
	imwrite(filename4.str(), croppedImg);
	pixel = calcPixel(croppedImg, windowX[3][1] - windowX[3][0], windowY[3][1] - windowY[3][0]);
	if(windowX[3][1] - windowX[3][0] < 12) {
	   result[3]='I';
	}
	else if(windowX[3][1] - windowX[3][0] < 14) {
	   result[3]='J';
	}
	else {
	   //result[3]=compareImg(croppedImg);
	}

	std::ostringstream filename5;
	filename5 << imgName << 5 << ".jpg";
	//std::cout << "filename: " << filename5.str() << std::endl;
	cv::Rect fifROI(windowX[4][0], windowY[4][0], windowX[4][1] - windowX[4][0], windowY[4][1] - windowY[4][0]);
	croppedImg = newImg(fifROI);
	imwrite(filename5.str(), croppedImg);
	pixel = calcPixel(croppedImg, windowX[4][1] - windowX[4][0], windowY[4][1] - windowY[4][0]);
	if(windowX[4][1] - windowX[4][0] < 12) {
	   result[4]='I';
	}
	else if(windowX[4][1] - windowX[4][0] < 14) {
	   result[4]='J';
	}
	else {
	   //result[4]=compareImg(croppedImg);
	}

	cv::waitKey(0);

	std::cout << result << std::endl;

	return;
}
int main()
{
	std::string dirName = "/home/seongmo/다운로드/git/temp/";
	DIR *dir;
	dir = opendir(dirName.c_str());
	std::string imgName;
	fileIdx = 0;
	cv::Mat image;
	int cnt = 1;
	struct dirent *ent;
	readFirst();
	readSecond();
	readThird();
	if (dir != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			imgName = ent->d_name;
			if (imgName == "." || imgName == "..") continue;
			std::string fullPath(dirName + imgName);
			std::cout << fullPath << std::endl;
			image = cv::imread(fullPath, 0);
			readImg(image, imgName);
			cnt++;
		}
		closedir(dir);
	}
	else {
		std::cout << "Error!" << std::endl;
	}
	if (!image.data)
		return 0;



	return 0;
}
