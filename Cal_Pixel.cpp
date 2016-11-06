#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>
#include <dirent.h>

using namespace std;

double fileIdx;

int calcPixel(cv::Mat crop, int x, int y) {
   int cnt=0;
   
   for(int i = 0; i<x; i++) {
      for(int j = 0; j<y; j++) {
         if(crop.at<uchar>(j, i)==255) cnt++;
      }
   }
   
   //cout<<"x " << x << " / y " << y << " / total "<< crop.total() <<" / count "<<cnt<<endl;
   return cnt;
}

int main()
{

   DIR *dir;
   int folder = 65 ; 
   string s_folder ;
   int cnt = 0 ;
   int temp_total = 0 ;
   cv::Mat image;
   string imgName;
   string dirName ;
   struct dirent *ent;

   for( ; folder < 91 ; folder++) {
      s_folder = (char)folder;
      dirName = "/home/ilyoung/preprocessingImg-master/101_200_sort/"+s_folder+"/";
      dir = opendir(dirName.c_str());
      fileIdx=0;
      if( dir !=NULL ) {
         while((ent=readdir(dir))!=NULL) {
            imgName = ent->d_name;
            if(imgName=="." || imgName=="..") continue;
            string fullPath(dirName + imgName);
            //cout<<fullPath<<endl;
            image = cv::imread(fullPath, 0);
            cnt++;
            temp_total += calcPixel(image,image.cols,image.rows);
         }
         if(cnt != 0) {
            cout << s_folder << " Average " << temp_total / cnt << endl ;
            cnt =0 ;
            temp_total =0;
         }
         else
            cout << s_folder << " No Image" << endl;
         closedir(dir);
      } 
      else
         cout<<"Error!"<<endl;
   }
   if(!image.data)
      return 0;
   return 0;
}