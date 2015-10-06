#include "include/oir/arealocate.h"
using namespace oir;

#include <opencv2\opencv.hpp>
#include <iostream>
#include <string>
#include<io.h>
#include<string.h>

using namespace cv;
using namespace std;


bool get_filelist_from_dir(string path, vector<string>& files)
{
	long   hFile = 0;
	struct _finddata_t fileinfo;
	files.clear();
	if ((hFile = _findfirst(path.c_str(), &fileinfo)) != -1)
	{
		do
		{
			if (!(fileinfo.attrib &  _A_SUBDIR))
				files.push_back(fileinfo.name);
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
		return true;
	}
	else
		return false;
}

void SSR(IplImage* src, int sigma, int scale)
{
	IplImage* src_fl = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, src->nChannels);
	IplImage* src_fl1 = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, src->nChannels);
	IplImage* src_fl2 = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, src->nChannels);
	float a = 0.0, b = 0.0, c = 0.0;
	cvConvertScale(src, src_fl, 1.0, 1.0);//转换范围，所有图像元素增加1.0保证cvlog正常
	cvLog(src_fl, src_fl1);

	cvSmooth(src_fl, src_fl2, CV_GAUSSIAN, 0, 0, sigma);        //SSR算法的核心之一，高斯模糊

	cvLog(src_fl2, src_fl2);
	cvSub(src_fl1, src_fl2, src_fl);//Retinex公式，Log(R(x,y))=Log(I(x,y))-Log(Gauss(I(x,y)))

	//计算图像的均值、方差，SSR算法的核心之二
	//使用GIMP中转换方法：使用图像的均值方差等信息进行变换
	//没有添加溢出判断
	CvScalar mean;
	CvScalar dev;
	cvAvgSdv(src_fl, &mean, &dev, NULL);//计算图像的均值和标准差
	double min[3];
	double max[3];
	double maxmin[3];
	for (int i = 0; i<3; i++)
	{
		min[i] = mean.val[i] - scale*dev.val[i];
		max[i] = mean.val[i] + scale*dev.val[i];
		maxmin[i] = max[i] - min[i];
	}
	float* data2 = (float*)src_fl->imageData;
	for (int i = 0; i<src_fl2->width; i++)
	{
		for (int j = 0; j<src_fl2->height; j++)
		{
			data2[j*src_fl->widthStep / 4 + 3 * i + 0] = 255 * (data2[j*src_fl->widthStep / 4 + 3 * i + 0] - min[0]) / maxmin[0];
			data2[j*src_fl->widthStep / 4 + 3 * i + 1] = 255 * (data2[j*src_fl->widthStep / 4 + 3 * i + 1] - min[1]) / maxmin[1];
			data2[j*src_fl->widthStep / 4 + 3 * i + 2] = 255 * (data2[j*src_fl->widthStep / 4 + 3 * i + 2] - min[2]) / maxmin[2];
		}
	}


	cvConvertScale(src_fl, src, 1, 0);
	cvReleaseImage(&src_fl);
	cvReleaseImage(&src_fl1);
	cvReleaseImage(&src_fl2);
}


int main()
{
	string file_path = "D:/workspace/nocolor/";
	string search_path = file_path + "*.jpg";
	vector<string> file_list;
	if (!get_filelist_from_dir(search_path, file_list))
		cout << "open file error!" << endl;
	for (int i = 0; i < file_list.size(); i++)
	{
		string image_path = file_path + file_list[i];
		Mat image = imread(image_path);

		

		//imshow("image", image);
		/*CPlateLocate PlateLocate;
		Mat srcgray;
		vector<Mat> vecRag;
		int index;*/
		//int result=PlateLocate.plateLocate(image, vecRag);
		//colorMatch(image, srcgray, BLUE, true);
		//size_t num = vecRag.size();
		
		//cout <<i<<endl;
		//colorMatch(image, srcgray, BLUE, true);

		//imwrite(s, image);

		

		/*ss << file_list[i];
		String s;
		ss >> s;*/
		//cout << s;
		//Mat resultMat = vecRag[j];
		//imshow("1", resultMat);
		//imwrite(s, srcgray);

		//string 转 char*
		//int len = file_list[i].length();
		//const char* filename;
		////filename = (char *)malloc((len)*sizeof(char));//len+1
		//filename = file_list[i].data();
		//cout << file_list[i]<<endl;
		////file_list[i].copy(filename, len, 0);
		//cout <<filename<< endl;
		stringstream ss(stringstream::in | stringstream::out);

		/////图像增强程序
		/*IplImage* frog;
		frog = &IplImage(image);
		IplImage* frog1 = cvCreateImage(cvGetSize(frog), IPL_DEPTH_32F, frog->nChannels);
		cvConvertScale(frog, frog1, 1.0 / 255, 0);
		
		SSR(frog, 30, 2);
		Mat(frog, false);
		Mat image1 = frog;*/
		//imshow("Ipflog", image1);
		//imwrite(file_list[i], image1);



		/////定位主程序
		vector<cv::Mat> resultVec;
		CPlateLocate plate;
		plate.setDebug(1);
		plate.setLifemode(true);

		int result = plate.plateLocate(image, resultVec);
		imwrite(file_list[i], image);

		/////输出每个疑似车牌图块
		if (result == 0) {
			size_t num = resultVec.size();
			
			cout << num << endl;
			String s;
			for (int j = 0; j < num; ++j) {
				cv::Mat resultMat = resultVec[j];
				ss << file_list[i] << j<<".jpg";
				//ss << j<<".jpg";
				cout << 'S'<<j << endl;
				
				ss >> s;
				
				//imshow(s, resultMat);
				//imwrite(s, resultMat);
				ss.clear();
				
				//waitKey(0);
			}
			//destroyWindow(s);
		}
			

		
		

		//Mat element2 = getStructuringElement(MORPH_RECT, Size(5, 1));
		//dilate(candPlate, srcgray, element2);//膨胀
		//imshow("srcgray", srcgray);


	}
	//waitKey(0);
	return 0;
}
