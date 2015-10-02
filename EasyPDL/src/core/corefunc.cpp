// ����ļ�������OIR������plate�жϵ�ͨ�ú���
// ���������ռ�Ϊoir
// ��������еĺ������ײ�Ҫ�Ķ�

#include <opencv2/opencv.hpp>
#include "../../include/oir/corefunc.h"//���������·��

using namespace cv;

/*! \namespace oir
Namespace where all the C++ EasyPR functionality resides
*/
namespace oir {

	//! ����һ��ͼ������ɫģ���ȡ��Ӧ�Ķ�ֵͼ
	//! ����RGBͼ��, ��ɫģ�壨��ɫ����ɫ��
	//! ����Ҷ�ͼ��ֻ��0��255����ֵ��255����ƥ�䣬0����ƥ�䣩
	Mat colorMatch(const Mat& src, Mat& match, const Color r,
		const bool adaptive_minsv) {
		// S��V����Сֵ��adaptive_minsv���boolֵ�ж�
		// ���Ϊtrue������Сֵȡ����Hֵ��������˥��
		// ���Ϊfalse����������Ӧ��ʹ�ù̶�����Сֵminabs_sv
		// Ĭ��Ϊfalse
		const float max_sv = 255;
		const float minref_sv = 64;

		const float minabs_sv = 95;//ԭ��95

		// blue��H��Χ
		const int min_blue = 100;  // 100
		const int max_blue = 140;  // 140

		// yellow��H��Χ
		const int min_yellow = 15;  // 15
		const int max_yellow = 40;  // 40

		// white��H��Χ
		const int min_white = 15;   // 15
		const int max_white = 40;  // 40

		Mat src_hsv;
		// ת��HSV�ռ���д�����ɫ������Ҫʹ�õ���H����������ɫ���ɫ��ƥ�乤��
		cvtColor(src, src_hsv, CV_BGR2HSV);

		vector<Mat> hsvSplit;
		split(src_hsv, hsvSplit);
		equalizeHist(hsvSplit[2], hsvSplit[2]);
		merge(hsvSplit, src_hsv);

		//ƥ��ģ���ɫ,�л��Բ�����Ҫ�Ļ�ɫ
		int min_h = 0;
		int max_h = 0;
		switch (r) {
		case BLUE:
			min_h = min_blue;
			max_h = max_blue;
			break;
		case YELLOW:
			min_h = min_yellow;
			max_h = max_yellow;
			break;
		case WHITE:
			min_h = min_white;
			max_h = max_white;
			break;
		default:
			// Color::UNKNOWN
			break;
		}

		float diff_h = float((max_h - min_h) / 2);
		float avg_h = min_h + diff_h;

		int channels = src_hsv.channels();
		int nRows = src_hsv.rows;
		//ͼ����������Ҫ����ͨ������Ӱ�죻
		int nCols = src_hsv.cols * channels;

		if (src_hsv.isContinuous()){  //�����洢�����ݣ���һ�д���
			nCols *= nRows;
			nRows = 1;
		}

		int i, j;
		uchar* p;
		float s_all = 0;
		float v_all = 0;
		float count = 0;
		for (i = 0; i < nRows; ++i) {
			p = src_hsv.ptr<uchar>(i);
			for (j = 0; j < nCols; j += 3) {
				int H = int(p[j]);      // 0-180
				int S = int(p[j + 1]);  // 0-255
				int V = int(p[j + 2]);  // 0-255

				s_all += S;
				v_all += V;
				count++;

				bool colorMatched = false;

				if (H > min_h && H < max_h) {
					float Hdiff = 0;
					if (H > avg_h)
						Hdiff = H - avg_h;
					else
						Hdiff = avg_h - H;

					float Hdiff_p = float(Hdiff) / diff_h;

					// S��V����Сֵ��adaptive_minsv���boolֵ�ж�
					// ���Ϊtrue������Сֵȡ����Hֵ��������˥��
					// ���Ϊfalse����������Ӧ��ʹ�ù̶�����Сֵminabs_sv
					float min_sv = 0;
					if (true == adaptive_minsv)
						min_sv =
						minref_sv -
						minref_sv / 2 *
						(1 - Hdiff_p);  // inref_sv - minref_sv / 2 * (1 - Hdiff_p)
					else
						min_sv = minabs_sv;  // add

					if ((S > min_sv && S < max_sv) && (V > min_sv && V < max_sv))
						colorMatched = true;
				}

				if (colorMatched == true) {
					p[j] = 0;
					p[j + 1] = 0;
					p[j + 2] = 255;
				}
				else {
					p[j] = 0;
					p[j + 1] = 0;
					p[j + 2] = 0;
				}
			}
		}

		// ��ȡ��ɫƥ���Ķ�ֵ�Ҷ�ͼ
		Mat src_grey;
		vector<Mat> hsvSplit_done;
		split(src_hsv, hsvSplit_done);
		src_grey = hsvSplit_done[2];

		match = src_grey;

		return src_grey;
	}

