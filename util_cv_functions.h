#ifndef __UTIL_CV_FUNCTIONS__
#define __UTIL_CV_FUNCTIONS__

#include <opencv2/core/core.hpp>

namespace util{
	
	//! アノテーションファイルの読み込み
	/*!
	opencv_createsamles.exeと同形式のアノテーションファイル読み書き
	\param[in] gt_file アノテーションファイル名
	\param[out] imgpathlist 画像ファイルへのパス
	\param[out] rectlist 各画像につけられたアノテーションのリスト
	\return 読み込みの成否
	*/
	bool LoadAnnotationFile(const std::string& gt_file, std::vector<std::string>& imgpathlist, std::vector<std::vector<cv::Rect>>& rectlist);

	//! アノテーションファイルの保存
	/*!
	opencv_createsamles.exeと同形式のアノテーションファイル読み書き
	\param[in] anno_file アノテーションファイル名
	\param[in] img_files 画像ファイルへのパス
	\param[in] obj_rects 各画像につけられたアノテーションのリスト
	\return 保存の成否
	*/
	bool SaveAnnotationFile(const std::string& anno_file, const std::vector<std::string>& img_files, const std::vector<std::vector<cv::Rect>>& obj_rects, const std::string& sep = " ");

	//! アノテーションファイルへ１行追記
	/*!
	opencv_createsamles.exeと同形式のアノテーションファイル読み書き
	\param[in] anno_file アノテーションファイル名
	\param[in] img_file 画像ファイルへのパス
	\param[in] obj_rects 画像につけられたアノテーション
	\return 保存の成否
	*/
	bool AddAnnotationLine(const std::string& anno_file, const std::string& img_file, const std::vector<cv::Rect>& obj_rects, const std::string& sep = " ");

	//! アノテーションファイルへヘッダを書き込み
	/*!
	\param[in] anno_file アノテーションファイル名
	\return 保存の成否
	*/
	bool AddHeaderLine(const std::string& anno_file);

	//! アノテーションがつけられた領域を切り取って画像として保存
	/*!
	\param[in] dir_path 保存先ディレクトリ
	\param[in] imgpathlist 画像ファイルのリスト
	\param[in] rectlist アノテーションのリスト
	*/
	void CropAnnotatedImageRegions(const std::string& dir_path, const std::vector<std::string>& imgpathlist, const std::vector<std::vector<cv::Rect>>& rectlist);

	//! 矩形をリスケール
	void RescaleRect(const cv::Rect& rect, cv::Rect& dst_rect, double scale);

	//! 矩形をリスケール
	void RescaleRect(const std::vector<cv::Rect>& rects, std::vector<cv::Rect>& dst_rects, double scale);


	//! 矩形が画像領域とかぶっているか確認
	inline bool CheckRectOverlapSize(const cv::Rect& rect, const cv::Size& size){
		return (rect.x < size.width && rect.y < size.height && rect.x + rect.width > 0 && rect.y + rect.height > 0);
	};
}

#endif