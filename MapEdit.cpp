#define NOMINMAX

#include <Windows.h>
#include "MapEdit.h"
#include <cassert>
#include "Source/Input.h"
#include "DxLib.h"
#include "MapChip.h"
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
using namespace std;



MapEdit::MapEdit()
	:GameObject(), myMap_(efg_.MAP_WIDTH* efg_.MAP_HEIGHT, -1), //初期値を-1で20*20の配列を初期化する
	efg_(GetMapEditConfig()), ScrollOffset_({ 0,0 }),
	isInMapEditArea_(false) //マップエディタ領域内にいるかどうか
{
	mapEditRect_ = { efg_.LEFT_MARGIN, efg_.TOP_MARGIN,
		efg_.MAPEDIT_VIEW_X * efg_.MAP_IMAGE_SIZE, efg_.MAPEDIT_VIEW_Y * efg_.MAP_IMAGE_SIZE };
}

MapEdit::~MapEdit()
{
}

void MapEdit::SetMap(Point p, int value)
{
	//マップの座標pにvalueをセットする
	//pが、配列の範囲外の時はassertにひっかかる
	assert(p.x >= 0 && p.x < efg_.MAP_WIDTH);
	assert(p.y >= 0 && p.y < efg_.MAP_HEIGHT);
	myMap_[p.y * efg_.MAP_WIDTH + p.x] = value; //y行x列にvalueをセットする

}

int MapEdit::GetMap(Point p) const
{
	//マップの座標pの値を取得する
	//pが、配列の範囲外の時はassertにひっかかる
	assert(p.x >= 0 && p.x < efg_.MAP_WIDTH);
	assert(p.y >= 0 && p.y < efg_.MAP_HEIGHT);
	return myMap_[p.y * efg_.MAP_WIDTH + p.x]; //y行x列の値を取得する

}

