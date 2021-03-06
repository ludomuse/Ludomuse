LOCAL_PATH := $(call my-dir)
LOCAL_SHORT_COMMANDS := true

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

SOURCE_FILES := $(wildcard ../../Classes/Engine/Source/*.cpp)

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../../Classes/AppDelegate.cpp \
                   ../../../Classes/Engine/Source/CKernel.cpp \
                   ../../../Classes/Engine/Source/CNode.cpp \
                   ../../../Classes/Engine/Source/CSequenceNode.cpp \
                   ../../../Classes/Engine/Source/CInputManager.cpp \
                   ../../../Classes/Engine/Source/CSoundManager.cpp \
                   ../../../Classes/Engine/Source/CParallelNode.cpp \
                   ../../../Classes/Engine/Source/CSceneNode.cpp \
                   ../../../Classes/Engine/Source/CJsonParser.cpp \
                   ../../../Classes/Engine/Source/CMacroManager.cpp \
                   ../../../Classes/Engine/Source/CSelectorNode.cpp \
                   ../../../Classes/Engine/Source/CVisitor.cpp \
                   ../../../Classes/Engine/Source/CEntityNode.cpp \
                   ../../../Classes/Engine/Source/CLabelNode.cpp \
                   ../../../Classes/Engine/Source/CSpriteNode.cpp \
                   ../../../Classes/Engine/Source/CAnimationNode.cpp \
                   ../../../Classes/Engine/Source/CScratchNode.cpp \
                   ../../../Classes/Engine/Source/CVideoNode.cpp \
                   ../../../Classes/Engine/Source/CMenuNode.cpp \
                   ../../../Classes/Engine/Source/CEditBoxNode.cpp \
                   ../../../Classes/Engine/Source/CTeamNode.cpp \
                   ../../../Classes/Engine/Source/CValidator.cpp \
                   ../../../Classes/Engine/Source/CTransitionVisitor.cpp \
                   ../../../Classes/Engine/Source/CTouchBeganVisitor.cpp \
                   ../../../Classes/Engine/Source/CGotoSceneVisitor.cpp \
                   ../../../Classes/Engine/Source/CValidateSceneVisitor.cpp \
                   ../../../Classes/Engine/Source/CFindEntityVisitor.cpp \
                   ../../../Classes/Engine/Source/CFindEntityTouchVisitor.cpp \
                   ../../../Classes/Engine/Source/CFindEntityFromIDVisitor.cpp \
                   ../../../Classes/Engine/Source/CFindTeamNodeIDVisitor.cpp \
                   ../../../Classes/Engine/Source/CFindTeamNodeVisitor.cpp \
                   ../../../Classes/Engine/Source/CDispatchMessageVisitor.cpp \
                   ../../../Classes/Engine/Source/CDispatchEventVisitor.cpp \
                   ../../../Classes/Engine/Source/CGroupNode.cpp \
                   ../../../Classes/Engine/Source/CGridNode.cpp \
                   ../../../Classes/Engine/Source/CPeerNode.cpp \
                   ../../../Classes/Engine/Source/CCameraFeedNode.cpp \
                   ../../../Classes/Modules/Networking/android/Source/CNetworkManager.cpp \
                   ../../../Classes/Modules/Networking/android/Source/LmWifiDirectFacade.cpp \
                   ../../../Classes/Modules/Networking/android/Source/LmJniJavaFacade.cpp \
                   ../../../Classes/Modules/Networking/android/Source/LmJniCppFacade.cpp \
                   ../../../Classes/Modules/Util/Source/Util.cpp \
                   ../../../Classes/Modules/Util/Source/CStats.cpp \




LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
