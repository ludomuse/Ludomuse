#include <pthread.h>

#if !defined _WIN32 & !defined _WIN64
#include <unistd.h>
#endif

#include "../Include/CSoundManager.h"

namespace LM
{

	void* WaitSoundFinished(void* a_pSoundEndedObject)
	{
		CCLOG("[SOUND] start waitSoundFinished");
		SSoundEndedObject* pSoundEndedObject = static_cast<SSoundEndedObject*>(a_pSoundEndedObject);
		CCLOG("[SOUND] after cast");
		std::string sThreadSoundURL = pSoundEndedObject->sSoundURL;
		CSoundManager* pSoundManager = pSoundEndedObject->pSoundManager;
		delete pSoundEndedObject;

		CCLOG("[SOUND] after getting string");
		while (CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
		{
#if defined _WIN32 | defined _WIN64
			Sleep(100);
#else
			usleep(100000);
#endif
			CCLOG("[SOUND] in while");
			if (pSoundManager->GetSoundURL() != sThreadSoundURL)
			{
				return NULL;
			}

		}
		CCLOG("[SOUND] before EndSound");
		if (pSoundManager->GetSoundURL() == sThreadSoundURL)
		{
			pSoundManager->EndSound(sThreadSoundURL);
			pSoundManager->SetSoundURL("");
		}
		return NULL;
	}

	CSoundManager::CSoundManager(CKernel* a_pKernel) : m_pKernel(a_pKernel), m_sPlayingSoundURL("")
	{

	}

	void CSoundManager::PlaySound(const std::string& a_rSoundURL)
	{
		m_oPlayingSoundMutex.lock();
		m_sPlayingSoundURL = a_rSoundURL;

		SSoundEndedObject* pSound = new SSoundEndedObject;
		pSound->sSoundURL = m_sPlayingSoundURL;
		pSound->pSoundManager = this;

		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(a_rSoundURL.c_str(), false);
		pthread_t thread;
		CCLOG("[SOUND] before creating thread");
		pthread_create(&thread, NULL, &WaitSoundFinished, pSound);
		CCLOG("[SOUND] after creating thread");

		m_oPlayingSoundMutex.unlock();

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
		CCLOG("[SOUND] sound ended : %s", a_rSoundURL.c_str());
	}

	std::string CSoundManager::GetSoundURL()
	{
		m_oPlayingSoundMutex.lock();
		std::string sSoundURL = m_sPlayingSoundURL;
		m_oPlayingSoundMutex.unlock();
		return sSoundURL;
	}

	void CSoundManager::SetSoundURL(const std::string& a_sNewSound)
	{
		m_oPlayingSoundMutex.lock();
		m_sPlayingSoundURL = a_sNewSound;
		m_oPlayingSoundMutex.unlock();
	}


} // namespace LM
