#ifndef __BATCH_H__
#define __BATCH_H__

#include "area.h"
#include "feature.h"
#include "corefunc.h"
#include <string.h>
using namespace std;

namespace oir {
	class Batch{
	public:
		vector<string> CBatch(string file_path, vector<string> file_list);
	};




}
#endif