package org.cocos2dx.cpp.jniFacade;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.lang.String;

import org.cocos2dx.cpp.AppActivity;
import org.cocos2dx.cpp.CameraActivity;
import org.cocos2dx.cpp.DebugManager;
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

	public static void takePicture(String maskPath)
	{
		AppActivity appActivity = (AppActivity) _wifiDirectFacade.getActivity();
//		appActivity.dispatchTakePictureIntent();
		
		Intent intent = new Intent(appActivity, CameraActivity.class);
		intent.putExtra("maskPath", maskPath);
		//appActivity.startActivity(intent);
		appActivity.startActivityForResult(intent, 1);
	}
	
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

			File statsFile = new File(file, "test.csv");
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
		
		
		
	}

	public static void getApplicationDirectory()
	{
		AppActivity appActivity = (AppActivity) _wifiDirectFacade.getActivity();
		JniCppFacade.setApplicationDirectory(appActivity.createOrReturnAppDir());
	}

}