package org.cocos2dx.cpp.wifiDirect;


import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import android.os.Debug;
import android.os.SystemClock;

import org.cocos2dx.cpp.AppActivity;
import org.cocos2dx.cpp.DebugManager;
import org.cocos2dx.cpp.sockets.CallBackMethod;
import org.cocos2dx.cpp.sockets.SocketHandler;

import java.lang.Object;

import android.opengl.Matrix;
import android.net.wifi.p2p.*;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.NetworkInfo;
import android.net.wifi.WifiManager;
import android.net.wifi.WpsInfo;
import android.net.wifi.WifiManager.WifiLock;
import android.net.wifi.p2p.WifiP2pConfig;
import android.net.wifi.p2p.WifiP2pDevice;
import android.net.wifi.p2p.WifiP2pDeviceList;
import android.net.wifi.p2p.WifiP2pInfo;
import android.net.wifi.p2p.WifiP2pManager;
import android.net.wifi.p2p.WifiP2pManager.ActionListener;
import android.net.wifi.p2p.WifiP2pManager.Channel;
import android.net.wifi.p2p.WifiP2pManager.ConnectionInfoListener;
import android.net.wifi.p2p.WifiP2pManager.DnsSdServiceResponseListener;
import android.net.wifi.p2p.WifiP2pManager.DnsSdTxtRecordListener;
import android.net.wifi.p2p.WifiP2pManager.PeerListListener;

import android.net.wifi.p2p.nsd.WifiP2pDnsSdServiceInfo;
import android.net.wifi.p2p.nsd.WifiP2pDnsSdServiceRequest;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Toast;
import android.view.Gravity;
import android.net.ConnectivityManager;
import android.net.Network;
import android.net.NetworkInfo;

public class WifiDirectManager
{
	private static final int LONG_DELAY = 20000;
	private static final int SHORT_DELAY = 2000;
	private static final int MEDIUM_DELAY = 5000;
	private Activity _activity;
	private WifiP2pManager _manager;
	private WifiManager.WifiLock _lock;
	private Channel _channel;
	private IntentFilter _intentFilter;
	private WiFiDirectBroadcastReceiver _receiver;

	private WifiManager _wifiManager;

	private static final String TXTRECORD_PROP_AVAILABLE = "available";
	private static final String SERVICE_INSTANCE = "_LudoMuse";
	private static final String SERVICE_REG_TYPE = "_presence._tcp";

	private int _timeBeforeTryingNewConnection = SHORT_DELAY;

	private Map<String, String> _mapAddressNameDevices = new HashMap<String, String>();
	private List<String> _deviceList = new ArrayList<String>();

	private Map<String, String> _mapAddressNameAllDevices = new HashMap<String, String>();
	private List<String> _allDeviceList = new ArrayList<String>();

	private List<DnsListenerServiceAvailableItem> _serviceAvailables;
	private List<DnsListenerTxtRecordItem> _txtRecordAvailables;

	public static final int DEBUGGER_CHANNEL = 3;

	private String lastPeerName = "";

	private CallBackMethod _cmPeerDiscovered;
	private boolean requestForServicePeersDiscoveringAlreadyLaunched = false;

	private boolean requestForServiceRequestPeersAlreadyLaunched = false;
	public boolean enabledAutoRelanchingServiceDiscoverPeers = false;

	private boolean _bWifiDirectEnable = false;

	private SocketHandler socket;
	// public final static int LISTENNING_PORT_OWNER = 666;
	// public final static int LISTENNING_PORT_OTHER = 777;
	public final static int LISTENNING_PORT = 40001;


	private CallBackMethod _cmPeerConnected;
	private boolean _bRequestForConnectionAlreadyLaunched = false;
	private boolean _bRequestForServiceClearLocalServicesAlreadyLaunched = false;
	private boolean _bRequestForServiceServicesDiscoveringAlreadyLaunched = false;
	private boolean _bRequestForLaunchingServiceDnsSdInfoAlreadyLaunched = false;
	private String _strThisDeviceName = "";
	private boolean _bRequestForAddingDnsRequestAlreadyLaunched = false;

	public boolean bForceConnectionRequest = true;

	public boolean autoReconnect = true;

	private Intent _lastIntent = null;

	//------------------------------------------------------------------------------------------------------------------
	//																									Constructor
	//------------------------------------------------------------------------------------------------------------------
	public WifiDirectManager(Activity activity)
	{
		_activity = activity;
	}