	bool bFindLeftRightBound1(Mat& bound_threshold, int& posLeft, int& posRight) {
		//������Ѱ�ұ߽�
		float span = bound_threshold.rows * 0.2f;
		//��߽���
		for (int i = 0; i < bound_threshold.cols - span - 1; i += 3) {
			int whiteCount = 0;
			for (int k = 0; k < bound_threshold.rows; k++) {
				for (int l = i; l < i + span; l++) {
					if (bound_threshold.data[k * bound_threshold.step[0] + l] == 255) {
						whiteCount++;
					}
				}
			}
			if (whiteCount * 1.0 / (span * bound_threshold.rows) > 0.15) {
				posLeft = i;
				break;
			}
		}
		span = bound_threshold.rows * 0.2f;
		//�ұ߽���
		for (int i = bound_threshold.cols - 1; i > span; i -= 2) {
			int whiteCount = 0;
			for (int k = 0; k < bound_threshold.rows; k++) {
				for (int l = i; l > i - span; l--) {
					if (bound_threshold.data[k * bound_threshold.step[0] + l] == 255) {
						whiteCount++;
					}
				}
			}

			if (whiteCount * 1.0 / (span * bound_threshold.rows) > 0.06) {
				posRight = i;
				if (posRight + 5 < bound_threshold.cols) {
					posRight = posRight + 5;
				}
				else {
					posRight = bound_threshold.cols - 1;
				}

				break;
			}
		}

		if (posLeft < posRight) {
			return true;
		}
		return false;
	}

	bool bFindLeftRightBound(Mat& bound_threshold, int& posLeft, int& posRight) {
		//������Ѱ�ұ߽�
		float span = bound_threshold.rows * 0.2f;
		//��߽���
		for (int i = 0; i < bound_threshold.cols - span - 1; i += 2) {
			int whiteCount = 0;
			for (int k = 0; k < bound_threshold.rows; k++) {
				for (int l = i; l < i + span; l++) {
					if (bound_threshold.data[k * bound_threshold.step[0] + l] == 255) {
						whiteCount++;
					}
				}
			}
			if (whiteCount * 1.0 / (span * bound_threshold.rows) > 0.36) {
				posLeft = i;
				break;
			}
		}
		span = bound_threshold.rows * 0.2f;
		//�ұ߽���
		for (int i = bound_threshold.cols - 1; i > span; i -= 2) {
			int whiteCount = 0;
			for (int k = 0; k < bound_threshold.rows; k++) {
				for (int l = i; l > i - span; l--) {
					if (bound_threshold.data[k * bound_threshold.step[0] + l] == 255) {
						whiteCount++;
					}
				}
			}

			if (whiteCount * 1.0 / (span * bound_threshold.rows) > 0.26) {
				posRight = i;
				break;
			}
		}

		if (posLeft < posRight) {
			return true;
		}
		return false;
	}

	bool bFindLeftRightBound2(Mat& bound_threshold, int& posLeft, int& posRight) {
		//������Ѱ�ұ߽�
		float span = bound_threshold.rows * 0.2f;
		//��߽���
		for (int i = 0; i < bound_threshold.cols - span - 1; i += 3) {
			int whiteCount = 0;
			for (int k = 0; k < bound_threshold.rows; k++) {
				for (int l = i; l < i + span; l++) {
					if (bound_threshold.data[k * bound_threshold.step[0] + l] == 255) {
						whiteCount++;
					}
				}
			}
			if (whiteCount * 1.0 / (span * bound_threshold.rows) > 0.32) {
				posLeft = i;
				break;
			}
		}
		span = bound_threshold.rows * 0.2f;
		//�ұ߽���
		for (int i = bound_threshold.cols - 1; i > span; i -= 3) {
			int whiteCount = 0;
			for (int k = 0; k < bound_threshold.rows; k++) {
				for (int l = i; l > i - span; l--) {
					if (bound_threshold.data[k * bound_threshold.step[0] + l] == 255) {
						whiteCount++;
					}
				}
			}

			if (whiteCount * 1.0 / (span * bound_threshold.rows) > 0.22) {
				posRight = i;
				break;
			}
		}

		if (posLeft < posRight) {
			return true;
		}
		return false;
	}

