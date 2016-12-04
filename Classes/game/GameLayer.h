#pragma once
#include "Tools\CommonFile.h"
class GameLayer : public Layer , public b2ContactListener
{
public:
	static GameLayer* create();
	void updateGameLayer(float _dt);
	void touchBegan(Touch* pTouch, Event* event, int _toucId);
	void touchMoved(Touch* pTouch, Event* event, int _toucId);
	void TouchEnded(Touch* pTouch, Event* event, int _toucId);
	void pauseGameLayer();  //��ͣ��Ϸ����
	void resumeGameLayer(); //�ָ���ͣ����Ϸ����



	~GameLayer();
private:

	bool init();
	void initDataOrRes();   //��ʼ������
	void checkImpact();               //�����ײ
	

	void buttonCallBack(Ref* _sender, Widget::TouchEventType _type);   

	void onFrameEvent(Bone* _bone, const string _event, int originFrameIndex, int currentFrameIndex);

protected:


};



