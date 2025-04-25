#pragma once
#include "Library/GameObject.h"

class Stage : public GameObject
{
public:
	Stage();
	~Stage();
	void Update() override;
	void Draw() override;
private:
	int hImage;
	VECTOR position;
	VECTOR imageSize;
};

//playsceneでstageクラスのインスタンスを作って
//playsceneに、bg.pngが表示される
