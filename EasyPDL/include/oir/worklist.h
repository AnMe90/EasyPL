#ifndef __WORKLIST_H__
#define __WORKLIST_H__

#include "arealocate.h"
#include "feature.h"
#include "corefunc.h"
#include"enhance_ssr.h"
#include "plate_judge.h"
#include "svm_train.h"
#include <string.h>
#include "batch.h"
using namespace std;

namespace oir {
	class WorkList{
	public:

		void Wlist(string &file_path, string &file_result, string &file_error, int &index, int& flagerr);
	};




}
#endif