	//! �ж�һ�����Ƶ���ɫ
	//! ���복��mat����ɫģ��
	//! ����true��fasle
	bool plateColorJudge(const Mat& src, const Color r, const bool adaptive_minsv,
		float& percent) {
		// �ж���ֵ
		const float thresh = 0.45f;

		Mat src_gray;
		colorMatch(src, src_gray, r, adaptive_minsv);

		percent =
			float(countNonZero(src_gray)) / float(src_gray.rows * src_gray.cols);
		// cout << "percent:" << percent << endl;

		if (percent > thresh)
			return true;
		else
			return false;
	}

	// getPlateType
	//�жϳ��Ƶ�����
	Color getPlateType(const Mat& src, const bool adaptive_minsv) {
		float max_percent = 0;
		Color max_color = UNKNOWN;

		float blue_percent = 0;
		float yellow_percent = 0;
		float white_percent = 0;

		if (plateColorJudge(src, BLUE, adaptive_minsv, blue_percent) == true) {
			// cout << "BLUE" << endl;
			return BLUE;
		}
		else if (plateColorJudge(src, YELLOW, adaptive_minsv, yellow_percent) ==
			true) {
			// cout << "YELLOW" << endl;
			return YELLOW;
		}
		else if (plateColorJudge(src, WHITE, adaptive_minsv, white_percent) ==
			true) {
			// cout << "WHITE" << endl;
			return WHITE;
		}
		else {
			// cout << "OTHER" << endl;

			// �������һ�߶���������ֵ����ȡֵ�����
			max_percent = blue_percent > yellow_percent ? blue_percent : yellow_percent;
			max_color = blue_percent > yellow_percent ? BLUE : YELLOW;

			max_color = max_percent > white_percent ? max_color : WHITE;
			return max_color;
		}
	}

	void clearLiuDingOnly(Mat& img) {
		const int x = 7;
		Mat jump = Mat::zeros(1, img.rows, CV_32F);
		for (int i = 0; i < img.rows; i++) {
			int jumpCount = 0;
			int whiteCount = 0;
			for (int j = 0; j < img.cols - 1; j++) {
				if (img.at<char>(i, j) != img.at<char>(i, j + 1)) jumpCount++;

				if (img.at<uchar>(i, j) == 255) {
					whiteCount++;
				}
			}

			jump.at<float>(i) = (float)jumpCount;
		}

		for (int i = 0; i < img.rows; i++) {
			if (jump.at<float>(i) <= x) {
				for (int j = 0; j < img.cols; j++) {
					img.at<char>(i, j) = 0;
				}
			}
		}
	}

	//ȥ�������Ϸ���ť��
	//����ÿ��Ԫ�صĽ�Ծ�������С��X��Ϊ��������������ȫ����0��Ϳ�ڣ�
	// X���Ƽ�ֵΪ���ɸ���ʵ�ʵ���
	bool clearLiuDing(Mat& img) {
		vector<float> fJump;
		int whiteCount = 0;
		const int x = 7;
		Mat jump = Mat::zeros(1, img.rows, CV_32F);
		for (int i = 0; i < img.rows; i++) {
			int jumpCount = 0;

			for (int j = 0; j < img.cols - 1; j++) {
				if (img.at<char>(i, j) != img.at<char>(i, j + 1)) jumpCount++;

				if (img.at<uchar>(i, j) == 255) {
					whiteCount++;
				}
			}

			jump.at<float>(i) = (float)jumpCount;
		}

		int iCount = 0;
		for (int i = 0; i < img.rows; i++) {
			fJump.push_back(jump.at<float>(i));
			if (jump.at<float>(i) >= 16 && jump.at<float>(i) <= 45) {
				//�����ַ�����һ����������
				iCount++;
			}
		}

		////�����Ĳ��ǳ���
		if (iCount * 1.0 / img.rows <= 0.40) {
			//�������������������ҲҪ��һ������ֵ��
			return false;
		}
		//�����㳵�Ƶ�����
		if (whiteCount * 1.0 / (img.rows * img.cols) < 0.15 ||
			whiteCount * 1.0 / (img.rows * img.cols) > 0.50) {
			return false;
		}

		for (int i = 0; i < img.rows; i++) {
			if (jump.at<float>(i) <= x) {
				for (int j = 0; j < img.cols; j++) {
					img.at<char>(i, j) = 0;
				}
			}
		}
		return true;
	}

