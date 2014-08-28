#include "util_functions.h"
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <iostream>

namespace util{

	std::string AskQuestionGetString(const std::string& question){
		std::cout << question;
		std::string ans;
		std::cin >> ans;
		return ans;
	}


	int AskQuestionGetInt(const std::string& question){
		std::string ans = AskQuestionGetString(question);
		return atoi(ans.c_str());
	}


	double AskQuestionGetDouble(const std::string& question){
		std::string ans = AskQuestionGetString(question);
		return atof(ans.c_str());
	}


	// ディレクトリから画像ファイル名一覧を取得
	bool ReadImageFilesInDirectory(const std::string& img_dir, std::vector<std::string>& image_lists)
	{
		using namespace boost::filesystem;

		path img_dir_path(img_dir);
		if (!is_directory(img_dir_path)){
			return false;
		}

		directory_iterator end;
		for (directory_iterator p(img_dir_path); p != end; ++p){
			path file_p = p->path();
			std::string ext = file_p.extension().string();
			if (ext == ".jpg" || ext == ".JPG" || ext == ".bmp" || ext == ".BMP" || ext == ".png" || ext == ".PNG"){
				image_lists.push_back(file_p.string());
			}
		}
		return true;
	}
}

