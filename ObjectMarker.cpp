#include "ObjectMarker.h"
#include "util_functions.h"
#include "util_cv_functions.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;

ObjectMarker::ObjectMarker(){
}


ObjectMarker::~ObjectMarker(void){
}



bool ObjectMarker::saveConfiguration(const std::string& config_name,
	const std::string& input_dir, const std::string& outputname,
	const MarkerViewer& marker_viewer)
{
	cv::FileStorage fs(config_name, cv::FileStorage::WRITE);

	if (!fs.isOpened())
		return false;

	fs << "image_folder" << input_dir;
	fs << "output_file" << outputname;
	marker_viewer.Write(fs, "Viewer");

	return true;
}


bool ObjectMarker::loadConfiguration(const std::string& config_name,
	std::string& input_dir, std::string& outputname,
	MarkerViewer& marker_viewer)
{
	cv::FileStorage fs(config_name, cv::FileStorage::READ);
	if (!fs.isOpened())
		return false;

	fs["image_folder"] >> input_dir;
	fs["output_file"] >> outputname;
	marker_viewer.Read(fs["Viewer"]);
	return true;
}


void ObjectMarker::printHelp() {
	printf("\nObject Marker: \n\t 画像中のオブジェクト位置を記録するためのツール\n");
	printf("\tGunawan Herman, April 2006\n");
	printf("\tAdapted from ObjectMarker.cpp by A.Florian\n");
	printf("\tModified and localized by Takuya MINAGAWA in 2014\n");
	printf("\n");
	printf("------------------------------------------------------------\n");
	printf("|  btn  |               function                           |\n");
	printf("|-------|--------------------------------------------------|\n");
	printf("|Enter  | 現在のマーカーをファイルに記述し、               |\n");
	printf("|       | 次の画像を読み込む                               |\n");
	printf("|Space  | Enterと同じ                                      |\n");
	printf("|BS     | ひとつ前の画像に戻る                             |\n");
	printf("|ESC    | プログラムを終了                                 |\n");
	printf("|d      | 一番新しいマーカーを消去                         |\n");
	printf("|r      | 前フレームのマーカーを１つ呼び出す               |\n");
	printf("|8      | 一番新しいマーカーを1 px上へ動かす               |\n");
	printf("|9      | 一番新しいマーカーを10px上へ動かす               |\n");
	printf("|2      | 一番新しいマーカーを1 px下へ動かす               |\n");
	printf("|3      | 一番新しいマーカーを10px下へ動かす               |\n");
	printf("|4      | 一番新しいマーカーを1 px左へ動かす               |\n");
	printf("|5      | 一番新しいマーカーを10px左へ動かす               |\n");
	printf("|6      | 一番新しいマーカーを1 px右へ動かす               |\n");
	printf("|7      | 一番新しいマーカーを10px右へ動かす               |\n");
	printf("|w      | 一番新しいマーカーの幅を1 px拡大する             |\n");
	printf("|W      | 一番新しいマーカーの幅を1 px縮小する             |\n");
	printf("|h      | 一番新しいマーカーの高さを1 px拡大する           |\n");
	printf("|H      | 一番新しいマーカーの高さを1 px縮小する           |\n");
	printf("|z      | 一番新しいマーカーのを大きさを2%%拡大する         |\n");
	printf("|Z      | 一番新しいマーカーのを大きさを2%%縮小する         |\n");
	printf("|m      | マーカーの縦横比を固定／固定解除する             |\n");
	printf("|a      | マーカーの縦横比を指定する                       |\n");
	printf("|s      | 画像の表示サイズを変更する                       |\n");
	printf("|c      | マーカー領域の画像を切り取ってファイル保存       |\n");
	printf("|p      | マーカーを矩形だけでなく点を許可する             |\n");
	printf("|e      | 現フレームにつけたマーカー座標を画面出力         |\n");
	printf("|g      | ガイドの表示/非表示                              |\n");
	printf("|G      | ガイドの設定                                     |\n");
	printf("|f      | 画像のフォルダ名を変更                           |\n");
	printf("|o      | 出力ファイルを変更                               |\n");
	printf("|O      | 出力ファイルを成形して新たに作成                 |\n");
	printf("|j      | 指定番号の画像へジャンプ                         |\n");
	printf("|t      | このヘルプを表示                                 |\n");
	printf("------------------------------------------------------------\n");
	printf("オブジェクトを右クリックで選択\n");
	printf("\n");
}

