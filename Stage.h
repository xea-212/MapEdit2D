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

//playscene��stage�N���X�̃C���X�^���X�������
//playscene�ɁAbg.png���\�������
