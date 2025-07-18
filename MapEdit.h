#pragma once
#include "Library/GameObject.h"
#include <vector>
#include "globals.h"
#include "MapEditConfig.h"

namespace
{
}

class MapEdit :
    public GameObject
{
public:
    MapEdit();
    ~MapEdit();

	void SetMap(Point p, int value);
	int GetMap(Point p) const;

    void Update() override;
    void Draw() override;
	void SaveMapData();
	void LoadMapData();
	void DrawMapData();
private:
	MapEditConfig cfg_;
    //std::vector<int> bgHandle_; // マップの配列
	std::vector<int> myMap_;
	Rect mapEditRect_; // マップ領域の矩形
	Rect drawAreaRect_;
	bool isInMapEditArea_;
	Point grid;
};

