#include <pthread.h>

#ifdef __ANDROID__
#include <unistd.h>
#endif // __ANDROID__

#include "../Include/CSoundManager.h"

namespace LM
{

	void* WaitSoundFinished(void* a_pSoundManager)
	{
		CSoundManager* pSoundManager = static_cast<CSoundManager*>(a_pSoundManager);
		std::string sSoundURL = pSoundManager->m_sPlayingSoundURL;
		while (CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
		{
#if defined _WIN32 | defined _WIN64
			Sleep(100);
#else
			usleep(100000);
#endif
		}
		if (pSoundManager->m_sPlayingSoundURL != "")
		{
			pSoundManager->EndSound(sSoundURL);
			pSoundManager->m_sPlayingSoundURL = "";
		}
		return NULL;
	}

	CSoundManager::CSoundManager(CKernel* a_pKernel) : m_pKernel(a_pKernel), m_sPlayingSoundURL("")
	{

	}

	void CSoundManager::PlaySound(const std::string& a_rSoundURL)
	{
		m_sPlayingSoundURL = a_rSoundURL;
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(a_rSoundURL.c_str(), false);
		pthread_t thread;
		pthread_create(&thread, NULL, &WaitSoundFinished, this);
	}


	void CSoundManager::PauseSound()
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	}

	void CSoundManager::PreloadSound(const std::string& a_rSoundURL)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(a_rSoundURL.c_str());
	}

	void CSoundManager::EndSound(const std::string& a_rSoundURL)
	{
		CCLOG("sound ended : %s", a_rSoundURL.c_str());
	}

} // namespace LM