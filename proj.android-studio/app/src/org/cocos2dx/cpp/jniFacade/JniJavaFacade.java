package org.cocos2dx.cpp.jniFacade;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.lang.String;
import java.util.Calendar;

import org.cocos2dx.cpp.AppActivity;
import org.cocos2dx.cpp.CameraActivity;
import org.cocos2dx.cpp.DebugManager;
import org.cocos2dx.cpp.VideoActivity;
import org.cocos2dx.cpp.wifiDirect.WifiDirectManager;

import android.os.Environment;
import android.sax.StartElementListener;
import android.util.Log;
import android.content.Context;
import android.content.Intent;

/**
 * These method are called by c++, when the cpp part of the WifiDirectFacade
 * want to send data or launch a service (request peer and connection)
 * 
 * @author Rani, Gregoire
 * 
 */
public class JniJavaFacade
{

	public static WifiDirectFacade _wifiDirectFacade;

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public static void send(String s)
	{
		_wifiDirectFacade.send(s);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public static void send(boolean b)
	{
		_wifiDirectFacade.send(b);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public static void send(int i)
	{
		DebugManager.print("sending int", WifiDirectManager.DEBUGGER_CHANNEL);
		_wifiDirectFacade.send(i);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public static void send(long l)
	{
		_wifiDirectFacade.send(l);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public static void send(char c)
	{
		_wifiDirectFacade.send(c);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public static void send(double d)
	{
		_wifiDirectFacade.send(d);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public static void sendFile(String path)
	{
		File f = new File(path);
		DebugManager.print("send file called from c++. File exists? " + f.exists(), WifiDirectManager.DEBUGGER_CHANNEL);
		_wifiDirectFacade.send(f);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public static void setServerTempFileName(String name)
	{
		WifiDirectFacade.setServerTempFileName(name);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public static void sendByte(byte b)
	{
		DebugManager.print("send byte is called from c++",  WifiDirectManager.DEBUGGER_CHANNEL);
		_wifiDirectFacade.send(b);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public static void sendBytes(byte[] bytes)
	{
		_wifiDirectFacade.send(bytes);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public static void send(float f)
	{
		_wifiDirectFacade.send(f);
	}

	//------------------------------------------------------------------------------------------------------------------
	//																									Called from c++
	//------------------------------------------------------------------------------------------------------------------
	public static void discoverPeers()
	{
        DebugManager.print("**************************************************", WifiDirectManager.DEBUGGER_CHANNEL);
		DebugManager.print("JniJavaFacade::discoverPeers ==>  called from c++", WifiDirectManager.DEBUGGER_CHANNEL);
        DebugManager.print("**************************************************", WifiDirectManager.DEBUGGER_CHANNEL);
		_wifiDirectFacade.discoverPeers();
	}

	//------------------------------------------------------------------------------------------------------------------
	//																									Called from c++
	//------------------------------------------------------------------------------------------------------------------
	public static void connectTo(String deviceName)
	{
		DebugManager.print("************************************************* ", WifiDirectManager.DEBUGGER_CHANNEL);
		DebugManager.print("JniJavaFacade::connectTo " + deviceName + " is called from c++ ==> IT ONLY SETS THE NAME OF PEER", WifiDirectManager.DEBUGGER_CHANNEL);
		DebugManager.print("************************************************* ", WifiDirectManager.DEBUGGER_CHANNEL);
		_wifiDirectFacade.connectTo(deviceName);
	}

	//------------------------------------------------------------------------------------------------------------------
	//																								Called from c++
	//------------------------------------------------------------------------------------------------------------------
	public static void getTabletName()
	{
		JniCppFacade.setTabletName(_wifiDirectFacade.getThisDeviceName());
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public static void clean()
	{
		_wifiDirectFacade.clear();
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public static void takePicture(String maskPath)
	{
		AppActivity appActivity = (AppActivity) _wifiDirectFacade.getActivity();
//		appActivity.dispatchTakePictureIntent();
		
		Intent intent = new Intent(appActivity, CameraActivity.class);
		intent.putExtra("maskPath", maskPath);
		//appActivity.startActivity(intent);
		appActivity.startActivityForResult(intent, 1);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public static void showVideo(String videoPath)
	{
		AppActivity appActivity = (AppActivity) _wifiDirectFacade.getActivity();
		Intent intent = new Intent(appActivity, VideoActivity.class);
		intent.putExtra("videoPath", videoPath);
		Log.d("LudoMuse", "starting video activity with path : " + videoPath);
		appActivity.startActivity(intent);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public static void saveStringToFile(String text)
	{
//		Log.d("[LUDO_STATS]", "saveStringToFile in Java");
//
//		String filename = "test.csv";
//		AppActivity appActivity = (AppActivity) _wifiDirectFacade.getActivity();
//
//		FileOutputStream os;
//		
//		try {
//			os = appActivity.openFileOutput(filename, Context.MODE_WORLD_READABLE);
//			os.write(text.getBytes());
//			os.close();
//			Log.d("[LUDO_STATS]", "Stats were successfully written to " + appActivity.getFilesDir() + "/" + filename);
//		} catch (Exception e) {
//			e.printStackTrace();
//		}
		
		
		// is external storgage writable
		String state = Environment.getExternalStorageState();
		if (Environment.MEDIA_MOUNTED.equals(state))
		{
			File file = new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOCUMENTS), "STATS");
			if (!file.mkdirs())
			{
				Log.d("[LUDO_STATS]", "Directory not created");
			}

			Calendar cal = Calendar.getInstance();
			String filename = cal.get(Calendar.DAY_OF_MONTH) + 
					"_"  + (cal.get(Calendar.MONTH) + 1) + 
					"_" + cal.get(Calendar.YEAR) + 
					"_" + cal.get(Calendar.HOUR_OF_DAY) + 
					"_" + cal.get(Calendar.MINUTE) + 
					"_" + cal.get(Calendar.MILLISECOND) + 
					".csv";
			File statsFile = new File(file, filename);
			try {
				if (statsFile.createNewFile())
				{
					Log.d("[LUDO_STATS]", "The public stats file was sucessfully created in the folder : " + statsFile.getAbsolutePath());
				}
				else
				{
					Log.e("LUDO_STATS]", "The file was not created, maybe overwritten");
				}
				Log.d("[LUDO_STATS]", "writing stats to file : " + statsFile.getAbsolutePath());
				FileOutputStream stream = new FileOutputStream(statsFile);
				stream.write(text.getBytes());
				stream.close();
				
			} catch (IOException e) {
				e.printStackTrace();
				Log.e("LUDO_STATS]", "An I/O error occured while trying to write the file : " + e.getLocalizedMessage());					
			}
			
		}
		else 
		{
			Log.e("[LUDO_STATS]", "Could not write to external storage");
		}
		
		JniCppFacade.closeApp();
		
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public static void getApplicationDirectory( )
	{
		AppActivity appActivity = (AppActivity) _wifiDirectFacade.getActivity();
		JniCppFacade.setApplicationDirectory(appActivity.createOrReturnAppDir());
	}

}