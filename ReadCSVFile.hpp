#include <string>
#include <vector>
#include <fstream>

namespace util{

	template <typename T>
	T TransString(const std::string& str){
		T val;
		if (typeid(T) == typeid(int) || typeid(T) == typeid(long) ||
			typeid(T) == typeid(unsigned int) || typeid(T) == typeid(unsigned long)){
			val = atoi(str.c_str());
		}
		else if (typeid(T) == typeid(float) || typeid(T) == typeid(double)){
			val = atof(str.c_str());
		}
		else if (typeid(T) == typeid(std::string)){
			val = str;
		}
		return val;
	};


	// input_stringをseparaterで分離
	template <typename T>
	std::vector<T> TokenizeString(const std::string& input_string, const std::vector<std::string>& separater_vec)
	{
		std::vector<std::string>::const_iterator separater_itr;
		std::vector<std::string::size_type>	index_vec;
		std::string::size_type	index;
		for (separater_itr = separater_vec.begin(); separater_itr != separater_vec.end(); separater_itr++){
			index = 0;
			while (true){
				index = input_string.find(*separater_itr, index);
				if (index == std::string::npos){
					break;
				}
				else{
					index_vec.push_back(index);
					index++;
				}
			}
		}
		sort(index_vec.begin(), index_vec.end());

		std::vector<T> ret_substr_vec;
		std::vector<std::string::size_type>::iterator idx_itr;
		std::string::size_type start_idx = 0;
		int str_size;
		for (idx_itr = index_vec.begin(); idx_itr != index_vec.end(); idx_itr++){
			str_size = *idx_itr - start_idx;
			ret_substr_vec.push_back(TransString<T>(input_string.substr(start_idx, str_size)));
			start_idx = *idx_itr + 1;
		}
		ret_substr_vec.push_back(TransString<T>(input_string.substr(start_idx)));

		return ret_substr_vec;
	}


	// CSVファイルからストリングリストを取得
	template <typename T>
	bool ReadCSVFile(const std::string& input_file, std::vector<std::vector<T>>& output_strings,
		const std::vector<std::string>& separater_vec = std::vector<std::string>())
	{
		std::vector<std::string> sep_vec;
		if (separater_vec.empty()){
			sep_vec.push_back(",");
		}
		else{
			sep_vec = separater_vec;
		}
		std::ifstream ifs(input_file);
		if (!ifs.is_open())
			return false;

		output_strings.clear();

		std::string buf;
		while (ifs && std::getline(ifs, buf)){
			std::vector<T> str_list = TokenizeString<T>(buf, sep_vec);
			output_strings.push_back(str_list);
		}
		return true;
	}


	//! CSVファイルをstd::vectorとして読み込み
	template<typename T> void ReadList(const std::string& filename, std::vector<T>& dst_vector)
	{
		std::ifstream ifs(filename);
		std::string buf;
		while (ifs && std::getline(ifs, buf)){
			dst_vector.push_back(TransString<T>(buf));
		}
	}

}