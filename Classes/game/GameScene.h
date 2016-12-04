#pragma once

#include "Tools\CommonFile.h"



class GameScene : public Layer ,public b2ContactListener
{
public:
	static GameScene* create();
	static Scene* scene();
	void setCanTouch(bool _isTouch){
		m_bCanToch = _isTouch;
	};
	bool getCanTouch(){ return m_bCanToch; };
	void pauseGameScene();    //��ͣ��Ϸ����
	void resumeGameScene();   //�ָ���ͣ����Ϸ����
	void gameTimeScale(float _scale);     
	~GameScene();
	
private:
	bool init();
	void update(float _dt);
	void onTouchesBegan(const std::vector<Touch*>& _touch, Event* event);
	void onTouchesMoved(const std::vector<Touch*>& _touch, Event* event);
	void onTouchesEnded(const std::vector<Touch*>& _touch, Event* event);



protected:
	EventListenerTouchAllAtOnce* m_pListener;

	EventListenerTouchOneByOne* m_pListenerOneByOne;

	bool m_bCanToch;   //���ƴ���

	GameLayer* m_pGameLayer;      //��Ϸ��
	GameUILayer* m_pGameUILayer;  //UI��
	GameBgLayer* m_pGameBgLayer;  //��Ϸ����

};


