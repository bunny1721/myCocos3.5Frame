#pragma once 

#include "Tools\EnmuFile.h"

typedef function<void()> SOUNDCALLBACL;

//=========�����¼�����
struct MYTOUCHATTRIBUTE
{
	int _toucId;
	Widget::TouchEventType _toucStatus;
	void* _targetPointer;
};

