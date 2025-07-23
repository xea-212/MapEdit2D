#include "MapEditConfig.h"
#include <windows.h>
#include <cstdlib>   // std::atoi
#include <string>    // std::string
#include <fstream>   // std::ifstream


static int ReadIntFromIni(const std::string& section, const std::string& key, int defaultValue, const std::string& iniPath)
{

	int val = GetPrivateProfileIntA(section.c_str(), key.c_str(), defaultValue, iniPath.c_str());
	//std::cerr << "Read [" << section << "] " << key << " = " << val << " from " << iniPath << std::endl;
	return val;
}

MapEditConfig LoadMapEditConfig(const std::string& iniPath)
{
	MapEditConfig cfg;
	cfg.MAP_WIDTH = ReadIntFromIni("MapEdit", "MAP_WIDTH", 0, iniPath);
	cfg.MAP_HEIGHT = ReadIntFromIni("MapEdit", "MAP_HEIGHT", 0, iniPath);
	cfg.MAP_IMAGE_SIZE = ReadIntFromIni("MapEdit", "MAP_IMAGE_SIZE", 0, iniPath);
	cfg.LEFT_MARGIN = ReadIntFromIni("MapEdit", "LEFT_MARGIN", 0, iniPath);
	cfg.TOP_MARGIN = ReadIntFromIni("MapEdit", "TOP_MARGIN", 0, iniPath);
	cfg.MAPEDIT_VIEW_X = ReadIntFromIni("MapEdit", "MAPEDIT_VIEW_X", 0, iniPath);
	cfg.MAPEDIT_VIEW_Y = ReadIntFromIni("MapEdit", "MAPEDIT_VIEW_Y", 0, iniPath);
	return cfg;
}

const MapEditConfig& GetMapEditConfig()
{
	//std::ifstream ifs("setting.ini");
	//if (ifs.good()) {
	//	MessageBoxA(nullptr, "setting.ini found. Using it for MapEditConfig.", "Info", MB_OK | MB_ICONINFORMATION);
	//}
	static MapEditConfig config = LoadMapEditConfig(".\\mySetup.ini");
	return config;
}