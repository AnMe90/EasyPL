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
		//file_path ��ԭͼ��Ŀ¼
		//file_list ��ͼ��������б�
		//file_result �ǽ��ͼ�����Ŀ¼
		//file_error �Ǵ���ʧ�ܵ�ͼ����Ŀ¼
		//index���㷨ѡ���ڱ�
		vector<string> CBatch(string &file_path, vector<string> &file_list);
	};




}
#endif