	void clearLiuDing(Mat mask, int& top, int& bottom) {
		const int x = 7;

		for (int i = 0; i < mask.rows / 2; i++) {
			int whiteCount = 0;
			int jumpCount = 0;
			for (int j = 0; j < mask.cols - 1; j++) {
				if (mask.at<char>(i, j) != mask.at<char>(i, j + 1)) jumpCount++;

				if ((int)mask.at<uchar>(i, j) == 255) {
					whiteCount++;
				}
			}
			if ((jumpCount < x && whiteCount * 1.0 / mask.cols > 0.15) ||
				whiteCount < 4) {
				top = i;
			}
		}
		top -= 1;
		if (top < 0) {
			top = 0;
		}

		// ok,�ҵ����±߽�
		for (int i = mask.rows - 1; i >= mask.rows / 2; i--) {
			int jumpCount = 0;
			int whiteCount = 0;
			for (int j = 0; j < mask.cols - 1; j++) {
				if (mask.at<char>(i, j) != mask.at<char>(i, j + 1)) jumpCount++;
				if (mask.at<uchar>(i, j) == 255) {
					whiteCount++;
				}
			}
			if ((jumpCount < x && whiteCount * 1.0 / mask.cols > 0.15) ||
				whiteCount < 4) {
				bottom = i;
			}
		}
		bottom += 1;
		if (bottom >= mask.rows) {
			bottom = mask.rows - 1;
		}

		if (top >= bottom) {
			top = 0;
			bottom = mask.rows - 1;
		}
	}

	int ThresholdOtsu(Mat mat) {
		int height = mat.rows;
		int width = mat.cols;

		// histogram
		float histogram[256] = { 0 };
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				unsigned char p = (unsigned char)((mat.data[i * mat.step[0] + j]));
				histogram[p]++;
			}
		}
		// normalize histogram
		int size = height * width;
		for (int i = 0; i < 256; i++) {
			histogram[i] = histogram[i] / size;
		}

		// average pixel value
		float avgValue = 0;
		for (int i = 0; i < 256; i++) {
			avgValue += i * histogram[i];
		}

		int thresholdV;
		float maxVariance = 0;
		float w = 0, u = 0;
		for (int i = 0; i < 256; i++) {
			w += histogram[i];
			u += i * histogram[i];

			float t = avgValue * w - u;
			float variance = t * t / (w * (1 - w));
			if (variance > maxVariance) {
				maxVariance = variance;
				thresholdV = i;
			}
		}

		return thresholdV;
	}

	//! ֱ��ͼ����
	Mat histeq(Mat in) {
		Mat out(in.size(), in.type());
		if (in.channels() == 3) {
			Mat hsv;
			vector<Mat> hsvSplit;
			cvtColor(in, hsv, CV_BGR2HSV);
			split(hsv, hsvSplit);
			equalizeHist(hsvSplit[2], hsvSplit[2]);
			merge(hsvSplit, hsv);
			cvtColor(hsv, out, CV_HSV2BGR);
		}
		else if (in.channels() == 1) {
			equalizeHist(in, out);
		}
		return out;
	}

