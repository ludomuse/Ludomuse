#! /bin/bash


echo "Before install path : $PWD"
export PREF=$PWD/../
cd $PREF
wget --quiet http://cdn.cocos2d-x.org/cocos2d-x-3.16.zip
unzip -q cocos2d-x-3.16.zip
cd cocos2d-x-3.16/
python2 setup.py
cd $PREF
export COCOS=$PREF/cocos2d-x-3.16/tools/cocos2d-console/bin/cocos
$COCOS new LudoMuse -l cpp com.IHMTEK.LudoMuse
cp -r Ludomuse/* LudoMuse
cd LudoMuse
