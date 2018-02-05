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
		_wifiManager = (WifiManager) _activity.getSystemService(Context.WIFI_SERVICE);
		// _wifiManager.setWifiEnabled(enabled)

		debugTrace("Initialize wifi direct manager");
		_manager = (WifiP2pManager) _activity.getSystemService(Context.WIFI_P2P_SERVICE);

		_channel = _manager.initialize(_activity, _activity.getMainLooper(), null);

		askToRemoveGroup();

		_receiver = new WiFiDirectBroadcastReceiver(this);



		// IntentFilter mIntentFilter;
		_intentFilter = new IntentFilter();
		_intentFilter.addAction(WifiP2pManager.WIFI_P2P_STATE_CHANGED_ACTION);
		_intentFilter.addAction(WifiP2pManager.WIFI_P2P_PEERS_CHANGED_ACTION);
		_intentFilter.addAction(WifiP2pManager.WIFI_P2P_CONNECTION_CHANGED_ACTION);
		_intentFilter.addAction(WifiP2pManager.WIFI_P2P_THIS_DEVICE_CHANGED_ACTION);
		debugTrace("WifiDirectManager started !");

		_lock = _wifiManager.createWifiLock(WifiManager.WIFI_MODE_FULL_HIGH_PERF, "ludomuse");
		_lock.acquire();


		//printMessage("Initiliazed !");
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	private void initDebugger()
	{
		DebugManager.activity = _activity;

		DebugManager.AddDebugButton("ClearLog", new OnClickListener()
		{
			@Override
			public void onClick(View arg0)
			{
				DebugManager.clear();

			}
		});

		DebugManager.AddDebugButton("Scan", new OnClickListener()
		{
			@Override
			public void onClick(View arg0)
			{
				launchServicePeersDiscovering(null);
			}
		});

		DebugManager.AddDebugButton("RegisterAndDiscoverService", new OnClickListener()
		{
			@Override
			public void onClick(View arg0)
			{
				startRegistrationAndDiscovery();
			}
		});

		DebugManager.AddDebugButton("SendString", new OnClickListener()
		{
			@Override
			public void onClick(View arg0)
			{
				send("this is a test");
			}
		});

		DebugManager.AddDebugButton("SendFile", new OnClickListener()
		{
			@SuppressLint("SdCardPath")
			@Override
			public void onClick(View arg0)
			{
				File f = new File("/sdcard/Screenshots/img.jpg");
				debugTrace(" file exits ? " + f.exists());
				send(f);
			}
		});

		DebugManager.AddDebugButton("SendLong", new OnClickListener()
		{
			@Override
			public void onClick(View arg0)
			{
				send(123456789l);
			}
		});

		DebugManager.AddDebugButton("Clear", new OnClickListener()
		{

			@Override
			public void onClick(View arg0)
			{
				clear();
			}
		});

		DebugManager.AddDebugButton("SwitchWifi", new OnClickListener()
		{

			@Override
			public void onClick(View arg0)
			{
				switchWifi();
			}
		});
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
	public void clear()
	{
		_lock.release();
		stopHandlers();
		//askToRemoveGroup();
		socket.stop();
		//turnOffWifi();
		//turnOnWifi();
		askToClearAllRequestsAndLocalServices();
		DebugManager.clear();

		debugTrace("Clear all !");
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
		debugTrace("Wifi is on !");
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void turnOffWifi()
	{
		_wifiManager.setWifiEnabled(false);
		debugTrace("Wifi is off !");
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
		if (_cmPeerDiscovered == null)
		{
			debugTrace("Callback for peer discovering not assigned ");
		}

		// 2. Launch service to discover peers
		_manager.discoverPeers(_channel, new WifiP2pManager.ActionListener()
		{
			// 2.1. Callback if peers discovering sucessfully launched
			@Override
			public void onSuccess()
			{
				debugTrace("RELAUNCH => WifiDirectManager::servicePeersDiscovering::Discover peers: success on : Set requestForServicePeersDiscoveringAlreadyLaunched to false ");
				requestForServicePeersDiscoveringAlreadyLaunched = false;
			}

			// 2.2 Callback if error occurred during service discovering
			@Override
			public void onFailure(int iReasonCode)
			{
				String text = "Discover peers: Fail to RELAUNCH service discovering because ";
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
	public void launchServicePeersDiscovering(CallBackMethod a_callbackMethod)
	{


		// 1. Check if we have launched peers discoveringlaunching service peersDiscovering
		if (requestForServicePeersDiscoveringAlreadyLaunched)
		{
			debugTrace("request for service peers discovering already Launched. ");
		}
		else
		{
			requestForServicePeersDiscoveringAlreadyLaunched = true;
		}

		// Set callbackmethod cmOnGettingsPeers defined in WifiDirectFacade. The callback recover the devices found
		_cmPeerDiscovered = a_callbackMethod;

		// 2. Launch service to discover peers
		_manager.discoverPeers(_channel, new WifiP2pManager.ActionListener()
		{
			// 2.1. Callback if peers discovering sucessfully launched
			@Override
			public void onSuccess()
			{
				debugTrace("Discover peers: success on ");
				requestForServicePeersDiscoveringAlreadyLaunched = false;
				if(autoReconnect)
				{
					debugTrace("Waiting 30s to check if connexion status has changed");
					_timeBeforeTryingNewConnection = LONG_DELAY;
					reconnectToPeer();
				}
			}

			// 2.2 Callback if error occurred during service discovering
			@Override
			public void onFailure(int iReasonCode)
			{
				String text = "Discover peers: Fail to launch service peersDiscovering because ";
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
					debugTrace("Try to reconnect to peer");
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
			debugTrace("In wifiDirectManager::reconnectToPeer socket is null");
		}
		//Debug
		if (socket.wasPreviouslyAttached() == false)
		{
			debugTrace("In wifiDirectManager::reconnectToPeer. The socket was not previously attached,");
		}

		if ( /* lastPeerName != null && !lastPeerName.equals("") && */ socket.wasPreviouslyAttached())
		{
			//socket.attachToRemoteHost();
			Handler handler = new Handler();
			handler.postDelayed(new Runnable()
			{
				@Override
				public void run()
				{
					debugTrace("In wifiDirectManager::reconnectToPeer. DiscoverAndConnect ");
					discoverAndConnect(!(lastPeerName != null && !lastPeerName.equals("")));
					//connectToPeer(lastPeerName, _cmPeerConnected);
				}
			}, _timeBeforeTryingNewConnection);
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


		String devAddress = _mapAddressNameAllDevices.get( a_strPeerName );

		if (devAddress == null || lastPeerName.equals("") )
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
			debugTrace("Return after checking the address");
			_bRequestForConnectionAlreadyLaunched = false;
			return;
		}

		_cmPeerConnected = a_cmPeerConnected;
		setPeerName(a_strPeerName);

		String strAddres = devAddress;
		WifiP2pConfig config = new WifiP2pConfig();
		config.deviceAddress = devAddress;
		config.wps.setup = WpsInfo.PBC;

		debugTrace("WifiDirectManager::connectToPeer ==> address: " + strAddres);
		debugTrace("CONNECTING... ...normally, toast should be launched here");
		debugTrace("NetworkInfo is : " + GetNetworkInfo().getState().toString());
		_manager.connect(_channel, config, new WifiP2pManager.ActionListener()
		{
			@Override
			public void onSuccess()
			{
				debugTrace("WifiDirectMannager::connectToPeer: sucess on connection request. Broadcast receiver must receive a connection message");

				_bRequestForConnectionAlreadyLaunched = false;
				_timeBeforeTryingNewConnection = LONG_DELAY;
				if (autoReconnect)
				{
					reconnectToPeer();
				}
				/*
				 * appear before the connection is established between the two
				 * devices initialize sockets in
				 * WiFiDirectBroadcastReceiver.onConnect();
				 */
			}

			@Override
			public void onFailure(int reason)
			{
				String text = "WifiDirectManager::connectToPeer: Fail to connect because ";
				_bRequestForConnectionAlreadyLaunched = false;
				_timeBeforeTryingNewConnection = SHORT_DELAY;
				switch (reason)
				{
					case 0:
						text += "of internal error";
						//_timeBeforeTryingNewConnection = LONG_DELAY;
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
		/*if(IsConnectedOrConnecting()) {
			debugTrace("device is already connected. We ignore request for peers.");
			return;
		}*/

		debugTrace("lastPeerName = " + lastPeerName + " in launchServiceRequestPeers");

		if (requestForServiceRequestPeersAlreadyLaunched)
		{
			debugTrace("request for service requestPeers already launched. Please wait.");
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

					int previousSize = _deviceList.size();

					if (enabledAutoRelanchingServiceDiscoverPeers && peers.getDeviceList().size() < previousSize)
					{
						debugTrace("requestPeers service seems not stable. We try to relaunch it...");
						// before making new request, make sure this method can be call again
						requestForServiceRequestPeersAlreadyLaunched = false;
						launchServicePeersDiscovering(_cmPeerDiscovered);
					}
					else
					{
						_deviceList.clear();
						_mapAddressNameDevices.clear();
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
							debugTrace("launchServiceRequestPeers:: device " + device.deviceName + " is mapped with " + device.deviceAddress);
						}

						debugTrace("there is " + _deviceList.size() + " peers available");

						// once the list is written
						requestForServiceRequestPeersAlreadyLaunched = false;

						// JniCppFacade.notify(JniCppFacade.DEVICE_LIST_CHANGE);

						// Launch call back method
						if (_cmPeerDiscovered != null)
						{
							_cmPeerDiscovered.Do();
							//_cmPeerDiscovered = null;
						}
						else
						{
							Log.d("debug", "LudoMuse - debug - _cmPeerDiscovered");
						}

						/* for debug */
						for (String device : _allDeviceList)
						{
							DebugManager.RemoveDebugButton(device);
						}
						for (final String device : _deviceList)
						{
							DebugManager.AddDebugButton(device, new OnClickListener()
									{
										@Override
										public void onClick(View arg0)
										{
											connectToPeer(device, null);
										}

									});
						}
						/* end debug */

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
			debugTrace("request for service ClearService already launched. Please wait.");
			return;
		}
		else
		{
			requestForServiceCleearServiceAlreadyLaunched = true;
		}

		_manager.clearServiceRequests(_channel, new ActionListener()
		{
			@Override
			public void onSuccess()
			{
				debugTrace("success on launching request to clear all service requests");
				requestForServiceCleearServiceAlreadyLaunched = false;
			}

			@Override
			public void onFailure(int arg0)
			{
				String text = "Fail to ask to clear all service requests because ";
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
			debugTrace("request for service clearLocalServices alreadyLaunched. Please wait.");
			return;
		}
		else
		{
			_bRequestForServiceClearLocalServicesAlreadyLaunched = true;
		}

		_manager.clearLocalServices(_channel, new ActionListener()
		{
			@Override
			public void onSuccess()
			{
				debugTrace("success on launching request to clear all local services");
				_bRequestForServiceClearLocalServicesAlreadyLaunched = false;
			}

			@Override
			public void onFailure(int arg0)
			{
				String text = "Fail to ask to clear all local services because ";
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


	private String getPeerName()
	{
		WifiP2pGroup group = _lastIntent.getParcelableExtra( WifiP2pManager.EXTRA_WIFI_P2P_GROUP );
		if(group != null)
		{
			Object[] clients = group.getClientList().toArray();
			if(clients.length > 0)
			{
				return (String) clients[0];
			}
		}
		return "";
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
		if(intent != null)
			_lastIntent = intent;

		_bRequestForConnectionAlreadyLaunched = false;

		// Respond to new connection or disconnections
		if ( _manager == null)
		{
			return;
		}

		NetworkInfo networkInfo = GetNetworkInfo();

		if ( networkInfo.isConnected()  && !_bWasConnected )
		{
			_bWasConnected = true;
			debugTrace("*********************************************************");
			debugTrace("************* C O N N E C T I O N ***********************");
			debugTrace("*********************************************************");
			onConnect();
		}
		else if ( networkInfo.isConnectedOrConnecting() )
		{
			debugTrace("*********************************************************");
			debugTrace("****  D E V I C E S  I N  C O N N E C T I O N ***********");
			debugTrace("*********************************************************");
			OnConnectedOrConnecting( );
		}
		else
		{
			_bWasConnected = false;
			debugTrace("*********************************************************");
			debugTrace("***  D E V I C E S        D I S C O N N E C T E D    ****");
			debugTrace("*********************************************************");

			onDisconnect(networkInfo);
		}

	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void setPeerName(String name)
	{
		debugTrace("WifiDirectManager::Setting peer name " + name);
		if (lastPeerName == "" || lastPeerName == null)
		{
			lastPeerName = name;
		}
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
		if ( socket.isDettachedFromRemoteHost() )
		{
			debugTrace("Discover and connect: isDettachedFromRemoteHost condition");
			if (_allDeviceList == null || _allDeviceList.size() == 0)
			{
				debugTrace("Peers are not discovered yet meaning EMPTY device list. Do AGAIN discoverAndConnect ... until something in device list");

				this.launchServicePeersDiscovering(new CallBackMethod()
				{
					@Override
					public void Do(Object... vars)
					{
						discoverAndConnect();
					}

				});
			}
			else
			{
				if(!IsConnectedOrConnecting())
					_connexionStatus.SetPending(this);


				debugTrace("Device list not empy. Launching connectToPeer:" + lastPeerName);
				//todo : post delay there

				connectToPeer( lastPeerName.equals("") ? _deviceList.get(0) : lastPeerName, null, a_bJustDiscover);  //TODO: potential bug here. We might try to connect to somebody else if other tablet is our discovering

				if ( lastPeerName.equals("") )
				{
					debugTrace("WifiDirectManager::DiscoverAndConnect: lastPeerName is empty string..(device list as well).");
					// Do something...

					if ( a_bJustDiscover) debugTrace("WifiDirectManager::DiscoverAndConnect: but only doing discover");
					if ( a_bJustDiscover == false) debugTrace("WifiDirectManager::DiscoverAndConnect: and doing both proccess");

				}
			//	else
			//	{
			//		connectToPeer( lastPeerName, null, a_bJustDiscover);
			//	}


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
		turnOffWifi();

		try {

				java.lang.reflect.Method[] methods = WifiP2pManager.class.getMethods();
				for (int i = 0; i < methods.length; i++)
				{
					if (methods[i].getName().equals("deletePersistentGroup"))
					{
						// Delete any persistent group
						for (int netid = 0; netid < 32; netid++)
						{
							methods[i].invoke(_manager, _channel, netid, null);
						}
					}
				}
				debugTrace("success on removing group (askToRemoveGroup method)");

			}
			catch(Exception e)
			{
				debugTrace("error when removing group (askToRemoveGroup method) " + e);
			}
		turnOnWifi();


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

				debugTrace("owner address is = " + ownerAddress);
				//SocketHandler.printAllNetworkInterfaceName();
				String myLocalAddress = SocketHandler.getIPAddress(true);

				if (myLocalAddress.equals(""))
				{
					myLocalAddress = SocketHandler.getIPAddress(false);
				}

				if (!myLocalAddress.equals(ownerAddress) && info.groupFormed)
				{
					debugTrace("I am not the group owner");
					//SocketHandler.getAnIpAddresForThisDevice();
					debugTrace("My IP Address is " + myLocalAddress);
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
					debugTrace("I am the group owner");
					debugTrace("My IP Address is " + myLocalAddress);
					//debugTrace();("get peer name " + getPeerName() );
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
		debugTrace("Network connecting. onConnectionChanged is post-delayed in order to wait the connection finish to be set.");
		AppActivity.postDelay(new Runnable() {
			@Override
			public void run() {
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

		_connexionStatus.SetDisConnected();

		socket.notifyIsDisconnectedFromNetwork();
		_allDeviceList.clear();
		_mapAddressNameAllDevices.clear();
		_mapAddressNameDevices.clear();

		debugTrace("Device has been disconnected from network");
		debugTrace("*** EXTRA INFOS due to non connection: ");


		if ( a_rNetworkInfo.isAvailable() )
		{
			debugTrace("NETWORK AVAILABLE on disconnection ==> Network connectivity might be possible");
		}
		else
		{
			debugTrace("NETWORK NOT AVAILABLE for one of these reasons");
			debugTrace("a) Device is out of the coverage area for any networks of this type");
			debugTrace("b) Device is on a network other than home network (i.e. roaming) and data roaming has been disabled");
			debugTrace("c) Airplane mode is enable");
		}

		if (a_rNetworkInfo.isConnected())
		{
			debugTrace("DEVICE CONNECTED on disconnection ?? ==> Network connectivity might be possible");
		}
		else
		{
			debugTrace("DEVICE IS NOT CONNECTED. Not possible to establish connections and pass data");
		}

		if (a_rNetworkInfo.isConnectedOrConnecting())
		{
			debugTrace("*********************************************************");
			debugTrace("************* ON CONNECTING  AGAIN***********************");
			debugTrace("*********************************************************");
			debugTrace("Network connecting. onConnectionChanged is post-delayed in order to wait the connection finish to be set.");
			AppActivity.postDelay(new Runnable() {
				@Override
				public void run() {
					onConnectionChanged(_lastIntent);
				}
			}, SHORT_DELAY);
			return;

		}
		else
		{
			debugTrace("NETWORK NOT CONNECTED");
		}

		if (a_rNetworkInfo.isFailover())
		{
			debugTrace("-> network is failover: current attempt to connect to the network resulted from the ConnectivityManager trying to fail over to this network following a disconnect from another network. " + a_rNetworkInfo.isFailover());
		}
		else
		{
			debugTrace("Disconnection & not failing over ");
		}

		if (a_rNetworkInfo.isRoaming())
		{
			debugTrace("-> network is currently roaming "); // Indicates whether the device is currently roaming on this network.
		}


		if (lastPeerName == "")
		{
			debugTrace("WifiDirectManager::OnDisconnect => lastPeer is not assigned ..."); // Indicates whether the device is currently roaming on this network.
			//return;
		}
		else
		{
			debugTrace("WifiDirectManager::OnDisconnect => lastPeer assigned. NAME: " + lastPeerName); // Indicates whether the device is currently roaming on this network.
		}

		if (autoReconnect)
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
	public static void setServerTempFileName(String name)
	{
		SocketHandler.setServerTempFileName(name);
	}


}
