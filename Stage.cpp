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
	delete mapEdit_; //マップチップのインスタンスを解放
	delete mapChip_; //マップチップのインスタンスを解放
}

void Stage::Update()
{
	/*if (Input::IsButtonDown(MOUSE_INPUT_LEFT))
	{
		DxLib::printfDx("左押された！！\n");
	}*/
	/*if (Input::IsButtonDown(MOUSE_INPUT_RIGHT))
	{
		DxLib::printfDx("右押された！！\n");
	}
	if (Input::IsButtonKeep(MOUSE_INPUT_LEFT))
	{
		DxLib::printfDx("左押されている！！\n");
	}
	if (Input::IsButtonKeep(MOUSE_INPUT_RIGHT))
	{
		DxLib::printfDx("右押されている！！\n");
	}
	if (Input::IsButtonUP(MOUSE_INPUT_LEFT))
	{
		DxLib::printfDx("左離された！！\n");
	}
	if (Input::IsButtonUP(MOUSE_INPUT_RIGHT))
	{
		DxLib::printfDx("右離された！！\n");
	}*/
	

}

void Stage::Draw()
{
	
}
