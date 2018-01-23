#! /bin/bash


echo "Before install path : $PWD"
export PREF=$PWD/../
cd $PREF


# setup sdk
if test ! -e $HOME/android-sdk-dl/sdk-tools.zip; then
    echo "no cached sdk, downloading ..."
    mkdir -p $HOME/android-sdk-dl/
    mkdir -p $HOME/android-sdk/
    
    curl -s https://dl.google.com/android/repository/sdk-tools-linux-3859397.zip > $HOME/android-sdk-dl/sdk-tools.zip ;
    curl -s https://dl.google.com/android/repository/tools_r25.2.5-linux.zip > $HOME/android-sdk-dl/sdk-tools-25.zip ;

    echo "unzipping tools"
    unzip -qq -n $HOME/android-sdk-dl/sdk-tools.zip -d $HOME/android-sdk
    echo "downloading build and platform tools"
    echo y | $HOME/android-sdk/tools/bin/sdkmanager 'tools' > /dev/null
    echo y | $HOME/android-sdk/tools/bin/sdkmanager 'platform-tools' > /dev/null
    echo y | $HOME/android-sdk/tools/bin/sdkmanager 'build-tools;23.0.1' > /dev/null
    echo y | $HOME/android-sdk/tools/bin/sdkmanager 'platforms;android-22' > /dev/null
    # echo y | $HOME/android-sdk/tools/bin/sdkmanager 'extras;google;m2repository' > /dev/null
    unzip -qq $HOME/android-sdk-dl/sdk-tools-25.zip -d $HOME/android-sdk

fi

export ANDROID_SDK_ROOT=$HOME/android-sdk


# setup ndk
if test ! -e $HOME/android-ndk-dl/android-ndk.zip; then
    mkdir -p $HOME/android-ndk-dl/
    mkdir -p $HOME/android-ndk/
    
    curl -s https://dl.google.com/android/repository/android-ndk-r13b-linux-x86_64.zip > $HOME/android-ndk-dl/android-ndk.zip ;
fi
unzip -qq -n $HOME/android-ndk-dl/android-ndk.zip -d $HOME/android-ndk
export NDK_ROOT=$HOME/android-ndk


# setup cocos
if test ! -e $HOME/cocos-dl/cocos.zip; then
    mkdir -p $HOME/cocos-dl/
    mkdir -p $HOME/cocos/
    
    curl -s http://cdn.cocos2d-x.org/cocos2d-x-3.16.zip > $HOME/cocos-dl/cocos.zip ;
fi
unzip -qq -n $HOME/cocos-dl/cocos.zip -d $HOME/cocos/
python2 $HOME/cocos/cocos2d-x-3.16/setup.py
export COCOS=$HOME/cocos/cocos2d-x-3.16/tools/cocos2d-console/bin/cocos


echo "creating LudoMuse project"
cd $HOME
yes N | $COCOS new LudoMuse -l cpp -p com.IHMTEK.LudoMuse
cp -r $PREF/Ludomuse/* $HOME/LudoMuse
cd $HOME/LudoMuse
