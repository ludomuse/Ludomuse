#include <pthread.h>

#include "../Include/CSoundManager.h"

namespace LM
{

	void* WaitSoundFinished(void* a_pSoundManager)
	{
		while (true)
		{
			if (!CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
			{
				CSoundManager* pSoundManager = static_cast<CSoundManager*>(a_pSoundManager);
				if (pSoundManager->m_sPlayingSoundURL != "")
				{
					pSoundManager->EndSound(pSoundManager->m_sPlayingSoundURL);
					pSoundManager->m_sPlayingSoundURL = "";
				}

			}
			//Sleep(1000);
		}
		return NULL;
	}

	CSoundManager::CSoundManager(CKernel* a_pKernel) : m_pKernel(a_pKernel), m_sPlayingSoundURL("")
	{
		//pthread_t thread;
		//pthread_create(&thread, NULL, &WaitSoundFinished, this);
	}

	void CSoundManager::PlaySound(const std::string& a_rSoundURL)
	{
		m_sPlayingSoundURL = a_rSoundURL;
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(a_rSoundURL.c_str(), false);
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

	}

} // namespace LM