	private ConnexionStatus _connexionStatus;
	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void initialize()
	{
		debugTrace("**************************************************************");
		debugTrace("**************************************************************");
		debugTrace("**************************************************************");
		debugTrace("******** S  T  A  R  T  : I N I T I A L I Z A T I O N ********");
		debugTrace("**************************************************************");
		debugTrace("**************************************************************");
		debugTrace("**************************************************************");


		initDebugger();
		initSocket();
		initManager();

		_connexionStatus = new ConnexionStatus(_activity);

		//disconnect from groups
		//disconnect();  commented because it does not do nothing. always give that it is not owner..
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public boolean IsWifiDirectEnabled( )
	{
		return _bWifiDirectEnable;
	}

	public void SetWifiDirectEnableState( boolean a_wifiDirectEnableState )
	{
		_bWifiDirectEnable = a_wifiDirectEnableState;
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void pause()
	{
		debugTrace("********** APP PAUSE **********");
		_activity.unregisterReceiver(_receiver);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void resume()
	{

		debugTrace("********** APP RESUME **********");
		_activity.registerReceiver(_receiver, _intentFilter);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	private void initSocket()
	{
		socket = new SocketHandler(1024, LISTENNING_PORT);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	private void initManager()
	{
		debugTrace("WifiDirectManager::initManager");
		_wifiManager = (WifiManager) _activity.getSystemService(Context.WIFI_SERVICE);
		_manager = (WifiP2pManager) _activity.getSystemService(Context.WIFI_P2P_SERVICE);
		_channel = _manager.initialize(_activity, _activity.getMainLooper(), null);

		_deviceList.clear();

		//In this method: 1) turn off wifi 2) delete persistent groups 3) turn on wifi
		askToRemoveGroup();

		_receiver = new WiFiDirectBroadcastReceiver(this);

		// IntentFilter mIntentFilter;
		_intentFilter = new IntentFilter();
		_intentFilter.addAction(WifiP2pManager.WIFI_P2P_STATE_CHANGED_ACTION);
		_intentFilter.addAction(WifiP2pManager.WIFI_P2P_PEERS_CHANGED_ACTION);
		_intentFilter.addAction(WifiP2pManager.WIFI_P2P_CONNECTION_CHANGED_ACTION);
		_intentFilter.addAction(WifiP2pManager.WIFI_P2P_THIS_DEVICE_CHANGED_ACTION);

		_lock = _wifiManager.createWifiLock(WifiManager.WIFI_MODE_FULL_HIGH_PERF, "ludomuse");
		_lock.acquire();

		debugTrace("WifiDirectManager::initManager ==> end initialization of initManager !");

	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	private void initDebugger()
	{
		DebugManager.activity = _activity;

	//	DebugManager.AddDebugButton("ClearLog", new OnClickListener()
	//	{
	//		@Override
	//		public void onClick(View arg0)
	//		{
	//			DebugManager.clear();
//
//			}
//		});

//		DebugManager.AddDebugButton("Scan", new OnClickListener()
//		{
//			@Override
//			public void onClick(View arg0)
//			{
//				launchServicePeersDiscovering(null);
//			}
//		});

//		DebugManager.AddDebugButton("RegisterAndDiscoverService", new OnClickListener()
//		{
//			@Override
//			public void onClick(View arg0)
//			{
//		startRegistrationAndDiscovery();
//			}
//		});

//		DebugManager.AddDebugButton("SendString", new OnClickListener()
//		{
//			@Override
//			public void onClick(View arg0)
//			{
//				send("this is a test");
//			}
//		}
//		);

//		DebugManager.AddDebugButton("SendFile", new OnClickListener()
//		{
//			@SuppressLint("SdCardPath")
//			@Override
//			public void onClick(View arg0)
//			{
//				File f = new File("/sdcard/Screenshots/img.jpg");
//				debugTrace(" file exits ? " + f.exists());
//				send(f);
//			}
//		});
//
//		DebugManager.AddDebugButton("SendLong", new OnClickListener()
//		{
//			@Override
//			public void onClick(View arg0)
//			{
//				send(123456789l);
//			}
//		});
//
//		DebugManager.AddDebugButton("Clear", new OnClickListener()
//		{
//
//			@Override
//			public void onClick(View arg0)
//			{
//				clear();
//			}
//		});
//
//		DebugManager.AddDebugButton("SwitchWifi", new OnClickListener()
//		{
//
//			@Override
//			public void onClick(View arg0)
//			{
//				switchWifi();
//			}
//		});
	}


	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void switchWifi()
	{
		if (isWifiEnabled())
		{
			turnOffWifi();
		}
		else
		{
			turnOnWifi();
		}
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void turnOfandthenOffWifi( )
	{
		turnOffWifi();

		turnOnWifi();

	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void clear()
	{
		debugTrace("WifiDirectManager::clear ==> release lock, stopHandlers, socket stop, clear debugManager");
		_lock.release();
		stopHandlers();
		//askToRemoveGroup();  //has been done before
		socket.stop();
		//turnOffWifi();      //has been done before
		//turnOnWifi();		  // has been done before
		//askToClearAllRequestsAndLocalServices(); // is done in another method
		DebugManager.clear();


	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void clearRequestsAndLocalServices( )
	{
		askToClearAllRequestsAndLocalServices();
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void stopHandlers()
	{
		socket.stopHandlers();
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void turnOnWifi()
	{
		_wifiManager.setWifiEnabled(true);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void turnOffWifi()
	{
		_wifiManager.setWifiEnabled(false);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public boolean isWifiEnabled()
	{
		return _wifiManager.isWifiEnabled();
	}

	//------------------------------------------------------------------------------------------------------------------
	//																						Relaunch peer discovering
	//------------------------------------------------------------------------------------------------------------------
	public void RelaunchServicePeersDiscovering()
	{
		debugTrace("WifiDirectManager::RelauchServicePeerDiscovering");
		if (_cmPeerDiscovered == null)
		{
			debugTrace("WifiDirectManager::RelaunchServicePeerDiscovering ==> Callback for peer discovering not assigned ");
		}

		// 2. Launch service to discover peers
		_manager.discoverPeers(_channel, new WifiP2pManager.ActionListener()
		{
			// 2.1. Callback if peers discovering sucessfully launched
			@Override
			public void onSuccess()
			{
				debugTrace("WifiDirectManager::RelaunchServicePeersDiscovering ==> Discover peers: success on : Set requestForServicePeersDiscoveringAlreadyLaunched to false ");
				requestForServicePeersDiscoveringAlreadyLaunched = false;
			}

			// 2.2 Callback if error occurred during service discovering
			@Override
			public void onFailure(int iReasonCode)
			{
				String text = "WifiDirectManager::RelaunchServicePeersDiscovering ==> Discover peers: Fail to RELAUNCH service discovering because ";
				switch (iReasonCode)
				{
					case 0:
						text += "of internal error : Probably the socket of the other tablet is closed";
						break;
					case 1:
						text += "P2P is unsupported on this device";
						break;
					case 2:
						text += "the framework is busy and unable to service the request";
						break;
					case 3:
						text = "because no service resquests are added";
						break;
					default:
						text = "of unknow error";
						break;
				}
				debugTrace(text);
				requestForServicePeersDiscoveringAlreadyLaunched = false;
			}
		});
	}


	//------------------------------------------------------------------------------------------------------------------
	//																							PEER's discovering
	//------------------------------------------------------------------------------------------------------------------
	public void launchPeerDiscoveringForPendingUserRequest( )
	{
		debugTrace("WifiDirectManager::launchPeerDiscoveringForPendingUserRequest");
		if ( _cmPeerDiscovered != null)
		{
			launchServicePeersDiscovering( _cmPeerDiscovered );
		}
		else
		{
			debugTrace("WifiDirectManager::launchPeerDiscoveringForPendingUserRequest ==> not launched since callback is null");
		}
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void launchServicePeersDiscovering(CallBackMethod a_callbackMethod)
	{
		debugTrace("WifiDirectManager::launchServicePeersDiscovering -using callback-");

		// 1. Check if we have launched peers discovering launching service before
		if ( requestForServicePeersDiscoveringAlreadyLaunched )
		{
			debugTrace("WifiDirectManager::launchServicePeersDiscovering ==> request for service peers discovering already Launched. Do not do anything and return ");
			return;
		}
		else
		{
			requestForServicePeersDiscoveringAlreadyLaunched = true;
		}

		// Set callbackmethod cmOnGettingsPeers defined in WifiDirectFacade. The callback recover the devices found
		_cmPeerDiscovered = a_callbackMethod;

		// 2. Launch service to discover peers
		debugTrace("WifiDirectManager::launchServicePeersDiscovering ==> launch native discoverPeers function");
		_manager.discoverPeers(_channel, new WifiP2pManager.ActionListener()
		{
			// 2.1. Callback if peers discovering sucessfully launched
			@Override
			public void onSuccess()
			{
				debugTrace("WifiDirectManager::launchServicePeerDiscovering ==> success on ");
				requestForServicePeersDiscoveringAlreadyLaunched = false;

				//After discovering, reconnect only if we have been previously connected, i.e. lastPeerName is not empty
			//	if ( lastPeerName != ""  )
			//	{
					if( autoReconnect )
					{
						debugTrace("WifiDirectManager::launchServicePeersDiscovering ==> Launch reconnectToPeer ");
						_timeBeforeTryingNewConnection = SHORT_DELAY;
						reconnectToPeer();
					}
			//	}
			//	else
			//	{
			//		debugTrace("WifiDirectManager::launchServicePeersDiscovering ==> has discovered peers, but does not do anything since it was not connected before ==> lastPeerName is empty");
			//	}


			}

			// 2.2 Callback if error occurred during service discovering
			@Override
			public void onFailure(int iReasonCode)
			{
				String text = "WifiDirectManager::launchServicePeersDiscovering ==> Discover peers: Fail to launch service peersDiscovering because ";
				switch (iReasonCode)
				{
					case 0:
						text += "of internal error : Probably the socket of the other tablet is closed.";
						//_timeBeforeTryingNewConnection = MEDIUM_DELAY;
						break;
					case 1:
						text += "P2P is unsupported on this device";
						break;
					case 2:
						text += "the framework is busy and unable to service the request";
						//_timeBeforeTryingNewConnection = LONG_DELAY;
						break;
					case 3:
						text = "because no service resquests are added";
						//_timeBeforeTryingNewConnection = SHORT_DELAY;
						break;
					default:
						text = "of unknow error";
						//_timeBeforeTryingNewConnection = SHORT_DELAY;
						break;
				}

				debugTrace(text);
				requestForServicePeersDiscoveringAlreadyLaunched = false;

				if(autoReconnect)
				{
					_timeBeforeTryingNewConnection = SHORT_DELAY;
					debugTrace("WifiDirectManager::launchServicePeersDiscovering ==> Try to reconnect to peer after a failure of reconnection");
					reconnectToPeer();
				}
			}
		});
	}




	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void reconnectToPeer()
	{
		debugTrace("WifiDirectManager::reconnectToPeer : was connected to peer with name: " + lastPeerName);

		if (socket == null)
		{
			debugTrace("WifiDirectManager::reconnectToPeer ==> socket is null");
		}

		if (  socket.wasPreviouslyAttached() )
		{
			debugTrace("WifiDirectManager::reconnectToPeer ==> The socket was not previously attached,");
			//socket.attachToRemoteHost();
			Handler handler = new Handler();
			handler.postDelayed(new Runnable()
			{
				@Override
				public void run()
				{
					boolean bJustDiscover = false;

					if ( lastPeerName.equals(""))
					{
						debugTrace("WifiDirectManager::reconnectToPeer ==> Launch discoverAndConnect using only discover boolean" );
						bJustDiscover = true;
					}
					else
					{
						debugTrace("WifiDirectManager::reconnectToPeer ==> LaunchDiscoverAndConnect" );
						bJustDiscover = false;
					}

					discoverAndConnect( bJustDiscover );  //Technical note : 20/08/20 We make a discover and connect and not a connectToPeer(lastPeerName, _cmPeerConnected)
														 //                          because we have erased the deviceList at the disconnection
				}
			}, _timeBeforeTryingNewConnection);
		}
		else
		{
			debugTrace("WifiDirectManager::reconnectToPeer ==> The socket was not previously attached,");
		}


		/*
		 * Handler handler = new Handler();
		 * debugTrace();("Trying to reconnect to peer",
		 * WifiDirectManager.DEBUGGER_CHANNEL); handler.postDelayed(new
		 * Runnable() {
		 * 
		 * @Override public void run() { connectToPeer(lastPeerName); }
		 * 
		 * }, 15000);
		 */

	}

	public void connectToPeer(String peerName, CallBackMethod cmPeerConnected)
	{
		connectToPeer(peerName, cmPeerConnected, false);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void connectToPeer(String a_strPeerName, CallBackMethod a_cmPeerConnected, boolean a_bJustCheckAdress)
	{

		debugTrace("WifiDirectManager::connectToPeer ==> with PEER NAME = " + a_strPeerName );

		if ( a_strPeerName.equals("") || a_strPeerName.equals(null) )
		{
			debugTrace("***********************");
			debugTrace(" ATTENTION : will try to connect to a peer with no name");
			debugTrace("***********************");
		}

		if ( !bForceConnectionRequest && (_bRequestForConnectionAlreadyLaunched || GetNetworkInfo().isConnected() ))
		{
			if(GetNetworkInfo().isConnected())
			{
				debugTrace("WifiDirectManager::connectoToPeer ==> Network is already connected. Have you received appropriate message from broadcast received? RETURN");
				_connexionStatus.SetConnected();
			}
			else
			{
				debugTrace("WifiDirectManager::connectoToPeer ==> request for connection already launched. Please wait. RETURN");
			}
			return;
		}
		else
		{
			debugTrace("WifiDirectManager::connectoToPeer ==> launching request for connection to: " + a_strPeerName);
			_bRequestForConnectionAlreadyLaunched = true;
		}


		String strPeerDeviceAddress = _mapAddressNameAllDevices.get( a_strPeerName );

		if (strPeerDeviceAddress == null || lastPeerName.equals("") )
		{
			// The name isn't mapped with any address in the Address - Name Map.
			// This could mean the names haven't been properly transfered from
			// Java to C++ the first time, or there was a mistake while filling up the map.
			debugTrace("WifiDirectManager::connectToPeer ==> PeerName not found in the mapAddress.. somehow, we have erased it.. We relance reconnectToPeer to make discoverAndConnect..");
			_allDeviceList.clear();
			_mapAddressNameDevices.clear();
			_mapAddressNameAllDevices.clear();

			_bRequestForConnectionAlreadyLaunched = false;
			reconnectToPeer();
			return;
		}

		if (a_bJustCheckAdress)
		{
			debugTrace("WifiDirectManager::connectToPeer ==> Return after checking the address, since we have onlydiscovering boolean activated in the argument of the function");
			_bRequestForConnectionAlreadyLaunched = false;
			return;
		}

		_cmPeerConnected = a_cmPeerConnected;
		setPeerName(a_strPeerName);

		WifiP2pConfig config = new WifiP2pConfig();
		config.deviceAddress = strPeerDeviceAddress;
		config.wps.setup = WpsInfo.PBC;

		debugTrace("WifiDirectManager::connectToPeer ==> address of the peer: " + strPeerDeviceAddress);
		debugTrace("WifiDirectManager::connectToPeer ==> CONNECTING... ...normally, toast invitation should appear in the other tablet" );
		debugTrace("WidiDirectManager::connectToPeer ==> NetworkInfo is : " + GetNetworkInfo().getState().toString() + " ... we are just sending the connection.. wait");
		_manager.connect(_channel, config, new WifiP2pManager.ActionListener()
		{
			@Override
			public void onSuccess()
			{
				debugTrace("WifiDirectMannager::connectToPeer ==> sucess on connection request. Broadcast receiver must receive a connection message");

				_bRequestForConnectionAlreadyLaunched = false;
				_timeBeforeTryingNewConnection = LONG_DELAY;
				if ( autoReconnect )
				{
					//reconnectToPeer();
				}

				 // appear before the connection is established between the two
				// devices initialize sockets in
				  //WiFiDirectBroadcastReceiver.onConnect();
				 onConnect();
			}

			@Override
			public void onFailure(int reason)
			{
				String text = "WifiDirectManager::connectToPeer ==>  Fail to connect because ";
				_bRequestForConnectionAlreadyLaunched = false;
				_timeBeforeTryingNewConnection = SHORT_DELAY;
				switch (reason)
				{
					case 0:
						text += "of internal error";
						//turnOfandthenOffWifi( );

						_timeBeforeTryingNewConnection = LONG_DELAY;
						break;
					case 1:
						text += "P2P is unsupported on this device";
						break;
					case 2:
						text += "the framework is busy and unable to service the request";
						//if this happend, we should restart wifi direct
						//_timeBeforeTryingNewConnection = LONG_DELAY;
						_bRequestForConnectionAlreadyLaunched = true;
						break;
					case 3:
						text = "because no service resquests are added";
						break;
					default:
						text = "of unknow error";
						break;
				}
				debugTrace(text);

				// A disconnection message will be passed to the wifidirectmanager
				if (autoReconnect)
				{
					reconnectToPeer();
				}
			}
		});
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void launchServiceDnsSdInfo()
	{
		if (_bRequestForLaunchingServiceDnsSdInfoAlreadyLaunched)
		{
			debugTrace("request for launching service dnsSdInfo already launched. Please wait.");
			return;
		}
		else
		{
			_bRequestForLaunchingServiceDnsSdInfoAlreadyLaunched = true;
		}

		Map<String, String> record = new HashMap<String, String>();
		record.put(TXTRECORD_PROP_AVAILABLE, "visible");
		WifiP2pDnsSdServiceInfo service = WifiP2pDnsSdServiceInfo.newInstance(SERVICE_INSTANCE, SERVICE_REG_TYPE, record);
		_manager.addLocalService(_channel, service, new ActionListener()
		{
			@Override
			public void onSuccess()
			{
				debugTrace("success on launching service dnsSdInfo");
				_bRequestForLaunchingServiceDnsSdInfoAlreadyLaunched = false;
			}

			@Override
			public void onFailure(int error)
			{
				String text = "Fail to launch service dnsSdInfo because ";
				switch (error)
				{
					case 0:
						text += "of internal error";
						break;
					case 1:
						text += "P2P is unsupported on this device";
						break;
					case 2:
						text += "the framework is busy and unable to service the request";
						break;
					case 3:
						text = "because no service resquests are added";
						break;
					default:
						text = "of unknow error";
						break;
				}

				debugTrace(text);
				_bRequestForLaunchingServiceDnsSdInfoAlreadyLaunched = false;
				// reconnectToPeer();
			}
		});
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void launchServiceServicesDiscovering()
	{
		if (_bRequestForServiceServicesDiscoveringAlreadyLaunched)
		{
			debugTrace("request for service ServicesDiscovering already launched. Please wait.");
			return;
		}
		else
		{
			_bRequestForServiceServicesDiscoveringAlreadyLaunched = true;
		}

		_manager.discoverServices(_channel, new ActionListener()
		{
			@Override
			public void onSuccess()
			{
				debugTrace("success on launching service serviceDiscovering");
				_bRequestForServiceServicesDiscoveringAlreadyLaunched = false;
			}

			@Override
			public void onFailure(int arg0)
			{
				String text = "Fail to launch service serviceDiscovering because ";
				switch (arg0)
				{
					case 0:
						text += "of internal error";
						break;
					case 1:
						text += "P2P is unsupported on this device";
						break;
					case 2:
						text += "the framework is busy and unable to service the request";
						break;
					case 3:
						text = "because no service resquests are added";
						break;
					default:
						text = "of unknow error";
						break;
				}
				debugTrace(text);
				_bRequestForServiceServicesDiscoveringAlreadyLaunched = false;
			}
		});
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void launchServiceRequestPeers()
	{
	//	if( IsConnectedOrConnecting() )
	//	{
	//		debugTrace("WifiDirectManager::launchServiceRequestPeers ==> device is already connected. We ignore request for peers.");
	//		return;
	//	}

		debugTrace("WifiDirectManager::launchServiceRequestPeers ==> lastPeerName = " + lastPeerName );

		if ( requestForServiceRequestPeersAlreadyLaunched )
		{
			debugTrace("WifiDirectManager::launchServiceRequestPeers ==>  request for service requestPeers already launched. Please wait.");
			return;
		}
		else
		{
			requestForServiceRequestPeersAlreadyLaunched = true;
		}

		if (_manager != null)
		{
			_manager.requestPeers(_channel, new PeerListListener()
			{
				@Override
				public void onPeersAvailable(WifiP2pDeviceList peers)
				{
					debugTrace("WifiDirectManager::launchServiceRequestPeers ==> Peers available :" + peers.getDeviceList().size() );

					int previousSize = _deviceList.size();

					if ( enabledAutoRelanchingServiceDiscoverPeers && peers.getDeviceList().size() < previousSize || peers.getDeviceList().size() == 0)
					{
						debugTrace("WifiDirectManager::launchServiceRequestPeers ==> requestPeers service seems not stable. We try to relaunch it...");
						// before making new request, make sure this method can be call again
						requestForServiceRequestPeersAlreadyLaunched = false;
						launchServicePeersDiscovering( _cmPeerDiscovered );
					}
					else
					{
						_deviceList.clear();
						_mapAddressNameDevices.clear();
						debugTrace("WifiDirectManager::launchServiceRequestPeers ==> List of available peers : ");
						for (WifiP2pDevice device : peers.getDeviceList())
						{
							if (!_allDeviceList.contains(device.deviceName))
							{
								_allDeviceList.add(device.deviceName);
							}

							if (!_mapAddressNameAllDevices.containsKey(device))
							{
								_mapAddressNameAllDevices.put(device.deviceName, device.deviceAddress);
							}

							_deviceList.add(device.deviceName);
							_mapAddressNameDevices.put(device.deviceName, device.deviceAddress);
							debugTrace("WifiDirectManager::launchServiceRequestPeers ==> device name " + device.deviceName + " is mapped with " + device.deviceAddress);

							//debugTrace("WifiDirectManage::launchServicesRequestPeers ==> properties of the device " + device.primaryDeviceType + " " + device.secondaryDeviceType + " " +
							//device.toString() );

						}

						debugTrace("WifiDirectManager::launchServiceRequestPeers ==> there is " + _deviceList.size() + " peers available");

						// once the list is written
						requestForServiceRequestPeersAlreadyLaunched = false;

						// JniCppFacade.notify(JniCppFacade.DEVICE_LIST_CHANGE);

						// Launch call back method
						if (_cmPeerDiscovered != null)
						{
							_cmPeerDiscovered.Do();
							//_cmPeerDiscovered = null;
						}

					}

				}
			});
		}
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void setDnsSdResponseListeners()
	{
		/*
		 * Register listeners for DNS-SD services. These are callbacks invoked
		 * by the system when a service is actually discovered.
		 */
		_serviceAvailables = new ArrayList<DnsListenerServiceAvailableItem>();
		_txtRecordAvailables = new ArrayList<DnsListenerTxtRecordItem>();

		_manager.setDnsSdResponseListeners(_channel, new DnsSdServiceResponseListener()
		{
			@Override
			public void onDnsSdServiceAvailable(String instanceName, String registrationType, WifiP2pDevice srcDevice)
			{
				debugTrace("new service available : " + registrationType);
				_serviceAvailables.add(new DnsListenerServiceAvailableItem(instanceName, registrationType, srcDevice));
			}
		}, new DnsSdTxtRecordListener()
		{
			/**
			 * A new TXT record is available. Pick up the advertised
			 * buddy name.
			 */
			@Override
			public void onDnsSdTxtRecordAvailable(String fullDomainName, Map<String, String> record, WifiP2pDevice device)
			{
				debugTrace("new text record available : " + fullDomainName);
				_txtRecordAvailables.add(new DnsListenerTxtRecordItem(fullDomainName, record, device));
			}
		});
	}


	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void askToClearAllRequestsAndLocalServices()
	{
		askToClearAllLocalServices();
		askToClearAllServiceRequests();
	}

	private boolean requestForServiceCleearServiceAlreadyLaunched = false;

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void askToClearAllServiceRequests()
	{
		if (requestForServiceCleearServiceAlreadyLaunched)
		{
			debugTrace("WifiDirectManager::askToClearAllServicesRequest ==> request for service ClearService already launched. Please wait.");
			return;
		}
		else
		{
			debugTrace("WifiDirectManager::askToClearAllServicesRequest ==> start procedure");
			requestForServiceCleearServiceAlreadyLaunched = true;
		}

		_manager.clearServiceRequests(_channel, new ActionListener()
		{
			@Override
			public void onSuccess()
			{
				debugTrace("WifiDirectManager::askToClearAllServicesRequests ==> success on launching request to clear all service requests");
				requestForServiceCleearServiceAlreadyLaunched = false;
			}

			@Override
			public void onFailure(int arg0)
			{
				String text = "WifiDirectManager::askToClearAllServicesRequests ==> Fail to ask to clear all service requests because ";
				switch (arg0)
				{
					case 0:
						text += "of internal error";
						break;
					case 1:
						text += "P2P is unsupported on this device";
						break;
					case 2:
						text += "the framework is busy and unable to service the request";
						break;
					case 3:
						text = "because no service resquests are added";
						break;
					default:
						text = "of unknow error";
						break;
				}
				debugTrace(text);
				requestForServiceCleearServiceAlreadyLaunched = false;
			}
		});
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void askToClearAllLocalServices()
	{
		if (_bRequestForServiceClearLocalServicesAlreadyLaunched)
		{
			debugTrace("WifiDirectManager::askToClearAllLocalServices ==> request for service clearLocalServices alreadyLaunched. Please wait.");
			return;
		}
		else
		{
			debugTrace("WifiDirectManager::askToClearAllLocalServices ==> start procedure");
			_bRequestForServiceClearLocalServicesAlreadyLaunched = true;
		}

		_manager.clearLocalServices(_channel, new ActionListener()
		{
			@Override
			public void onSuccess()
			{
				debugTrace("WifiDirectManager::askToClearAllLocalServices ==> success on launching request to clear all local services");
				_bRequestForServiceClearLocalServicesAlreadyLaunched = false;
			}

			@Override
			public void onFailure(int arg0)
			{
				String text = "WifiDirectManager::askToClearAllLocalServices ==> Fail to ask to clear all local services because ";
				switch (arg0)
				{
					case 0:
						text += "of internal error";
						break;
					case 1:
						text += "P2P is unsupported on this device";
						break;
					case 2:
						text += "the framework is busy and unable to service the request";
						break;
					case 3:
						text = "because no service resquests are added";
						break;
					default:
						text = "of unknow error";
						break;
				}
				debugTrace(text);
				_bRequestForServiceClearLocalServicesAlreadyLaunched = false;
			}
		});
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	private String getPeerInfo()
	{
		debugTrace("WifiDirectManager::getPeerInfo ==> ATTENTION: this function seems to work only for the owner of the group");
		WifiP2pGroup group = _lastIntent.getParcelableExtra( WifiP2pManager.EXTRA_WIFI_P2P_GROUP );

		if(group != null)
		{
			Object[] clients = group.getClientList().toArray();

			debugTrace("WifiDirectManager::getPeerInfo ==> number of clients in the group " + clients.length );
			if(clients.length > 0)
			{
				String strPeerInfo = String.valueOf( clients[0] );
				return strPeerInfo;
			}
		}

		debugTrace("WifiDirectManager::getPeerInfo ==> group is null, so return null. No peer info");

		return "";
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	private String getPeerName( )
	{
		debugTrace("WifiDirectManager::getPeerName ==> ATTENTION : this function uses getPeerInfo that seems to work only for group owner");
		String strPeerInfo = getPeerInfo();

		//Early out
		if ( strPeerInfo.equals("") )
		{
			debugTrace("WifiDirectManager::getPeerName ==> Not peer information, so could not find peer name");
			return "";
		}


		String[] strSeparatedInfos = strPeerInfo.split(":");
		String strSeparated = strSeparatedInfos[1]; //normally we obtain : playerName \n DeviceAdress so we divide it again
		String strPeerName = strSeparated.split("\n")[0];

		debugTrace("***************************************");
		debugTrace("*** WifiDirectManager::getPeerName  ==> Peer name is " + strPeerName );
		debugTrace("***************************************");

		return strPeerName;
	}


	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void launchServiceDnsRequest()
	{
		if (_bRequestForAddingDnsRequestAlreadyLaunched)
		{
			debugTrace("request for adding DnsRequest already launched. Please wait.");
			return;
		}
		else
		{
			_bRequestForAddingDnsRequestAlreadyLaunched = true;
		}

		// After attaching listeners, create a service request and initiate
		// discovery.
		WifiP2pDnsSdServiceRequest serviceRequest = WifiP2pDnsSdServiceRequest.newInstance();
		_manager.addServiceRequest(_channel, serviceRequest, new ActionListener()
		{
			@Override
			public void onSuccess()
			{
				debugTrace("success on launching service dnsRequest");
				_bRequestForAddingDnsRequestAlreadyLaunched = false;
			}

			@Override
			public void onFailure(int arg0)
			{
				String text = "Fail to launch service dnsRequest ";
				switch (arg0)
				{
					case 0:
						text += "of internal error";
						break;
					case 1:
						text += "P2P is unsupported on this device";
						break;
					case 2:
						text += "the framework is busy and unable to service the request";
						break;
					case 3:
						text = "because no service resquests are added";
						break;
					default:
						text = "of unknow error";
						break;
				}
				debugTrace(text);
				_bRequestForAddingDnsRequestAlreadyLaunched = false;
			}
		});
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void searchAvailableServices()
	{
		setDnsSdResponseListeners();
		launchServiceDnsRequest();
		launchServiceServicesDiscovering();
	}

	//------------------------------------------------------------------------------------------------------------------
	//																							used after connecting
	//------------------------------------------------------------------------------------------------------------------
	public void startRegistrationAndDiscovery()
	{
		launchServiceDnsSdInfo();
		searchAvailableServices();
	}

	private NetworkInfo GetNetworkInfo()
	{
		if(_lastIntent == null)
			return null;
		return (NetworkInfo) _lastIntent.getParcelableExtra( WifiP2pManager.EXTRA_NETWORK_INFO );
	}


	public boolean IsConnected( )
	{
		NetworkInfo networkInfo = GetNetworkInfo();
		if(networkInfo == null)
			return false;
		return networkInfo.isConnected();
	}

	public boolean IsConnectedOrConnecting( )
	{
		NetworkInfo networkInfo = GetNetworkInfo();
		if(networkInfo == null)
			return false;
		return networkInfo.isConnectedOrConnecting();
	}

	private void debugTrace(String a_strMsg)
	{
		DebugManager.print(a_strMsg, DEBUGGER_CHANNEL);
	}


	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	private boolean _bWasConnected = false;
	void onConnectionChanged(Intent intent)
	{
		debugTrace("WifiDirectManager::onConnectionChanged");
		if(intent != null)
		{
			_lastIntent = intent;
		}
		else
		{
			debugTrace("WifiDirectManager::OnConnectionChanged ==> intent is null !  return");
		}

		_bRequestForConnectionAlreadyLaunched = false;

		// Respond to new connection or disconnections
		if ( _manager == null)
		{
			return;
		}

		NetworkInfo networkInfo = GetNetworkInfo();

		if ( IsConnected()  && !_bWasConnected )   // if me removed _bWasConneted, another client/server will be created each time the WifiReceiver will receive a ON_CONNEXION_CHANGED
												   // (that may happens when adbswitching display on/off)*/
		{
			_bWasConnected = true;
			debugTrace("*********************************************************");
			debugTrace("************* C O N N E C T I O N ***********************");
			debugTrace("*********************************************************");

			onConnect();
		}
		else if ( networkInfo.isConnectedOrConnecting() )
		{
			if( !IsConnected() )
			{
				debugTrace("*********************************************************");
				debugTrace("****  D E V I C E S  I N  C O N N E C T I O N ***********");
				debugTrace("*********************************************************");
				OnConnectedOrConnecting();
			}
		}
		else
		{
			if ( _bWasConnected )
			{
				NetworkInfo.DetailedState detailedState = networkInfo.getDetailedState();

				if ( detailedState == NetworkInfo.DetailedState.DISCONNECTED  )
				{
					_bWasConnected = false;
					debugTrace("*********************************************************");
					debugTrace("***  D E V I C E S        D I S C O N N E C T E D    ****");
					debugTrace("*********************************************************");

					if ( _bWifiDirectEnable )
					{
						debugTrace("WifiDirectManager::OnConnectionChanged ==>  Devices disconnected but wifiDirect enabled ");
					}
					else
					{
						debugTrace( "WifiDirectManager::OnConnectionChanged ==>  Devices disconnected & wifiDirect disabled");
					}


					onDisconnect( networkInfo );
				}
				else if (detailedState == NetworkInfo.DetailedState.DISCONNECTING )
				{
					debugTrace("WifiDirectManager::OnConnectionChanged ==> don't do anything... in process of disconnection.. wait" );
				}
				else
				{
					debugTrace("WifiDirectManager::OnConnectionChanged ==> other connection state"); //TODO: treat other states
				}

			}
			else
			{
				debugTrace("WifiDirectManager::OnConnectionChanged ==> Devices were not connected before.");

				_bWasConnected = false;
				debugTrace("*********************************************************");
				debugTrace("***  D E V I C E S   N O T   C O N N E C T E D       ****");
				debugTrace("***  (probably fist connection time)                 ****");
				debugTrace("*********************************************************");

				onDisconnect( networkInfo );
			}




		}

	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void setPeerName(String a_strName)
	{
		if (  a_strName.equals("Pair") )
		{
			debugTrace("WifiDirectManager::Setting Tries to assign " + a_strName + " so, do not assign and return");
			return;
		}

		debugTrace("WifiDirectManager::Setting peer name only if not done before with name:"  + a_strName);
		if ( lastPeerName.equals("") || lastPeerName == null  )
		{
			lastPeerName = a_strName;
		}
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	private boolean _bPendingUserActionOfDiscoverPeers;
	public void SetUserRequestToDiscoverPeers(boolean a_bPendingActionStatus )
	{
		_bPendingUserActionOfDiscoverPeers = a_bPendingActionStatus;
	}

	public boolean IsThereAPendingUserActionToDiscoverPeers( )
	{
		return _bPendingUserActionOfDiscoverPeers;
	}


	public void SetCallbackForDiscoverPeers( CallBackMethod a_callbackForDiscoveringPeers )
	{
		_cmPeerDiscovered = a_callbackForDiscoveringPeers;
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void discoverAndConnect()
	{
		discoverAndConnect(false);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void discoverAndConnect( boolean a_bJustDiscover )
	{
		debugTrace("WifiDirectManager::discoverAndConnect");
		if ( a_bJustDiscover )
		{
			debugTrace("WifiDirectManager::discoverAndConnect ==> do only the discover");
		}
		else
		{
			debugTrace("WifiDirectManager::discoverAndConnect ==> do discover and connect");
		}

		if ( socket.isDettachedFromRemoteHost() )
		{
			debugTrace("WifiDirectManager::discoverAndConnect ==> passed isDettachedFromRemoteHost condition");

			if (_allDeviceList == null || _allDeviceList.size() == 0)
			{
				debugTrace("WifiDirectManager::discoverAndConnect ==> Peers are not discovered yet meaning EMPTY device list. Execute launchServicePeersDiscovering with a callbackmethod");

				this.launchServicePeersDiscovering(
						new CallBackMethod()
						{
							@Override
							public void Do(Object... vars)
							{
								discoverAndConnect();
							}
						}
				);
			}
			else
			{
				debugTrace("WifiDirectManager::discoverAndConnect ==> Device list not emtpy. Launching connectToPeer:" + lastPeerName);

				if( !IsConnectedOrConnecting() )
				{
					debugTrace("WifiDirectManager::discoverAndConnect ==> Show orange window.... IsConnectedOrConnecting =false");
					_connexionStatus.SetPending(this);
				}

				String strPeerName = lastPeerName;
				if ( lastPeerName.equals("") )
				{
					strPeerName = _deviceList.get(0);

					debugTrace("WifiDirectManager::discoverAndConnect ==> deviceList.get(0) gives pair of name " + strPeerName + "  but we DO NOT LAUNCH RECONNECTION since lastPeerName = nothing, meaning that the other tablet send the request" );
					return;
				}


				connectToPeer( strPeerName, null, a_bJustDiscover);  //TODO: potential bug here. We might try to connect to somebody else if other tablet is our discovering


			}
		}
		else
		{
			debugTrace("WifiDirectManager::discoverAndConnect: socket attached to host... doing nothing ");
		}
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void send(final File f)
	{
		discoverAndConnect();
		socket.send(f);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void send(final byte[] bytes)
	{
		discoverAndConnect();
		socket.send(bytes);
	}

	public void send(final double d)
	{
		discoverAndConnect();
		socket.send(d);
	}

	public void send(final long l)
	{
		discoverAndConnect();
		socket.send(l);
	}

	public void send(final float f)
	{
		discoverAndConnect();
		socket.send(f);
	}

	public void send(final char c)
	{
		discoverAndConnect();
		socket.send(c);
	}

	public void send(final byte b)
	{
		discoverAndConnect();
		socket.send(b);
	}

	public void send(final int i)
	{
		discoverAndConnect();
		socket.send(i);
	}

	public void send(final boolean b)
	{
		discoverAndConnect();
		socket.send(b);
	}

	public void send(final String s)
	{
		discoverAndConnect();
		socket.send(s);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public static void registerCallBackReceiver(CallBackMethod onReceiveString, CallBackMethod onReceiveInt, CallBackMethod onReceiveBool, CallBackMethod onReceiveFloat, CallBackMethod onReceiveDouble, CallBackMethod onReceiveByte, CallBackMethod onReceiveLong, CallBackMethod onReceiveFile, CallBackMethod onReceiveByteArray, CallBackMethod onReceiveChar)
	{
		SocketHandler.registerCallBackReceiver(onReceiveString, onReceiveInt, onReceiveBool, onReceiveFloat, onReceiveDouble, onReceiveByte, onReceiveLong, onReceiveFile, onReceiveByteArray, onReceiveChar);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public String getPeerMacAddress()
	{
		return _mapAddressNameAllDevices.get(lastPeerName);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void askToRemoveGroup()
	{
		debugTrace("WifiDirectManager::askToRemoveGroup");
		turnOffWifi(); //Technical note : We need to turn off the wifi, otherwise, the tablet might not be visible.
					   //                 For some reasons, the devices does not found themselves because they do not
		               //                 appear in the list.
		               //				  We also noted that turning on the wifi might take 1 sec approx. which is long,
		              // 	              and next steps might fail. (e.g. it will put "pair" instead of the name of the discovered devices)

		try {

				java.lang.reflect.Method[] methods = WifiP2pManager.class.getMethods();

				int iNumberOfDeletedGroups = 0;
				for (int i = 0; i < methods.length; i++)
				{
					if (methods[i].getName().equals("deletePersistentGroup"))
					{
						// Delete any persistent group
						for (int netid = 0; netid < 32; netid++)
						{
							methods[i].invoke(_manager, _channel, netid, null);
							iNumberOfDeletedGroups++;
						}
					}
				}

				debugTrace("WifiDirectManager::askToRemoveGroup ==> success on removing group. Number of deleted groups= " + iNumberOfDeletedGroups);

			}
			catch(Exception e)
			{
				debugTrace("WifiDirectManager::askToRemoveGroup ==> error when removing group " + e);
			}


			turnOnWifi();   //Technical note : As we said in previous technical note, we need to turn off the
							// 					wifi, otherwise sometimes, the tablet is not visible.

			if ( _wifiManager.isWifiEnabled() )
			{
				debugTrace("WifiDirectManager::askRemoveGroup ==> Wifi is Enabled");
			}
			else
			{
				debugTrace("WifiDirectManager::askRemoveGroup ==> Wifi is not Enabled");
			}
	}


	//------------------------------------------------------------------------------------------------------------------
	//																						Disconnect owner
	//------------------------------------------------------------------------------------------------------------------
	public void disconnect()
	{
		debugTrace("WififDirectManager::disconnect Try to disconnect only if manager exist");

		if ( _manager != null && _channel != null)
		{
			_manager.requestGroupInfo( _channel, new WifiP2pManager.GroupInfoListener()
			{
				@Override
				public void onGroupInfoAvailable(WifiP2pGroup group)
				{
					if (group != null && _manager != null && _channel != null && group.isGroupOwner())
					{
						_manager.removeGroup( _channel, new ActionListener()
						{
							@Override
							public void onSuccess()
							{
								debugTrace("WifiDirectManager::disconnect ==> removeGroup onSuccess -");
							}

							@Override
							public void onFailure(int reason)
							{
								debugTrace("WifiDirectManager::disconnect ==> removeGroup onFailure -");
							}
						});
					}
					else
					{
						debugTrace("WifiDirectManager::disconnect ==> failure to disconnect because group is null or not an group owner");
					}
				}
			});
		}
		else
		{
			debugTrace("WifiDirectManager::disconnect Disconnect failure due to manager or channel nulls");
		}
	}




	//------------------------------------------------------------------------------------------------------------------
	//																								onConnect callback
	//------------------------------------------------------------------------------------------------------------------
	private void onConnect()
	{
		/*debugTrace();("peer mac = " + peerMacAddress);
		String myIpv6Address = SocketHandler.getMyIpAddress(peerMacAddress);
		String peerIpv6Address = SocketHandler.getPeerIpAddress(peerMacAddress);//SocketHandler.generateIpv6FromMacAddress(peerMacAddress, true);
		
		debugTrace();("listenning on " + myIpv6Address);
		debugTrace();("connecting to " + lastPeerName + " "+ peerIpv6Address);
		debugTrace();(SocketHandler.getAnIpAddresForThisDevice());
		
		socket.listen(LISTENNING_PORT, myIpv6Address);
		socket.connectTo(peerIpv6Address);*/
		if(_connexionStatus != null)
			_connexionStatus.Enable(true);


		_manager.requestConnectionInfo(_channel, new ConnectionInfoListener()
		{
			@Override
			public void onConnectionInfoAvailable(WifiP2pInfo info)
			{
				String ownerAddress = "";

				if (info.groupOwnerAddress != null)
				{
					ownerAddress = info.groupOwnerAddress.getHostAddress();
				}

				String strPeerInfo = getPeerInfo();
				String strPeerName = getPeerName();
				debugTrace("WifiDirectManager::OnConnect ==> Try to recover info of the peer. The info recovered is : " + strPeerInfo );
				debugTrace("WifiDirectManager::OnConnect ==> Try to recover name of the peer. The name recovered is : " + strPeerName +"***********" );
				debugTrace("WifiDirectManager::OnConnect ==> owner address is = " + ownerAddress);
				//SocketHandler.printAllNetworkInterfaceName();

				String myLocalAddress = SocketHandler.getIPAddress(true);

				if (myLocalAddress.equals(""))
				{
					myLocalAddress = SocketHandler.getIPAddress(false);
				}

				///Assign peer name
				if (  lastPeerName.equals("") )
				{
					if ( !strPeerName.equals("") )
					{
						lastPeerName = strPeerName;
						debugTrace("WifiDirectManager::OnConnect ==> ASSIGN NAME TO PEER");
					}
					else
					{
						debugTrace("**************************************************************");
						debugTrace("********* WARNING : ***************************************************");
						debugTrace("WifiDirectManager::OnConnect ==> COULD NOT ASSIGN NAME TO PEER");
						debugTrace("**************************************************************");

					}
				}
				else
				{
					debugTrace("WifiDirectManager::OnConnect ==> NO NEED TO ASSIGN NAME TO PEER, it is already done -probably because this tablet send the invitation to the other tablet. Name was :" + lastPeerName );
				}


				if (!myLocalAddress.equals(ownerAddress) && info.groupFormed)
				{
					debugTrace("*********************************************************");
					debugTrace("WifiDirectManager::OnConnect => I am NOT THE GROUP OWNER");
					debugTrace("*********************************************************");

					//SocketHandler.getAnIpAddresForThisDevice();
					debugTrace("WifiDirectManager::OnConnect =>  My IP Address is " + myLocalAddress);
					//debugTrace();("GroupOwner host name is " + info.groupOwnerAddress.getHostName());
					//create local server
					socket.listen(LISTENNING_PORT, myLocalAddress);
					//connect to owner to give your server's ip

					try
					{
						socket.connectTo(ownerAddress);
						_connexionStatus.SetConnected();
					}
					catch(Exception e)
					{
						debugTrace("WifiDirectManager::OnConnect => Couldn't connect because: " + e);
					}

				}
				else if (info.groupFormed)
				{
					debugTrace("*********************************************************");
					debugTrace("WifiDirectManager::OnConnect => I am the GROUP OWNER");
					debugTrace("*********************************************************");

					debugTrace("WifiDirectManager::OnConnect =>  My IP Address is " + myLocalAddress);

					//lastPeerName = getPeerInfo().getChars();

					//create local server
					socket.listen(LISTENNING_PORT, myLocalAddress);
					//and wait for receiving pair'ip address
					_connexionStatus.SetConnected();
				}

				_cmPeerConnected = null;
			}
		});
	}


	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public String get_strThisDeviceName()
	{
		return _strThisDeviceName.equals("") ? BluetoothAdapter.getDefaultAdapter().getName() : _strThisDeviceName;
	}

	private void printMessage(String message)
	{
		Toast toast = Toast.makeText(_activity.getApplicationContext(), message, Toast.LENGTH_LONG);
		toast.setGravity(Gravity.CENTER, 0, 0);
		toast.show();
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	private void OnConnectedOrConnecting( )
	{
		debugTrace("WifiDirectManager::OnConnectedOrConnecting ==> the process is post-delayed in order to wait the connection finish to be set.");
		AppActivity.postDelay(new Runnable()
		{
			@Override
			public void run()
			{
				onConnectionChanged( null );
			}
		}, SHORT_DELAY);
		return;
	}

	//------------------------------------------------------------------------------------------------------------------
	//																								onConnect callback
	//------------------------------------------------------------------------------------------------------------------
	private void onDisconnect(NetworkInfo a_rNetworkInfo)
	{
		debugTrace("WifiDirectManager::onDisconnect ***********************");
		_connexionStatus.SetDisConnected();

		socket.notifyIsDisconnectedFromNetwork();
		debugTrace("WifiDirectManager::onDisconnect ==> Device has been disconnected from network");

		_allDeviceList.clear();
		_mapAddressNameAllDevices.clear();
		_mapAddressNameDevices.clear();
		debugTrace("WifiDirectManager::onDisconnect ==> _allDeviceList, and maps have just been erased due to deconnection. "); //we erased so we can discover them again;
		debugTrace("***WifiDirectManager::onDisconnect ==> EXTRA INFOS due to non connection: ");

		if ( a_rNetworkInfo.isAvailable() )
		{
			debugTrace("WifiDirectManager::onDisconnect ==> NETWORK AVAILABLE on disconnection ==> Network connectivity might be possible");
		}
		else
		{
			debugTrace("WifiDirectManager::onDisconnect ==> NETWORK NOT AVAILABLE for one of these reasons");
			debugTrace("a) Device is out of the coverage area for any networks of this type");
			debugTrace("b) Device is on a network other than home network (i.e. roaming) and data roaming has been disabled");
			debugTrace("c) Airplane mode is enable");
		}

		if (a_rNetworkInfo.isConnected() )
		{
			debugTrace("WifiDirectManager::onDisconnect ==> DEVICE CONNECTED on disconnection ?? ==> Network connectivity might be possible");
		}
		else
		{
			debugTrace("WifiDirectManager::onDisconnect ==> DEVICE IS NOT CONNECTED. Not possible to establish connections and pass data");
		}

		if (a_rNetworkInfo.isConnectedOrConnecting())
		{
			debugTrace("*********************************************************");
			debugTrace("************* ON CONNECTING  AGAIN***********************");
			debugTrace("*********************************************************");
			debugTrace("Network connecting. onConnectionChanged is post-delayed in order to wait the connection finish to be set.");
			AppActivity.postDelay(new Runnable()
			{
				@Override
				public void run()
				{
					onConnectionChanged(_lastIntent);
				}
			}, SHORT_DELAY);
			return;

		}
		else
		{
			debugTrace("WifiDirectManager::onDisconnect ==> tablets are not connected. ConnectedOrConnecting=false");
		}

		if (a_rNetworkInfo.isFailover())
		{
			debugTrace("WifiDirectManager::onDisconnect ==> network is failover: current attempt to connect to the network resulted from the ConnectivityManager trying to fail over to this network following a disconnect from another network. " + a_rNetworkInfo.isFailover());
		}
		else
		{
			debugTrace("WifiDirectManager::onDisconnect => network not failing over ");
		}

		if (a_rNetworkInfo.isRoaming())
		{
			debugTrace("WifiDirectManager::onDisconnect ==>  network is currently roaming "); // Indicates whether the device is currently roaming on this network.
		}


		if (lastPeerName.equals("") )
		{
			debugTrace("WifiDirectManager::OnDisconnect => lastPeer is not assigned ..."); // Indicates whether the device is currently roaming on this network.
			//return;
		}
		else
		{
			debugTrace("WifiDirectManager::OnDisconnect => lastPeer already assigned (and stored). NAME: " + lastPeerName); // Indicates whether the device is currently roaming on this network.
		}

		if ( autoReconnect )
		{
			_timeBeforeTryingNewConnection = SHORT_DELAY;
			reconnectToPeer();
		}

	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public Activity getActivity()
	{
		return _activity;
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public Channel getChannel()
	{
		return _channel;
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public WifiP2pManager getManager()
	{
		return _manager;
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public String getFormatedMap()
	{
		String res = "";

		Iterator<Entry<String, String>> it = _mapAddressNameDevices.entrySet().iterator();

		while (it.hasNext())
		{
			Entry<String, String> entry = it.next();
			res += entry.getKey() + "/" + entry.getValue() + "\n";
		}

		return res;
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public List<String> getDeviceList()
	{
		debugTrace("WifiDirectManager::GetDeviceList" );
		return _deviceList;
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public Map<String, String> getMapAddressNameDevices()
	{
		return _mapAddressNameDevices;
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public Map<String, String> getMapAddressNameAllDevices()
	{
		return _mapAddressNameAllDevices;
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public List<String> getAllDeviceList()
	{
		return _allDeviceList;
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public List<DnsListenerServiceAvailableItem> getAvailableServices()
	{
		return _serviceAvailables;
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public List<DnsListenerTxtRecordItem> getAvailableTxtRecords()
	{
		return _txtRecordAvailables;
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void set_strThisDeviceName(String _strThisDeviceName)
	{
		this._strThisDeviceName = _strThisDeviceName;
	}
	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public String get_strGetThisDeviceName( )
	{
		return this._strThisDeviceName;
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public static void setServerTempFileName(String name)
	{
		SocketHandler.setServerTempFileName(name);
	}


}
