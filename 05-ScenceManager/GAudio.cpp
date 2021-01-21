//
//#include "GAudio.h"
//#include "Game.h"
//
//int GAudio::Init_DirectSound()
//{
//    HRESULT result;
//
//    //create DirectSound manager object
//    dsound = new GSoundManager();
//
//    //initialize DirectSound
//    result = dsound->Initialize(CGame::GetInstance()->gethWnd(), DSSCL_PRIORITY);
//    if (result != DS_OK)
//    {
//		
//		return 0;
//	}
//
//    //set the primary buffer format
//    result = dsound->SetPrimaryBufferFormat(2, 22050, 16);
//    if (result != DS_OK)
//    {
//		 return 0;
//	}
//    //return success
//    return 1;
//}
//
//GSound* GAudio::LoadSound(LPTSTR filename)
//{
//    HRESULT result;
//
//    //create local reference to wave data
//    GSound *wave;
//
//    //attempt to load the wave file
//    result = dsound->Create(&wave, filename);
//    if (result != DS_OK)
//    {
//		 return NULL;
//	}
//    //return the wave
//    return wave;
//}
//
//void GAudio::PlaySound(GSound *sound)
//{
//    sound->Play();
//}
//
//void GAudio::LoopSound(GSound *sound)
//{
//    sound->Play(0, DSBPLAY_LOOPING);
//}
//
//void GAudio::StopSound(GSound *sound)
//{
//    sound->Stop();
//}
