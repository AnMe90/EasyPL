#include "../../include/oir/worklist.h"

namespace oir
{
	Batch bitch;
	CPlateLocate plate;
	CPlateJudge cplatejudge;
	EnhanceSsr enhancessr;
	vector<cv::Mat> resultVec2;//判断为车牌
	

	//函数功能：选择对应的定位算法（1是颜色定位，2是Sobel，3是增强后再进行1，2处理，直到达到需求或达到算法指定次数）
	//image是原始图像，eimage是增强后的图像,resultVec是保存疑似车牌的图块容器，index是选择标识，分别为1，2，3的时候
	//进行不同的算法调用；
	//result是处理成功后返回的一个指示符，为0表示定位成功
	void Chosefunc(Mat &image,Mat& eimage,vector<cv::Mat>& resultVec, int &index, int &result)
	{
		//Mat eimage;
		switch (index)
		{
		case 1:
			result = plate.plateLocatecolor(image, resultVec);//颜色
			cout <<"case1" << endl;
			break;
		case 2:
			result = plate.plateLocatesobel(image, resultVec);//Sobel
			cout << "case2" << endl;
			break;
		case 3:
			enhancessr.ESSR(image, eimage);
			//image = eimage;
			//result = plate.plateLocatecolor(image, resultVec);

			break;
		default:
			//result = plate.plateLocatesobel(image, resultVec);//默认Sobel
			break;
		}
	}

	//批量将图片保存到指定文件夹
	//i是循环的输入量，指的是第i个图像
	//file_list是图片的名的容器
	//file_result是输出目录
	//result 是需要什么时候保存的一个指示值，当为0就需要保存，不为0不进行保存
	void Writefile(int& i, vector<string> &file_list, string &file_result, int& result)
	{
		stringstream ss(stringstream::in | stringstream::out);
		if (result == 0) {
			size_t num = resultVec2.size();

			cout << num << endl;

			String s;
			for (int j = 0; j < num; ++j) {
				cv::Mat resultMat = resultVec2[j];
				ss << file_result << file_list[i] << j << ".jpg";
				//ss << j<<".jpg";
				cout << 'S' << j << endl;

				ss >> s;
				cout << s << endl;
				//imshow(s, resultMat);
				imwrite(s, resultMat);
				ss.clear();

				//waitKey(0);
			}
			//destroyWindow(s);
		}
	}

	//函数功能，根据
	void WorkList::Wlist(string &file_path, string &file_result,string &file_error, int &index, int& flagerr)
	{
		vector<string> file_list;
		//得到原始路径中的所有图片名称列表

		bitch.CBatch(file_path, file_list);
		
		//

		for (int i = 0; i < file_list.size(); i++)
		{
			string image_path = file_path + file_list[i];
			Mat image = imread(image_path);
			Mat eimage;
			
			//stringstream ss(stringstream::in | stringstream::out);
			//Mat eimage;

			//图像增强

			//enhancessr.ESSR(image, eimage);

			/////定位主程序
			vector<cv::Mat> resultVec;//疑似车牌
			//vector<cv::Mat> resultVec2;//判断为车牌
			int flag = 0;
			flagerr = 0;//控制只进行一次图像增强操作
			int result=0;
			for (int k = 1; k < 4; k++)
			{
				index = k;
				//选择用哪个方法
				Chosefunc(image,eimage,resultVec, index, result);
				cout << k << endl;
				if (k == 3&&flagerr<1)
				{
					k = 0;
					flagerr++;
					//image = eimage;
					continue;
				}
				plate.setDebug(1);
				plate.setLifemode(true);

				//imwrite(file_result + file_list[i], image);
				flag = cplatejudge.plateJudge(resultVec, resultVec2, flag);//新增加flag标记
				if (flag == 0)
				{
					//imwrite(file_error + file_list[i]+".jpg",image);
					cout << "fail" << endl;
					continue;
					
				}
				else{
					Writefile(i, file_list, file_result, result);//保存结果图片
					resultVec2.clear();
					cout << "success" << endl;
					break;
				}
			}
			
		
		}

	}
}