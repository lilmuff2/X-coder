#include "color.hpp"
#include "SupercellFlash.h"
#include "nfd.h"
#include "opencv2/opencv.hpp"
#include "nlohmann/json.hpp"

#include <windows.h>
#include <string>
#include <iostream>
#include <chrono>
#include <filesystem>

namespace fs = std::filesystem;

using namespace std;
using namespace std::chrono;
using json = nlohmann::json;

fs::path ChooseFile() {
	fs::path resultPath;

	NFD_Init();
	nfdnchar_t* path;
	nfdnfilteritem_t filterItem[1] = { { L"Supercell Flash file", L"sc" } };
	nfdresult_t result = NFD_OpenDialogN(&path, filterItem, 1, (nfdnchar_t*)fs::current_path().u32string().c_str());
	if (result == NFD_OKAY)
	{
		resultPath = fs::path(path);
		cout << "File: " << resultPath.string() << endl;
		NFD_FreePathN(path);
	}
	else
	{
		cout << dye::red("Error: %s\n") << NFD_GetError() << endl;
	}

	NFD_Quit();
	return resultPath;
}
fs::path ChooseSaveFile(nfdnchar_t* defname, fs::path defaultpath = "") {
	fs::path resultPath;

	NFD_Init();
	nfdnchar_t* path;
	nfdnfilteritem_t filterItem[1] = { { L"Supercell Flash file", L"sc" } };
	nfdresult_t result = NFD_SaveDialogN(&path, filterItem, 1, (nfdnchar_t*)fs::current_path().u32string().c_str(), defname);
	if (result == NFD_OKAY)
	{
		resultPath = fs::path(path);
		cout << "File Path: " << resultPath.string() << endl;
		NFD_FreePathN(path);
	}
	else if (result == NFD_CANCEL) {
		if (defaultpath != "") {
			cout << "File Path selected automaticaly. File Path: " << defaultpath.string() << endl;
			if (!fs::exists(defaultpath.parent_path())) {
				fs::create_directories(defaultpath.parent_path());
			}
			return defaultpath;
		}
	}
	else
	{
		cout << dye::red("Error: %s\n") << NFD_GetError() << endl;
	}

	NFD_Quit();
	return resultPath;
}
fs::path ChooseFolder(fs::path defaultpath = "") {
	NFD_Init();
	fs::path resultPath;
	nfdnchar_t* folderPath;
	nfdresult_t result = NFD_PickFolderN(&folderPath, (nfdnchar_t*)fs::current_path().u32string().c_str());
	if (result == NFD_OKAY)
	{
		resultPath = fs::path(folderPath);
		cout << "Folder: " << resultPath.string() << endl;
		NFD_FreePathN(folderPath);
	}
	else if (result == NFD_CANCEL) {
		if (defaultpath != "") {
			cout << "Folder selected automaticaly. Folder: " << defaultpath.string() << endl;
			if (!fs::exists(defaultpath)) {
				fs::create_directories(defaultpath);
			}
			return defaultpath;
		}
	}
	else
	{
		cout << dye::red("Error: %s\n") << NFD_GetError() << endl;
	}

	NFD_Quit();
	return resultPath;
}


int GetConsoleWith() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	return columns;

}
void printCenter(string s)
{
	int linelength = GetConsoleWith();
	int spaces = (linelength - s.size()) / 2;

	if (spaces > 0) cout << string(spaces, ' ');
	cout << dye::light_green(s) << '\n';
}
void PrintTime(std::chrono::time_point<std::chrono::high_resolution_clock> start,
	std::chrono::time_point<std::chrono::high_resolution_clock> end
) {
	milliseconds msTime = duration_cast<milliseconds>(end - start);
	if (msTime.count() < 1000) {
		std::cout << msTime.count() << " miliseconds." << std::endl;
	}
	else {
		seconds secTime = duration_cast<seconds>(msTime);
		std::cout << secTime.count() << "." << msTime.count() - 1000 * secTime.count() << " seconds." << std::endl;
	}
}


bool hasEnding(std::string const& fullString, std::string const& ending) {
	if (fullString.length() >= ending.length()) {
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	}
	else {
		return false;
	}
}

void writeconfig(json config) {
	ofstream f("config.json");
	f << config.dump(1);
	f.close();
}
json readconfig() {
	json config;
	if (fs::exists("config.json")) {
		ifstream f("config.json");
		config = json::parse(f);
		f.close();
	}
	else {
		config = {
			{"Skip Folder Select",false},
			{"Skip Save Select",false},
			{"Default Compresion",sc::CompressionSignature::ZSTD}
		};
		writeconfig(config);
	}
	return config;
}
bool invert(bool option) {
	if (option) {
		return false;
	}
	return true;
}


