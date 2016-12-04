#pragma once 

#include "Tools\MacroFile.h"
//---------------------层的优先级
enum MyPriority
{
	Priority_GameBG = 1,
	Priority_GameLayer,


	Priority_Map,
	Priority_Stage,
	Priority_PassDoor,
	Priority_MoveableSwitch,
	Priority_Player,
	Priority_Obstacle,
	

	Priority_EnvironmentParticle,
	Priority_ChangeBGEffect,


	Priority_GameUI,
	Priority_Highest
};
enum MyLayerId{
	LayerID_MainMenu = 0,
	LayerID_SelectPass,
	LayerID_SelectPlayer,
	LayerID_LotteryLayer,
	LayerID_PayCoinLayer,
	LayerID_PayBodyLayer,
	LayerID_SmallLayer,
	LayerID_ConfirmPassLayer,


	LayerID_Default,

};

enum StatusEnum
{
	Status_Normal = 1,
	Status_Die,
	Status_Other,
};
enum Tags
{
	TAG_Player,
	TAG_Enemy,
};

enum PlayerStageProperty
{
	SP_NONE = 0,
	SP_GOLD_KEY,
	SP_WHITE_KEY
};
enum MySceneType
{
	SCENE_SELECTPASS = 0,
	SCENE_GAME,
};

enum BlurModeType
{
	myBlurModeNormal = 0,
	myBlurModeScreenSection,
};