void ObjectMarker::printStatus() const
{
	std::cout << "このプログラムは入力画像を'" << _input_dir << "'フォルダの中から探します\n";
	std::cout << "オブジェクトの位置はファイル'" << _annotation_file << "'にテキスト出力されます\n";
	_marker_viewer.PrintStatus();
}


//! アノテーションの並び替え
/*!
ロードされたアノテーションに対応する画像ファイル名が、参照画像ファイル名と対応するように並び替え
\param[in] loaded_img_list 入力画像リスト
\param[in] loaded_annotation 読み込まれたアノテーション
\param[in] ref_img_list 参照アノテーション
\return 整理されたアノテーション
*/
std::vector<std::vector<cv::Rect>> ObjectMarker::reorderAnnotation(
	const std::vector<std::string>& loaded_img_list,
	const std::vector<std::vector<cv::Rect>>& loaded_annotation,
	const std::vector<std::string>& ref_img_list)
{
	assert(loaded_img_list.size() == loaded_annotation.size());

	std::vector<std::vector<cv::Rect>> annotation(ref_img_list.size());

	int num_loaded = loaded_img_list.size();

	for (int i = 0; i < num_loaded; i++){
		std::vector<std::string>::const_iterator find_it
			= std::find(ref_img_list.begin(), ref_img_list.end(), loaded_img_list[i]);

		if (find_it == ref_img_list.end())
			continue;

		int idx = (int)std::distance(ref_img_list.begin(), find_it);
		annotation[idx] = loaded_annotation[i];
	}

	return annotation;
}


bool ObjectMarker::Load(const std::string& image_dir, const std::string& anno_file)
{
	// フォルダから画像一覧を取得
	std::string input_dir = image_dir;
	_file_list.clear();
	while (!util::ReadImageFilesInDirectory(input_dir, _file_list)){
		std::cout << "no appropriate input files in directory " << input_dir << std::endl;
		std::string flag = util::AskQuestionGetString("Quit?(1:Yes, 0:No): ");
		if (flag == "1")
			return false;
		input_dir = util::AskQuestionGetString("New Image Directory Name: ");
	}

	_image_idx = 0;

	_input_dir = input_dir;

	return LoadAnnotationFile(anno_file);
}


bool ObjectMarker::LoadAnnotationFile(const std::string& anno_file)
{
	// アノテーションファイルを読み込み
	std::vector<std::string> anno_file_list;
	std::vector<std::vector<cv::Rect>> anno_rect_list;
	util::LoadAnnotationFile(anno_file, anno_file_list, anno_rect_list);

	// フォルダの画像一覧とアノテーションファイルを紐づけ
	_rectlist = reorderAnnotation(anno_file_list, anno_rect_list, _file_list);
	anno_file_list.clear();
	anno_rect_list.clear();

	_annotation_file = anno_file;

	// ヘッダの書き込み
	return !util::AddHeaderLine(anno_file);
}



bool ObjectMarker::jump(int idx)
{
	if (_marker_viewer.is_changed()){
		_rectlist[_image_idx] = _marker_viewer.GetMarkers();
		util::AddAnnotationLine(_annotation_file, _file_list[_image_idx], _rectlist[_image_idx]);
		_marker_viewer.reset_change();
	}

	if (idx < 0 || idx >= _file_list.size())
		return false;

	std::string load_img_file = _file_list[idx];
	cv::Mat img = cv::imread(load_img_file);
	if (img.empty()){
		std::cerr << "Fail to read file " << load_img_file << "." << std::endl;
		return false;
	}

	// マーカーをセット
	_marker_viewer.SetMarkers(_rectlist[idx]);
	_marker_viewer.reset_change();
	
	std::ostringstream oss;
	oss << idx + 1 << " - " << load_img_file;
	_marker_viewer.Open(img, oss.str());

	_image_idx = idx;

	return true;
}


