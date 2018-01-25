#include "../../../Classes/AppDelegate.h"
#include "cocos2d.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include <sstream>

#define  LOG_TAG    "main"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

using namespace cocos2d;

void cocos_android_app_init(JNIEnv* env) {
	CCLOG("cocos_android_app_init");

        int out = open("/sdcard/LudoMuse/run.log", O_RDWR|O_CREAT, 0600);
        int err = open("/sdcard/LudoMuse/error.log", O_RDWR|O_CREAT, 0600);
        if (out == -1 || err == -1)
        {
          perror("cannot open log files");
          return;
        }
        
        // to still output to real stdout/stderr use these
        // int save_out = dup(fileno(stdout));
        // int save_err = dup(fileno(stderr));
        
        if (dup2(out, fileno(stdout)) == -1)
        {
          perror("cannot redirect stdout");
          return;
        }
        if (dup2(err, fileno(stderr)) == -1)
        {
          perror("cannot redirect stderr");
          return;
        }

        puts("successfully redirected outputs");

        fflush(stdout);
        fflush(stderr);
        
		std::ifstream infile("/sdcard/LudoMuse/LudoMuse.conf");
		std::string line;
		std::string filename = "/sdcard/LudoMuse/";
		if (std::getline(infile, line))
		{
			std::stringstream ss(line);
			filename += ss.str();
                        CCLOG("LUDOMUSE_SCENARIO : %s", filename.c_str());
		}
                else
                {
                  CCLOG("LUDOMUSE_SCENARIO error, cannot get scenario from LudoMuse.conf");
                }
        
		AppDelegate *pAppDelegate = new AppDelegate(false, filename);

        close(out);
        close(err);
}
