#ifndef EASYPR_SVM_TRAIN_H
#define EASYPR_SVM_TRAIN_H

#include <vector>
#include <opencv2/opencv.hpp>

namespace oir {

	class Svm {
	public:
		typedef enum {
			kForward = 1, // correspond to "has plate"
			kInverse = 0  // correspond to "no plate"
		} Label;

		Svm(const char* forward_data_folder, const char* inverse_data_folder);

		void train(bool divide = true, float divide_percentage = 0.7,
			bool train = true, const char* out_svm_folder = NULL);

	private:
		/*
		* divide images into train part and test part by percentage
		*/
		void divide(const char* images_folder, float percentage = 0.7);

		void get_train();

		void get_test();

		const char* forward_;
		const char* inverse_;

		// these two variables are used for cv::CvSVM::train_auto()
		cv::Mat classes_;
		cv::Mat trainingData_;
		// these two variables are used for cv::CvSVM::predict()
		std::vector<cv::Mat> test_imgaes_;
		std::vector<Label> test_labels_;
	};


}


#endif //EASYPR_SVM_TRAIN_H
