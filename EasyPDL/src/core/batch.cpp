#include "../../include/oir/batch.h"
#include<io.h>
using namespace std;
bool get_filelist_from_dir(string path, vector<string>& files)
{
	long   hFile = 0;
	struct _finddata_t fileinfo;
	files.clear();
	if ((hFile = _findfirst(path.c_str(), &fileinfo)) != -1)
	{
		do
		{
			if (!(fileinfo.attrib &  _A_SUBDIR))
				files.push_back(fileinfo.name);
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
		return true;
	}
	else
		return false;
}
//namespace oir {
//
//	vector<string> Batch::CBatch(string file_path,vector<string> file_list ){
//		//string file_path = "D:/workspace/nocolor/";
//		string search_path = file_path + "*.jpg";
//		//vector<string> file_list;
//		if (!get_filelist_from_dir(search_path, file_list))
//			cout << "open file error!" << endl;
//		else return file_list;
//
//		
//	}
//}

//¼¯ºÏ³ÌÐò
namespace oir {

	vector<string> Batch::CBatch(string &file_path,vector<string> &file_list ){
		//string file_path = "D:/workspace/nocolor/";
		string search_path = file_path + "*.jpg";
		//vector<string> file_list;
		if (!get_filelist_from_dir(search_path, file_list))
			cout << "open file error!" << endl;
		else return file_list;

		
	}
}