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

//=============box2d ��������ر���
#define PTM_RATIO 32.f

//-------��׼���
#define STANDARD_WIDTH  1280
#define STANDARD_HEIGHT 720

//==========��Ƭ��ͼ�ı�׼����
//#define STANDARD_TILEDMAP_WIDTH_COUNT  26
//#define STANDARD_TILEDMAP_HEIGHT_COUNT  15
//
//#define STANDARD_BLOCKS_WIDTH 50.f
//#define STANDARD_BLOCKS_HEIGHT 50.f

#define STANDARD_TILEDMAP_WIDTH_COUNT  22
#define STANDARD_TILEDMAP_HEIGHT_COUNT  12

#define STANDARD_BLOCKS_WIDTH 60.f
#define STANDARD_BLOCKS_HEIGHT 60.f

//------------�ҵ����ֵ
#define MY_MAX_VALUE  2147483647

#define SQLITE_FILE_NAME "jump.zyl"


#define SQLITE_TABLE_KEYVALUE "keyAndValue"
#define SQLITE_TABLE_PLAYER "playerVirtue"
#define SQLITE_TABLE_PASSDATA "passData"


//=========================ѡ��ɫ�����������
#define PLAYER_NUMBER 27
//--------------------------------ѡ�ص����½�
#define SELECTPASS_CHAPTER_NUMBER 4