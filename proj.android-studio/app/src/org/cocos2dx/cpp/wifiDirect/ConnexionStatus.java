package org.cocos2dx.cpp.wifiDirect;

import android.app.Activity;
import android.app.AlertDialog;
import android.os.Debug;
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

import org.cocos2dx.cpp.AppActivity;
import org.cocos2dx.cpp.wifiDirect.WifiDirectManager;
import org.cocos2dx.cpp.DebugManager;

import java.util.Set;

public class ConnexionStatus {
    private AlertDialog _alertConnection;
    private AlertDialog _alertDeconnection;
    private AlertDialog _alertDiscoverPairs;
    private boolean bEnabled = false;
    private long _timeStart = -1;
    private Activity _activity;
    private static final long DELAYED_START = 5000;


    public ConnexionStatus(Activity activity)
    {
        _activity = activity;
        _timeStart = System.currentTimeMillis();

        // Create alerts with given style
        AlertDialog.Builder builderConnectionAlarmDialog = new AlertDialog.Builder(_activity, com.IHMTEK.LudoMuse.R.style.StatusC);
        AlertDialog.Builder builderDeconnectionAlarmDialog = new AlertDialog.Builder(_activity, com.IHMTEK.LudoMuse.R.style.StatusD);
        AlertDialog.Builder builderDiscoverPairs = new AlertDialog.Builder(_activity, com.IHMTEK.LudoMuse.R.style.StatusW);


        _alertConnection = MakeNewAlert("TABLETTES CONNECTEES !", builderConnectionAlarmDialog);
        _alertDeconnection = MakeNewAlert("A T T E N T I O N : \n" +
                "Vous avez perdu la connexion avec la tablette de votre partenaire. \n" +
                "Rapprochez-vous de lui !", builderDeconnectionAlarmDialog);
        _alertDiscoverPairs = MakeNewAlert("P A T I E N T E Z : \n" +
                "Recherche de tablette et tentative de reconnexion ...\n" +
                "Cela peut prendre 2 petites minutes...\n" +
                "\n" +
                "Ne vous ELOIGNEZ PAS !", builderDiscoverPairs);
    }

    public void Enable(boolean a_bEnabled)
    {
        if (System.currentTimeMillis() - _timeStart < DELAYED_START)
            return;

        if (!a_bEnabled) {
            CancelAll();
        }
        bEnabled = a_bEnabled;
    }

    private AlertDialog MakeNewAlert(String text, AlertDialog.Builder a_builder) {

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

       // DebugManager.print("ConnexionStatus-Toast: " + text, WifiDirectManager.DEBUGGER_CHANNEL);

        return alert;
    }

    private void CancelAll() {

        try {
            _activity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    try {

                        _alertConnection.cancel();
                        _alertDeconnection.cancel();
                        _alertDiscoverPairs.cancel();

                    } catch (Exception e) {
                        DebugManager.print("Error while hidding messagebox " + e.getLocalizedMessage(), WifiDirectManager.DEBUGGER_CHANNEL);
                    }
                }
            });
        } catch (Exception e) {
            DebugManager.print("Error while hidding messagebox " + e.getLocalizedMessage(), WifiDirectManager.DEBUGGER_CHANNEL);
        }
    }





    private void Show(final AlertDialog a_alertDialog) {

        if (!bEnabled)
            return;

        CancelAll();

        try {
            _activity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    try {
                        a_alertDialog.show();
                    } catch (Exception e) {
                        DebugManager.print("Error while showing messagebox ", WifiDirectManager.DEBUGGER_CHANNEL);
                    }
                }
            });
        } catch (Exception e) {
            DebugManager.print("Error while showing messagebox " + e.getLocalizedMessage(), WifiDirectManager.DEBUGGER_CHANNEL);
        }
    }

    public void SetPending(final WifiDirectManager _wifiDirect) {
        Show(_alertDiscoverPairs);
    }

    public void SetConnected() {
        Show(_alertConnection);
        AppActivity.postDelay(new Runnable() {
            @Override
            public void run() {

                CancelAll();
            }
        }, 3000);

    }

    public void SetDisConnected() {
        Show(_alertDeconnection);
    }


}