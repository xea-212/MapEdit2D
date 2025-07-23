#include <Windows.h>
#include "MapEdit.h"
#include <cassert>
#include "Source/Input.h"
#include "DxLib.h"
#include "MapChip.h"
#include <fstream>
#include <sstream>
#include <string>
using namespace std;



MapEdit::MapEdit()
	:GameObject(), myMap_(efg_.MAP_WIDTH* efg_.MAP_HEIGHT, -1), //�����l��-1��20*20�̔z�������������
	efg_(GetMapEditConfig()),
	isInMapEditArea_(false) //�}�b�v�G�f�B�^�̈���ɂ��邩�ǂ���
{
	mapEditRect_ = { efg_.LEFT_MARGIN, efg_.TOP_MARGIN,
		efg_.MAP_WIDTH * efg_.MAP_IMAGE_SIZE, efg_.MAP_HEIGHT * efg_.MAP_IMAGE_SIZE };
}

MapEdit::~MapEdit()
{
}

void MapEdit::SetMap(Point p, int value)
{
	//�}�b�v�̍��Wp��value���Z�b�g����
	//p���A�z��͈̔͊O�̎���assert�ɂЂ�������
	assert(p.x >= 0 && p.x < efg_.MAP_WIDTH);
	assert(p.y >= 0 && p.y < efg_.MAP_HEIGHT);
	myMap_[p.y * efg_.MAP_WIDTH + p.x] = value; //y�sx���value���Z�b�g����

}

int MapEdit::GetMap(Point p) const
{
	//�}�b�v�̍��Wp�̒l���擾����
	//p���A�z��͈̔͊O�̎���assert�ɂЂ�������
	assert(p.x >= 0 && p.x < efg_.MAP_WIDTH);
	assert(p.y >= 0 && p.y < efg_.MAP_HEIGHT);
	return myMap_[p.y * efg_.MAP_WIDTH + p.x]; //y�sx��̒l���擾����

}

