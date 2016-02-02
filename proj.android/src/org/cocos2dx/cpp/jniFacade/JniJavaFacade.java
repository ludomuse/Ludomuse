package org.cocos2dx.cpp.jniFacade;

import java.io.File;
import java.lang.String;

import org.cocos2dx.cpp.AppActivity;
import org.cocos2dx.cpp.DebugManager;
import org.cocos2dx.cpp.wifiDirect.WifiDirectManager;

import android.os.Environment;

/**
 * These method are called by c++, when the cpp part of the WifiDirectFacade
 * want to send data or launch a service (request peer and connection)
 * 
 * @author Rani, Gregoire
 * 
 */
public class JniJavaFacade {

	public static WifiDirectFacade _wifiDirectFacade;

	public static void send(String s)
	{
		_wifiDirectFacade.send(s);
	}

	public static void send(boolean b)
	{
		_wifiDirectFacade.send(b);
	}

	public static void send(int i)
	{
		DebugManager.print("sending int", WifiDirectManager.DEBUGGER_CHANNEL);
		_wifiDirectFacade.send(i);
	}

	public static void send(long l)
	{
		_wifiDirectFacade.send(l);
	}

	public static void send(char c)
	{
		_wifiDirectFacade.send(c);
	}

	public static void send(double d)
	{
		_wifiDirectFacade.send(d);
	}

	public static void sendFile(String path)
	{
		File f = new File(path);
		DebugManager.print("send file called from c++. File exists? " + f.exists(),
				WifiDirectManager.DEBUGGER_CHANNEL);
		_wifiDirectFacade.send(f);
	}

	public static void setServerTempFileName(String name)
	{
		WifiDirectFacade.setServerTempFileName(name);
	}

	public static void sendByte(byte b)
	{
		DebugManager.print("send byte is called from c++",
				WifiDirectManager.DEBUGGER_CHANNEL);
		_wifiDirectFacade.send(b);
	}

	public static void sendBytes(byte[] bytes)
	{
		_wifiDirectFacade.send(bytes);
	}

	public static void send(float f)
	{
		_wifiDirectFacade.send(f);
	}

	public static void discoverPeers()
	{
		DebugManager.print("discover peers is called from c++",
				WifiDirectManager.DEBUGGER_CHANNEL);
		_wifiDirectFacade.discoverPeers();
	}

	public static void connectTo(String deviceName)
	{
		_wifiDirectFacade.connectTo(deviceName);
	}

	public static void getTabletName()
	{
		JniCppFacade.setTabletName(_wifiDirectFacade.getThisDeviceName());
	}

	public static void clean()
	{
		_wifiDirectFacade.clear();
	}

	public static void takePicture()
	{
		AppActivity appActivity = (AppActivity) _wifiDirectFacade.getActivity();
		appActivity.dispatchTakePictureIntent();
	}

	public static void getApplicationDirectory()
	{
		AppActivity appActivity = (AppActivity) _wifiDirectFacade.getActivity();
		JniCppFacade.setApplicationDirectory(appActivity.createOrReturnAppDir());
	}

}