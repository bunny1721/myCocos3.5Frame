#pragma once
#include "Tools\CommonFile.h"
class GameBgLayer : public Layer
{
public:
	static GameBgLayer* create();
	void updateGameBG();   //��Ϸ�����ĸ���
	void touchBegan(Touch* pTouch, Event* event, int _toucId);
	void touchMoved(Touch* pTouch, Event* event, int _toucId);
	void TouchEnded(Touch* pTouch, Event* event, int _toucId);
	
	

private:
	bool init();
	void initDataOrRes();  //��ʼ�����ݻ�ͼƬ��Դ
protected:

	bool m_bIsPauseBg;
};

