#include "JavaAndCpp.h"
#include "CommonFile.h"
void MyJniClass::runJavaCallBackFunc()
{
	((Ref*)m_pSender->*m_pCallBackFunc)();
}
void MyJniClass::setJavaCallBackFunc(MYJAVACALLBACK _func, void* _sender)
{
	m_pCallBackFunc = _func;
	m_pSender = _sender;
}
void MyJniClass::buyStageSenderToJava(int _nPayId, MYJAVACALLBACK _func, void* _sender)
{
	this->setJavaCallBackFunc(_func, _sender);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, JAVA_PATH_NAME, "payPack", "(I)V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID, _nPayId);
	}
#else
	this->runJavaCallBackFunc();
#endif
	
}
void MyJniClass::senderExitGame()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, JAVA_PATH_NAME, "exitGame", "()V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
	}
#else
	Director::getInstance()->end();
#endif
}



//==================================================java回调方法
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
	void Java_org_cocos2dx_cpp_JavaToCocos_CocosExit(JNIEnv *env, jobject thiz)
	{
		Director::getInstance()->end();
	}

	void Java_org_cocos2dx_cpp_JavaToCocos_GetBuy(JNIEnv *env, jobject thiz, jint _id)
	{
		Global::getInstance()->getMyJniClass()->runJavaCallBackFunc();
	}
	void Java_org_cocos2dx_cpp_JavaToCocos_GetBuyNoneParameter(JNIEnv *env, jobject thiz)
	{
		Global::getInstance()->getMyJniClass()->runJavaCallBackFunc();
	}
}

#endif