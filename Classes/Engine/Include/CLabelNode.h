#ifndef _CLABELNODE_H_
#define _CLABELNODE_H_

#include <string>
#include "cocos2d.h"

#include "CEntityNode.h"

namespace LM
{

class CLabelNode : public CEntityNode
{
 protected:
  std::string m_sText;
  std::string m_sFontName;
  int m_iFontSize;
  std::string m_sTextAlign;

  /// \brief font color : "r,g,b,a"
  std::string m_sFontColor;

  //cocos2d::Label* m_pLabel;

 public:
	 CLabelNode(const std::string& a_rText, 
			 const std::string& a_rFontName, 
		     int a_iFontSize,
		     const std::string& a_sTextAlign,
		     const std::string& a_rFontColor,
			 EAnchor a_eAnchor = EAnchor::FLOAT,
			 int a_iWidth = 0,
			 int a_iHeight = 0,
	         int a_iXPosition = 0, 
	         int a_iYPosition = 0);

  /// \brief init this node
  virtual void Init();

  virtual void SetText(const std::string& a_rText);

  virtual const std::string& GetText() const;
#ifdef LUDOMUSE_EDITOR
    virtual void ToJson(rapidjson::Value& a_rParent, rapidjson::Document::AllocatorType& a_rAllocator);
#endif
    virtual void Copy(CEntityNode* a_pLabel, bool a_bRecCopy = true) override;

    virtual int GetFontSize();

    virtual void SetFontSize(int a_iFontSize);

    virtual const std::string& GetFont() const;

    virtual void SetFont(const std::string& a_rFontName);

    virtual void SetColor(const std::string& a_rFontColor);

    virtual const std::string& GetColor() const;

    virtual cocos2d::Color4B ParseColor(const std::string& a_rFontColor) const;

    virtual bool UseFile(const std::string &a_sFilename) override;
};

} // namespace LM

#endif /* _CLABELNODE_H_ */
