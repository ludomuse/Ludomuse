//package com.IHMTEK.LudoMuse;
package org.cocos2dx.cpp.wifiDirect;

/**
 * Created by ThisIsIt on 18/08/2015.
 * Totally inspired by daniel's code (05/02/15)
 * from Erasme. Credit goes to him.
 */

import android.content.*;
import android.net.wifi.p2p.*;

import java.lang.String;

import org.cocos2dx.cpp.DebugManager;

/**
 * A BroadcastReceiver that notifies of important Wi-Fi p2p events.
 */
public class WiFiDirectBroadcastReceiver extends BroadcastReceiver
{

	private static WifiDirectManager _wifiDirectManager;


	//------------------------------------------------------------------------------------------------------------------
	//																										Constructor
	//------------------------------------------------------------------------------------------------------------------
	public WiFiDirectBroadcastReceiver(WifiDirectManager wifiDirectManager)
	{
		super();
		_wifiDirectManager = wifiDirectManager;
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	@Override
	public void onReceive(Context context, final Intent intent)
	{
		String action = intent.getAction();

		DebugManager.print(action, WifiDirectManager.DEBUGGER_CHANNEL);

		if ( action.equals(WifiP2pManager.WIFI_P2P_STATE_CHANGED_ACTION) )
		{
			// Determine if Wifi P2P mode is enabled or not, alert the Activity.
			int state = intent.getIntExtra( WifiP2pManager.EXTRA_WIFI_STATE, -1 );

			if ( state == WifiP2pManager.WIFI_P2P_STATE_ENABLED )
			{
				DebugManager.print("WifiP2P is enabled !", WifiDirectManager.DEBUGGER_CHANNEL);
			}
			else
			{
				DebugManager.print("WifiP2P is not enabled !", WifiDirectManager.DEBUGGER_CHANNEL);
				// _wifiDirectManager.turnOnWifi();
			}
		}
		else if ( action.equals( WifiP2pManager.WIFI_P2P_PEERS_CHANGED_ACTION ) )  // indicates if the peer list has changed
		{
			// Request available peers from the wifi p2p manager. This is an asynchronous call and the calling activity
			// is notified with a callback on PeerListListener.onPeersAvailable() :

			// Only launch request peers if we are not connecting or connecting
		//	if ( _wifiDirectManager.IsConnectedOrConnecting( ) == false )
		//	{
				_wifiDirectManager.launchServiceRequestPeers(); // it works fine like this, so we leave like this.
		//	}
			// _wifiDirectManager.onConnectionChanged(intent);
		}
		else if ( action.equals( WifiP2pManager.WIFI_P2P_CONNECTION_CHANGED_ACTION ) )
		{
			/*
			 * Handler handler = new Handler(); handler.postDelayed(new
			 * Runnable() {
			 * 
			 * @Override public void run() {
			 * _wifiDirectManager.onConnectionChanged(intent);
			 * 
			 * }
			 * 
			 * }, 10000);
			 */

			_wifiDirectManager.onConnectionChanged(intent);
			// Respond to new connection or disconnections

		}
		else if (action.equals( WifiP2pManager.WIFI_P2P_THIS_DEVICE_CHANGED_ACTION ) )
		{
			// TODO
			// Respond to this device's wifi state changing
			WifiP2pDevice device = (WifiP2pDevice) intent.getParcelableExtra(WifiP2pManager.EXTRA_WIFI_P2P_DEVICE);
			DebugManager.print("THIS device name is " + device.deviceName, WifiDirectManager.DEBUGGER_CHANNEL);
			_wifiDirectManager.set_strThisDeviceName(device.deviceName);
		}

	}


}