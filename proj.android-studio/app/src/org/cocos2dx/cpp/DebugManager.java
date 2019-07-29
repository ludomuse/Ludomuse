package org.cocos2dx.cpp;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Map.Entry;

//import java.util.GregorianCalendar;

import org.cocos2dx.cpp.wifiDirect.WifiDirectManager;

import android.app.Activity;
import android.app.AlertDialog;
import android.os.Environment;
import android.os.Handler;
import android.text.format.DateFormat;
import android.text.format.Time;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager.LayoutParams;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

public class DebugManager {

	public static boolean mute = true;

	private static String tag = "LudoMuseDebug";
	public static int currentDebugLevel = WifiDirectManager.DEBUGGER_CHANNEL;

	public static final int DEFAULT_LEVEL = 0;
	public static final int IMPORTANT_LEVEL = 1;
	public static boolean useToast = false;

	public static final boolean askForImmortalToast = false;

	public static boolean append = false;
	public static boolean displayExtraInfo = false;

	public static Activity activity;

	/*************************FILE LOG*****************************/

	private static File logFile;
	private static InputStream fileInputStream = null;
	private static FileOutputStream fileOutputStream = null;

	/*************************FILE LOG*****************************/
	// View

	private static View layout;
	private static LinearLayout linear_layout;
	private static TextView message;
	private static TextView className;
	private static TextView fileName;
	private static TextView methodName;
	private static TextView lineNumber;
	private static TextView messageType;

	private static TextView message_lbl;
	private static TextView className_lbl;
	private static TextView fileName_lbl;
	private static TextView methodName_lbl;
	private static TextView lineNumber_lbl;
	private static TextView messageType_lbl;

	private static boolean isInit = false;

	// Info to display
	private static String messageStr = "";
	private static String classNameStr;
	private static String methodNameStr;
	private static String fileNameStr;
	private static String lineNumberStr;

	public enum MsgType {
		DEBUG, INFO, ERROR
	}

	public static void printDefault(String message)
	{
		print(message, DEFAULT_LEVEL);
	}

	public static void printInfo()
	{
		printDefault("None");
	}

	public static void printImportant(String message)
	{
		print(message, IMPORTANT_LEVEL);
	}

	public static void clear()
	{
		messageStr = "";
	}

	private static void init()
	{
		if (isInit || mute)
			return;

		isInit = true;

		LayoutInflater inflater = activity.getLayoutInflater();
		layout = inflater
				.inflate(
						com.IHMTEK.LudoMuse.R.layout.debugview,
						(ViewGroup) activity
								.findViewById(com.IHMTEK.LudoMuse.R.id.toast_layout_root));

		linear_layout = (LinearLayout) layout
				.findViewById(com.IHMTEK.LudoMuse.R.id.linear_layout_toast);

		message = (TextView) layout.findViewById(com.IHMTEK.LudoMuse.R.id.msg);
		className = (TextView) layout
				.findViewById(com.IHMTEK.LudoMuse.R.id.classname);
		fileName = (TextView) layout
				.findViewById(com.IHMTEK.LudoMuse.R.id.filename);
		methodName = (TextView) layout
				.findViewById(com.IHMTEK.LudoMuse.R.id.methodname);
		lineNumber = (TextView) layout
				.findViewById(com.IHMTEK.LudoMuse.R.id.lnumber);
		messageType = (TextView) layout
				.findViewById(com.IHMTEK.LudoMuse.R.id.msgtype);

		message.setText("");
		className.setText("");
		fileName.setText("");
		methodName.setText("");
		lineNumber.setText("");
		messageType.setText("");

		message_lbl = (TextView) layout
				.findViewById(com.IHMTEK.LudoMuse.R.id.msg_lbl);
		className_lbl = (TextView) layout
				.findViewById(com.IHMTEK.LudoMuse.R.id.classname_lbl);
		fileName_lbl = (TextView) layout
				.findViewById(com.IHMTEK.LudoMuse.R.id.filename_lbl);
		methodName_lbl = (TextView) layout
				.findViewById(com.IHMTEK.LudoMuse.R.id.methodname_lbl);
		lineNumber_lbl = (TextView) layout
				.findViewById(com.IHMTEK.LudoMuse.R.id.lnumber_lbl);
		messageType_lbl = (TextView) layout
				.findViewById(com.IHMTEK.LudoMuse.R.id.msgtype_lbl);

		if (askForImmortalToast)
		{
			AlertDialog.Builder view = new AlertDialog.Builder(activity);
			view.setView(layout);
			alert = view.create();

			Window alertWindow = alert.getWindow();
			alertWindow.setLayout(1000, 100);
			// Make us non-modal, so that others can receive touch events.
			alertWindow.setGravity(Gravity.LEFT);
			alertWindow.setFlags(LayoutParams.FLAG_NOT_TOUCH_MODAL,
					LayoutParams.FLAG_NOT_TOUCH_MODAL);

			// ...but notify us that it happened.
			alertWindow.setFlags(LayoutParams.FLAG_WATCH_OUTSIDE_TOUCH,
					LayoutParams.FLAG_WATCH_OUTSIDE_TOUCH);

			alert.show();
		}
	}

