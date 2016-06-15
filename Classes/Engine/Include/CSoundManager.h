#ifndef _SOUNDMANAGER_H_
#define _SOUNDMANAGER_H_


#include "cocos2d.h"
#include "audio/include/SimpleAudioEngine.h"
#include "CKernel.h"

namespace LM
{

class CSoundManager
{
private:
	CKernel* m_pKernel;

public:
	std::string m_sPlayingSoundURL;
  
 public:
  CSoundManager(CKernel* a_pKernel);
  void PlaySound(const std::string& a_rSoundURL);
  void PreloadSound(const std::string& a_rSoundURL);
  void PauseSound();
  void EndSound(const std::string& a_rSoundURL);
};

} // namespace LM

#endif /* _SOUNDMANAGER_H_ */
