package org.cocos2dx.cpp.jniFacade;

import java.io.File;
import java.util.List;

import org.cocos2dx.cpp.DebugManager;
import org.cocos2dx.cpp.sockets.CallBackMethod;
import org.cocos2dx.cpp.wifiDirect.WifiDirectManager;

import android.app.Activity;
import android.util.Log;

/**
 * This is a wrapper for the WifiDirectManager, that expose simple method such
 * as onReceive(data) or send(data)F
 * 
 * @author Gregoire
 * 
 */
public class WifiDirectFacade {

	private WifiDirectManager _wifiDirectManager;
	
	//There, we set callback that are going to be called by the WifiDirectManager
	//when receiving data such as int, char, byte, string, ....
	private CallBackMethod _cmOnGettingsPeers = new CallBackMethod() {
		@Override
		public void Do(Object... vars)
		{
			onGettingPeers(_wifiDirectManager.getDeviceList());
		}
	};
	private CallBackMethod cmOnReceiveString = new CallBackMethod() {
		@Override
		public void Do(Object... vars)
		{
			onReceiving((String) vars[0]);
		}
	};
	private CallBackMethod cmOnReceiveInt = new CallBackMethod() {
		@Override
		public void Do(Object... vars)
		{
			onReceiving((Integer) vars[0]);
		}
	};
	private CallBackMethod cmOnReceiveBool = new CallBackMethod() {
		@Override
		public void Do(Object... vars)
		{
			onReceiving((Boolean) vars[0]);
		}
	};
	private CallBackMethod cmOnReceiveFloat = new CallBackMethod() {
		@Override
		public void Do(Object... vars)
		{
			onReceiving((Float) vars[0]);
		}
	};
	private CallBackMethod cmOnReceiveDouble = new CallBackMethod() {
		@Override
		public void Do(Object... vars)
		{
			onReceiving((Double) vars[0]);
		}
	};
	private CallBackMethod cmOnReceiveByte = new CallBackMethod() {
		@Override
		public void Do(Object... vars)
		{
			onReceiving((Byte) vars[0]);
		}
	};
	private CallBackMethod cmOnReceiveLong = new CallBackMethod() {
		@Override
		public void Do(Object... vars)
		{
			onReceiving((Long) vars[0]);
		}
	};
	private CallBackMethod cmOnReceiveFile = new CallBackMethod() {
		@Override
		public void Do(Object... vars)
		{
			onReceiving((File) vars[0]);
		}
	};
	private CallBackMethod cmOnReceiveByteArray = new CallBackMethod() {
		@Override
		public void Do(Object... vars)
		{
			onReceiving((byte[]) vars[0]);
		}
	};
	private CallBackMethod cmOnReceiveChar = new CallBackMethod() {
		@Override
		public void Do(Object... vars)
		{
			onReceiving((Character) vars[0]);
		}
	};

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public WifiDirectFacade(Activity activity)
	{
		JniJavaFacade._wifiDirectFacade = this;

		WifiDirectManager.registerCallBackReceiver(cmOnReceiveString,
				cmOnReceiveInt, cmOnReceiveBool, cmOnReceiveFloat,
				cmOnReceiveDouble, cmOnReceiveByte, cmOnReceiveLong,
				cmOnReceiveFile, cmOnReceiveByteArray, cmOnReceiveChar);

		_wifiDirectManager = new WifiDirectManager(activity);

		//If true, wifi manager will automatically reconnect when is deconnected
		_wifiDirectManager.autoReconnect = true;

		//allow wifi manager to relaunch discovery service when the list of found devices decrease
		_wifiDirectManager.enabledAutoRelanchingServiceDiscoverPeers = false;

		//If true, it will launch a connection request, even if a connection was already launched
		//(but still not answered)
		_wifiDirectManager.bForceConnectionRequest = false;
		_wifiDirectManager.initialize();

		//should be done in the exit method of the application
		_wifiDirectManager.clear();

//		LudoMuseThread.stopAllLudoMuseThread();
		
		DebugManager.print("WifiDirectFacade is created !",  WifiDirectManager.DEBUGGER_CHANNEL);
		DebugManager.print("tablet name is",  WifiDirectManager.DEBUGGER_CHANNEL);

	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void onGettingPeers(List<String> peers)
	{
		JniCppFacade.onGettingPeers(peers);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void onReceiving(String s)
	{
		DebugManager.print("onReceive string= " + s,  WifiDirectManager.DEBUGGER_CHANNEL);
		JniCppFacade.onReceivingString(s);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void onReceiving(int i)
	{
		DebugManager.print("Java WifiDirectFacade received int",
				WifiDirectManager.DEBUGGER_CHANNEL);
		JniCppFacade.onReceivingInt(i);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void onReceiving(boolean b)
	{
		Log.d("debug","LUDOMUSE - bool");
		JniCppFacade.onReceivingBool(b);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void onReceiving(long l)
	{
		Log.d("debug","LUDOMUSE - long");
		JniCppFacade.onReceivingLong(l);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void onReceiving(File f)
	{
		if (f != null)
		{
//			String fileName = f.getPath() + ".jpg";
//			f.renameTo(new File(fileName));
			JniCppFacade.onReceivingPhoto(f.getPath());
		}
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void onReceiving(double d)
	{
		Log.d("debug","LUDOMUSE - double");
		JniCppFacade.onReceivingDouble(d);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void onReceiving(float f)
	{
		Log.d("debug","LUDOMUSE - float");
		JniCppFacade.onReceivingFloat(f);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void onReceiving(char c)
	{
		Log.d("debug","LUDOMUSE - char");
		JniCppFacade.onReceivingChar(c);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void onReceiving(byte b)
	{
		Log.d("debug","LUDOMUSE - byte");
		JniCppFacade.onReceivingByte(b);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void onReceiving(byte[] bytes)
	{
		Log.d("debug","LUDOMUSE - bytes");
		JniCppFacade.onReceivingBytes(bytes);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void discoverPeers()
	{
		_wifiDirectManager.launchServicePeersDiscovering(_cmOnGettingsPeers);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void connectTo(String deviceName)
	{
		DebugManager.print("peer is: " + deviceName, WifiDirectManager.DEBUGGER_CHANNEL);
		_wifiDirectManager.setPeerName(deviceName);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void send(String s)
	{
		_wifiDirectManager.send(s);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void send(int i)
	{
		_wifiDirectManager.send(i);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void send(boolean b)
	{
		_wifiDirectManager.send(b);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void send(long l)
	{
		_wifiDirectManager.send(l);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void send(File f)
	{
		_wifiDirectManager.send(f);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void send(double d)
	{
		_wifiDirectManager.send(d);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void send(float f)
	{
		_wifiDirectManager.send(f);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void send(char c)
	{
		_wifiDirectManager.send(c);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void send(byte b)
	{
		_wifiDirectManager.send(b);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void send(byte[] bytes)
	{
		_wifiDirectManager.send(bytes);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void pause()
	{
		_wifiDirectManager.pause();
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void resume()
	{
		_wifiDirectManager.resume();
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void clear()
	{
		_wifiDirectManager.clear();
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public Activity getActivity()
	{
		return _wifiDirectManager.getActivity();
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public String getThisDeviceName()
	{
		return _wifiDirectManager.get_strThisDeviceName();
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public static void setServerTempFileName(String name)
	{
		WifiDirectManager.setServerTempFileName(name);
	}
}