void MapEdit::Update()
{
	Point mousePos;
	if (GetMousePoint(&mousePos.x, &mousePos.y) == -1) {
		return;
	}
	// �}�E�X�̍��W���}�b�v�G�f�B�^�̈���ɂ��邩�ǂ����𔻒肷��
	isInMapEditArea_ = mousePos.x >= mapEditRect_.x && mousePos.x <= mapEditRect_.x + mapEditRect_.w &&
		mousePos.y >= mapEditRect_.y && mousePos.y <= mapEditRect_.y + mapEditRect_.h;

	//����@mapEditRect_.x, mapEditRect_.y
	//�E��@mapEditRect_.x + mapEditRect_.w, mapEditRect_.y
	//�E��  mapEditRect_.x + mapEditRect_.w, mapEditRect_.y + mapEditRect_.h
	//����  mapEditRect_.x, mapEditRect_.y + mapEditRect_.h
		// �O���b�h���W�ɕϊ�
	if (!isInMapEditArea_) {
		return; //�}�b�v�G�f�B�^�̈�O�Ȃ牽�����Ȃ�
	}

	int gridX = (mousePos.x - efg_.LEFT_MARGIN) / efg_.MAP_IMAGE_SIZE;
	int gridY = (mousePos.y - efg_.TOP_MARGIN) / efg_.MAP_IMAGE_SIZE;

	drawAreaRect_ = { efg_.LEFT_MARGIN + gridX * efg_.MAP_IMAGE_SIZE, efg_.TOP_MARGIN + gridY * efg_.MAP_IMAGE_SIZE,
		efg_.MAP_IMAGE_SIZE, efg_.MAP_IMAGE_SIZE };

	//�}�E�X�̃{�^���������ꂽ��A�����Ă�摜�����̍��W�ɓ\��
	if (Input::IsButtonKeep(MOUSE_INPUT_LEFT)) //���N���b�N�Ń}�b�v�ɒl���Z�b�g
	{
		MapChip* mapChip = FindGameObject<MapChip>();

		if (mapChip && mapChip->IsHold()) //�}�b�v�`�b�v�������Ă���Ȃ�
		{
			if (CheckHitKey(KEY_INPUT_LSHIFT))
			{
				SetMap({ gridX, gridY }, -1);
			}
			else
			{
				SetMap({ gridX, gridY }, mapChip->GetHoldImage()); //�}�b�v�ɒl���Z�b�g
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
}

void MapEdit::Draw()
{//�w�i��`�悷��

	for (int j = 0;j < efg_.MAP_HEIGHT;j++)
	{
		for (int i = 0; i < efg_.MAP_WIDTH; i++)
		{
			int value = GetMap({ i,j });
			if (value != -1) //-1�Ȃ牽���`�悵�Ȃ�
			{
				DrawGraph(efg_.LEFT_MARGIN + i * efg_.MAP_IMAGE_SIZE, efg_.TOP_MARGIN + j * efg_.MAP_IMAGE_SIZE,
					value, TRUE);
			}
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(efg_.LEFT_MARGIN + 0, efg_.TOP_MARGIN + 0,
		efg_.LEFT_MARGIN + efg_.MAP_WIDTH * efg_.MAP_IMAGE_SIZE, efg_.TOP_MARGIN + efg_.MAP_HEIGHT * efg_.MAP_IMAGE_SIZE, GetColor(255, 255, 0), FALSE, 5);
	for (int j = 0; j < efg_.MAP_HEIGHT; j++) {
		for (int i = 0; i < efg_.MAP_WIDTH; i++) {
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



}

void MapEdit::SaveMapData()
{
	//�撣���ăt�@�C���I���_�C�A���O��������
	TCHAR filename[255] = "";
	OPENFILENAME ofn = {};

	ofn.lStructSize = sizeof(ofn);
	//�E�B���h�E�̃I�[�i�[���e�E�B���h�E�̃n���h��
	ofn.hwndOwner = GetMainWindowHandle();
	ofn.lpstrFile = filename;
	ofn.nMaxFile = 255;
	ofn.Flags = OFN_OVERWRITEPROMPT;

	if (GetSaveFileName(&ofn))
	{
		printfDx("�t�@�C�����I�����ꂽ\n");
		//�t�@�C�����J���ăZ�[�u
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
		//�t�@�C���̑I�����L�����Z��
		printfDx("�Z�[�u���L�����Z��\n");
	}


	
}

void MapEdit::LoadMapData()
{
	//�撣���ăt�@�C���I���_�C�A���O���o����
	TCHAR filename[255] = "";
	OPENFILENAME ifn = { 0 };

	ifn.lStructSize = sizeof(ifn);
	//�E�B���h�E�̃I�[�i�[���e�E�B���h�E�̃n���h��
	ifn.hwndOwner = GetMainWindowHandle();
	ifn.lpstrFilter = "�S�Ẵt�@�C�� (*.*)\0*.*\0";
	ifn.lpstrFile = filename;
	ifn.nMaxFile = 255;
	//ifn.Flags = OFN_OVERWRITEPROMPT;

	//GetOpenFileName()

	if (GetOpenFileName(&ifn))
	{
		printfDx("�t�@�C�����I�����ꂽ��%s\n", filename);
		//�t�@�C�����J���āA�Z�[�u
		//std::filesystem �t�@�C�����������o��
		//ifstream���J�� input file stream
		std::ifstream inputfile(filename);
		//�t�@�C�����I�[�v���������̓`�F�b�N���K�v
		std::string line;


		while (std::getline(inputfile, line)) {
			// ��s�̓X�L�b�v
			if (line.empty()) continue;
			printfDx("%s\n", line.c_str());
		}
	}
	else
	{
		//�t�@�C���̑I�����L�����Z��
		printfDx("�Z�[�u���L�����Z��\n");
	}
}

void MapEdit::DrawMapData()
{
	//�撣���ăt�@�C���I���_�C�A���O���o����
	TCHAR filename[255] = "";
	OPENFILENAME ifn = { 0 };

	ifn.lStructSize = sizeof(ifn);
	//�E�B���h�E�̃I�[�i�[���e�E�B���h�E�̃n���h��
	ifn.hwndOwner = GetMainWindowHandle();
	ifn.lpstrFilter = "�S�Ẵt�@�C�� (*.*)\0*.*\0";
	ifn.lpstrFile = filename;
	ifn.nMaxFile = 255;
	//ifn.Flags = OFN_OVERWRITEPROMPT;

	//GetOpenFileName()

	if (GetOpenFileName(&ifn))
	{
		printfDx("�t�@�C�����I�����ꂽ��%s\n", filename);
		//�t�@�C�����J���āA�Z�[�u
		//std::filesystem �t�@�C�����������o��
		//ifstream���J�� input file stream
		std::ifstream inputfile(filename);
		//�t�@�C�����I�[�v���������̓`�F�b�N���K�v
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
		//�t�@�C���̑I�����L�����Z��
		printfDx("�Z�[�u���L�����Z��\n");
	}
}
