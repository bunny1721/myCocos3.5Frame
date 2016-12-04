#pragma once
#include "Tools\CommonFile.h"

class MainMenu : public Layer
{
public:
	static MainMenu* create();
	static Scene* scene();
private:
	bool init();
	void initData();

	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);

	void onKeyReleased(EventKeyboard::KeyCode _code , Event* event);

protected:
	
};