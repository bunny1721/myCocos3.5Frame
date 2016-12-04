#pragma once
#include "StructFile.h"
class MySoundClass
{
public:
	static MySoundClass* createNewInstance();
	void useAsyLoadSoundEffect(string _path, SOUNDCALLBACL _callBack);
private:
	void preloadSoundEffectProcessFunc();
	void const destroy(){ delete this; };
protected:
	~MySoundClass(){};
	SOUNDCALLBACL m_funcSoundCallback;
	string m_strResPath;
};