#pragma once


namespace Input {
	//キーボード取得関連
	void KeyStateUpdate();
	bool IsKeyUP(int keyCode);
	bool IsKeyDown(int keyCode);
	int IsKeepKeyDown(int keyCode);


	bool IsMouseDown(); //マウスのボタンが押された瞬間
	bool IsMouseUp(); //マウスのボタンが離された瞬間
	bool IsMouseKeep(); //マウスのボタンが押されてる間
	bool IsButtonDown(int button); //押された瞬間
	bool IsButtonUP(int button); //離した瞬間
	bool IsButtonKeep(int button); //押してる間
}

//MOUSE_INPUT_LEFT=>0x0001
//MOUSE_INPUT_MIDDLE=>0x0004
