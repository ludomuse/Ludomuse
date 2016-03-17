/*************************************************************************
 LmJniBridge  -  description
 -------------------
 début                : 2015
 copyright            : (C) 2015 par Rani Baghezza
 *************************************************************************/

//---------- Interface de la classe <LmJniBridge> (fichier LmJniBridge.h) ------
#ifndef LMJNICPPFACADE_H_
#define LMJNICPPFACADE_H_

#include "../cocos2d/cocos/platform/android/jni/JniHelper.h"
#include <string>
#include <vector>
#include <sstream>
#include "LmWifiDirectFacade.h"

extern "C" {
JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_jniFacade_JniCppFacade_onGettingPeers(
		JNIEnv* env, jobject thiz, jstring peers);

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_jniFacade_JniCppFacade_onReceivingString(
		JNIEnv* env, jobject thiz, jstring s);

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_jniFacade_JniCppFacade_onReceivingInt(
		JNIEnv* env, jobject thiz, jint i);

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_jniFacade_JniCppFacade_onReceivingBool(
		JNIEnv* env, jobject thiz, jboolean b);

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_jniFacade_JniCppFacade_onReceivingLong(
		JNIEnv* env, jobject thiz, jlong l);

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_jniFacade_JniCppFacade_onReceivingFloat(
		JNIEnv* env, jobject thiz, jfloat f);

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_jniFacade_JniCppFacade_onReceivingDouble(
		JNIEnv* env, jobject thiz, jdouble d);

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_jniFacade_JniCppFacade_onReceivingBytes(
		JNIEnv* env, jobject thiz, jbyteArray bytes);

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_jniFacade_JniCppFacade_onReceivingFile(
		JNIEnv* env, jobject thiz, jstring path);

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_jniFacade_JniCppFacade_onReceivingChar(
		JNIEnv* env, jobject thiz, jchar c);

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_jniFacade_JniCppFacade_onReceivingByte(
		JNIEnv* env, jobject thiz, jbyte byte);

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_jniFacade_JniCppFacade_setTabletName(
		JNIEnv* env, jobject thiz, jstring name);

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_jniFacade_JniCppFacade_setCurrentPicturePath(
		JNIEnv* env, jobject thiz, jstring path);

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_jniFacade_JniCppFacade_setApplicationDirectory(
		JNIEnv* env, jobject thiz, jstring path);

}








class LmJniCppFacade {

public:

	static LmWifiDirectFacade* getWifiFacade();

	static void setWifiFacade(LmWifiDirectFacade* wifiFacade);

	static std::string getCurrentPicturePath();

	static void setCurrentPicturePath(std::string);

	static std::string getApplicationDirectory();

	static void setApplicationDirectory(std::string);

	inline static int toCObject(jint i) {
		return (int) i;
	}

	inline static bool toCObject(jboolean b) {
		return (bool) b;
	}

	inline static char toCObject(jchar c) {
		return (char) c;
	}

	inline static double toCObject(jdouble d) {
		return (double) d;
	}

	inline static float toCObject(jfloat f) {
		return (float) f;
	}

	inline static long toCObject(jlong l) {
		return (long) l;
	}

	inline static lmByte toCObject(jbyte b) {
		return (byte) b;
	}

	inline static bytes toCObject(jbyteArray byteArray, JNIEnv* env) {
		jboolean isCopy;
		lmByte* data = (lmByte*) env->GetByteArrayElements(byteArray, &isCopy);
		int len = env->GetArrayLength(byteArray);
		return bytes(data, len);
	}

	inline static std::string toCObject(jstring jstr, JNIEnv* env) {
		const char *s = env->GetStringUTFChars(jstr, 0);
		return std::string(s);
	}

	inline static std::vector<std::string> toSTDStringList(jstring jstr,
			JNIEnv* env) {
		std::string str = toCObject(jstr, env);
		return split(str, '-');
	}

private:
	inline static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	    std::stringstream ss(s);
	    std::string item;
	    while (std::getline(ss, item, delim)) {
	        elems.push_back(item);
	    }
	    return elems;
	}


	inline static std::vector<std::string> split(const std::string &s, char delim) {
	    std::vector<std::string> elems;
	    split(s, delim, elems);
	    return elems;
	}

	static LmWifiDirectFacade* _wifiDirectFacade;
	static std::string _currentPicturePath;
	static std::string _applicationDirectory;
};

#endif //LMJNICPPFACADE_H
