#include "../../include/oir/worklist.h"

namespace oir
{
	Batch bitch;
	CPlateLocate plate;
	CPlateJudge cplatejudge;
	EnhanceSsr enhancessr;
	vector<cv::Mat> resultVec2;//�ж�Ϊ����
	

	//�������ܣ�ѡ���Ӧ�Ķ�λ�㷨��1����ɫ��λ��2��Sobel��3����ǿ���ٽ���1��2����ֱ���ﵽ�����ﵽ�㷨ָ��������
	//image��ԭʼͼ��eimage����ǿ���ͼ��,resultVec�Ǳ������Ƴ��Ƶ�ͼ��������index��ѡ���ʶ���ֱ�Ϊ1��2��3��ʱ��
	//���в�ͬ���㷨���ã�
	//result�Ǵ���ɹ��󷵻ص�һ��ָʾ����Ϊ0��ʾ��λ�ɹ�
	void Chosefunc(Mat &image,Mat& eimage,vector<cv::Mat>& resultVec, int &index, int &result)
	{
		//Mat eimage;
		switch (index)
		{
		case 1:
			result = plate.plateLocatecolor(image, resultVec);//��ɫ
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
			//result = plate.plateLocatesobel(image, resultVec);//Ĭ��Sobel
			break;
		}
	}

	//������ͼƬ���浽ָ���ļ���
	//i��ѭ������������ָ���ǵ�i��ͼ��
	//file_list��ͼƬ����������
	//file_result�����Ŀ¼
	//result ����Ҫʲôʱ�򱣴��һ��ָʾֵ����Ϊ0����Ҫ���棬��Ϊ0�����б���
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

	//�������ܣ�����
	void WorkList::Wlist(string &file_path, string &file_result,string &file_error, int &index, int& flagerr)
	{
		vector<string> file_list;
		//�õ�ԭʼ·���е�����ͼƬ�����б�

		bitch.CBatch(file_path, file_list);
		
		//

		for (int i = 0; i < file_list.size(); i++)
		{
			string image_path = file_path + file_list[i];
			Mat image = imread(image_path);
			Mat eimage;
			
			//stringstream ss(stringstream::in | stringstream::out);
			//Mat eimage;

			//ͼ����ǿ

			//enhancessr.ESSR(image, eimage);

			/////��λ������
			vector<cv::Mat> resultVec;//���Ƴ���
			//vector<cv::Mat> resultVec2;//�ж�Ϊ����
			int flag = 0;
			flagerr = 0;//����ֻ����һ��ͼ����ǿ����
			int result=0;
			for (int k = 1; k < 4; k++)
			{
				index = k;
				//ѡ�����ĸ�����
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
				flag = cplatejudge.plateJudge(resultVec, resultVec2, flag);//������flag���
				if (flag == 0)
				{
					//imwrite(file_error + file_list[i]+".jpg",image);
					cout << "fail" << endl;
					continue;
					
				}
				else{
					Writefile(i, file_list, file_result, result);//������ͼƬ
					resultVec2.clear();
					cout << "success" << endl;
					break;
				}
			}
			
		
		}

	}
}