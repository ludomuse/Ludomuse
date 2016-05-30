#ifndef LMJNIJAVAFACADE_H_
#define LMJNIJAVAFACADE_H_

#include <string>
#include "LmHelpers.h"
#include "cocos2d.h"
#include "../cocos2d/cocos/platform/android/jni/JniHelper.h"


class LmJniJavaFacade {

public:
		static void takePicture(const std::string&);

		static void saveStringToFile(const std::string& text);

		static void getApplicationDirectory();

		static void sendFile(std::string path);
private:

	static void send(std::string s);

	static void send(bool b);

	static void send(int i);

	static void send(long l);

	static void send(char c);

	static void send(double d);

	static void setServerTempFileName(std::string name);

	static void sendByte(lmByte b);

	static void sendBytes(bytes byte);

	static void send(float f);

	static void discoverPeers();

	static void connectTo(std::string deviceName);

	static void getTabletName();

	static void clean();

	inline static jint toJObject(int i) {
		return (jint) i;
	}

	inline static jboolean toJObject(bool b) {
		return (jboolean) b;
	}

	inline static jchar toJObject(char c) {
		return (jchar) c;
	}

	inline static jdouble toJObject(double d) {
		return (jdouble) d;
	}

	inline static jfloat toJObject(float f) {
		return (jfloat) f;
	}

	inline static jlong toJObject(long l) {
		return (jlong) l;
	}

	inline static jbyte toJByte(lmByte b) {
		return (jbyte) b;
	}

	inline static jbyteArray toJObject(bytes byteArray, JNIEnv* env) {
		jbyteArray res = env->NewByteArray(toJObject(byteArray.getLen()));
		env->SetByteArrayRegion(res, (jsize) 0, (jsize) (byteArray.getLen()),
				(const jbyte *) byteArray.toByteArray());
		//env->SetByteArrayRegion(jbytearray array, jsize start, jsize len, const jbyte *);
		return res;
	}

	inline static jstring toJObject(std::string jstr, JNIEnv* env) {
		return env->NewStringUTF(jstr.c_str());
	}

	static void callJavaMethod(const char* name, void* o, const char* param);

	LmJniJavaFacade(const LmJniJavaFacade & unLmJniBridge);

	LmJniJavaFacade();

	virtual ~LmJniJavaFacade();

	friend class LmWifiDirectFacade;

};

#endif // LMJNIJAVAFACADE_H