//! アノテーションファイルを整形して出力
inline bool ObjectMarker::ExportAnnotationFile(const std::string& filename){
	return util::SaveAnnotationFile(filename, _file_list, _rectlist);
};


void ObjectMarker::CopyFormerMarkers()
{
	if (_image_idx > 0){
		if (!_rectlist[_image_idx].empty() || !_rectlist[_image_idx - 1].empty()){
			_marker_viewer.SetMarkers(_rectlist[_image_idx - 1]);
		}
	}
}

//! アノテーションで画像を切り取って保存
inline void ObjectMarker::CropAndSaveImages(const std::string& dir_name){
	util::CropAnnotatedImageRegions(dir_name, _file_list, _rectlist);
}


int ObjectMarker::run(const std::string& conf_file)
{
	///////// パラメータ ///////////
	std::string annotation_file;	// 出力テキストファイル名
	std::string input_dir = "rawdata";	// 入力フォルダ
	///////////////////////////////////

	bool ret = loadConfiguration(conf_file, input_dir, annotation_file, _marker_viewer);

	if (annotation_file.empty())
		annotation_file = "annotation.txt";

	Load(input_dir, annotation_file);

	printHelp();
	printStatus();

	enum KeyBindings {
		Key_Enter = 13, Key_ESC = 27, Key_Space = 32, Key_BS = 8
	};

	bool loop = this->begin();
	while(loop){
		// Get user input
		int iKey = _marker_viewer.GetWindowKey();

		// Press ESC to close this program, any unsaved changes will be discarded
		if (iKey == Key_ESC){
			loop = false;
		}
		// Press Space or Enter to save marked objects on current image and proceed to the next image
		else if (iKey == Key_Space || iKey == Key_Enter){
			loop = this->next();
		}
		else if (iKey == Key_BS){
			if(!this->prev())
				std::cout << "Can't back to the former image." << std::endl;
		}
		// Press d to remove the last added object
		else if (iKey == 'd'){
			_marker_viewer.DeleteMarker();
		}
		else if (iKey == '8'){
			_marker_viewer.ReshapeMarker(cv::Rect(0,-1,0,0));
		}
		else if(iKey == '9'){
			_marker_viewer.ReshapeMarker(cv::Rect(0, -10, 0, 0));
		}
		else if (iKey == '2'){
			_marker_viewer.ReshapeMarker(cv::Rect(0, 1, 0, 0));
		}
		else if(iKey == '3'){
			_marker_viewer.ReshapeMarker(cv::Rect(0, 10, 0, 0));
		}
		else if (iKey == '4'){
			_marker_viewer.ReshapeMarker(cv::Rect(-1, 0, 0, 0));
		}
		else if (iKey == '5'){
			_marker_viewer.ReshapeMarker(cv::Rect(-10, 0, 0, 0));
		}
		else if (iKey == '6'){
			_marker_viewer.ReshapeMarker(cv::Rect(1, 0, 0, 0));
		}
		else if (iKey == '7'){
			_marker_viewer.ReshapeMarker(cv::Rect(10, 0, 0, 0));
		}
		else if (iKey == 'w'){
			_marker_viewer.ReshapeMarker(cv::Rect(0, 0, 1, 0));
		}
		else if (iKey == 'W'){
			_marker_viewer.ReshapeMarker(cv::Rect(0, 0, -1, 0));
		}
		else if (iKey == 'h'){
			_marker_viewer.ReshapeMarker(cv::Rect(0, 0, 0, 1));
		}
		else if (iKey == 'H'){
			_marker_viewer.ReshapeMarker(cv::Rect(0, 0, 0, -1));
		}
		else if (iKey == 'z'){
			_marker_viewer.ResizeMarker(1.02);
		}
		else if (iKey == 'Z'){
			_marker_viewer.ResizeMarker(0.98);
		}
		else if (iKey == 'c'){
			std::cout << "Cropping images with annotated rectangles." << std::endl;
			std::string dir_name = util::AskQuestionGetString("Folder name to save images: ");
			CropAndSaveImages(dir_name);
		}
		else if (iKey == 'm'){
			_marker_viewer.SwitchFixAR();
			printStatus();
		}
		else if (iKey == 'a'){
			double ratio = util::AskQuestionGetDouble("Aspect ratio of object (Width / Height): ");
			if (ratio <= 0){
				std::cerr << "Illegal aspect ratio: " << ratio;
			}
			else{
				_marker_viewer.SetAspectRatio(ratio);
			}
		}
		else if (iKey == 's'){
			double scale = util::AskQuestionGetDouble("Display Image Scale: ");
			_marker_viewer.SetDisplayScale(scale);
			this->reload();
		}
		else if (iKey == 'e'){
			std::vector<cv::Rect> objects = _marker_viewer.GetMarkers();
			for (int i = 0; i < objects.size(); i++) {
				cv::Rect rect = objects[i];
				std::cout << rect.x << " " << rect.y << " " << rect.width << " " << rect.height << std::endl;
			}
		}
		else if (iKey == 'p'){
			std::cout << "点形状の許可： " << (_marker_viewer.SwitchAcceptPointShape() ? "YES" : "NO") << std::endl;
		}
		else if (iKey == 't'){
			printHelp();
			printStatus();
		}
		else if (iKey == 'j'){
			int target_id = util::AskQuestionGetInt("Jump to image#: ");
			if (!jump(target_id - 1)){
				std::cerr << "Fail to jump #" << target_id << std::endl;
			}
		}
		else if (iKey == 'r'){
			this->CopyFormerMarkers();
		}
		else if (iKey == 'g'){
			std::cout << "GUIDE: " << (_marker_viewer.SwitchShowGuide() ? "ON" : "OFF") << std::endl;
		}
		else if (iKey == 'G'){
			int shape_id = util::AskQuestionGetInt("Choose shape (0:NONE, 1:SQUARE, 2:RECTANGLE, 3:CIRCLE, 4:ELLIPSE): ");
			if (shape_id >= 0 && shape_id < 5){
				_marker_viewer.SetGuideShape(shape_id);
				if (shape_id > 0){
					cv::Rect grect;
					grect.x = util::AskQuestionGetInt("input x: ");
					grect.y = util::AskQuestionGetInt("input y: ");
					grect.width = util::AskQuestionGetInt("input width: ");
					grect.height = util::AskQuestionGetInt("input height: ");
					_marker_viewer.SetGuideRectangle(grect);
				}
				_marker_viewer.ShowGuide();
			}
			else{
				std::cout << "Wrong Input" << std::endl;
			}
		}
		// change work folder (= reboot)
		else if (iKey == 'f'){
			input_dir = util::AskQuestionGetString("New Image Directory Name: ");
			Load(input_dir, _annotation_file);
			this->begin();
		}
		else if (iKey == 'o'){
			std::string outputname = util::AskQuestionGetString("New Output File Name: ");
			LoadAnnotationFile(outputname);
			this->reload();
		}
		else if (iKey == 'O'){
			std::string save_file = util::AskQuestionGetString("Export Annotation File Name: ");
			if (!ExportAnnotationFile(save_file))
				std::cerr << "Fail to export annotation to file " << save_file << "." << std::endl;
		}
		else{
			std::cout << "Unrecognised command" << std::endl;
		}
	};

	saveConfiguration(conf_file, _input_dir, _annotation_file, _marker_viewer);

	return 0;
}

