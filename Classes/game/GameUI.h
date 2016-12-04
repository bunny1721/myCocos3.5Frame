#pragma once

#include "Tools\CommonFile.h"

class GameUILayer : public Layer
{
public:
	static GameUILayer* create(int _type);
	void updateGameUI();
	void touchBegan(Touch* pTouch, Event* event, int _toucId);
	void touchMoved(Touch* pTouch, Event* event, int _toucId);
	void TouchEnded(Touch* pTouch, Event* event, int _toucId);
	void pauseUILayer();           //暂停UI界面
	void resumeUILayer();          //恢复UI界面
	void onFrameEvent(Bone* _bone, const string _event, int originFrameIndex, int currentFrameIndex);
private:
	bool init(int _type);
	void initSelectPassUI();
	void initButtonAndOther();
	void initPlayerBlood();
	void initProgressTimer();
	void initLabelNumber();
	
	

	
protected:
	void createButton(Button* &_Btn, string _pathName, Vec2 _pos);
	void createBtnColdBar(ProgressTimer* &_bar, string _name, Vec2 _pos);
	void createNormalSprite(Sprite*& _sp, string _name, Vec2 _pos);
	void createLabelNum(Label*&_label, string _name, Vec2 _pos, Size _size);
	void checkNumber();
	void pauseGameAndBG();
	void resumeGameAndBG();
	void buttonCallBack(Ref* _sender, Widget::TouchEventType _type);
private:

	int m_nUIType;


};


