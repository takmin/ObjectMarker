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

#ifndef __MARKER_VIEWER__
#define __MARKER_VIEWER__

#include <opencv2/core/core.hpp>

class MarkerViewer
{
public:
	//////////// ガイドの形状 //////////////
	const static int GUIDE_NONE = 0;
	const static int GUIDE_SQUARE = 1;
	const static int GUIDE_RECTANGLE = 2;
	const static int GUIDE_CIRCLE = 3;
	const static int GUIDE_ELLIPSE = 4;
	////////////////////////////////////////

public:
	//!< 画像とウィンドウ名のセット
	MarkerViewer();
	~MarkerViewer();

	//! 画像とウィンドウ名で起動
	void Open(const cv::Mat& image, const std::string& window_name);

	//! 窓を閉じる
	void Close();

	//! 開いているかどうか
	bool is_open(){
		return !_window_name.empty(); 
	}

	bool is_changed(){
		return _change_flag;
	}

	void reset_change(){
		_change_flag = false;
	}

	//! パラメータ読み込み
	void Read(const cv::FileNode& fn);

	//! パラメータ書き込み
	void Write(cv::FileStorage& fs, const std::string& node_name) const;

	//! ウィンドウに入力されたキーを取得
	int GetWindowKey();

	//! マーカーの取得
	const std::vector<cv::Rect> GetMarkers() const;

	//! マーカーの設定
	void SetMarkers(const std::vector<cv::Rect>& objects);

	//! マーカーを消す
	void DeleteMarker();
	
	//! マーカーの位置をずらしたり、大きさの変更を行う
	void ReshapeMarker(const cv::Rect& mv);

	//! マーカーの大きさを変更する
	void ResizeMarker(float scale);

	//! 表示スケールをセットする
	void SetDisplayScale(double scale){
		_display_scale = scale;
	};

	//! アスペクト比固定の設定/解除
	bool SwitchFixAR();

	//! アスペクト比の設定
	void SetAspectRatio(double ratio){
		if (ratio > 0)
			_aspect_ratio = ratio;
	}

	// 点形状の許可/不許可
	bool SwitchAcceptPointShape(){
		_ACCEPT_POINT = !_ACCEPT_POINT;
		return _ACCEPT_POINT;
	}

	// ガイドの表示/非表示切り替え
	bool SwitchShowGuide(){
		_SHOW_GUIDE = !_SHOW_GUIDE;
		RedrawImage();
		return _SHOW_GUIDE;
	}

	// ガイドの表示
	void ShowGuide(){
		_SHOW_GUIDE = true;
		RedrawImage();
	}

	//! ガイド用矩形を設定
	void SetGuideRectangle(const cv::Rect& rect);

	//! ガイド用矩形を設定
	void SetGuideShape(int shape);

	//! マウスのボタンが押された時のアクション
	void MouseButtonDown(int x, int y);

	//! マウスのボタンが押されたまま動かした時のアクション
	void MouseMove(int x, int y);

	//! マウスのボタンを上げた時のアクション
	void MouseButtonUp();

	//! マウスの右ボタンを上げた時のアクション
	void MouseRButtonUp(int x, int y);

	//! ステータスの表示
	void PrintStatus() const;

private:
	bool _change_flag;	//! 描画等の変更があったかどうか

	std::string _window_name;	//!< 描画窓

	cv::Point2d _roi_b, _roi_e;	//!< マウスドラッグの始点と終点

	cv::Mat _image;	//!< 表示画像

	std::vector<cv::Rect> _objects;	//!< 表示画像に対し付与された全マーカー

	/////// パラメータ /////////////
	bool _FIX_MARKER_AR;	//!< マーカーの縦横比を固定するかどうか
	double _aspect_ratio; //!< マーカーのアスペクト比（幅/高さ）
	bool _ACCEPT_POINT;	//!< マーカーの点形状を認めるか否か
	double _display_scale;	// ディスプレイ表示の原画像からの縮尺

	bool _SHOW_GUIDE;	//!< ガイドの表示
	int _GUIDE_SHAPE;	//!< ガイドの形状
	cv::Rect _guide_rect;	//!< ガイドの位置とサイズ
	cv::Rect _guide_rect_org;	//!< ガイドの位置とサイズ(縮尺前)
	///////////////////////////////////

private:
	//! パラメータ初期化
	void Init();

	//! マウス操作要コールバック関数
	static void on_mouse(int event, int x, int y, int flag, void* param);	

	//! 画像枠外の矩形を削除
	static std::vector<cv::Rect> removeOutRangeRect(const std::vector<cv::Rect>& objects, const cv::Size& img_size);

	//! ウィンドウの再描画
	void RedrawImage();

	//! 矩形の１つを選択
	/*!
	\return x,yに最も近いオブジェクトのID
	*/
	int SelectObject(int x, int y);
};

#endif