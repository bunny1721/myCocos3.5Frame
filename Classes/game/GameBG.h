#pragma once
#include "Tools\CommonFile.h"
class GameBgLayer : public Layer
{
public:
	static GameBgLayer* create();
	void updateGameBG();   //游戏背景的更新
	void touchBegan(Touch* pTouch, Event* event, int _toucId);
	void touchMoved(Touch* pTouch, Event* event, int _toucId);
	void TouchEnded(Touch* pTouch, Event* event, int _toucId);
	
	

private:
	bool init();
	void initDataOrRes();  //初始化数据或图片资源
protected:

	bool m_bIsPauseBg;
};

