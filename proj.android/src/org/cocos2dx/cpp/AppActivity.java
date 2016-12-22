//package com.IHMTEK.LudoMuse;
package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;

import org.cocos2dx.cpp.jniFacade.WifiDirectFacade;
import org.cocos2dx.cpp.jniFacade.JniCppFacade;
import org.cocos2dx.lib.Cocos2dxActivity;

import android.os.Bundle;
import android.provider.MediaStore;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import android.util.Log;
import java.lang.Override;

/**
 * This is the main activity for LudoMuse. THERE MUST NOT BE ANY GRAPHICS
 * COMPONENTS (Buttons, TextFiels...) IN THE WHOLE JAVA PART OF LUDOMUSE, BUT
 * ESPECIALLY IN THIS CLASS. If there are any graphics component, they might
 * show up upon LudoMuse startup, regardless of the AppDelegate or any C++ entry
 * point This class is to replace the MainActivity.java class, but should not
 * depend on any graphics component as stated earlier.
 */

public class AppActivity extends Cocos2dxActivity {
	private static AppActivity instance;

	private WifiDirectFacade _wifiFacade;

	public static Activity getInstance()
	{
		return instance;
	}

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		// DebugManager.printInfo();
		instance = this;
		super.onCreate(savedInstanceState);

                // redirects logcat to filesystem
                File filename = new File("/sdcard/LudoMuse/logcat.log");
                try // to create logcat redirection file
                {
                  filename.createNewFile();
                  String cmd = "logcat -f " + filename.getAbsolutePath();
                  Runtime.getRuntime().exec(cmd);
                }
                catch (IOException e)
                {
                  e.printStackTrace();
                  Log.e("LudoMuse", "could not open logcat log file", e);
                }


		_wifiFacade = new WifiDirectFacade(this);
		
		if ((getIntent().getFlags() & Intent.FLAG_ACTIVITY_BROUGHT_TO_FRONT) != 0) 
		{
		    finish();
		    return;
		}

	}

	//private boolean stopAllowed = true;
	//@Override
	/*protected void onStop()
	{
		if(stopAllowed)
		{
			DebugManager.print("[FINISHING] on stop",
					WifiDirectManager.DEBUGGER_CHANNEL);
			//_wifiFacade.clear(); // TODO totest
			super.onStop();
		}
	}*/

	/* register the broadcast receiver with the intent values to be matched */
	@Override
	protected void onResume()
	{
		super.onResume();
		_wifiFacade.resume();
	}

	/* unregister the broadcast receiver */
	@Override
	protected void onPause()
	{
		super.onPause();
		_wifiFacade.pause();
	}

	// Taking photo

	//static final int REQUEST_TAKE_PHOTO = 1;
	static final int REQUEST_IMAGE_CAPTURE = 1;
	
	public void dispatchTakePictureIntent()
	{
		//4096 * 4096
	    Intent takePictureIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
	    if (takePictureIntent.resolveActivity(getPackageManager()) != null) {
	        startActivityForResult(takePictureIntent, REQUEST_IMAGE_CAPTURE);
	    }
	}

	String mCurrentPhotoPath;

	public String createOrReturnAppDir()
	{
		return getInstance().getApplicationInfo().dataDir;
		/*File f = new File(Environment.getExternalStorageDirectory() + "/Android/data/" + getPackageName());
		if(!f.exists())
			f.mkdir();
		return f.getAbsolutePath();*/
	}
		
	private File createImageFile() throws IOException
	{
		// Create an image file name
		/*String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss")
				.format(new Date());
		String imageFileName = "JPEG_" + timeStamp + "_";*/
		File image = new File(createOrReturnAppDir() + "/MyPic" + ".jpg");
		if(image.exists())
		{	
			image.delete();
		}

		image.createNewFile();
		// Save a file: path for use with ACTION_VIEW intents
		mCurrentPhotoPath = image.getAbsolutePath();
		return image;
	}
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		
	    super.onActivityResult(requestCode, resultCode, data);

	    if (requestCode == REQUEST_IMAGE_CAPTURE && resultCode == RESULT_OK) {
	        Bundle extras = data.getExtras();
	        Bitmap imageBitmap = (Bitmap) extras.get("data");
	        imageBitmap = Bitmap.createScaledBitmap(imageBitmap, Math.min(imageBitmap.getWidth(), 4096), Math.min(imageBitmap.getHeight(), 4096), true);
	      
	        FileOutputStream fos;
			try
			{
				fos = new FileOutputStream(createImageFile());
				imageBitmap.compress(Bitmap.CompressFormat.PNG, 50, fos);
				fos.close();
				JniCppFacade.setCurrentPicturePath(mCurrentPhotoPath);
			}
			catch (FileNotFoundException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			catch (IOException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
	        
	        
	    }
	}
	
}