void PrintHelp() {
	system("cls");
	printCenter(R"( ____  __    __   __      ____  _  _    __    __  __    _  _  _  _  ____  ____  __    _
(_  _)/  \  /  \ (  )    (  _ \( \/ )  (  )  (  )(  )  ( \/ )/ )( \(  __)(  __)/  \  / \
  )( (  O )(  O )/ (_/\   ) _ ( )  /   / (_/\ )( / (_/\/ \/ \) \/ ( ) _)  ) _)(_/ /  \_/
 (__) \__/  \__/ \____/  (____/(__/    \____/(__)\____/\_)(_/\____/(__)  (__)  (__)  (_)
)");
	printCenter("V1.1 | github.com/lilmuff2/X-coder");
	cout << endl;
}
void PrintFunctions() {
	cout << dye::blue("\n1. Decode") << endl;
	cout << dye::light_yellow("2. Encode") << endl;
	cout << dye::green("3. Settings") << endl;
	cout << dye::aqua("Your choice: ");
}

void Decode(fs::path filepath, json config) {
	time_point startTime = high_resolution_clock::now();

	cout << dye::light_blue("Loading sc tex file...") << endl;

	sc::SupercellSWF swf;
	swf.textures.resize(255);

	try {
		swf.loadInternal(filepath, true);
	}
	catch (const std::exception& err) {
		std::cout << dye::red("Error. ") << endl << "Message: " << err.what() << endl;
		return;
	}

	auto LoadingEndTime = high_resolution_clock::now();
	cout << dye::green("Loading took: ");
	PrintTime(startTime, LoadingEndTime);

	cout << dye::light_blue("Decoding textures...") << endl;

	
	fs::path folderpath;
	while (folderpath.empty() || !fs::exists(folderpath)) {
		if (config["Skip Folder Select"]) {
			folderpath = filepath.parent_path() / filepath.stem();
			cout << "Folder selected automaticaly. Folder: " << folderpath.string() << endl;
		}
		else {
			cout << dye::light_yellow("Select pngs folder") << endl;
			folderpath = ChooseFolder(filepath.parent_path() / filepath.stem());
		}
	}

	time_point decodingStartTime = high_resolution_clock::now();
	json data = json::array();
	for (uint32_t i = 0; swf.textures.size() > i; i++) {
		sc::pSWFTexture texture = swf.textures[i];
		if (texture == nullptr) break;
		json texturedata = {
			{"PixelFormat",(uint8_t)texture->pixelFormat()},
			{"Filter",(uint8_t)texture->textureFilter()},
			{"Linear",texture->linear()},
			{"Downscaling",texture->downscaling()},
			{"IsKhronos",(bool)texture->textureEncoding()}
		};
		data.push_back(texturedata);
		fs::path imagePath = folderpath / filepath.stem().concat(string("_")+ to_string(i) + ".png");
		texture->textureEncoding(sc::SWFTexture::TextureEncoding::Raw);
		texture->pixelFormat(sc::SWFTexture::PixelFormat::RGBA8);
		texture->linear(true);
		cv::Mat image = cv::Mat(cv::Size(texture->width(), texture->height()), CV_8UC4, texture->textureData.data(), cv::Mat::AUTO_STEP);
		cv::cvtColor(image, image, cv::COLOR_BGRA2RGBA);
		cv::imwrite(imagePath.string(), image);
	}
	fs::path jsonPath = folderpath / "data.json";
	ofstream file(jsonPath);
	file << data.dump(1);
	file.close();

	auto decodingEndTime = high_resolution_clock::now();
	cout << dye::green("Decoding pngs took: ");
	PrintTime(decodingStartTime, decodingEndTime);
}
void Encode(fs::path folderpath, json config) {
	time_point startTime = high_resolution_clock::now();
	cout << dye::light_blue("Reading and encoding pngs...") << endl;
	sc::SupercellSWF swf;
	
	fs::path jsonPath = folderpath / "data.json";
	json TexturesData = json::array();
	if(fs::exists(jsonPath)){
		ifstream f(jsonPath);
		TexturesData = json::parse(f);
	}
	
	int TextureIndex = 0;
	for (auto file : fs::directory_iterator(folderpath)) {
		string path = file.path().string();
		if (hasEnding(path, ".png")) {
			json TextureData = TexturesData[TextureIndex];
			if (TextureData.empty()) {
				TextureData = {
					{"PixelFormat",0},
					{"Filter",0},
					{"Linear",true},
					{"Downscaling",true},
					{"IsKhronos",true}
				};
			}
			cv::Mat image = cv::imread(path,cv::IMREAD_UNCHANGED);
			auto texture = new sc::SWFTexture();
			texture->width(image.cols);
			texture->height(image.rows);	
			cv::cvtColor(image, image, cv::COLOR_RGBA2BGRA);
			if (image.channels() != 4) {
				cout << dye::red("Error. Not correct channel count: ") << image.channels() << endl;
				return;
			}
			texture->textureData = std::vector<uint8_t>(image.cols * image.rows * 4);
			memcpy(texture->textureData.data(), image.data, texture->textureData.size());
			texture->downscaling(TextureData["Downscaling"]);
			texture->textureFilter(TextureData["Filter"]);
			texture->linear(TextureData["Linear"]);
			texture->pixelFormat(TextureData["PixelFormat"]);
			if (TextureData["IsKhronos"]) {
				texture->textureEncoding(sc::SWFTexture::TextureEncoding::KhronosTexture);
			}
			sc::pSWFTexture tex = sc::pSWFTexture(texture);
			swf.textures.push_back(tex);
			TextureIndex++;
		}
	}
	auto decodingEndTime = high_resolution_clock::now();
	cout << dye::green("Encoding pngs took: ");
	PrintTime(startTime, decodingEndTime);
	
	cout << dye::light_blue("Saving...") << endl;
	fs::path filepath;
	if (config["Skip Save Select"]) {
		filepath = folderpath / folderpath.stem().concat(".sc");
		cout << "File Path selected automaticaly. File Path: " << filepath.string() << endl;
	}
	else {
		cout << dye::light_yellow("Select out file path") << endl;
		filepath = ChooseSaveFile(L"any_tex.sc", folderpath / folderpath.stem().concat(".sc"));
	}
	time_point startsavingTime = high_resolution_clock::now();
	swf.stream.init();
	for (sc::pSWFTexture texture : swf.textures) {
		texture->save(&swf, true, false);
	}
	swf.stream.writeTag(0);
	swf.stream.save(filepath, config["Default Compresion"]);
	auto savingEndTime = high_resolution_clock::now();
	cout << dye::green("Saving took: ");
	PrintTime(startsavingTime, savingEndTime);
}
int main();
void Setings(json config) {
	vector <string> Compresions = {"NONE","LZMA","LZHAM","ZSTD"};
	PrintHelp();
	string seting;
	cout << dye::blue("1. Skip Folder Select | ") << config["Skip Folder Select"] << endl;
	cout << dye::light_yellow("2. Skip Save Select | ") << config["Skip Save Select"] << endl;
	cout << dye::light_blue("3. Default Compresion | ") << Compresions[config["Default Compresion"]] << endl;
	cout << dye::green("4. Save and exit") << endl;
	cout << dye::aqua("Select seting you want to change: ");
	cin >> seting;
	if (seting == "1") {
		config["Skip Folder Select"] = invert(config["Skip Folder Select"]);
		Setings(config);
	}
	else if (seting == "2") {
		config["Skip Save Select"] = invert(config["Skip Save Select"]);
		Setings(config);
	}
	else if (seting == "3") {
		int compresion;
		cout << "1. LZMA | Good for big files, like ui" << endl;
		cout << "2. LZHAM | IDK, just dont use it" << endl;
		cout << "3. ZSTD | Best for most of sc, used by default" << endl;
		cout << dye::aqua("Select comresion: ");
		cin >> compresion;
		if (compresion > 0 && compresion <= 3) {
			config["Default Compresion"] = compresion;
		}
		Setings(config);
	}
	else {
		writeconfig(config);
		main();
	}
}


int main()
{
	PrintHelp();
	json config;
	string mode;
	while (true) {
		PrintFunctions();
		cin >> mode;
		config = readconfig();
		if (mode == "1") {
			PrintHelp();
			cout << dye::light_yellow("Select in _tex.sc file") << endl;
			fs::path filepath = ChooseFile();

			if (filepath.empty() || !fs::exists(filepath)) {
				return 1;
			}

			Decode(filepath, config);
		}
		else if (mode == "2") {
			PrintHelp();
			cout << dye::light_yellow("Select folder with pngs") << endl;
			fs::path folderpath = ChooseFolder();
			if (folderpath.empty() || !fs::exists(folderpath)) {
				return 1;
			}

			Encode(folderpath, config);
		}
		else if (mode == "3") {
			Setings(config);
		}
		else {
			cout << dye::red("Icorrect choise.");
		}
	}
	return 0;
}