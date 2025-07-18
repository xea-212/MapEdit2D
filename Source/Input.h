#pragma once


namespace Input {
	//�L�[�{�[�h�擾�֘A
	void KeyStateUpdate();
	bool IsKeyUP(int keyCode);
	bool IsKeyDown(int keyCode);
	int IsKeepKeyDown(int keyCode);


	bool IsMouseDown(); //�}�E�X�̃{�^���������ꂽ�u��
	bool IsMouseUp(); //�}�E�X�̃{�^���������ꂽ�u��
	bool IsMouseKeep(); //�}�E�X�̃{�^����������Ă��
	bool IsButtonDown(int button); //�����ꂽ�u��
	bool IsButtonUP(int button); //�������u��
	bool IsButtonKeep(int button); //�����Ă��
}

//MOUSE_INPUT_LEFT=>0x0001
//MOUSE_INPUT_MIDDLE=>0x0004
