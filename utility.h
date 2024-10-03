#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace Utility
{
	// Open the database from the temp folder
// append the filename to the temp folder
// support Windows and Linux 
	static std::string get_temporary_folder(std::string filename)
	{
		std::string temp_folder;
		if (std::getenv("TEMP"))
		{
			temp_folder = std::getenv("TEMP");
		}
		else if (std::getenv("TMP"))
		{
			temp_folder = std::getenv("TMP");
		}
		else
		{
			// use the current folder
			temp_folder = ".";
		}

		return temp_folder + "/" + filename;
	}

	static void deletedatabase(std::string filename_db)
	{
		if (fs::exists(get_temporary_folder(filename_db)))
		{
			fs::remove(get_temporary_folder(filename_db));
		}
	}
	static void UnescapePostData(std::string& data) {
		size_t pos;
		while ((pos = data.find("+")) != std::string::npos) {
			data.replace(pos, 1, " ");
		}
		while ((pos = data.find("%")) != std::string::npos) {
			if (pos <= data.length() - 3) {
				char replace[2] = { (char)(std::stoi("0x" + data.substr(pos + 1,2), NULL, 16)), '\0' };
				data.replace(pos, 3, replace);
			}
		}
	}
}