#pragma once
#include "Library/GameObject.h"
#include <vector> //�ϒ��z��
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
	//std::vector<int> bgHandle_; //�w�i�摜�̃n���h���ւ̃|�C���^�i�z�񎩑́j
	MapChip* mapChip_; //�}�b�v�`�b�v�̃|�C���^
	MapEdit* mapEdit_;

	

	/*VECTOR position;
	VECTOR imageSize;*/
};

//playscene��stage�N���X�̃C���X�^���X�������
//playscene�ɁAbg.png���\�������
