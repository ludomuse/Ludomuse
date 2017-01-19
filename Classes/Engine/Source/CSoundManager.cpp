#include <pthread.h>

#if !(defined _WIN32 | defined _WIN64)
#include <unistd.h>
#endif

#include "../Include/CSoundManager.h"
#include "../Include/CJsonParser.h"
#include "../Include/CMacroManager.h"

namespace LM
{

	void* WaitSoundFinished(void* a_pSoundEndedObject)
	{
		SSoundEndedObject* pSoundEndedObject = static_cast<SSoundEndedObject*>(a_pSoundEndedObject);
		std::string sThreadSoundURL = pSoundEndedObject->sSoundURL;
		CSoundManager* pSoundManager = pSoundEndedObject->pSoundManager;
		delete pSoundEndedObject;

		while (CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
		{
#if defined _WIN32 | defined _WIN64
			Sleep(100);
#else
			usleep(100000);
#endif
			if (pSoundManager->GetSoundURL() != sThreadSoundURL)
			{
				return NULL;
			}

		}
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
        CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		//m_oPlayingSoundMutex.lock();
//        m_sPlayingSoundURL = a_rSoundURL;
        std::string fullSoundPath;
        if (CMacroManager::Instance()->HasDefinition(a_rSoundURL))
        {
            fullSoundPath = CMacroManager::Instance()->GetDefinition(a_rSoundURL);
        }
        else
        {
            fullSoundPath = m_pKernel->GetJsonParser()->GetBasePath() + "/" + a_rSoundURL;
        }
		//SSoundEndedObject* pSound = new SSoundEndedObject;
		//pSound->sSoundURL = m_sPlayingSoundURL;
		//pSound->pSoundManager = this;

//#ifndef __ANDROID__
//		std::string fullSoundPath = m_pKernel->GetJsonParser()->GetBasePath() + "/" + a_rSoundURL;
//		CCLOG("FULLSOUNDPATH : %s", fullSoundPath.c_str());
//#else // !__ANDROID__
//		std::string fullSoundPath = a_rSoundURL;
//#endif
//		std::string fullSoundPath = m_pKernel->GetJsonParser()->GetBasePath() + "/" + a_rSoundURL;

		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(fullSoundPath.c_str(), false);
		//pthread_t thread;
		//pthread_create(&thread, NULL, &WaitSoundFinished, pSound);

		//m_oPlayingSoundMutex.unlock();

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
		m_pKernel->OnSoundEnded(a_rSoundURL);
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