#define HORIZONTAL 1
#define VERTICAL 0

	Mat CutTheRect(Mat& in, Rect& rect) {
		int size = in.cols;  // (rect.width>rect.height)?rect.width:rect.height;
		Mat dstMat(size, size, CV_8UC1);
		dstMat.setTo(Scalar(0, 0, 0));

		int x = (int)floor((float)(size - rect.width) / 2.0f);
		int y = (int)floor((float)(size - rect.height) / 2.0f);

		//��rect�е����� ��ȡ��dstMat���м�
		for (int i = 0; i < rect.height; ++i) {
			//��
			for (int j = 0; j < rect.width; ++j) {
				dstMat.data[dstMat.step[0] * (i + y) + j + x] =
					in.data[in.step[0] * (i + rect.y) + j + rect.x];
			}
		}

		//
		return dstMat;
	}
	Rect GetCenterRect(Mat& in) {
		Rect _rect;

		int top = 0;
		int bottom = in.rows - 1;
		//����
		for (int i = 0; i < in.rows; ++i) {
			bool bFind = false;
			for (int j = 0; j < in.cols; ++j) {
				if (in.data[i * in.step[0] + j] > 20) {
					top = i;
					bFind = true;
					break;
				}
			}
			if (bFind) {
				break;
			}

			//ͳ����һ�л�һ���У�����Ԫ�صĸ���
		}
		for (int i = in.rows - 1; i >= 0; --i) {
			bool bFind = false;
			for (int j = 0; j < in.cols; ++j) {
				if (in.data[i * in.step[0] + j] > 20) {
					bottom = i;
					bFind = true;
					break;
				}
			}
			if (bFind) {
				break;
			}
			//ͳ����һ�л�һ���У�����Ԫ�صĸ���
		}

		//����
		int left = 0;
		int right = in.cols - 1;
		for (int j = 0; j < in.cols; ++j) {
			bool bFind = false;
			for (int i = 0; i < in.rows; ++i) {
				if (in.data[i * in.step[0] + j] > 20) {
					left = j;
					bFind = true;
					break;
				}
			}
			if (bFind) {
				break;
			}
			//ͳ����һ�л�һ���У�����Ԫ�صĸ���
		}
		for (int j = in.cols - 1; j >= 0; --j) {
			bool bFind = false;
			for (int i = 0; i < in.rows; ++i) {
				if (in.data[i * in.step[0] + j] > 20) {
					right = j;
					bFind = true;

					break;
				}
			}
			if (bFind) {
				break;
			}
			//ͳ����һ�л�һ���У�����Ԫ�صĸ���
		}

		_rect.x = left;
		_rect.y = top;
		_rect.width = right - left + 1;
		_rect.height = bottom - top + 1;

		return _rect;
	}

	Mat features(Mat in, int sizeData) {
		//��ȡ�м�����
		Rect _rect = GetCenterRect(in);

		Mat tmpIn = CutTheRect(in, _rect);
		// Mat tmpIn = in.clone();
		// Low data feature
		Mat lowData;
		resize(tmpIn, lowData, Size(sizeData, sizeData));

		// Histogram features
		Mat vhist = ProjectedHistogram(lowData, VERTICAL);
		Mat hhist = ProjectedHistogram(lowData, HORIZONTAL);

		// Last 10 is the number of moments components
		int numCols = vhist.cols + hhist.cols + lowData.cols * lowData.cols;

		Mat out = Mat::zeros(1, numCols, CV_32F);
		// Asign values to
		// feature,ANN����������Ϊˮƽ����ֱֱ��ͼ�͵ͷֱ���ͼ������ɵ�ʸ��
		int j = 0;
		for (int i = 0; i < vhist.cols; i++) {
			out.at<float>(j) = vhist.at<float>(i);
			j++;
		}
		for (int i = 0; i < hhist.cols; i++) {
			out.at<float>(j) = hhist.at<float>(i);
			j++;
		}
		for (int x = 0; x < lowData.cols; x++) {
			for (int y = 0; y < lowData.rows; y++) {
				out.at<float>(j) += (float)lowData.at<unsigned char>(x, y);
				j++;
			}
		}
		// if(DEBUG)
		//	cout << out << "\n===========================================\n";
		return out;
	}

	float countOfBigValue(Mat& mat, int iValue) {
		float iCount = 0.0;
		if (mat.rows > 1) {
			for (int i = 0; i < mat.rows; ++i) {
				if (mat.data[i * mat.step[0]] > iValue) {
					iCount += 1.0;
				}
				///*if(mat.ptr<uchar>(i)[0] > 0)
				//{
				//	iCount+=1.0;
				//}*/
				//
				// if(mat.at<uchar>(Point(0,i)) > 0)
				//{
				//	iCount+=1.0;
				//}
			}
			return iCount;

		}
		else {
			for (int i = 0; i < mat.cols; ++i) {
				if (mat.data[i] > iValue) {
					iCount += 1.0;
				}
			}

			return iCount;
		}
	}
	// ����ȡ��ֱ��ˮƽ����ֱ��ͼ
	Mat ProjectedHistogram(Mat img, int t) {
		int sz = (t) ? img.rows : img.cols;
		Mat mhist = Mat::zeros(1, sz, CV_32F);

		for (int j = 0; j < sz; j++) {
			Mat data = (t) ? img.row(j) : img.col(j);

			mhist.at<float>(j) = countOfBigValue(
				data, 20);  //ͳ����һ�л�һ���У�����Ԫ�صĸ����������浽mhist��
		}

		// Normalize histogram
		double min, max;
		minMaxLoc(mhist, &min, &max);

		if (max > 0)
			mhist.convertTo(mhist, -1, 1.0f / max,
			0);  //��mhistֱ��ͼ�е����ֵ����һ��ֱ��ͼ

		return mhist;
	}

} /* \namespace easypr  */