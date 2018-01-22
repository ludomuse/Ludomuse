#! /bin/bash


echo "Before install path : $PWD"
export PREF=$PWD/../
cd $PREF
wget --quiet https://dl.google.com/android/repository/android-ndk-r13b-linux-x86_64.zip
unzip -q android-ndk-r13b-linux-x86_64.zip
export NDK_ROOT=$PREF/android-ndk-r13b-linux-x86_64
export ANDROID_SDK_ROOT=/usr/local/android-sdk/
wget --quiet http://cdn.cocos2d-x.org/cocos2d-x-3.16.zip
unzip -q cocos2d-x-3.16.zip
cd cocos2d-x-3.16/
python2 setup.py
cd $PREF
export COCOS=$PREF/cocos2d-x-3.16/tools/cocos2d-console/bin/cocos
yes N | $COCOS new LudoMuse -l cpp com.IHMTEK.LudoMuse
cp -r Ludomuse/* LudoMuse
cd LudoMuse
