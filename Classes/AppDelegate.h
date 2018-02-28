#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"
#include "Engine/Include/CKernel.h"
#ifdef LUDOMUSE_EDITOR
#include "CocosQtPort/CCQApplication.h"
#endif
// #include "Modules/Wifi/Include/LmWifiDirectFacade.h"
// #include "Modules/Wifi/Include/LmJniCppFacade.h"

/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by Director.
*/
#ifdef LUDOMUSE_EDITOR
class  AppDelegate : private cocos2d::CCQApplication
#else
class  AppDelegate : private cocos2d::Application
#endif
{
public:
#ifdef LUDOMUSE_EDITOR
    AppDelegate(int& argc, char *argv[]);
#else
    AppDelegate(bool a_bIsServer = true, const std::string& a_sPath = "");
#endif
    virtual ~AppDelegate();

    virtual void initGLContextAttrs();

    /**
    @brief    Implement Director and Scene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  The function be called when the application enter background
    @param  the pointer of the application
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  The function be called when the application enter foreground
    @param  the pointer of the application
    */
    virtual void applicationWillEnterForeground();

    /**
    @brief return application m_oKernel
    */
    virtual LM::CKernel* getKernel();

    virtual void setPath(const std::string& a_sPath);
    #ifdef LUDOMUSE_EDITOR
    virtual void setParentWidget(QWidget *a_pWidget);
    #endif

private:

    //********************ATTRIBUTES********************

    // TODO : replace with NetworkManager
    // LmWifiDirectFacade m_oWifiFacade;

    //handle the good behavior of the game
    // LmGameManager* m_pLmGameManager;
    LM::CKernel m_oKernel;
    std::string m_sPath;
    #ifdef LUDOMUSE_EDITOR
    QWidget *m_pWidget;
    #endif
    //this object handle log of user & wifi direct & get info to set after gamanager
    // LmMenu* m_pLmMenu;

    //********************METHODS********************

    //init paths so the app find good ressources
    void initPathsForResolution();

    bool init();
};

#endif // _APP_DELEGATE_H_

