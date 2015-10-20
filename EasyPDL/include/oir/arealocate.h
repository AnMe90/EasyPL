#ifndef __PLATE_LOCATE_H__
#define __PLATE_LOCATE_H__

#include "area.h"
#include "corefunc.h"

namespace oir {

	class CPlateLocate {
	public:
		CPlateLocate();

		//! Sobel��һ������
		//! �����ƴ�С����״����ȡ��BoundRect������һ��
		int sobelFrtSearch(const Mat& src, vector<Rect_<float>>& outRects);

		//! Sobel�ڶ�������
		//! �Դ�С����״�����ƣ����ɲο�����
		int sobelSecSearch(Mat& bound, Point2f refpoint,
			vector<RotatedRect>& outRects);
		int sobelSecSearchPart(Mat& bound, Point2f refpoint,
			vector<RotatedRect>& outRects);

		//! ��Ťб����
		int deskew(const Mat& src, const Mat& src_b, vector<RotatedRect>& inRects,
			vector<CPlate>& outPlates);

		//! �Ƿ�ƫб
		//! �����ֵ��ͼ������жϽ��
		bool isdeflection(const Mat& in, const double angle, double& slope);

		//! Sobel����
		//! �����ɫͼ�������ֵ��ͼ��
		int sobelOper(const Mat& in, Mat& out, int blurSize, int morphW, int morphH);

		//! ����һ����ȫ��Rect
		bool calcSafeRect(const RotatedRect& roi_rect, const Mat& src,
			Rect_<float>& safeBoundRect);

		//! ��ת����
		bool rotation(Mat& in, Mat& out, const Size rect_size, const Point2f center,
			const double angle);

		//! Ť�����
		void affine(const Mat& in, Mat& out, const double slope);

		//! ��ɫ��λ��
		int plateColorLocate(Mat src, vector<CPlate>& candPlates, int index = 0);

		//! Sobel��λ��
		int plateSobelLocate(Mat src, vector<CPlate>& candPlates, int index = 0);
		int sobelOperT(const Mat& in, Mat& out, int blurSize, int morphW, int morphH);

		//! Color����
		int colorSearch(const Mat& src, const Color r, Mat& out,
			vector<RotatedRect>& outRects, int index = 0);

		//! δʹ�ú��������
		//! ��ʼ------------
		//! ����------------
		//! δʹ�ú��������

		//! ���ƶ�λ
		int plateLocatecolor(Mat, vector<Mat>&, int = 0);
		int plateLocatesobel(Mat, vector<Mat>&, int = 0);//�����ӵķ������ɫ��sobel
		//! ���Ƶĳߴ���֤
		bool verifySizes(RotatedRect mr);

		//! ����ģʽ�빤ҵģʽ�л�
		void setLifemode(bool param);

		//! �������ȡ����
		inline void setGaussianBlurSize(int param) { m_GaussianBlurSize = param; }
		inline int getGaussianBlurSize() const { return m_GaussianBlurSize; }

		inline void setMorphSizeWidth(int param) { m_MorphSizeWidth = param; }
		inline int getMorphSizeWidth() const { return m_MorphSizeWidth; }

		inline void setMorphSizeHeight(int param) { m_MorphSizeHeight = param; }
		inline int getMorphSizeHeight() const { return m_MorphSizeHeight; }

		inline void setVerifyError(float param) { m_error = param; }
		inline float getVerifyError() const { return m_error; }
		inline void setVerifyAspect(float param) { m_aspect = param; }
		inline float getVerifyAspect() const { return m_aspect; }

		inline void setVerifyMin(int param) { m_verifyMin = param; }
		inline void setVerifyMax(int param) { m_verifyMax = param; }

		inline void setJudgeAngle(int param) { m_angle = param; }

		//! �Ƿ�������ģʽ
		inline void setDebug(bool param) { m_debug = param; }

		//! ��ȡ����ģʽ״̬
		inline bool getDebug() { return m_debug; }

		//! PlateLocate���ó���
		static const int DEFAULT_GAUSSIANBLUR_SIZE = 5;
		static const int SOBEL_SCALE = 1;
		static const int SOBEL_DELTA = 0;
		static const int SOBEL_DDEPTH = CV_16S;
		static const int SOBEL_X_WEIGHT = 1;
		static const int SOBEL_Y_WEIGHT = 0;
		static const int DEFAULT_MORPH_SIZE_WIDTH = 17;  // 17
		static const int DEFAULT_MORPH_SIZE_HEIGHT = 3;  // 3

		//! showResultMat���ó���
		static const int WIDTH = 136;
		static const int HEIGHT = 36;
		static const int TYPE = CV_8UC3;

		//! verifySize���ó���
		static const int DEFAULT_VERIFY_MIN = 1;  // 3
		static const int DEFAULT_VERIFY_MAX = 24;  // 20

		//! �Ƕ��ж����ó���
		static const int DEFAULT_ANGLE = 60;  // 30

		//! �Ƿ�������ģʽ������Ĭ��0����ر�
		static const int DEFAULT_DEBUG = 0;

	protected:
		//! ��˹ģ�����ñ���
		int m_GaussianBlurSize;

		//! ���Ӳ������ñ���
		int m_MorphSizeWidth;
		int m_MorphSizeHeight;

		//! verifySize���ñ���
		float m_error;
		float m_aspect;
		int m_verifyMin;
		int m_verifyMax;

		//! �Ƕ��ж����ñ���
		int m_angle;

		//! �Ƿ�������ģʽ��Ĭ��0�ر�temp�ļ��±����м䴦��ͼƬ����0����
		bool m_debug;
	};

} /*! \namespace oir*/

#endif /* endif __PLATE_LOCATE_H__ */