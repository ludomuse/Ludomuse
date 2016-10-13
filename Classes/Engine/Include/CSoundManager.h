#ifndef _SOUNDMANAGER_H_
#define _SOUNDMANAGER_H_


#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "CKernel.h"

namespace LM
{

class CSoundManager
{
private:
	CKernel* m_pKernel;
	std::string m_sPlayingSoundURL;
	std::mutex m_oPlayingSoundMutex;
  
 public:
  CSoundManager(CKernel* a_pKernel);
  void PlaySound(const std::string& a_rSoundURL);
  void PreloadSound(const std::string& a_rSoundURL);
  void PauseSound();
  void EndSound(const std::string& a_rSoundURL);

  std::string GetSoundURL();
  void SetSoundURL(const std::string& a_sNewSound);
};


struct SSoundEndedObject
{
	std::string sSoundURL;
	CSoundManager* pSoundManager;
};

} // namespace LM

#endif /* _SOUNDMANAGER_H_ */
