package org.cocos2dx.cpp.wifiDirect;

import android.app.Activity;
import android.app.AlertDialog;
import android.os.Handler;
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

import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;

import android.os.Handler;

import org.cocos2dx.cpp.wifiDirect.WifiDirectManager;
import org.cocos2dx.cpp.DebugManager;

public class ConnexionStatus
{
	private AlertDialog alertC;
	private AlertDialog alertD;
	private AlertDialog alertW;
	private boolean bEnabled = false;

	public ConnexionStatus(Activity activity)
	{

		AlertDialog.Builder builderC = new AlertDialog.Builder(activity, com.IHMTEK.LudoMuse.R.style.StatusC);
		AlertDialog.Builder builderD = new AlertDialog.Builder(activity, com.IHMTEK.LudoMuse.R.style.StatusD);
		AlertDialog.Builder builderW = new AlertDialog.Builder(activity, com.IHMTEK.LudoMuse.R.style.StatusW);
		alertC = MakeNewAlert("Connecte", builderC);
		alertD= MakeNewAlert("Rapprochez-vous!", builderD);
		alertW = MakeNewAlert("Je cherche...", builderW);
	}

	public void Enable(boolean a_bEnabled)
	{
		if(!a_bEnabled)
		{
			alertC.cancel();
			alertD.cancel();
			alertW.cancel();
		}
		bEnabled = a_bEnabled;
	}

	private AlertDialog MakeNewAlert(String text, AlertDialog.Builder a_builder)
	{

			AlertDialog alert = a_builder.create();
			Window alertWindow = alert.getWindow();
			alertWindow.setLayout(100, 10);
			// Make us non-modal, so that others can receive touch events.
			alertWindow.setGravity(Gravity.CENTER);
			alertWindow.setFlags(LayoutParams.FLAG_NOT_TOUCH_MODAL, LayoutParams.FLAG_NOT_TOUCH_MODAL);
			//no fading
			alertWindow.clearFlags(LayoutParams.FLAG_DIM_BEHIND);
			// ...but notify us that it happened.
			alertWindow.setFlags(LayoutParams.FLAG_WATCH_OUTSIDE_TOUCH, LayoutParams.FLAG_WATCH_OUTSIDE_TOUCH);
			alertWindow.setBackgroundDrawable(new ColorDrawable(Color.TRANSPARENT));

			alert.setMessage(text);
			//alert.show();

			DebugManager.print("ConnexionStatus-Toast: " + text, WifiDirectManager.DEBUGGER_CHANNEL);

		return alert;
	}

	public void SetPending()
	{
		if(!bEnabled)
			return;

		alertC.cancel();
		alertD.cancel();
		alertW.show();
	}

	public void SetConnected()
	{
		if(!bEnabled)
			return;

		alertC.show();
		alertD.cancel();
		alertW.cancel();
		Handler handler = new Handler();
		handler.postDelayed(new Runnable()
		{
			@Override
			public void run()
			{
				alertC.cancel();
			}
		}, 5000);
	}

	public void SetDisConnected()
	{
		if(!bEnabled)
			return;

		alertC.cancel();
		alertD.show();
		alertW.cancel();
	}


}