#include "SoundManager.h"

MySoundClass* MySoundClass::createNewInstance()
{
	MySoundClass* _msc = new MySoundClass();
	if (!_msc)
	{
		CC_SAFE_DELETE(_msc);
	}
	return _msc;
}
void MySoundClass::useAsyLoadSoundEffect(string _path, SOUNDCALLBACL _callBack)
{
	m_strResPath = _path;
	m_funcSoundCallback = _callBack;
	thread _startSoundAsync(&MySoundClass::preloadSoundEffectProcessFunc, this);
	_startSoundAsync.detach();
}

void MySoundClass::preloadSoundEffectProcessFunc()
{
	SimpleAudioEngine::getInstance()->preloadEffect(m_strResPath.c_str());
	m_funcSoundCallback();
	destroy();
}
