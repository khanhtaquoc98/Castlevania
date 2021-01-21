//
//#define __SOUND_H__
//
//#include "Game.h"
//#include "dsutil.h"
//#include <unordered_map>
//#include <string>
//
//
//#define SOUND_WHIP 1
//#define SOUND_HIT 2
//#define SOUND_COLLECT_ITEM 3
//#define SOUND_COLLECT_WEAPON 4
//#define SOUND_DAGGER 6
//#define SOUND_MONEY 7
//#define SOUND_HURTING 8
//#define SOUND_BROKEN_BRICK 9
//#define SOUND_HOLYWATER 10
//#define SOUND_SPLASHWATER 11
//#define SOUND_FALLING_DOWN_WATER 12
//#define SOUND_GET_SCORE_TIME	13
//#define SOUND_GET_SCORE_HEART	14
//#define SOUND_AXE	15
//#define SOUND_STOPWATCH	16
//#define SOUND_CROSS	17
//#define SOUND_STOPWATCH_BEGIN 18
//#define SOUND_STOPWATCH_END 19
//#define SOUND_DIE 20
//#define SOUND_BOSS 21
//#define STATE_01 22
//
//
//class Sound
//{
//private:
//	unordered_map<int, GSound*> _ArrSound;
//
//public:
//	static Sound* __Instance;
//	static Sound* GetInstance();
//
//	GSoundManager* dsound;
//
//	void Init();
//	Sound();
//
//	void AddSound(int type, LPTSTR filename);
//	void Play(int type, bool isLoop = false, LONG lVolume = 0L);
//	void Stop(int type);
//	void LoadResourceSound();
//
//	bool isPlaying(int type);
//
//	void StopAll();
//
//};