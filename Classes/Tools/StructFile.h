#pragma once 

#include "Tools\EnmuFile.h"

typedef function<void()> SOUNDCALLBACL;

//=========¥•∆¡ ¬º˛ Ù–‘
struct MYTOUCHATTRIBUTE
{
	int _toucId;
	Widget::TouchEventType _toucStatus;
	void* _targetPointer;
};

