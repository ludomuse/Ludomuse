package org.cocos2dx.cpp.jniFacade;

import java.util.List;

import org.cocos2dx.cpp.DebugManager;
import org.cocos2dx.cpp.wifiDirect.WifiDirectManager;


/**
 * This class define all the native methods to be called
 * by the java part of the WifiDirectFacade when receiving data, 
 * in order to notifiy cpp part of the WifiDirectFacade object.
 * 
 * @author Gregoire
 *
 */
public class JniCppFacade {

	public static void onGettingPeers(List<String> peers)
	{
		DebugManager.print("Jni cpp facade has got peers !", WifiDirectManager.DEBUGGER_CHANNEL);
		String devices = concat(peers);
		DebugManager.print("Devices are: " + devices, WifiDirectManager.DEBUGGER_CHANNEL);
		onGettingPeers(devices);
	}
	
	private static String concat(List<String> peers)
	{
		String res = "";
		for(String s : peers)
		{
			res += "-" + s;
		}
		if (res.length() < 2)
			return "";
		return res.substring(1);
	}
	
	private static native void onGettingPeers(String peers);
	
	public static native void onReceivingString(String s);

	public static native void onReceivingInt(int i);

	public static native void onReceivingBool(boolean b);

	public static native void onReceivingLong(long l);

	public static native void onReceivingFile(String path);

	public static native void onReceivingFloat(float f);
	
	public static native void onReceivingDouble(double d);

	public static native void onReceivingChar(char d);
	
	public static native void onReceivingByte(byte d);
	
	public static native void onReceivingBytes(byte[] bytes);
	
	public static native void setTabletName(String name);
	
	public static native void setCurrentPicturePath(String name);
	
	public static native void setApplicationDirectory(String path);

	public static native void onReceivingPhoto(String path);
}
