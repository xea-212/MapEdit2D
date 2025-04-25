#pragma once
#include "../Library/SceneBase.h"
#include "../Stage.h"

class PlayScene : public SceneBase
{
public:
	PlayScene();
	~PlayScene();
	void Update() override;
	void Draw() override;
};
