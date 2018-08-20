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
	static int _iBroadcastReceiverCallbackCount = 0;

	@Override
	public void onReceive(Context context, final Intent intent)
	{
		String action = intent.getAction();
		DebugManager.print("   ", WifiDirectManager.DEBUGGER_CHANNEL);
		DebugManager.print("*************************************************************", WifiDirectManager.DEBUGGER_CHANNEL);
		_iBroadcastReceiverCallbackCount++;
		DebugManager.print("* WifiDirectBroadcastReceiver::OnReceive callback started ! now + " + _iBroadcastReceiverCallbackCount + "  times *", WifiDirectManager.DEBUGGER_CHANNEL);
		DebugManager.print(action, WifiDirectManager.DEBUGGER_CHANNEL);
		DebugManager.print("--", WifiDirectManager.DEBUGGER_CHANNEL);

		if ( action.equals(WifiP2pManager.WIFI_P2P_STATE_CHANGED_ACTION ) )
		{
			int state = intent.getIntExtra( WifiP2pManager.EXTRA_WIFI_STATE, -1 );

			// Determine if Wifi P2P mode is enabled or not, alert the Activity.
			if ( state == WifiP2pManager.WIFI_P2P_STATE_ENABLED )
			{
				DebugManager.print("WifiDirectBroadcastReceiver::OnReceive ==> StateChangedAction = WifiP2P is enabled !", WifiDirectManager.DEBUGGER_CHANNEL);
				_wifiDirectManager.SetWifiDirectEnableState(true);
				LaunchDiscoverPeersIfRequiredByUser();
			}
			else
			{
				DebugManager.print("WifiDirectBroadcastReceiver::OnReceive ==> StateChangedAction = WARNING ! WifiP2P is NOT enabled ! ********", WifiDirectManager.DEBUGGER_CHANNEL);
				_wifiDirectManager.SetWifiDirectEnableState(false);
			}
		}
		else if ( action.equals( WifiP2pManager.WIFI_P2P_PEERS_CHANGED_ACTION ) )  // indicates if the peer list has changed
		{
			// Request available peers from the wifi p2p manager. This is an asynchronous call and the calling activity
			// is notified with a callback on PeerListListener.onPeersAvailable() :

			// Only launch request peers if we are not connecting or connecting. This condition is inside the serviceRequestPeers
            DebugManager.print("WifiDirectBroadCastReceiver::OnReceive ==> PeersChangedAction", _wifiDirectManager.DEBUGGER_CHANNEL );

            //Check if we are still connected.
			if ( _wifiDirectManager.IsConnected( ) )
			{
				DebugManager.print("WifiDirectBroadCastReceiver:OnReceive ==> PeerChangeAction : probably a new device in the wifiDirect range tried to connect, we don't do anything", _wifiDirectManager.DEBUGGER_CHANNEL);
			}
			else if ( _wifiDirectManager.IsConnectedOrConnecting() )
			{
				DebugManager.print("WifiDirectBroadCastReceiver:OnReceive ==> PeerChangeAction : probably a new device in the wifiDirect range, we don't do anything", _wifiDirectManager.DEBUGGER_CHANNEL);
			}
			else
			{
				DebugManager.print("WifiDirectBroadCastReceiver:OnReceive ==> PeerChangeAction : launch service request peers", _wifiDirectManager.DEBUGGER_CHANNEL);
				_wifiDirectManager.launchServiceRequestPeers();
			}
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

            DebugManager.print("WifiDirectBroadCastReceiver::OnReceive ==> CONNECTION_CHANGED", _wifiDirectManager.DEBUGGER_CHANNEL );

            if ( _wifiDirectManager.get_strGetThisDeviceName() != "")
            {
                DebugManager.print("WifiDirectBroadCastReceiver::OnReceive ==> treat state_connection_changed", _wifiDirectManager.DEBUGGER_CHANNEL );
                _wifiDirectManager.onConnectionChanged(intent);
            }
            else
            {
                DebugManager.print("WifiDirectBroadCastReceiver::OnReceive ==> CONNECTION_CHANGED ==> don't do anything since the device has not recover its own name yet..", _wifiDirectManager.DEBUGGER_CHANNEL );
            }

		}
		else if (action.equals( WifiP2pManager.WIFI_P2P_THIS_DEVICE_CHANGED_ACTION ) )
		{
			WifiP2pDevice device = (WifiP2pDevice) intent.getParcelableExtra(WifiP2pManager.EXTRA_WIFI_P2P_DEVICE);
			DebugManager.print("WifiDirectBroadcastReceiver::OnReceive ==> DeviceChangedAction ... THIS device name is " + device.deviceName, WifiDirectManager.DEBUGGER_CHANNEL);
			_wifiDirectManager.set_strThisDeviceName(device.deviceName);
			LaunchDiscoverPeersIfRequiredByUser();
		}


	}

	//------------------------------------------------------------------------------------------------------------------
	// Technical note : 19/08/2018
	// We have implemented this function because the C++ request to discover peers is aborted since the WifiDirect is
	// not activated at the the moment of the request (first request).
	// ISSUE: still we do not know why the PAIR label appears at the beginning of available peers.
	//------------------------------------------------------------------------------------------------------------------
	private void LaunchDiscoverPeersIfRequiredByUser()
	{
		if ( _wifiDirectManager.IsThereAPendingUserActionToDiscoverPeers() )
		{
			DebugManager.print( "WifiDirectManager::LaunchDiscoverPeersIfRequiredByUser ==> Try to launch the request to discover peers", WifiDirectManager.DEBUGGER_CHANNEL);
			_wifiDirectManager.SetUserRequestToDiscoverPeers(false);
			_wifiDirectManager.launchPeerDiscoveringForPendingUserRequest();
		}
		else
		{
			DebugManager.print( "WifiDirectManager::LaunchDiscoverPeersIfRequiredByUser ==> NOT LAUNCHED request since no pending user request to discover peers exists", WifiDirectManager.DEBUGGER_CHANNEL);
		}

	}


}