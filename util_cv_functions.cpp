#include "util_cv_functions.h"
#include "util_functions.h"
#include "ReadCSVFile.hpp"
#include <time.h>
#include <opencv2/highgui/highgui.hpp>

namespace util{

	bool LoadAnnotationFile(const std::string& gt_file, std::vector<std::string>& imgpathlist, std::vector<std::vector<cv::Rect>>& rectlist)
	{
		std::vector<std::vector<std::string>> tokenized_strings;
		std::vector<std::string> sep;
		sep.push_back(" ");
		if (!ReadCSVFile(gt_file, tokenized_strings, sep))
			return false;

		std::vector<std::vector<std::string>>::iterator it, it_end = tokenized_strings.end();
		for (it = tokenized_strings.begin(); it != it_end; it++){
			int num_str = it->size();
			if (num_str < 2)
				continue;

			std::string filename = (*it)[0];
			if (filename.empty() || filename.find("#") != std::string::npos){
				continue;
			}

			imgpathlist.push_back(filename);
			int obj_num = atoi((*it)[1].c_str());
			std::vector<cv::Rect> rects;
			for (int i = 0; i < obj_num && 4 * i + 6 <= num_str; i++){
				int j = 4 * i + 2;
				cv::Rect obj_rect;
				obj_rect.x = atoi((*it)[j].c_str());
				obj_rect.y = atoi((*it)[j + 1].c_str());
				obj_rect.width = atoi((*it)[j + 2].c_str());
				obj_rect.height = atoi((*it)[j + 3].c_str());
				rects.push_back(obj_rect);
			}
			rectlist.push_back(rects);
		}

		return true;
	}



	bool SaveAnnotationFile(const std::string& anno_file, const std::vector<std::string>& img_files, const std::vector<std::vector<cv::Rect>>& obj_rects, const std::string& sep)
	{
		assert(img_files.size() == obj_rects.size());

		std::ofstream ofs(anno_file);
		if (!ofs.is_open())
			return false;

			int num = img_files.size();
		for (int i = 0; i < num; i++){
			ofs << img_files[i] << sep << obj_rects[i].size();
			for (int j = 0; j < obj_rects[i].size(); j++){
				cv::Rect rect = obj_rects[i][j];
				ofs << sep << rect.x << sep << rect.y << sep << rect.width << sep << rect.height;
			}
			ofs << std::endl;
		}

		return true;
	}


	bool AddHeaderLine(const std::string& anno_file)
	{
		// 出力ファイルを開く
		std::ofstream ofs(anno_file, std::ios::app);
		if (!ofs.is_open()){
			return false;
		}
		time_t rawtime;
		struct tm * timeinfo;
		time(&rawtime);
		timeinfo = localtime(&rawtime);

		ofs << std::endl;
		ofs << "########################" << std::endl;
		ofs << "#  " << asctime(timeinfo);
		ofs << "########################" << std::endl;

		return true;
	}


	bool AddAnnotationLine(const std::string& anno_file, const std::string& img_file, const std::vector<cv::Rect>& obj_rects, const std::string& sep)
	{
		// 出力ファイルを開く
		std::ofstream ofs(anno_file, std::ios::app);
		if (!ofs.is_open()){
			return false;
		}

		ofs << img_file << sep << obj_rects.size();
		for (int i = 0; i < obj_rects.size(); i++){
			cv::Rect rect = obj_rects[i];
			ofs << sep << rect.x << sep << rect.y << sep << rect.width << sep << rect.height;
		}
		ofs << std::endl;
		return true;
	}


	//! アノテーションをつけられた画像の領域を切り取って別ファイルとして保存
	void CropAnnotatedImageRegions(const std::string& dir_path, const std::vector<std::string>& imgpathlist, const std::vector<std::vector<cv::Rect>>& rectlist)
	{
		assert(imgpathlist.size() == rectlist.size());

		int count = 0;
		int num_img = imgpathlist.size();
		for (int i = 0; i<num_img; i++){
			cv::Mat img = cv::imread(imgpathlist[i]);
			if (img.empty())
				continue;

			int rect_num = rectlist[i].size();
			for (int j = 0; j<rect_num; j++){
				count++;
				std::stringstream strstr;
				strstr << dir_path << "/" << count << ".png";
				cv::imwrite(strstr.str(), img(rectlist[i][j]));
			}
		}
	}

	void RescaleRect(const cv::Rect& rect, cv::Rect& dst_rect, double scale)
	{
		dst_rect.x = round(scale * rect.x);
		dst_rect.y = round(scale * rect.y);
		dst_rect.width = round(scale * rect.width);
		dst_rect.height = round(scale * rect.height);
	}


	void RescaleRect(const std::vector<cv::Rect>& rects, std::vector<cv::Rect>& dst_rects, double scale)
	{
		int num_rect = rects.size();
		dst_rects.resize(num_rect);
		for (int i = 0; i < num_rect; i++){
			cv::Rect dst;
			RescaleRect(rects[i], dst, scale);
			dst_rects[i] = dst;
		}
	}

}