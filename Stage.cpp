#include "Stage.h"
#include <assert.h>
#include "Source/Input.h"
#include "Source/Screen.h"

namespace
{
	
}

Stage::Stage()
	: GameObject()
{
	mapChip_ = new MapChip();
	mapEdit_ = new MapEdit();
}

Stage::~Stage()
{
	delete mapEdit_; //�}�b�v�`�b�v�̃C���X�^���X�����
	delete mapChip_; //�}�b�v�`�b�v�̃C���X�^���X�����
}

void Stage::Update()
{
	/*if (Input::IsButtonDown(MOUSE_INPUT_LEFT))
	{
		DxLib::printfDx("�������ꂽ�I�I\n");
	}*/
	/*if (Input::IsButtonDown(MOUSE_INPUT_RIGHT))
	{
		DxLib::printfDx("�E�����ꂽ�I�I\n");
	}
	if (Input::IsButtonKeep(MOUSE_INPUT_LEFT))
	{
		DxLib::printfDx("��������Ă���I�I\n");
	}
	if (Input::IsButtonKeep(MOUSE_INPUT_RIGHT))
	{
		DxLib::printfDx("�E������Ă���I�I\n");
	}
	if (Input::IsButtonUP(MOUSE_INPUT_LEFT))
	{
		DxLib::printfDx("�������ꂽ�I�I\n");
	}
	if (Input::IsButtonUP(MOUSE_INPUT_RIGHT))
	{
		DxLib::printfDx("�E�����ꂽ�I�I\n");
	}*/
	

}

void Stage::Draw()
{
	
}
