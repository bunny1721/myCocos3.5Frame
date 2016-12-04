#pragma once
#include "Tools\CommonFile.h"

#define SELECTPASS_ROTATE_RADIUS 2265.f
#define SELECTPASS_ROTATE_SPACING_ANGLE 27.4f
#define SELECTPASS_FIRST_BTN_ANGLE -13.5f
#define SELECTPASS_BTN_SPACING_ANGLE 4.3
#define SELECTPASS_CHAPTER_SCROLL_SPEED 2.f

typedef function<void(int)>SCREENSHOTFUNC;
typedef function<void()>CLOSESCREENSHOTFUNC;
class ChapterButtonNode;
class SelectPassRotate : public Layer
{
public:
	
	static SelectPassRotate* create();
	static Scene* scene();
	~SelectPassRotate();
	void openReplaceBlenk();
private:
	bool init();
	void initDataOrRes();
	void createCloudEffect();
	void createOtherRes();
	void update(float _dt);
	bool onTouchbegan(Touch* touch , Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);
	void touchSchedule(float _dt);
	void dealTouchBeganSetMove(Vec2 _touchPos);
	void dealTouchMoveSetMove(Vec2 _touchPos);
	void dealTouchEndedSetMove(Vec2 _touchPos);
	void scrollMoveSchedule(float _time);
	void mySelectPassRotate(float _slideDistance , bool _passSet = false);
	Vec2 calculateOnCirclePoint(int _index);
	float calculateOnCircleButtonAngle(int _index);
	void checkJumpArticle();
	void articleRunActionFunc(Node* _article , bool _isJump , Vec2 _beginScale);
	void checkChapterIndex();
	void changeBgAndArticle(float _disValuse , bool _isNext , bool _opacity);
	void screenShotFunc(int _layerId);
	void destroyScreenShot();
	void createTopUI();
	void createSelectPlayerLayer();
	void createLotteryLayer();
	void createConfirmPassLayer();
	void createCoinAndBodyPowerPay(int _layerID);
	void buttonCallBack(Ref* _sender, Widget::TouchEventType _type);
	void checkShowChapterStatus();
	void checkStarNumber();
	void dealUnlockChapter(Ref* _sender);
	void cloudMoveFunc(float _rotation , bool _passSet = false);
protected:

	struct JUMPARTICLE
	{
		Node* _article;
		bool _isOnScreen = true;
		float _beginScaleX = 0.f;
		float _beginScaleY = 0.f;
	};

	bool m_bIsTouch;
	bool m_bIsTouchToLeft;

	float m_fTouchTime;
	float m_fScrollLimitTime;
	float m_fLimitMinRotate;
	float m_fLimitMaxRotate;
	float m_fScrollSpeed;
	float m_fScorllStartAngle;
	float m_fTouchTimeSchedlue;
	float m_fLastRotation;
	float m_fNowShowBgOpacity;
	float m_fWillShowBgOpacity;

	Vec2 m_tTouchStartPos;
	Vec2 m_tFirstTouchPos;
	Vec2 m_tCircleHeart;

	Sprite* m_pGameBgNowShow;
	Sprite* m_pGameBgWillShow;
	SpriteBlur* m_pSpriteBlur;
	Sprite* m_pblurSprite;


	float m_arrCloudSpeed[3];

	vector<Node*>m_vecChapterChildNode;
	vector<ChapterButtonNode*>m_vecChapterBtnNodeArray[SELECTPASS_CHAPTER_NUMBER];
	vector<JUMPARTICLE*>m_vecJumpArticle[SELECTPASS_CHAPTER_NUMBER];
	vector<Node*>m_vecChapterBgArticleNode;
	vector<Button*>m_vecUnlockChapter;
	vector<Sprite*>m_vecCloudSprite;
	
	
	int m_arrnUnlockChapterNum[SELECTPASS_CHAPTER_NUMBER];
	

	Node* m_pSelectPassParentNode;
	Node* m_pChapterNode;
	Node* m_pCloudNode;

	int m_nChapterIndex;
	int m_nCreateLayerindex;
	int m_nLastStarNum;



	string m_strWillShowBgFile;
	string m_strShowBgFile;


	TopUI* m_pTopUI;

	Button* m_pLotteryBtn;

	Label* m_pStarNumLabel;


	ClippBlenkCustomShape* m_pClippBlenk;

	LayerColor* m_pBlenkLayer;
};

//============================================================================chapter Button Node
class ChapterButtonNode : public Node
{
public:
	static ChapterButtonNode* create(Vec2 _vec2 , float _rotation , int _levelId);
	void chapterButtonNodeUpdate(Vec2 _circleHeart);
	void setScreenShotFunc(SCREENSHOTFUNC _funcScreenShot){ m_funcScreenShot = _funcScreenShot; };
private:
	bool init(Vec2 _vec2, float _rotation, int _levelId);
	void initDataRes();
	void buttonCallBack(Ref* _sender , Widget::TouchEventType _type);
	void createLabelNum(Label*&_label, string _name, Vec2 _pos, Size _size);
protected:
	Button* m_pButton;
	int m_nLevelId;

	Sprite* m_pStarSp[3];
	Sprite* m_pStarBottomSp[3];
	SpriteGray* m_pLockButton;

	Label* m_pPassNum;

	SCREENSHOTFUNC m_funcScreenShot;
};
//===============================================================================选关二级弹窗
class ConfirmEnterPass : public Layer
{
public:
	static ConfirmEnterPass* create(int _level);
	void setCloseScreenShot(CLOSESCREENSHOTFUNC _closeScreenShot){ m_funcCloseScreenShot = _closeScreenShot; };
private:
	bool init(int _level);
	void initDataRes();

	bool onTouchBegan(Touch* touch, Event* event){ return true; };
	void onTouchMoved(Touch* touch, Event* event){ };
	void onTouchEnded(Touch* touch, Event* event);
	void buttonCallBack(Ref* _sender , Widget::TouchEventType _type);

	void closeSelfLayer();
protected:
	int m_nLevelIndex;
	Button* m_pConfirmStartBtn;

	CLOSESCREENSHOTFUNC m_funcCloseScreenShot;
};