	private static void InitLogFile(){

	    if (logFile == null) {
            /*********************FILE LOG*******************************/
		/*String extStorage = Environment.getExternalStorageState();
		String path = extStorage+"/Android/data/com.mydomain.myapp/";*/
            logFile = new File("/sdcard/LudoMuse/wifi_direct.log");
            if (!logFile.exists()) {
                try {
                    logFile.createNewFile();
                } catch (IOException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }
            }
            try
            {
                BufferedWriter buf = new BufferedWriter(new FileWriter(logFile, true));
				SimpleDateFormat date = new SimpleDateFormat("dd MMM yyyy, HH:mm");
                buf.append("============================ NEW SESSION STARTED : " + date.format(Calendar.getInstance().getTime()) + "============================");
				buf.newLine();
				buf.close();
            }
            catch (IOException e)
            {
                e.printStackTrace();
            }

        }
        /*********************FILE LOG*******************************/
    }

	private static void PrintIntoFile(String log){
        InitLogFile();
		try
		{
			//BufferedWriter for performance, true to set append to file flag
			BufferedWriter buf = new BufferedWriter(new FileWriter(logFile, true));
			buf.append(log);
            buf.newLine();
			//buf.append("\n\r");
			buf.close();
		}
		catch (IOException e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	@SuppressWarnings("unused")
	public static void RemoveDebugButton(String name)
	{
		if (!askForImmortalToast || mute)
			return;
		linear_layout.removeView(linear_layout.findViewWithTag(name));
	}

	@SuppressWarnings("unused")
	public static void AddDebugButton(String name, OnClickListener effect)
	{
		if (!askForImmortalToast || mute)
			return;

		init();

		Button button = new Button(activity.getApplicationContext());
		button.setText(name);
		button.setTag(name);
		button.setOnClickListener(effect);

		linear_layout.addView(button);
	}

	public static void print(final String message, int debugLevel)
	{
		if (debugLevel != currentDebugLevel || mute)
			return;

		StackTraceElement[] stackTraceElements = Thread.currentThread()
				.getStackTrace();
		StackTraceElement caller = null;
		for (StackTraceElement element : stackTraceElements)
		{
			if (!element.getClassName().equals("org.cocos2dx.cpp.DebugManager")
					&& !element.getClassName().equals("dalvik.system.VMStack")
					&& !element.getClassName().equals("java.lang.Thread"))
			{
				caller = element;
				break;
			}
		}

		if (caller == null)
			return;

		classNameStr = caller.getClassName();
		methodNameStr = caller.getMethodName();
		fileNameStr = caller.getFileName();
		lineNumberStr = caller.getLineNumber() + "";

		if (!useToast)
		{
			String formatedMessage = "";
			if (DebugManager.displayExtraInfo)
			{
				formatedMessage = ">\n[CALLING CLASS] " + className
						+ "\n[CALLING METHOD] " + methodName + "\n[FILE NAME] "
						+ fileName + "\n[LINE NUMBER] " + lineNumber
						+ "\n[MESSAGE]\n" + message + "\n>\n";
			}
			else
			{
				formatedMessage = message;
			}

			messageStr = append ? messageStr + formatedMessage
					: formatedMessage;
			Log.d(tag, formatedMessage);
			//printintofile
			PrintIntoFile(formatedMessage);
		}
		else
		{

			init();

			messageStr = append ? messageStr + "\n" + message : message;

			if (!askForImmortalToast)
			{
				DebugManager.message.setText(messageStr);
				showToast(activity);
			}
			else
			{
				startToastRefreshTimer();
			}

		}

	}

	private static Handler handler;

	private static boolean isTimerStarting = false;
	private static Runnable worker;

	public static void startToastRefreshTimer()
	{
		if (isTimerStarting || mute)
			return;

		isTimerStarting = true;
		handler = new Handler();

		worker = new Runnable() {

			@Override
			public void run()
			{
				if (useToast)
				{
					init();

					DebugManager.message.setText(messageStr);
					if (DebugManager.displayExtraInfo)
					{
						DebugManager.className.setText(classNameStr);
						DebugManager.fileName.setText(fileNameStr);
						DebugManager.methodName.setText(methodNameStr);
						DebugManager.lineNumber.setText(lineNumberStr);
						DebugManager.messageType.setText("debug");
					}
					else
					{
						DebugManager.className.setText("");
						DebugManager.fileName.setText("");
						DebugManager.methodName.setText("");
						DebugManager.lineNumber.setText("");
						DebugManager.messageType.setText("");

						DebugManager.message_lbl.setText("");
						DebugManager.className_lbl.setText("");
						DebugManager.fileName_lbl.setText("");
						DebugManager.methodName_lbl.setText("");
						DebugManager.lineNumber_lbl.setText("");
						DebugManager.messageType_lbl.setText("");
					}
				}
				else
				{
					Log.d(tag, messageStr);
				}
				handler.postDelayed(worker, 1000);
			}

		};

		handler.post(worker);
	}

	static AlertDialog alert;

	private static void showToast(Activity activity)
	{
		/*
		 * AlertDialog.Builder view = new AlertDialog.Builder(activity);
		 * view.setView(layout); alert = view.create(); alert.show();
		 * 
		 * handler.postDelayed(new Runnable(){
		 * 
		 * @Override public void run() { alert.cancel();
		 * 
		 * }}, 2000);
		 */

		Toast toast = new Toast(activity.getApplicationContext());
		toast.setGravity(Gravity.LEFT, 0, 0);
		toast.setView(layout);

		toast.show();
	}

	public static void printAllStackTraces(int channel)
	{
		String res = "";
		for(Entry<Thread, StackTraceElement[]> entry : Thread.getAllStackTraces().entrySet())
		{
			String name = "[THREAD=" + entry.getKey() + "]";
			for(StackTraceElement el : entry.getValue())
			{
				res += name + el + "\n";
			}
		}
		print(res, channel);
	}
}
