/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//
// Copyright (C) 2014 Takuya MINAGAWA, 2006 Gunawan Herman, 2003 Florian Adolf.
// Third party copyrights are property of their respective owners.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is furnished to do
// so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//M*/

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

