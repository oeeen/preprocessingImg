#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32) || defined(__WINDOWS__) || defined(__TOS_WIN__)

  #include <windows.h>

  inline void delay( unsigned long ms )
    {
    Sleep( ms );
    }

#else  /* presume POSIX */

  #include <unistd.h>

  inline void delay( unsigned long ms )
    {
    usleep( ms * 1000 );
    }

#endif 
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>
#include <dirent.h>
#include <fstream>

#define DIR_NAME "/home/seongmo/다운로드/git/temp/"
#define ANSWER_DIR "/home/seongmo/다운로드/git/Answerset/"

int windowX[5][2];
int windowY[5][2];
double fileIdx;
cv::Mat answer[78];


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
            temp =0 ;
         }
      
      }
   }
}

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

void readAnswerSet() {
   std::string dirName = ANSWER_DIR;
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
			answer[cnt] = cv::imread(fullPath, 0);
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


char compareImg(cv::Mat croppedImg) {
   int cols = croppedImg.cols;
   int rows = croppedImg.rows;
   int cnt=0;
   float max=-100.0;
   int maxIdx=100;
   int pixel[72];
   char result='A';
   float percent[72];
   float alpha[24];
   
   for(int i=0; i<72; i++) {
      pixel[i]=calcPixel(answer[i], answer[i].cols, answer[i].rows);
      for(int j=0; j<cols; j++) {
         for(int k=0; k<rows; k++) {
            if(answer[i].at<uchar>(k, j)==255) {
               if(croppedImg.at<uchar>(k, j)==255) {
                  cnt++;
               }
               else {
                  cnt--;
               }
            }
            if(croppedImg.at<uchar>(k, j)==0) {
               if(answer[i].at<uchar>(k, j)==255) {
                  cnt--;
               }
            }
         }
      }
      if(max<((float)cnt)/(cols*rows) ) {

         max=((float)cnt)/(cols*rows);
         maxIdx=i;
      }
      //percent[i]=((float)cnt)/(cols*rows);
      cnt=0;
   }

/*
   alpha[0]=percent[8]+percent[21]+percent[48]; //B
   alpha[1]=percent[1]+percent[34]+percent[50]; //C
   alpha[2]=percent[12]+percent[67]+percent[71]; //D
   alpha[3]=percent[15]+percent[43]+percent[46]; //E
   alpha[4]=percent[4]+percent[29]+percent[62]; //F
   alpha[5]=percent[6]+percent[35]+percent[64]; //G
   alpha[6]=percent[0]+percent[61]+percent[69]; //H
   alpha[7]=percent[23]+percent[27]+percent[63]; //I
   alpha[8]=percent[2]+percent[30]+percent[49]; //J
   alpha[9]=percent[33]+percent[59]+percent[68]; //K
   alpha[10]=percent[32]+percent[42]+percent[65]; //L
   alpha[11]=percent[14]+percent[56]+percent[48]; //M
   alpha[12]=percent[13]+percent[26]+percent[36]; //N
   alpha[13]=percent[7]+percent[24]+percent[44]; //O
   alpha[14]=percent[37]+percent[52]+percent[53]; //P
   alpha[15]=percent[16]+percent[17]+percent[18]; //R
   alpha[16]=percent[10]+percent[11]+percent[70]; //S 
   alpha[17]=percent[20]+percent[22]+percent[45]; //T
   alpha[18]=percent[47]+percent[60]+percent[66]; //U
   alpha[19]=percent[41]+percent[51]+percent[58]; //V
   alpha[20]=percent[19]+percent[25]+percent[31]; //W
   alpha[21]=percent[5]+percent[39]+percent[54]; //X
   alpha[22]=percent[3]+percent[38]+percent[57]; //Y
   alpha[23]=percent[28]+percent[40]+percent[55]; //Z
   
   max=0.0;
   for(int i = 0; i<24; i++) {
      if(max<alpha[i]) {
         max=((float)cnt)/(cols*rows);
         maxIdx=i;
      }
   }
   switch(maxIdx) {
      case 0: result='B';
      break;
      case 1: result='C';
      break;
      case 2: result='D';
      break;
      case 3: result='E';
      break;
      case 4: result='F';
      break;
      case 5: result='G';
      break;
      case 6: result='H';
      break;
      case 7: result='I';
      break;
      case 8: result='J';
      break;
      case 9: result='K';
      break;
      case 10: result='L';
      break;
      case 11: result='M';
      break;
      case 12: result='N';
      break;
      case 13: result='O';
      break;
      case 14: result='P';
      break;
      case 15: result='R';
      break;
      case 16: result='S';
      break;
      case 17: result='T';
      break;
      case 18: result='U';
      break;
      case 19: result='V';
      break;
      case 20: result='W';
      break;
      case 21: result='X';
      break;
      case 22: result='Y';
      break;
      case 23: result='Z';
      break;
   }*/
   
   switch(maxIdx) {
      case 0: result='H';
      break;
      case 1: result='C';
      break;
      case 2: result='J';
      break;
      case 3: result='Y';
      break;
      case 4: result='F';
      break;
      case 5: result='X';
      break;
      case 6: result='G';
      break;
      case 7: result='O';
      break;
      case 8: result='B';
      break;
      case 9: result='M';
      break;
      case 10: result='S';
      break;
      case 11: result='S';
      break;
      case 12: result='D';
      break;
      case 13: result='N';
      break;
      case 14: result='M';
      break;
      case 15: result='E';
      break;
      case 16: result='R';
      break;
      case 17: result='R';
      break;
      case 18: result='R';
      break;
      case 19: result='W';
      break;
      case 20: result='T';
      break;
      case 21: result='B';
      break;
      case 22: result='T';
      break;
      case 23: result='I';
      break;
      case 24: result='O';
      break;
      case 25: result='W';
      break;
      case 26: result='N';
      break;
      case 27: result='I';
      break;
      case 28: result='Z';
      break;
      case 29: result='F';
      break;
      case 30: result='J';
      break;
      case 31: result='W';
      break;
      case 32: result='L';
      break;
      case 33: result='K';
      break;
      case 34: result='C';
      break;
      case 35: result='G';
      break;
      case 36: result='N';
      break;
      case 37: result='P';
      break;
      case 38: result='Y';
      break;
      case 39: result='X';
      break;
      case 40: result='Z';
      break;
      case 41: result='V';
      break;
      case 42: result='L';
      break;
      case 43: result='E';
      break;
      case 44: result='O';
      break;
      case 45: result='T';
      break;
      case 46: result='E';
      break;
      case 47: result='U';
      break;
      case 48: result='B';
      break;
      case 49: result='J';
      break;
      case 50: result='C';
      break;
      case 51: result='V';
      break;
      case 52: result='P';
      break;
      case 53: result='P';
      break;
      case 54: result='X';
      break;
      case 55: result='Z';
      break;
      case 56: result='M';
      break;
      case 57: result='Y';
      break;
      case 58: result='V';
      break;
      case 59: result='K';
      break;
      case 60: result='U';
      break;
      case 61: result='H';
      break;
      case 62: result='F';
      break;
      case 63: result='I';
      break;
      case 64: result='G';
      break;
      case 65: result='L';
      break;
      case 66: result='U';
      break;
      case 67: result='D';
      break;
      case 68: result='K';
      break;
      case 69: result='H';
      break;
      case 70: result='S';
      break;
      case 71: result='D';
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
	return cnt;
}

std::string readImg(cv::Mat image, std::string imgName) {
	cv::Mat thresholded; 
	cv::threshold(~image, thresholded, 1, 255, cv::THRESH_BINARY);

	cv::Mat newImg = rmNoise(thresholded);


	char result[5];
	int pixel;



	calcX(newImg);
	calcY(newImg);
	
	


	cv::imshow("aaaaa", newImg);
	std::ostringstream filename;
	filename << imgName << 1 << ".jpg";
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

	//imwrite(filename.str(), croppedImg);
	pixel = calcPixel(croppedImg, windowX[0][1] - windowX[0][0], windowY[0][1] - windowY[0][0]);

	std::ostringstream filename2;
	filename2 << imgName << 2 << ".jpg";
	cv::Rect secROI(windowX[1][0], windowY[1][0], windowX[1][1] - windowX[1][0], windowY[1][1] - windowY[1][0]);
	croppedImg = newImg(secROI);
	//imwrite(filename2.str(), croppedImg);
	pixel = calcPixel(croppedImg, windowX[1][1] - windowX[1][0], windowY[1][1] - windowY[1][0]);
	if(windowX[1][1] - windowX[1][0] < 12) {
	   result[1]='I';
	}
	else if(windowX[1][1] - windowX[1][0] < 14) {
	   result[1]='J';
	}
	else {
	   result[1]=compareImg(croppedImg);
	}

	std::ostringstream filename3;
	filename3 << imgName << 3 << ".jpg";
	cv::Rect thiROI(windowX[2][0], windowY[2][0], windowX[2][1] - windowX[2][0], windowY[2][1] - windowY[2][0]);
	croppedImg = newImg(thiROI);
	//imwrite(filename3.str(), croppedImg);
	pixel = calcPixel(croppedImg, windowX[2][1] - windowX[2][0], windowY[2][1] - windowY[2][0]);
	if(windowX[2][1] - windowX[2][0] < 12) {
	   result[2]='I';
	}
	else if(windowX[2][1] - windowX[2][0] < 14) {
	   result[2]='J';
	}
	else {
	   result[2]=compareImg(croppedImg);
	}

	std::ostringstream filename4;
	filename4 << imgName << 4 << ".jpg";
	cv::Rect forROI(windowX[3][0], windowY[3][0], windowX[3][1] - windowX[3][0], windowY[3][1] - windowY[3][0]);
	croppedImg = newImg(forROI);
	//imwrite(filename4.str(), croppedImg);
	pixel = calcPixel(croppedImg, windowX[3][1] - windowX[3][0], windowY[3][1] - windowY[3][0]);
	if(windowX[3][1] - windowX[3][0] < 12) {
	   result[3]='I';
	}
	else if(windowX[3][1] - windowX[3][0] < 14) {
	   result[3]='J';
	}
	else {
	   result[3]=compareImg(croppedImg);
	}

	std::ostringstream filename5;
	filename5 << imgName << 5 << ".jpg";
	cv::Rect fifROI(windowX[4][0], windowY[4][0], windowX[4][1] - windowX[4][0], windowY[4][1] - windowY[4][0]);
	croppedImg = newImg(fifROI);
	//imwrite(filename5.str(), croppedImg);
	pixel = calcPixel(croppedImg, windowX[4][1] - windowX[4][0], windowY[4][1] - windowY[4][0]);
	if(windowX[4][1] - windowX[4][0] < 12) {
	   result[4]='I';
	}
	else if(windowX[4][1] - windowX[4][0] < 14) {
	   result[4]='J';
	}
	else {
	   result[4]=compareImg(croppedImg);
	}

	cv::waitKey(0);
	return result;
}
int main()
{
	std::string dirName = DIR_NAME;
	DIR *dir;
	dir = opendir(dirName.c_str());
	std::string imgName;
	fileIdx = 0;
	cv::Mat image;
	int cnt = 0;
	int ans = 0;
	int ansSet[24]={0, };
	int ansCnt[24]={0, };
	struct dirent *ent;
	std::string tmp;
	std::string tmpchar;
	std::string tmpImgName;
	std::ofstream outFile("output.txt");
	readAnswerSet();
	if (dir != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			imgName = ent->d_name;
			if (imgName == "." || imgName == "..") continue;
			std::string fullPath(dirName + imgName);
			std::cout << fullPath << std::endl;
			image = cv::imread(fullPath, 0);
			tmp=readImg(image, imgName);
			cnt++;
			outFile << imgName << "\t" << tmp.substr(0,5) << std::endl;
			if(tmp.substr(0,5).compare(imgName.substr(0,5))==0)ans++;
			for(int i =0; i<5; i++) {
			   if(tmp.substr(i,1).compare(imgName.substr(i,1))==0) {
			      tmpchar=tmp.substr(i,1);
			      if(tmpchar=="B") ansSet[0]++;
			      if(tmpchar=="C") ansSet[1]++;
			      if(tmpchar=="D") ansSet[2]++;
			      if(tmpchar=="E") ansSet[3]++;
			      if(tmpchar=="F") ansSet[4]++;
			      if(tmpchar=="G") ansSet[5]++;
			      if(tmpchar=="H") ansSet[6]++;
			      if(tmpchar=="I") ansSet[7]++;
			      if(tmpchar=="J") ansSet[8]++;
			      if(tmpchar=="K") ansSet[9]++;
			      if(tmpchar=="L") ansSet[10]++;
			      if(tmpchar=="M") ansSet[11]++;
			      if(tmpchar=="N") ansSet[12]++;
			      if(tmpchar=="O") ansSet[13]++;
			      if(tmpchar=="P") ansSet[14]++;
			      if(tmpchar=="R") ansSet[15]++;
			      if(tmpchar=="S") ansSet[16]++;
			      if(tmpchar=="T") ansSet[17]++;
			      if(tmpchar=="U") ansSet[18]++;
			      if(tmpchar=="V") ansSet[19]++;
			      if(tmpchar=="W") ansSet[20]++;
			      if(tmpchar=="X") ansSet[21]++;
			      if(tmpchar=="Y") ansSet[22]++;
			      if(tmpchar=="Z") ansSet[23]++;
			   }
			   tmpImgName=imgName.substr(i,1);
			   if(tmpImgName=="B") ansCnt[0]++;
			   if(tmpImgName=="C") ansCnt[1]++;
			   if(tmpImgName=="D") ansCnt[2]++;
			   if(tmpImgName=="E") ansCnt[3]++;
			   if(tmpImgName=="F") ansCnt[4]++;
			   if(tmpImgName=="G") ansCnt[5]++;
			   if(tmpImgName=="H") ansCnt[6]++;
			   if(tmpImgName=="I") ansCnt[7]++;
            if(tmpImgName=="J") ansCnt[8]++;
		      if(tmpImgName=="K") ansCnt[9]++;
		      if(tmpImgName=="L") ansCnt[10]++;
		      if(tmpImgName=="M") ansCnt[11]++;
		      if(tmpImgName=="N") ansCnt[12]++;
		      if(tmpImgName=="O") ansCnt[13]++;
		      if(tmpImgName=="P") ansCnt[14]++;
		      if(tmpImgName=="R") ansCnt[15]++;
		      if(tmpImgName=="S") ansCnt[16]++;
		      if(tmpImgName=="T") ansCnt[17]++;
		      if(tmpImgName=="U") ansCnt[18]++;
		      if(tmpImgName=="V") ansCnt[19]++;
		      if(tmpImgName=="W") ansCnt[20]++;
		      if(tmpImgName=="X") ansCnt[21]++;
		      if(tmpImgName=="Y") ansCnt[22]++;
		      if(tmpImgName=="Z") ansCnt[23]++;
			}
		}
		closedir(dir);
		
	}
	else {
		std::cout << "Error!" << std::endl;
	}
	if (!image.data)
		return 0;
   outFile<<"B: "<<((float)ansSet[0])/ansCnt[0]*100<<"%"<<std::endl;
   outFile<<"C: "<<((float)ansSet[1])/ansCnt[1]*100<<"%"<<std::endl;
   outFile<<"D: "<<((float)ansSet[2])/ansCnt[2]*100<<"%"<<std::endl;
   outFile<<"E: "<<((float)ansSet[3])/ansCnt[3]*100<<"%"<<std::endl;
   outFile<<"F: "<<((float)ansSet[4])/ansCnt[4]*100<<"%"<<std::endl;
   outFile<<"G: "<<((float)ansSet[5])/ansCnt[5]*100<<"%"<<std::endl;
   outFile<<"H: "<<((float)ansSet[6])/ansCnt[6]*100<<"%"<<std::endl;
   outFile<<"I: "<<((float)ansSet[7])/ansCnt[7]*100<<"%"<<std::endl;
   outFile<<"J: "<<((float)ansSet[8])/ansCnt[8]*100<<"%"<<std::endl;
   outFile<<"K: "<<((float)ansSet[9])/ansCnt[9]*100<<"%"<<std::endl;
   outFile<<"L: "<<((float)ansSet[10])/ansCnt[10]*100<<"%"<<std::endl;
   outFile<<"M: "<<((float)ansSet[11])/ansCnt[11]*100<<"%"<<std::endl;
   outFile<<"N: "<<((float)ansSet[12])/ansCnt[12]*100<<"%"<<std::endl;
   outFile<<"O: "<<((float)ansSet[13])/ansCnt[13]*100<<"%"<<std::endl;
   outFile<<"P: "<<((float)ansSet[14])/ansCnt[14]*100<<"%"<<std::endl;
   outFile<<"R: "<<((float)ansSet[15])/ansCnt[15]*100<<"%"<<std::endl;
   outFile<<"S: "<<((float)ansSet[16])/ansCnt[16]*100<<"%"<<std::endl;
   outFile<<"T: "<<((float)ansSet[17])/ansCnt[17]*100<<"%"<<std::endl;
   outFile<<"U: "<<((float)ansSet[18])/ansCnt[18]*100<<"%"<<std::endl;
   outFile<<"V: "<<((float)ansSet[19])/ansCnt[19]*100<<"%"<<std::endl;
   outFile<<"W: "<<((float)ansSet[20])/ansCnt[20]*100<<"%"<<std::endl;
   outFile<<"X: "<<((float)ansSet[21])/ansCnt[21]*100<<"%"<<std::endl;
   outFile<<"Y: "<<((float)ansSet[22])/ansCnt[22]*100<<"%"<<std::endl;
   outFile<<"Z: "<<((float)ansSet[23])/ansCnt[23]*100<<"%"<<std::endl;
   outFile<<"Total count: " <<cnt <<" Total answer: "<<ans<<std::endl;
   outFile<<"Success rate: "<<((float)ans)/cnt*100<<"%"<<std::endl;

   outFile.close();
	return 0;
}