void MapEdit::Update()
{
#if false
	Point mousePos;
	if (GetMousePoint(&mousePos.x, &mousePos.y) == -1) {
		return;
	}
	// マウスの座標がマップエディタ領域内にいるかどうかを判定する
	isInMapEditArea_ = mousePos.x >= mapEditRect_.x && mousePos.x <= mapEditRect_.x + mapEditRect_.w &&
		mousePos.y >= mapEditRect_.y && mousePos.y <= mapEditRect_.y + mapEditRect_.h;

	//左上　mapEditRect_.x, mapEditRect_.y
	//右上　mapEditRect_.x + mapEditRect_.w, mapEditRect_.y
	//右下  mapEditRect_.x + mapEditRect_.w, mapEditRect_.y + mapEditRect_.h
	//左下  mapEditRect_.x, mapEditRect_.y + mapEditRect_.h
		// グリッド座標に変換
	if (!isInMapEditArea_) {
		return; //マップエディタ領域外なら何もしない
	}
	else
	{
		if (Input::IsKeyDown(KEY_INPUT_LEFT))
			ScrollOffset_.x = std::max(0, ScrollOffset_.x - 1);
		if (Input::IsKeyDown(KEY_INPUT_RIGHT))
			ScrollOffset_.x = std::min(std::max(0, efg_.MAP_WIDTH - efg_.MAPEDIT_VIEW_X), ScrollOffset_.x + 1);
		if (Input::IsKeyDown(KEY_INPUT_UP))
			ScrollOffset_.y = std::max(0, ScrollOffset_.y - 1);
		if (Input::IsKeyDown(KEY_INPUT_DOWN))
			ScrollOffset_.y = std::min(std::max(0, efg_.MAP_WIDTH - efg_.MAPEDIT_VIEW_Y), ScrollOffset_.y + 1);
	}

	int gridX = (mousePos.x - efg_.LEFT_MARGIN) / efg_.MAP_IMAGE_SIZE;
	int gridY = (mousePos.y - efg_.TOP_MARGIN) / efg_.MAP_IMAGE_SIZE;

	drawAreaRect_ = { efg_.LEFT_MARGIN + gridX * efg_.MAP_IMAGE_SIZE, efg_.TOP_MARGIN + gridY * efg_.MAP_IMAGE_SIZE,
		efg_.MAP_IMAGE_SIZE, efg_.MAP_IMAGE_SIZE };

	//マウスのボタンが押されたら、持ってる画像をその座標に貼る
	if (Input::IsButtonKeep(MOUSE_INPUT_LEFT)) //左クリックでマップに値をセット
	{
		MapChip* mapChip = FindGameObject<MapChip>();

		if (mapChip && mapChip->IsHold()) //マップチップを持っているなら
		{
			if (CheckHitKey(KEY_INPUT_LSHIFT))
			{
				SetMap({ gridX, gridY }, -1);
			}
			else
			{
				SetMap({ gridX, gridY }, mapChip->GetHoldImage()); //マップに値をセット
			}
		}
	}
	if (Input::IsKeyDown(KEY_INPUT_S))
	{
		SaveMapData();
	}
	if (Input::IsKeyDown(KEY_INPUT_L))
	{
		LoadMapData();
	}
	if (Input::IsKeyDown(KEY_INPUT_D))
	{
		DrawMapData();
	}
#else
	// ... isInMapEditArea_ の判定と、矢印キーによるScrollOffset_の更新処理の後 ...
	Point mousePos;
	if (GetMousePoint(&mousePos.x, &mousePos.y) == -1) {
		return;
	}
	// マウスの座標がマップエディタ領域内にいるかどうかを判定する
	isInMapEditArea_ = mousePos.x >= mapEditRect_.x && mousePos.x <= mapEditRect_.x + mapEditRect_.w &&
		mousePos.y >= mapEditRect_.y && mousePos.y <= mapEditRect_.y + mapEditRect_.h;

	//左上　mapEditRect_.x, mapEditRect_.y
	//右上　mapEditRect_.x + mapEditRect_.w, mapEditRect_.y
	//右下  mapEditRect_.x + mapEditRect_.w, mapEditRect_.y + mapEditRect_.h
	//左下  mapEditRect_.x, mapEditRect_.y + mapEditRect_.h
		// グリッド座標に変換
	if (!isInMapEditArea_) {
		return; //マップエディタ領域外なら何もしない
	}
	else
	{
		if (Input::IsKeyDown(KEY_INPUT_LEFT))
			ScrollOffset_.x = std::max(0, ScrollOffset_.x - 1);
		if (Input::IsKeyDown(KEY_INPUT_RIGHT))
			ScrollOffset_.x = std::min(std::max(0, efg_.MAP_WIDTH - efg_.MAPEDIT_VIEW_X), ScrollOffset_.x + 1);
		if (Input::IsKeyDown(KEY_INPUT_UP))
			ScrollOffset_.y = std::max(0, ScrollOffset_.y - 1);
		if (Input::IsKeyDown(KEY_INPUT_DOWN))
			ScrollOffset_.y = std::min(std::max(0, efg_.MAP_WIDTH - efg_.MAPEDIT_VIEW_Y), ScrollOffset_.y + 1);
	}
	// マウスのスクリーン座標から「見た目上」のグリッド座標を計算
	int viewGridX = (mousePos.x - efg_.LEFT_MARGIN) / efg_.MAP_IMAGE_SIZE;
	int viewGridY = (mousePos.y - efg_.TOP_MARGIN) / efg_.MAP_IMAGE_SIZE;

	// カーソル下の矩形を描画するための座標は「見た目上」の座標でOK
	drawAreaRect_ = { efg_.LEFT_MARGIN + viewGridX * efg_.MAP_IMAGE_SIZE, efg_.TOP_MARGIN + viewGridY * efg_.MAP_IMAGE_SIZE,
		efg_.MAP_IMAGE_SIZE, efg_.MAP_IMAGE_SIZE };

	// スクロール量を加味して、マップ全体の「絶対」グリッド座標を計算
	int mapGridX = viewGridX + ScrollOffset_.x;
	int mapGridY = viewGridY + ScrollOffset_.y;

	//マウスのボタンが押されたら、持ってる画像をその座標に貼る
	if (Input::IsButtonKeep(MOUSE_INPUT_LEFT)) //左クリックでマップに値をセット
	{
		MapChip* mapChip = FindGameObject<MapChip>();

		if (mapChip && mapChip->IsHold()) //マップチップを持っているなら
		{
			if (CheckHitKey(KEY_INPUT_LSHIFT))
			{
				// SetMapには絶対座標を渡す
				SetMap({ mapGridX, mapGridY }, -1);
			}
			else
			{
				// SetMapには絶対座標を渡す
				SetMap({ mapGridX, mapGridY }, mapChip->GetHoldImage()); //マップに値をセット
			}
		}
	}
	// ... 以下、S, L, Dキーの処理はそのまま ...
	if (Input::IsKeyDown(KEY_INPUT_S))
	{
		SaveMapData();
	}
	if (Input::IsKeyDown(KEY_INPUT_L))
	{
		LoadMapData();
	}
	if (Input::IsKeyDown(KEY_INPUT_D))
	{
		DrawMapData();
	}
#endif
}

