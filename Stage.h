#pragma once
#include "Library/GameObject.h"
#include <vector> //可変長配列
#include "Mapchip.h"
#include "MapEdit.h"

class Stage : public GameObject
{
public:
	Stage();
	~Stage();
	void Update() override;
	void Draw() override;
private:
	//std::vector<int> bgHandle_; //背景画像のハンドルへのポインタ（配列自体）
	MapChip* mapChip_; //マップチップのポインタ
	MapEdit* mapEdit_;

	

	/*VECTOR position;
	VECTOR imageSize;*/
};

//playsceneでstageクラスのインスタンスを作って
//playsceneに、bg.pngが表示される
