#pragma once 
#include <iostream>
#include <vector>
#include <locale>
#include <iomanip>
#include <type_traits>
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "cocostudio\CocoStudio.h"
#include "ui\CocosGUI.h"
#include "sqlite3.h"
#include "cocos-ext.h"
#include "network\HttpClient.h"
#include <thread>
#include <mutex>
#include <fstream>
#include "Box2D\Box2D.h"
#include "GLES-Render.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <unistd.h>
#include "platform/android/jni/JniHelper.h"
#else
#include <Windows.h>
#endif

using namespace cocos2d::ui;
using namespace std;
using namespace cocos2d;
using namespace CocosDenshion;
using namespace cocostudio;
using namespace cocostudio::timeline;
using namespace cocos2d::extension;
using namespace cocos2d::network;

//=============box2d 世界和像素比例
#define PTM_RATIO 32.f

//-------标准宽高
#define STANDARD_WIDTH  1280
#define STANDARD_HEIGHT 720

//==========瓦片地图的标准长宽
//#define STANDARD_TILEDMAP_WIDTH_COUNT  26
//#define STANDARD_TILEDMAP_HEIGHT_COUNT  15
//
//#define STANDARD_BLOCKS_WIDTH 50.f
//#define STANDARD_BLOCKS_HEIGHT 50.f

#define STANDARD_TILEDMAP_WIDTH_COUNT  22
#define STANDARD_TILEDMAP_HEIGHT_COUNT  12

#define STANDARD_BLOCKS_WIDTH 60.f
#define STANDARD_BLOCKS_HEIGHT 60.f

//------------我的最大值
#define MY_MAX_VALUE  2147483647

#define SQLITE_FILE_NAME "jump.zyl"


#define SQLITE_TABLE_KEYVALUE "keyAndValue"
#define SQLITE_TABLE_PLAYER "playerVirtue"
#define SQLITE_TABLE_PASSDATA "passData"


//=========================选角色的总玩家数量
#define PLAYER_NUMBER 27
//--------------------------------选关的总章节
#define SELECTPASS_CHAPTER_NUMBER 4