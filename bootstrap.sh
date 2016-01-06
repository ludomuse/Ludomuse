
# Update and dependencies
apt-get update
apt-get install -y python openjdk-7-jdk ant unzip

# Android SDK
cd /home/vagrant
echo "Downloading android sdk ..."
su vagrant -c "wget -nv /home/vagrant http://dl.google.com/android/android-sdk_r24.4.1-linux.tgz"
su vagrant -c "tar xvf android-sdk_r24.4.1-linux.tgz"

#echo "#! /usr/bin/env bash" >> /home/vagrant/setup.sh
cd android-sdk-linux/tools
# FILTER=tool,platform,build-tools-20.0.0,android-19
#(while : ; do echo 'y'; sleep 2; done)
su vagrant -c "(while : ; do echo 'y'; sleep 2; done) | ./android update sdk --no-ui --all --filter  build-tools-23.0.2,tool,platform-tool,android-19"
#su vagrant -c "bash android create project --target 1 --name DummyProject --path /home/vagrant/DummyProject --activity DummyActivity --package com.ihmtek.dummy"

echo "export ANDROID_SDK_ROOT=/home/vagrant/android-sdk-linux" >> /home/vagrant/.bashrc
echo "export JAVA_HOME=/usr/lib/jvm/java-7-openjdk-i386/" >> /home/vagrant/.bashrc


# Android NDK
cd /home/vagrant
echo "Downloading android ndk ..."
su vagrant -c "wget -nv http://dl.google.com/android/ndk/android-ndk-r10e-linux-x86.bin"
chmod +x android-ndk-r10e-linux-x86.bin
su vagrant -c "./android-ndk-r10e-linux-x86.bin"

#echo "#chmod +x android-ndk-r10e-linux-x86.bin" >> /home/vagrant/setup.sh
#echo "#./android-ndk-r10e-linux-x86.bin" >> /home/vagrant/setup.sh

echo "export NDK_ROOT=/home/vagrant/android-ndk-r10e" >> /home/vagrant/.bashrc

su vagrant -c "source .bashrc"

# cocos2d-x
echo "Downloading cocos2d-x ..."
su vagrant -c "wget -nv http://www.cocos2d-x.org/filedown/cocos2d-x-3.9.zip"
su vagrant -c "mkdir cocos2d-x"
su vagrant -c "unzip cocos2d-x-3.9.zip -d cocos2d-x"
cd cocos2d-x
su vagrant -c "python setup.py"

#echo "#cd cocos2d-x" >> /home/vagrant/setup.sh
#echo "#python setup.py" >> /home/vagrant/setup.sh

#chmod +x /home/vagrant/setup.sh
