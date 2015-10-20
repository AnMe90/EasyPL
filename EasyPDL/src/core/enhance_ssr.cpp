#include "../../include/oir/enhance_ssr.h"

void SSR(IplImage* src, int sigma, int scale)
{
	IplImage* src_fl = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, src->nChannels);
	IplImage* src_fl1 = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, src->nChannels);
	IplImage* src_fl2 = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, src->nChannels);
	float a = 0.0, b = 0.0, c = 0.0;
	cvConvertScale(src, src_fl, 1.0, 1.0);//ת����Χ������ͼ��Ԫ������1.0��֤cvlog����
	cvLog(src_fl, src_fl1);

	cvSmooth(src_fl, src_fl2, CV_GAUSSIAN, 0, 0, sigma);        //SSR�㷨�ĺ���֮һ����˹ģ��

	cvLog(src_fl2, src_fl2);
	cvSub(src_fl1, src_fl2, src_fl);//Retinex��ʽ��Log(R(x,y))=Log(I(x,y))-Log(Gauss(I(x,y)))

	//����ͼ��ľ�ֵ�����SSR�㷨�ĺ���֮��
	//ʹ��GIMP��ת��������ʹ��ͼ��ľ�ֵ�������Ϣ���б任
	//û���������ж�
	CvScalar mean;
	CvScalar dev;
	cvAvgSdv(src_fl, &mean, &dev, NULL);//����ͼ��ľ�ֵ�ͱ�׼��
	double min[3];
	double max[3];
	double maxmin[3];
	for (int i = 0; i < 3; i++)
	{
		min[i] = mean.val[i] - scale*dev.val[i];
		max[i] = mean.val[i] + scale*dev.val[i];
		maxmin[i] = max[i] - min[i];
	}
	float* data2 = (float*)src_fl->imageData;
	for (int i = 0; i < src_fl2->width; i++)
	{
		for (int j = 0; j < src_fl2->height; j++)
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

namespace oir{
	//ͼ����ǿ����ESSR������SSR
	void EnhanceSsr::ESSR(Mat& image, Mat& result)
	{
		IplImage* frog;
		frog = &IplImage(image);
		IplImage* frog1 = cvCreateImage(cvGetSize(frog), IPL_DEPTH_32F, frog->nChannels);
		cvConvertScale(frog, frog1, 1.0 / 255, 0);

		SSR(frog, 30, 2);
		Mat(frog, true);
		result = frog;
	}
}