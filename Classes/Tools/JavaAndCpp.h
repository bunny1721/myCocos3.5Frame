#include "StructFile.h"

#define JAVA_PATH_NAME "org/cocos2dx/cpp/JavaToCocos"

typedef void (Ref::*MYJAVACALLBACK)();
#define myJavaCallBackFunc(_func_)MYJAVACALLBACK(&_func_)
class MyJniClass
{
public:
	void runJavaCallBackFunc();
	void buyStageSenderToJava(int _nPayId, MYJAVACALLBACK _func, void* _sender);
	void senderExitGame();
private:
	void setJavaCallBackFunc(MYJAVACALLBACK _func, void* _sender);
protected:
	MYJAVACALLBACK m_pCallBackFunc;
	void* m_pSender;
};