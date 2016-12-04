#pragma once
#include "Tools/StructFile.h"
#include "MyAsyncFile.h"
#include "MySqliteFile.h"
#include "MyTimeManager.h"
#include "SoundManager.h"
#include "Mesh.h"
#include "ShaderProgramTest.h"


//========================================������ ���������ʼ
class Global;
class MyAsync;
class MySqlite;
class BodyPowerTimeCheck;
class MyJniClass;
class NoLimitBodyPowerTime;
//class DayLoginTimeCheck;
//========================================������ �����������

//========================================��Ϸ�� ���������ʼ
class GameScene;
class GameLayer;
class GameUILayer;
class GameBgLayer;

//========================================��Ϸ�� �����������


//============================================��������


#define mySqliteOpen(_fileName) Global::getInstance()->getMySqlite()->openMySqlite(_fileName)   //��sql�ļ�
#define mySqliteClose() Global::getInstance()->getMySqlite()->closeMySqlite()  //�ر�sql�ļ�
#define mySqliteCreateTable(_tableName , _value)Global::getInstance()->getMySqlite()->createTable(_tableName , _value)
#define mySqliteInsertData(_tableName , _value)Global::getInstance()->getMySqlite()->insetDataToTable(_tableName , _value)
#define mySqliteGetStringData(_tableName,_target,_where, _whereValue) Global::getInstance()->getMySqlite()->searchStringDataFromTable(_tableName,_target,_where, _whereValue)               //����table�е�key��ȡvalue  atoi(  ().c_str()   )
#define mySqliteSetStringData(_tableName,_target ,_value ,_where , _whereValue) Global::getInstance()->getMySqlite()->setStringDataValue(_tableName,_target ,_value ,_where , _whereValue)    //����table�е�key�޸�value
#define mySqliteAddColumn(_tableName ,_colName ,_type)Global::getInstance()->getMySqlite()->addColumn(_tableName ,_colName ,_type)

#define mySqliteGetIntData(_tableName,_target ,_where , _whereValue) Global::getInstance()->getMySqlite()->searchIntDataFromTable(_tableName,_target ,_where , _whereValue)               //����table�е�key��ȡvalue  atoi(  ().c_str()   )
#define mySqliteSetIntData(_tableName,_target ,_value ,_where , _whereValue) Global::getInstance()->getMySqlite()->setIntDataValue(_tableName,_target ,_value ,_where , _whereValue)    //����table�е�key�޸�value
#define mySqliteGetFloatData(_tableName,_target ,_where , _whereValue) Global::getInstance()->getMySqlite()->searchFloatDataFromTable(_tableName,_target ,_where , _whereValue)               //����table�е�key��ȡvalue  atoi(  ().c_str()   )
#define mySqliteSetFloatData(_tableName,_target ,_value ,_where , _whereValue) Global::getInstance()->getMySqlite()->setFloatDataValue(_tableName,_target ,_value ,_where , _whereValue)    //����table�е�key�޸�value

#define mySqliteDeleteData( _tableName,_where,_key) Global::getInstance()->getMySqlite()->deleteData( _tableName,_where,_key)                     //ɾ��table��key�����������


#define BEAR_GBK2312_TO_UTF8(_value)  Global::getInstance()->G2U(_value)   //�ݲ�֧��

#define SET_ACTION_BIND_SCHEDULER(_node , _newSchedule , _newActionManager)    \
	Node* __node = static_cast<Node*>(_node);                                  \
	Global::getInstance()->setActionBind(__node, _newSchedule, _newActionManager)

#define myGameInstance Global::getInstance()     //��ȡȫ�����ָ��궨��

#define CODING_VALUE(_x) myGameInstance->codingVlaue(_x)
#define DISSECT_VALUE(_x) myGameInstance->dissectVlaue(_x)



static Global* s_globalInstace = nullptr;
class Global
{
	
public:
	Global();
	~Global();

	//LU_GameScene *gameLayer;

	static Global* getInstance();


	Node* backNodeForCSB(const string _name, Node* _parent);  //��csb�õ��Ľڵ��л�ȡ�ӽڵ�
	void pauseOrResumeNode(Node* _node, bool _pause);   //��ͣ��ָ��ڵ�
	void setActionBind(Node*& _node, Scheduler*& _newSchedule, ActionManager*& _newActionManager);    //����ĳ���ڵ�İ�һ��schedule�����������˶�


	void setLayerID(int _id){ m_nLayerID = _id; };
	int getLayerID(){ return m_nLayerID; };
	void setPassIndex(int _passIndex){ m_nPassIndex = _passIndex; };
	int getPassIndex(){ return m_nPassIndex; };

	bool getIsMusicOpen(){ return m_bIsMusicOpen; };
	void setMusic(bool _isOpen){ m_bIsMusicOpen = _isOpen; };
	bool getIsSoubdEffectOpen(){ return m_bIsSoundEffectOpne; };
	void setSoundEffect(bool _isOpen){ m_bIsSoundEffectOpne = _isOpen; };

