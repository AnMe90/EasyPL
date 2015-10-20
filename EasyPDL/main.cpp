#include "include/oir/arealocate.h"
#include "include/oir/batch.h"
#include "include/oir/svm_train.h"
#include "include/oir/plate_judge.h"
#include "include/oir/enhance_ssr.h"
#include "include/oir/worklist.h"
using namespace oir;

int main()
{

	CPlateJudge cplatejudge;
	WorkList wlist;
	string file_path = "D:/workspace/nocolor3/";
	string file_result = "D:/workspace/onlycolor/";
	string file_error = "D:/workspace/noplate/";
	int index = 1;
	int flagerr = 0;
	wlist.Wlist(file_path, file_result, file_error, index, flagerr);
	
	
	//waitKey(0);
	system("pause");
	return 0;
}

