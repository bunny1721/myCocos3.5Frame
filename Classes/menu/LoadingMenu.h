#include "Tools\CommonFile.h"

class LoadingLayer : public Layer
{
public:
	static LoadingLayer* create();
	static Scene* scene();
private:
	bool init();
	void initData();
	void imageLoading();
	void imageCallFunc();
	void musicLoading();
	void musicCallFunc();
	void initGlobalData();
	void threadGlobalInit();
	void addResCount();
protected:
	LoadingBar* m_pLoadingBar;

	Node* m_pRunPlayerNode;
	Node* m_pCSBNode;

	int m_nLoadingNumber;
	int m_nPointShowCount;
	int m_nJumpCount;
	
	float m_runPlayerStartX;

	bool m_bIsReplace;
	bool m_bIsJumpPlayer[5];

	thread m_thread;
};