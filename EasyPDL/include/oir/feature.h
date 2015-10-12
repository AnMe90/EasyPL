#ifndef __FEATURE_H__
#define __FEATURE_H__

#include <opencv2/opencv.hpp>

namespace oir {

	//! ��ó��Ƶ�������
	cv::Mat getTheFeatures(cv::Mat in);

	//! EasyPR��getFeatures�ص�����
	//! ���ڴӳ��Ƶ�image����svm��ѵ������features
	typedef void(*svmCallback)(const cv::Mat& image, cv::Mat& features);

	//! EasyPR��getFeatures�ص�����
	//! ������������ֱ��ͼ���������Ļص�����
	void getHisteqFeatures(const cv::Mat& image, cv::Mat& features);

	//!  EasyPR��getFeatures�ص�����
	//! �������ǻ�ȡ��ֱ��ˮƽ��ֱ��ͼͼֵ
	void getHistogramFeatures(const cv::Mat& image, cv::Mat& features);

	//! �������ǻ�ȡSIFT������
	void getSIFTFeatures(const cv::Mat& image, cv::Mat& features);

	//! �������ǻ�ȡHOG������
	void getHOGFeatures(const cv::Mat& image, cv::Mat& features);

	//! �������ǻ�ȡHSV�ռ�������ֱ��ͼ������
	void getHSVHistFeatures(const cv::Mat& image, cv::Mat& features);

}	/*! \namespace easypr*/

#endif
/* endif __FEATURE_H__ */
