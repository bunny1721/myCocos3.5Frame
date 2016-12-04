#include "MainMenu.h"
#include "Tools\JavaAndCpp.h"
MainMenu* MainMenu::create()
{
	MainMenu* mm = new MainMenu();
	if (mm && mm->init())
	{
		mm->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(mm);
	}
	return mm;
}
Scene* MainMenu::scene()
{
	Scene* se = Scene::create();
	MainMenu* mm = MainMenu::create();
	se->addChild(mm);
	return se;
}

bool MainMenu::init()
{
	if (!Layer::init())return false;
	auto  _dispatcher = Director::getInstance()->getEventDispatcher();
	auto _listener = EventListenerTouchOneByOne::create();
	_listener->setSwallowTouches(true);
	_listener->onTouchBegan = CC_CALLBACK_2(MainMenu::onTouchBegan, this);
	_listener->onTouchMoved = CC_CALLBACK_2(MainMenu::onTouchMoved, this);
	_listener->onTouchEnded = CC_CALLBACK_2(MainMenu::onTouchEnded, this);
	_listener->setEnabled(true);
	_dispatcher->addEventListenerWithSceneGraphPriority(_listener, this);

	this->setKeyboardEnabled(true);

	this->initData();



	return true;
}
void MainMenu::initData()
{
	
}

bool MainMenu::onTouchBegan(Touch* touch, Event* event)
{ 

	return true;
}
void MainMenu::onTouchMoved(Touch* touch, Event* event)
{

};
void MainMenu::onTouchEnded(Touch* touch, Event* event)
{
	
	
}
void MainMenu::onKeyReleased(EventKeyboard::KeyCode _code, Event* event)
{
	if (_code == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		myGameInstance->getMyJniClass()->senderExitGame();
	}
}