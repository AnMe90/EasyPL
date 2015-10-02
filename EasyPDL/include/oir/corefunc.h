#ifndef __COREFUNC_H__
#define __COREFUNC_H__

#include <opencv2/opencv.hpp>

using namespace cv;

namespace oir {

	enum Color { BLUE, YELLOW, WHITE, UNKNOWN };

	enum LocateType { SOBEL, COLOR, OTHER };

	//! ����һ��ͼ������ɫģ���ȡ��Ӧ�Ķ�ֵͼ
	//! ����RGBͼ��, ��ɫģ�壨��ɫ����ɫ��
	//! ����Ҷ�ͼ��ֻ��0��255����ֵ��255����ƥ�䣬0����ƥ�䣩
	Mat colorMatch(const Mat& src, Mat& match, const Color r,
		const bool adaptive_minsv);

	//! �ж�һ�����Ƶ���ɫ
	//! ���복��mat����ɫģ��
	//! ����true��fasle
	bool plateColorJudge(const Mat& src, const Color r, const bool adaptive_minsv,
		float& percent);

	bool bFindLeftRightBound(Mat& bound_threshold, int& posLeft, int& posRight);
	bool bFindLeftRightBound1(Mat& bound_threshold, int& posLeft, int& posRight);
	bool bFindLeftRightBound2(Mat& bound_threshold, int& posLeft, int& posRight);

	//ȥ�������Ϸ���ť��
	//����ÿ��Ԫ�صĽ�Ծ�������С��X��Ϊ��������������ȫ����0��Ϳ�ڣ�
	// X���Ƽ�ֵΪ���ɸ���ʵ�ʵ���
	bool clearLiuDing(Mat& img);
	void clearLiuDingOnly(Mat& img);
	void clearLiuDing(Mat mask, int& top, int& bottom);

	//! ��ó�����ɫ
	Color getPlateType(const Mat& src, const bool adaptive_minsv);

	//! ֱ��ͼ����
	Mat histeq(Mat in);
	Mat features(Mat in, int sizeData);
	Rect GetCenterRect(Mat& in);
	Mat CutTheRect(Mat& in, Rect& rect);
	int ThresholdOtsu(Mat mat);

	//! ��ȡ��ֱ��ˮƽ����ֱ��ͼ
	Mat ProjectedHistogram(Mat img, int t);

} /*! \namespace oir*/

#endif
/* endif __COREFUNC_H__ */