void MapEdit::Draw()
{//背景を描画する
#if false
	for (int j = 0;j < efg_.MAP_HEIGHT;j++)
	{
		for (int i = 0; i < efg_.MAP_WIDTH; i++)
		{
			int value = GetMap({ i,j });
			if (value != -1) //-1なら何も描画しない
			{
				DrawGraph(efg_.LEFT_MARGIN + i * efg_.MAP_IMAGE_SIZE, efg_.TOP_MARGIN + j * efg_.MAP_IMAGE_SIZE,
					value, TRUE);
			}
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(efg_.LEFT_MARGIN + 0, efg_.TOP_MARGIN + 0,
		efg_.LEFT_MARGIN + efg_.MAPEDIT_VIEW_X * efg_.MAP_IMAGE_SIZE, efg_.TOP_MARGIN + efg_.MAPEDIT_VIEW_Y * efg_.MAP_IMAGE_SIZE, GetColor(255, 255, 0), FALSE, 5);
	for (int j = 0; j < efg_.MAPEDIT_VIEW_Y; j++) {
		for (int i = 0; i < efg_.MAPEDIT_VIEW_X; i++) {
			DrawLine(efg_.LEFT_MARGIN + i * efg_.MAP_IMAGE_SIZE, efg_.TOP_MARGIN + j * efg_.MAP_IMAGE_SIZE,
				efg_.LEFT_MARGIN + (i + 1) * efg_.MAP_IMAGE_SIZE, efg_.TOP_MARGIN + j * efg_.MAP_IMAGE_SIZE, GetColor(255, 255, 255), 1);
			DrawLine(efg_.LEFT_MARGIN + i * efg_.MAP_IMAGE_SIZE, efg_.TOP_MARGIN + j * efg_.MAP_IMAGE_SIZE,
				efg_.LEFT_MARGIN + i * efg_.MAP_IMAGE_SIZE, efg_.TOP_MARGIN + (j + 1) * efg_.MAP_IMAGE_SIZE, GetColor(255, 255, 255), 1);
		}
	}
	if (isInMapEditArea_) {

		DrawBox(drawAreaRect_.x, drawAreaRect_.y,
			drawAreaRect_.x + drawAreaRect_.w, drawAreaRect_.y + drawAreaRect_.h,
			GetColor(255, 255, 0), TRUE);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
#else
	
	 // ビューポートの範囲内だけを描画する
	for (int j = 0; j < efg_.MAPEDIT_VIEW_Y; j++)
	{
		for (int i = 0; i < efg_.MAPEDIT_VIEW_X; i++)
		{
			// スクロール量を加味した、マップ本来の座標を計算
			int mapX = i + ScrollOffset_.x;
			int mapY = j + ScrollOffset_.y;

			// マップの範囲外は描画しない
			if (mapX >= efg_.MAP_WIDTH || mapY >= efg_.MAP_HEIGHT) continue;

			// 描画するマップチップの値を取得
			int value = GetMap({ mapX, mapY });

			if (value != -1)
			{
				// 描画位置はビューポート内の座標(i, j)を基準にする
				DrawGraph(efg_.LEFT_MARGIN + i * efg_.MAP_IMAGE_SIZE,
					efg_.TOP_MARGIN + j * efg_.MAP_IMAGE_SIZE,
					value, TRUE);
			}
		}
	}

	// --- 以下は元のコードのまま ---
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(efg_.LEFT_MARGIN + 0, efg_.TOP_MARGIN + 0,
		efg_.LEFT_MARGIN + efg_.MAPEDIT_VIEW_X * efg_.MAP_IMAGE_SIZE, efg_.TOP_MARGIN + efg_.MAPEDIT_VIEW_Y * efg_.MAP_IMAGE_SIZE, GetColor(255, 255, 0), FALSE, 5);
	for (int j = 0; j < efg_.MAPEDIT_VIEW_Y; j++) {
		for (int i = 0; i < efg_.MAPEDIT_VIEW_X; i++) {
			DrawLine(efg_.LEFT_MARGIN + i * efg_.MAP_IMAGE_SIZE, efg_.TOP_MARGIN + j * efg_.MAP_IMAGE_SIZE,
				efg_.LEFT_MARGIN + (i + 1) * efg_.MAP_IMAGE_SIZE, efg_.TOP_MARGIN + j * efg_.MAP_IMAGE_SIZE, GetColor(255, 255, 255), 1);
			DrawLine(efg_.LEFT_MARGIN + i * efg_.MAP_IMAGE_SIZE, efg_.TOP_MARGIN + j * efg_.MAP_IMAGE_SIZE,
				efg_.LEFT_MARGIN + i * efg_.MAP_IMAGE_SIZE, efg_.TOP_MARGIN + (j + 1) * efg_.MAP_IMAGE_SIZE, GetColor(255, 255, 255), 1);
		}
	}
	if (isInMapEditArea_) {
		DrawBox(drawAreaRect_.x, drawAreaRect_.y,
			drawAreaRect_.x + drawAreaRect_.w, drawAreaRect_.y + drawAreaRect_.h,
			GetColor(255, 255, 0), TRUE);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
#endif


}

void MapEdit::SaveMapData()
{
	//頑張ってファイル選択ダイアログをだす回
	TCHAR filename[255] = "";
	OPENFILENAME ofn = {};

	ofn.lStructSize = sizeof(ofn);
	//ウィンドウのオーナー＝親ウィンドウのハンドル
	ofn.hwndOwner = GetMainWindowHandle();
	ofn.lpstrFile = filename;
	ofn.nMaxFile = 255;
	ofn.Flags = OFN_OVERWRITEPROMPT;

	if (GetSaveFileName(&ofn))
	{
		printfDx("ファイルが選択された\n");
		//ファイルを開いてセーブ
		std::ofstream openedFile(filename);
		printfDx("File Saved!!\n");
		std::ofstream file(filename);
		MapChip* m = FindGameObject<MapChip>();

		/*
		file << "#header" << std::endl;
		file << "WIDTH " << efg_.MAP_WIDTH << std::endl;
		file << "HEIGHT " << efg_.MAP_HEIGHT << std::endl;
		*/

		file << "\n#data" << std::endl;
		for (int j = 0;j < efg_.MAP_HEIGHT;j++)
		{
			for (int i = 0; i < efg_.MAP_WIDTH; i++)
			{
				file << m->GetChipIndex(myMap_[j * efg_.MAP_WIDTH + i]) << ",";
			}
			file << std::endl;
		}
		file.close();
	}
	else
	{
		//ファイルの選択がキャンセル
		printfDx("セーブがキャンセル\n");
	}


	
}

void MapEdit::LoadMapData()
{
	//頑張ってファイル選択ダイアログを出す回
	TCHAR filename[255] = "";
	OPENFILENAME ifn = { 0 };

	ifn.lStructSize = sizeof(ifn);
	//ウィンドウのオーナー＝親ウィンドウのハンドル
	ifn.hwndOwner = GetMainWindowHandle();
	ifn.lpstrFilter = "全てのファイル (*.*)\0*.*\0";
	ifn.lpstrFile = filename;
	ifn.nMaxFile = 255;
	//ifn.Flags = OFN_OVERWRITEPROMPT;

	//GetOpenFileName()

	if (GetOpenFileName(&ifn))
	{
		printfDx("ファイルが選択された→%s\n", filename);
		//ファイルを開いて、セーブ
		//std::filesystem ファイル名だけ取り出す
		//ifstreamを開く input file stream
		std::ifstream inputfile(filename);
		//ファイルがオープンしたかはチェックが必要
		std::string line;


		while (std::getline(inputfile, line)) {
			// 空行はスキップ
			if (line.empty()) continue;
			printfDx("%s\n", line.c_str());
		}
	}
	else
	{
		//ファイルの選択がキャンセル
		printfDx("セーブがキャンセル\n");
	}
}

void MapEdit::DrawMapData()
{
	//頑張ってファイル選択ダイアログを出す回
	TCHAR filename[255] = "";
	OPENFILENAME ifn = { 0 };

	ifn.lStructSize = sizeof(ifn);
	//ウィンドウのオーナー＝親ウィンドウのハンドル
	ifn.hwndOwner = GetMainWindowHandle();
	ifn.lpstrFilter = "全てのファイル (*.*)\0*.*\0";
	ifn.lpstrFile = filename;
	ifn.nMaxFile = 255;
	//ifn.Flags = OFN_OVERWRITEPROMPT;

	//GetOpenFileName()

	if (GetOpenFileName(&ifn))
	{
		printfDx("ファイルが選択された→%s\n", filename);
		//ファイルを開いて、セーブ
		//std::filesystem ファイル名だけ取り出す
		//ifstreamを開く input file stream
		std::ifstream inputfile(filename);
		//ファイルがオープンしたかはチェックが必要
		std::string line;

		MapChip* mc = FindGameObject<MapChip>();
		myMap_.clear();
		while (std::getline(inputfile, line))
		{
			if (line.empty()) continue;
			//printfDx("%s\n", line.c_str());
			if (line[0] != '#')
			{
				istringstream iss(line);
				string tmp;
				while (getline(iss, tmp, ','))
				{
					/*if (tmp == -1)
					{
						myMap_.push_back(-1);
					}
					else
					{
						myMap_.push_back(mc->GetHandle(tmp));
					}*/
					printfDx("%s", tmp.c_str());
					if (tmp == "-1")
					{
						myMap_.push_back(-1);
					}
					else
					{
						int index = stoi(tmp);
						int handle = mc->GetHandle(index);
						myMap_.push_back(handle);
					}
				}
				printfDx("\n");
			}
		}
	}
	else
	{
		//ファイルの選択がキャンセル
		printfDx("セーブがキャンセル\n");
	}
}
