#ifndef __BATCH_H__
#define __BATCH_H__

#include "arealocate.h"
#include "feature.h"
#include "corefunc.h"
#include"enhance_ssr.h"
#include "plate_judge.h"
#include "svm_train.h"
#include <string.h>
using namespace std;

namespace oir {
	class Batch{
	public:
		//file_path 是原图像目录
		//file_list 是图像的名称列表
		//file_result 是结果图像输出目录
		//file_error 是处理失败的图像存放目录
		//index是算法选用哨兵
		vector<string> CBatch(string &file_path, vector<string> &file_list);
	};




}
#endif