	string G2U(const std::string& src);   //�ݲ�֧��
	void createSpriteAnimation(Sprite*& _sp, string _name, int _count, int _loopCount = -1, float _time = 1.f / 15.f);
	void createActionForFrameSprite(ActionInterval* &_act, string _name, int _count, int _loopCount = -1);
	void spriteCleanUpBlackBottom(Sprite*& _sprite);
	void parabolaCalculateParameter(Vec2 _posA, Vec2 _posB, Vec2 _posC, float& _NumA, float& _NumB, float& _NumC);
	float parabolaCalculateY(float _a, float _b, float _c, float _x);
	bool getIsFirstLoadSound(){ return m_bIsFirstLoadSound; };
	void setFirstLoadSoundOver(){ m_bIsFirstLoadSound = false; };
	int dealSoundEffect(int _index, int _status, bool _isLoop = false, int _stopID = 0);    //������Ч����  0 ��ʾ���� 1��ʾ���� 2 ��ʾֹͣ
	void dealSoundMusic(int _index, int _status, bool _isLoop = false);     //�������ֺ���  0 ��ʾ���� 1��ʾ���� 2 ��ʾֹͣ
	MyAsync* getMyAsync(){ return m_pMyAsync; };   //��ȡ�첽���ص���ָ��
	MySqilteObjec* getMySqlite(){ return m_pMySqlite; }; //��ȡsqliteָ��
	BodyPowerTimeCheck* getBodyPowerTime(){ return m_pBodyPowerTime; };
	MyJniClass* getMyJniClass(){ return m_pMyJniClass; };
	void initRestartChance(){ m_nLeverRestartChance = m_nPlayerLeveRestartLimit; };
	void decreaseOnceRestartChance(){ m_nLeverRestartChance--; };;
	int getRestartChance(){ return m_nLeverRestartChance; };

	void shakeNode(Node* _node ,Vec2 _beforeStartPos, float _extent , int _count , float _time);  //�ζ��ڵ������

	inline int codingVlaue(int _value){ _value += m_nRandomCodeValue; return  _value; };
	inline int dissectVlaue(int _value){ _value -= m_nRandomCodeValue; return  _value; };

	struct MY_NET_TIME{
		string year;
		string month;
		string day;
		string hour;
		string min;
		string sec;
	}myNetTime;

	int m_nBGMoveSpeed;
	
	bool m_bIsCanTCreateOnceNpcOrObstacle;
	bool m_bIsOneStartOneControl;
	bool m_bIsRestartLevel;
	

	GameScene* m_pGameScene;
	GameUILayer* m_pGameUI;
	GameBgLayer* m_pGameBg;
	GameLayer* m_pGameLayer;
	NoLimitBodyPowerTime* m_pNoLimitBodyPowerTime;
	

	int m_nMoveDistance;
	int m_nNowBodyPowerNum;
	int m_nBodyLimit;
	int m_nCoin;
	int m_nLevelEatStarNum;
	int m_nPlayerLeveRestartLimit;
	int m_nUnlockChapter;
	int m_nStarNum;
	int m_nUnlimitBodyPower;
	int m_nLastUnlcokPass;
	int m_nPlayBGMusicStatus;
	int m_nChangeSceneStatus;

	vector<string>m_vecPassData[SELECTPASS_CHAPTER_NUMBER * 19];

private:
	template<typename T , typename... Args>
	void myPrint(int nCount, T _head, Args..._info);
	void readSomeDataEveryOnce();
private:
	
	MyAsync* m_pMyAsync;
	MySqilteObjec* m_pMySqlite;
	BodyPowerTimeCheck* m_pBodyPowerTime;
	DayLoginTimeCheck* m_pDayLoginCheck;
	MyJniClass* m_pMyJniClass;

protected:
	int m_nLayerID;
	int m_nPassIndex;
	int m_nPlayerBlood;
	int m_nRandomCodeValue;

	bool m_bIsMusicOpen;
	bool m_bIsSoundEffectOpne;

	bool m_bIsFirstLoadSound;

	int m_nLeverRestartChance;

	

};

//-------------------------------------------ʵ�ֹ���Ч������ Ŀǰ����Բ��
class ClippLight : public Node
{
public:
	static ClippLight* create(string _stencil, string _light, float _time, float _angle, bool _isToUp = false);
private:
	bool init(string _stencil, string _light, float _time, float _angle, bool _isToUp);
protected:
	ClippingNode* m_pClippNode;
	Sprite* m_pBottom;
	Sprite* m_pStencil;

};


class MyPageView : public PageView
{
public:
	void myMove(float _offset);
};

//=================================================���ֿ��Զ�����״��
class ClippBlenkCustomShape : public Node
{
public:
	static ClippBlenkCustomShape* create();
	bool getClippOver(){ return m_bIsClippOver; };
	void setClippOver(bool _isOver){ m_bIsClippOver = _isOver; };
	void runBlenkAction(Vec2 _startPos, float _startScale, float _scaleTarget, float _time , function<void()> _callBack = nullptr);
private:
	bool init();
	
protected:
	ClippingNode* m_pClippNode;
	bool m_bIsClippOver;
	DrawNode* m_pStencil;
};


