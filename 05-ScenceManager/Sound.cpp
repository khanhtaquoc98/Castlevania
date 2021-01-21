//#include "Sound.h"
//#include "Utils.cpp"
//
//Sound* Sound::__Instance = NULL;
//
//Sound::Sound()
//{
//	Init();
//	LoadResourceSound();
//}
//
//
//
//void Sound::LoadResourceSound()
//{
//	AddSound(SOUND_WHIP, L"Resources/sound/usingwhip.wav");
//	AddSound(SOUND_HOLYWATER, L"Resources/sound/holywater.wav");
//	AddSound(SOUND_HIT, L"Resources/sound/hit.wav");
//	AddSound(SOUND_COLLECT_ITEM, L"Resources/sound/collectitem.wav");
//	AddSound(SOUND_COLLECT_WEAPON, L"Resources/sound/collectweapon.wav");
//	AddSound(SOUND_CROSS, L"Resources/sound/holycross.wav");
//
//	AddSound(SOUND_DAGGER, L"Resources/sound/usingdagger.wav");
//	AddSound(SOUND_MONEY, L"Resources/sound/display_monney.wav");
//	AddSound(SOUND_AXE, L"Resources/sound/Axe.wav");
//	AddSound(SOUND_STOPWATCH, L"Resources/sound/StopWatch.wav");
//	AddSound(SOUND_STOPWATCH_BEGIN, L"Resources/sound/InvisibilityPotion_Begin.wav");
//	AddSound(SOUND_STOPWATCH_END, L"Resources/sound/InvisibilityPotion_End.wav");
//
//
//	AddSound(SOUND_HURTING, L"Resources/sound/hurting.wav");
//	AddSound(SOUND_BROKEN_BRICK, L"Resources/sound/broken_brick.wav");
//	AddSound(SOUND_SPLASHWATER, L"Resources/sound/splashwater.wav");
//	AddSound(SOUND_FALLING_DOWN_WATER, L"Resources/sound/fallingdownwatersurface.wav");
//	AddSound(SOUND_GET_SCORE_TIME, L"Resources/sound/getscoretime.wav");
//	AddSound(SOUND_GET_SCORE_HEART, L"Resources/sound/getscoreheart-[AudioTrimmer.com].wav");
//
//
//
//	AddSound(STATE_01, L"Resources/music/Stage_01_Vampire_Killer.wav");
//	AddSound(SOUND_DIE, L"Resources/music/Life_Lost.wav");
//
//	AddSound(SOUND_BOSS, L"Resources/music/Boss_Battle_Poison_Mind.wav");
//
//}
//
//bool Sound::isPlaying(int type)
//{
//	return _ArrSound[type]->IsSoundPlaying();
//}
//
//void Sound::StopAll()
//{
//	for (auto& x : _ArrSound)
//	{
//		if (x.second->IsSoundPlaying())
//			x.second->Stop();
//	}
//}
//
//
//void Sound::AddSound(int type, LPTSTR filename)
//{
//	GSound* wave;
//	HRESULT result = dsound->Create(&wave, filename);
//	if (result != DS_OK)
//	{
//		DebugOut(L"[SOUND] Load Sound that bai. eSound = %d \n", (int)type);
//		return;
//	}
//	_ArrSound[type] = wave;
//}
//
//void Sound::Play(int type, bool isLoop, LONG lVolume)
//{
//	_ArrSound[type]->Play(0, isLoop, lVolume);
//	//DSBPLAY_LOOPING             0x00000001 -> int = 1
//
//}
//
//void Sound::Stop(int type)
//{
//	_ArrSound[type]->Stop();
//}
//
//
//
//void Sound::Init()
//{
//	dsound = new GSoundManager();
//	dsound->Initialize(CGame::GetInstance()->gethWnd(), DSSCL_PRIORITY);
//	dsound->SetPrimaryBufferFormat(2, 22050, 16);
//}
//
//Sound* Sound::GetInstance()
//{
//	if (__Instance == NULL)
//		__Instance = new Sound();
//	return __Instance;
//}