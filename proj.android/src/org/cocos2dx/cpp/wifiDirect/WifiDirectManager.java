package org.cocos2dx.cpp.wifiDirect;

import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import android.os.SystemClock;
import org.cocos2dx.cpp.DebugManager;
import org.cocos2dx.cpp.sockets.CallBackMethod;
import org.cocos2dx.cpp.sockets.SocketHandler;

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

public class WifiDirectManager {

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

	private Map<String, String> _mapAddressNameDevices = new HashMap<String, String>();
	private List<String> _deviceList = new ArrayList<String>();

	private Map<String, String> _mapAddressNameAllDevices = new HashMap<String, String>();
	private List<String> _allDeviceList = new ArrayList<String>();

	private List<DnsListenerServiceAvailableItem> _serviceAvailables;
	private List<DnsListenerTxtRecordItem> _txtRecordAvailables;

	public static final int DEBUGGER_CHANNEL = 3;

	public WifiDirectManager(Activity activity)
	{
		_activity = activity;
	}

	public void initialize()
	{
		initDebugger();
		initSocket();
		initManager();
	}

	public void pause()
	{
		_activity.unregisterReceiver(_receiver);
	}

	public void resume()
	{
		_activity.registerReceiver(_receiver, _intentFilter);
	}

	private void initSocket()
	{
		socket = new SocketHandler(1024, LISTENNING_PORT);
	}

	private void initManager()
	{
		_wifiManager = (WifiManager) _activity
				.getSystemService(Context.WIFI_SERVICE);
		// _wifiManager.setWifiEnabled(enabled)

		_manager = (WifiP2pManager) _activity
				.getSystemService(Context.WIFI_P2P_SERVICE);

		_channel = _manager.initialize(_activity, _activity.getMainLooper(),
				null);
		_receiver = new WiFiDirectBroadcastReceiver(this);

		// IntentFilter mIntentFilter;
		_intentFilter = new IntentFilter();
		_intentFilter.addAction(WifiP2pManager.WIFI_P2P_STATE_CHANGED_ACTION);
		_intentFilter.addAction(WifiP2pManager.WIFI_P2P_PEERS_CHANGED_ACTION);
		_intentFilter
				.addAction(WifiP2pManager.WIFI_P2P_CONNECTION_CHANGED_ACTION);
		_intentFilter
				.addAction(WifiP2pManager.WIFI_P2P_THIS_DEVICE_CHANGED_ACTION);
		DebugManager.print("WifiDirectManager started !", DEBUGGER_CHANNEL);
		_lock = _wifiManager.createWifiLock(WifiManager.WIFI_MODE_FULL_HIGH_PERF, "ludomuse");
		_lock.acquire();
	}

	private void initDebugger()
	{
		DebugManager.activity = _activity;

		DebugManager.AddDebugButton("ClearLog", new OnClickListener() {

			@Override
			public void onClick(View arg0)
			{
				DebugManager.clear();

			}
		});

		DebugManager.AddDebugButton("Scan", new OnClickListener() {

			@Override
			public void onClick(View arg0)
			{
				launchServicePeersDiscovering(null);
			}
		});

		DebugManager.AddDebugButton("RegisterAndDiscoverService",
				new OnClickListener() {

					@Override
					public void onClick(View arg0)
					{
						startRegistrationAndDiscovery();
					}
				});

		DebugManager.AddDebugButton("SendString", new OnClickListener() {

			@Override
			public void onClick(View arg0)
			{
				send("this is a test");
			}
		});

		DebugManager.AddDebugButton("SendFile", new OnClickListener() {

			@SuppressLint("SdCardPath")
			@Override
			public void onClick(View arg0)
			{
				File f = new File("/sdcard/Screenshots/img.jpg");
				DebugManager.print(" file exits ? " + f.exists(),
						WifiDirectManager.DEBUGGER_CHANNEL);
				send(f);
			}
		});

		DebugManager.AddDebugButton("SendLong", new OnClickListener() {

			@Override
			public void onClick(View arg0)
			{
				send(123456789l);
			}
		});

		DebugManager.AddDebugButton("Clear", new OnClickListener() {

			@Override
			public void onClick(View arg0)
			{
				clear();
			}
		});

		DebugManager.AddDebugButton("SwitchWifi", new OnClickListener() {

			@Override
			public void onClick(View arg0)
			{
				switchWifi();
			}
		});
	}

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

	public void clear()
	{
		_lock.release();
		stopHandlers();
		askToRemoveGroup();
		socket.stop();
		turnOffWifi();
		turnOnWifi();
		askToClearAllRequestsAndLocalServices();
		DebugManager.clear();
	}

	public void stopHandlers()
	{
		socket.stopHandlers();
	}

	public void turnOnWifi()
	{
		_wifiManager.setWifiEnabled(true);
		DebugManager.print("Wifi is on !", WifiDirectManager.DEBUGGER_CHANNEL);
	}

	public void turnOffWifi()
	{
		_wifiManager.setWifiEnabled(false);
		DebugManager.print("Wifi is off !", WifiDirectManager.DEBUGGER_CHANNEL);
	}

	public boolean isWifiEnabled()
	{
		return _wifiManager.isWifiEnabled();
	}

	private CallBackMethod _cmPeerDiscovered;

	private boolean requestForServicePeersDiscoveringAlreadyLaunched = false;

	public void launchServicePeersDiscovering(CallBackMethod cm)
	{
		if (requestForServicePeersDiscoveringAlreadyLaunched)
		{
			/*DebugManager
					.print("request for service peers discovering already Launched. Please wait.",
							DEBUGGER_CHANNEL);
			return;*/
			DebugManager
					.print("request for service peers discovering already Launched. Re-launch ",
							DEBUGGER_CHANNEL);
		}
		else
		{
			requestForServicePeersDiscoveringAlreadyLaunched = true;
		}

		_cmPeerDiscovered = cm;

		_manager.discoverPeers(_channel, new WifiP2pManager.ActionListener() {
			@Override
			public void onSuccess()
			{
				DebugManager.print(
						"success on launching service peersDiscovering",
						DEBUGGER_CHANNEL);
				requestForServicePeersDiscoveringAlreadyLaunched = false;
			}

			@Override
			public void onFailure(int reasonCode)
			{
				String text = "Fail to launch service peersDiscovering because ";
				switch (reasonCode)
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
				DebugManager.print(text, DEBUGGER_CHANNEL);
				requestForServicePeersDiscoveringAlreadyLaunched = false;
			}
		});
	}

	private String lastPeerName = "";

	public void reconnectToPeer()
	{
		if(lastPeerName != null && !lastPeerName.equals("") && socket.wasPreviouslyAttached())
		{
			//socket.attachToRemoteHost();
			
			Handler handler = new Handler();
			handler.postDelayed(new Runnable(){

				@Override
				public void run()
				{
					connectToPeer(lastPeerName, _cmPeerConnected);
					
				}}, 2000);
			
		}

		/*
		 * Handler handler = new Handler();
		 * DebugManager.print("Trying to reconnect to peer",
		 * WifiDirectManager.DEBUGGER_CHANNEL); handler.postDelayed(new
		 * Runnable() {
		 * 
		 * @Override public void run() { connectToPeer(lastPeerName); }
		 * 
		 * }, 15000);
		 */

	}

	private CallBackMethod _cmPeerConnected;

	private boolean requestForConnectionAlreadyLaunched = false;

	public boolean forceConnectionRequest = true;

	public void connectToPeer(String peerName, CallBackMethod cmPeerConnected)
	{
		if (!forceConnectionRequest && requestForConnectionAlreadyLaunched)
		{
			DebugManager.print(
					"request for connection already launched. Please wait.",
					DEBUGGER_CHANNEL);
			return;
		}
		else
		{
			DebugManager.print("launching request for connection...",
					DEBUGGER_CHANNEL);
			requestForConnectionAlreadyLaunched = true;
		}

		// DebugManager.print("setting callback method",
		// WifiDirectManager.DEBUGGER_CHANNEL);
		_cmPeerConnected = cmPeerConnected;

		lastPeerName = peerName;
		String devAddress = _mapAddressNameAllDevices.get(peerName);

		if (devAddress == null)
		{
			// The name isn't mapped with any address in the Address - Name Map.
			// This could mean the names haven't been properly transfered from
			// Java to C++ the first time, or there was a mistake while filling
			// up
			// the map.
			return;
		}

		WifiP2pConfig config = new WifiP2pConfig();
		config.deviceAddress = devAddress;
		config.wps.setup = WpsInfo.PBC;

		_manager.connect(_channel, config, new WifiP2pManager.ActionListener() {

			@Override
			public void onSuccess()
			{
				DebugManager
						.print("sucess on connection request. Broadcast receiver must receive a connection message",
								DEBUGGER_CHANNEL);

				requestForConnectionAlreadyLaunched = false;
				/*
				 * appear before the connection is established between the two
				 * devices initialize sockets in
				 * WiFiDirectBroadcastReceiver.onConnect();
				 */
			}

			@Override
			public void onFailure(int reason)
			{
				String text = "Fail to connect because ";
				switch (reason)
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
				DebugManager.print(text, DEBUGGER_CHANNEL);
				requestForConnectionAlreadyLaunched = false;
				// A disconnection message will be passed to the
				// wifidirectmanager
				if(autoReconnect)
				{
					reconnectToPeer();
				}
			}
		});
	}

	private boolean requestForLaunchingServiceDnsSdInfoAlreadyLaunched = false;

	public void launchServiceDnsSdInfo()
	{
		if (requestForLaunchingServiceDnsSdInfoAlreadyLaunched)
		{
			DebugManager
					.print("request for launching service dnsSdInfo already launched. Please wait.",
							DEBUGGER_CHANNEL);
			return;
		}
		else
		{
			requestForLaunchingServiceDnsSdInfoAlreadyLaunched = true;
		}

		Map<String, String> record = new HashMap<String, String>();
		record.put(TXTRECORD_PROP_AVAILABLE, "visible");
		WifiP2pDnsSdServiceInfo service = WifiP2pDnsSdServiceInfo.newInstance(
				SERVICE_INSTANCE, SERVICE_REG_TYPE, record);
		_manager.addLocalService(_channel, service, new ActionListener() {
			@Override
			public void onSuccess()
			{
				DebugManager.print("success on launching service dnsSdInfo",
						DEBUGGER_CHANNEL);
				requestForLaunchingServiceDnsSdInfoAlreadyLaunched = false;
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

				DebugManager.print(text, DEBUGGER_CHANNEL);
				requestForLaunchingServiceDnsSdInfoAlreadyLaunched = false;
				// reconnectToPeer();
			}
		});
	}

	private boolean requestForServiceServicesDiscoveringAlreadyLaunched = false;

	public void launchServiceServicesDiscovering()
	{
		if (requestForServiceServicesDiscoveringAlreadyLaunched)
		{
			DebugManager
					.print("request for service ServicesDiscovering already launched. Please wait.",
							DEBUGGER_CHANNEL);
			return;
		}
		else
		{
			requestForServiceServicesDiscoveringAlreadyLaunched = true;
		}

		_manager.discoverServices(_channel, new ActionListener() {
			@Override
			public void onSuccess()
			{
				DebugManager.print(
						"success on launching service serviceDiscovering",
						DEBUGGER_CHANNEL);
				requestForServiceServicesDiscoveringAlreadyLaunched = false;
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
				DebugManager.print(text, DEBUGGER_CHANNEL);
				requestForServiceServicesDiscoveringAlreadyLaunched = false;
			}
		});
	}

	private boolean requestForServiceRequestPeersAlreadyLaunched = false;

	public boolean enabledAutoRelanchingServiceDiscoverPeers = false;

	public void launchServiceRequestPeers()
	{
		if (requestForServiceRequestPeersAlreadyLaunched)
		{
			DebugManager
					.print("request for service requestPeers already launched. Please wait.",
							DEBUGGER_CHANNEL);
			return;
		}
		else
		{
			requestForServiceRequestPeersAlreadyLaunched = true;
		}

		if (_manager != null)
		{
			_manager.requestPeers(_channel, new PeerListListener() {
				@Override
				public void onPeersAvailable(WifiP2pDeviceList peers)
				{
					int previousSize = _deviceList.size();

					if (enabledAutoRelanchingServiceDiscoverPeers
							&& peers.getDeviceList().size() < previousSize)
					{
						DebugManager
								.print("requestPeers service seems not stable. We try to relaunch it...",
										DEBUGGER_CHANNEL);
						// before making new request, make sure this method can
						// be call again
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
								_mapAddressNameAllDevices
										.put(device.deviceName,
												device.deviceAddress);
							}

							_deviceList.add(device.deviceName);
							_mapAddressNameDevices.put(device.deviceName,
									device.deviceAddress);

						}

						DebugManager.print("there is " + _deviceList.size()
								+ " peers available", DEBUGGER_CHANNEL);

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
							Log.d("debug","LudoMuse - debug - _cmPeerDiscovered");
						}

						/* for debug */
						for (String device : _allDeviceList)
						{
							DebugManager.RemoveDebugButton(device);
						}
						for (final String device : _deviceList)
						{
							DebugManager.AddDebugButton(device,
									new OnClickListener() {

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

	public void setDnsSdResponseListeners()
	{
		/*
		 * Register listeners for DNS-SD services. These are callbacks invoked
		 * by the system when a service is actually discovered.
		 */
		_serviceAvailables = new ArrayList<DnsListenerServiceAvailableItem>();
		_txtRecordAvailables = new ArrayList<DnsListenerTxtRecordItem>();

		_manager.setDnsSdResponseListeners(_channel,
				new DnsSdServiceResponseListener() {
					@Override
					public void onDnsSdServiceAvailable(String instanceName,
							String registrationType, WifiP2pDevice srcDevice)
					{
						DebugManager.print("new service available : "
								+ registrationType, DEBUGGER_CHANNEL);
						_serviceAvailables
								.add(new DnsListenerServiceAvailableItem(
										instanceName, registrationType,
										srcDevice));
					}
				}, new DnsSdTxtRecordListener() {
					/**
					 * A new TXT record is available. Pick up the advertised
					 * buddy name.
					 */
					@Override
					public void onDnsSdTxtRecordAvailable(
							String fullDomainName, Map<String, String> record,
							WifiP2pDevice device)
					{
						DebugManager.print("new text record available : "
								+ fullDomainName, DEBUGGER_CHANNEL);
						_txtRecordAvailables.add(new DnsListenerTxtRecordItem(
								fullDomainName, record, device));
					}
				});
	}

	public void askToClearAllRequestsAndLocalServices()
	{
		askToClearAllLocalServices();
		askToClearAllServiceRequests();
	}

	private boolean requestForServiceCleearServiceAlreadyLaunched = false;

	public void askToClearAllServiceRequests()
	{
		if (requestForServiceCleearServiceAlreadyLaunched)
		{
			DebugManager
					.print("request for service ClearService already launched. Please wait.",
							DEBUGGER_CHANNEL);
			return;
		}
		else
		{
			requestForServiceCleearServiceAlreadyLaunched = true;
		}

		_manager.clearServiceRequests(_channel, new ActionListener() {
			@Override
			public void onSuccess()
			{
				DebugManager
						.print("success on launching request to clear all service requests",
								DEBUGGER_CHANNEL);
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
				DebugManager.print(text, DEBUGGER_CHANNEL);
				requestForServiceCleearServiceAlreadyLaunched = false;
			}
		});
	}

	private boolean requestForServiceClearLocalServicesAlreadyLaunched = false;

	public void askToClearAllLocalServices()
	{
		if (requestForServiceClearLocalServicesAlreadyLaunched)
		{
			DebugManager
					.print("request for service clearLocalServices alreadyLaunched. Please wait.",
							DEBUGGER_CHANNEL);
			return;
		}
		else
		{
			requestForServiceClearLocalServicesAlreadyLaunched = true;
		}

		_manager.clearLocalServices(_channel, new ActionListener() {
			@Override
			public void onSuccess()
			{
				DebugManager
						.print("success on launching request to clear all local services",
								DEBUGGER_CHANNEL);
				requestForServiceClearLocalServicesAlreadyLaunched = false;
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
				DebugManager.print(text, DEBUGGER_CHANNEL);
				requestForServiceClearLocalServicesAlreadyLaunched = false;
			}
		});
	}

	private boolean requestForAddingDnsRequestAlreadyLaunched = false;

	public void launchServiceDnsRequest()
	{
		if (requestForAddingDnsRequestAlreadyLaunched)
		{
			DebugManager
					.print("request for adding DnsRequest already launched. Please wait.",
							DEBUGGER_CHANNEL);
			return;
		}
		else
		{
			requestForAddingDnsRequestAlreadyLaunched = true;
		}

		// After attaching listeners, create a service request and initiate
		// discovery.
		WifiP2pDnsSdServiceRequest serviceRequest = WifiP2pDnsSdServiceRequest
				.newInstance();
		_manager.addServiceRequest(_channel, serviceRequest,
				new ActionListener() {
					@Override
					public void onSuccess()
					{
						DebugManager.print(
								"success on launching service dnsRequest",
								DEBUGGER_CHANNEL);
						requestForAddingDnsRequestAlreadyLaunched = false;
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
						DebugManager.print(text, DEBUGGER_CHANNEL);
						requestForAddingDnsRequestAlreadyLaunched = false;
					}
				});
	}

	public void searchAvailableServices()
	{
		setDnsSdResponseListeners();
		launchServiceDnsRequest();
		launchServiceServicesDiscovering();
	}

	/**
	 * After connecting
	 */
	public void startRegistrationAndDiscovery()
	{
		launchServiceDnsSdInfo();
		searchAvailableServices();
	}

	void onConnectionChanged(Intent intent)
	{

		requestForConnectionAlreadyLaunched = false;
		// Respond to new connection or disconnections
		if (_manager == null)
		{
			return;
		}

		NetworkInfo networkInfo = (NetworkInfo) intent
				.getParcelableExtra(WifiP2pManager.EXTRA_NETWORK_INFO);

		if (networkInfo.isConnected())
		{
			onConnect();
		}
		else
		{
			onDisconnect(networkInfo);
		}

	}

	private SocketHandler socket;

	// public final static int LISTENNING_PORT_OWNER = 666;
	// public final static int LISTENNING_PORT_OTHER = 777;
	public final static int LISTENNING_PORT = 40001;

	public void setPeerName(String name)
	{
		lastPeerName = name;
	}

	public void discoverAndConnect()
	{
		if (socket.isDettachedFromRemoteHost())
		{
			if (_allDeviceList == null || _allDeviceList.size() == 0)
			{
				DebugManager
						.print("Peers are not discovered yet. Launching required service",
								WifiDirectManager.DEBUGGER_CHANNEL);
				this.launchServicePeersDiscovering(new CallBackMethod() {

					@Override
					public void Do(Object... vars)
					{
						discoverAndConnect();
					}

				});
			}
			else
			{
				DebugManager.print(
						"Device is not connected. Launching required service",
						WifiDirectManager.DEBUGGER_CHANNEL);
				this.connectToPeer(lastPeerName.equals("") ? _deviceList.get(0)
						: lastPeerName, null);
			}
		}
	}



	public void send(final File f)
	{
		discoverAndConnect();
		socket.send(f);
	}

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

	public static void registerCallBackReceiver(CallBackMethod onReceiveString,
			CallBackMethod onReceiveInt, CallBackMethod onReceiveBool,
			CallBackMethod onReceiveFloat, CallBackMethod onReceiveDouble,
			CallBackMethod onReceiveByte, CallBackMethod onReceiveLong,
			CallBackMethod onReceiveFile, CallBackMethod onReceiveByteArray,
			CallBackMethod onReceiveChar)
	{
		SocketHandler.registerCallBackReceiver(onReceiveString, onReceiveInt,
				onReceiveBool, onReceiveFloat, onReceiveDouble, onReceiveByte,
				onReceiveLong, onReceiveFile, onReceiveByteArray,
				onReceiveChar);
	}

	public String getPeerMacAddress()
	{
		return _mapAddressNameAllDevices.get(lastPeerName);
	}
	
	public void askToRemoveGroup()
	{
		_manager.removeGroup(_channel, new ActionListener() {
			@Override
			public void onSuccess()
			{
				DebugManager.print(
						"success on launching service RemoveGroup",
						DEBUGGER_CHANNEL);
				requestForAddingDnsRequestAlreadyLaunched = false;
			}

			@Override
			public void onFailure(int arg0)
			{
				String text = "Fail to launch service RemoveGroup ";
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
				DebugManager.print(text, DEBUGGER_CHANNEL);
				requestForAddingDnsRequestAlreadyLaunched = false;
			}
		});
	}
	
	private void onConnect()
	{
		/*DebugManager.print("peer mac = " + peerMacAddress, DEBUGGER_CHANNEL);
		String myIpv6Address = SocketHandler.getMyIpAddress(peerMacAddress);
		String peerIpv6Address = SocketHandler.getPeerIpAddress(peerMacAddress);//SocketHandler.generateIpv6FromMacAddress(peerMacAddress, true);
		
		DebugManager.print("listenning on " + myIpv6Address, DEBUGGER_CHANNEL);
		DebugManager.print("connecting to " + lastPeerName + " "+ peerIpv6Address, DEBUGGER_CHANNEL);
		DebugManager.print(SocketHandler.getAnIpAddresForThisDevice(), WifiDirectManager.DEBUGGER_CHANNEL);
		
		socket.listen(LISTENNING_PORT, myIpv6Address);
		socket.connectTo(peerIpv6Address);*/
		
		_manager.requestConnectionInfo(_channel, new ConnectionInfoListener() {
			@Override
			public void onConnectionInfoAvailable(WifiP2pInfo info)
			{
				String ownerAddress = info.groupOwnerAddress.getHostAddress();
				DebugManager.print("owner address is = " + ownerAddress, WifiDirectManager.DEBUGGER_CHANNEL);
				DebugManager.print("<font color='red'>owner sdk ? </font>" + info.isGroupOwner, DEBUGGER_CHANNEL);
				SocketHandler.printAllNetworkInterfaceName();
				String myLocalAddress = SocketHandler.getIPAddress(true);
				if(myLocalAddress.equals("")) myLocalAddress = SocketHandler.getIPAddress(false);
				
				if (!myLocalAddress.equals(ownerAddress) && info.groupFormed)
				{
					DebugManager.print("I am not the group owner", DEBUGGER_CHANNEL);
					//SocketHandler.getAnIpAddresForThisDevice();
					DebugManager.print("My IP Address is " + myLocalAddress, DEBUGGER_CHANNEL);
					//create local server
					socket.listen(LISTENNING_PORT, myLocalAddress);
					//connect to owner to give your server's ip
					socket.connectTo(ownerAddress);

				}
				else if(info.groupFormed)
				{
					DebugManager.print("I am the group owner", DEBUGGER_CHANNEL);
					DebugManager.print("My IP Address is " + myLocalAddress, DEBUGGER_CHANNEL);
					//create local server
					socket.listen(LISTENNING_PORT, myLocalAddress);
					//and wait for receiving pair'ip address
					DebugManager.print("I am the group owner", DEBUGGER_CHANNEL);
				}

				
				_cmPeerConnected = null;
			}
		});
	}

	public boolean autoReconnect = true;

	
	public String getThisDeviceName()
	{
		return thisDeviceName.equals("") ? BluetoothAdapter.getDefaultAdapter().getName() : thisDeviceName;
	}
	
	private void onDisconnect(NetworkInfo ni)
	{
		socket.notifyIsDisconnectedFromNetwork();
		//socket.stop();

		DebugManager
				.print("device is not connect to network", DEBUGGER_CHANNEL);
		DebugManager.print("Extra infos : ", DEBUGGER_CHANNEL);
		DebugManager.print("-> network available = " + ni.isAvailable(),
				DEBUGGER_CHANNEL);
		DebugManager.print(
				"-> network connected or connecting = "
						+ ni.isConnectedOrConnecting(), DEBUGGER_CHANNEL);
		DebugManager.print("-> network is fail over = " + ni.isFailover(),
				DEBUGGER_CHANNEL);
		DebugManager.print("-> network is roaming = " + ni.isRoaming(),
				DEBUGGER_CHANNEL);
		if (autoReconnect)
			reconnectToPeer();
	}

	public Activity getActivity()
	{
		return _activity;
	}

	public Channel getChannel()
	{
		return _channel;
	}

	public WifiP2pManager getManager()
	{
		return _manager;
	}

	public String getFormatedMap()
	{
		String res = "";

		Iterator<Entry<String, String>> it = _mapAddressNameDevices.entrySet()
				.iterator();

		while (it.hasNext())
		{
			Entry<String, String> entry = it.next();
			res += entry.getKey() + "/" + entry.getValue() + "\n";
		}

		return res;
	}

	public List<String> getDeviceList()
	{
		return _deviceList;
	}

	public Map<String, String> getMapAddressNameDevices()
	{
		return _mapAddressNameDevices;
	}

	public Map<String, String> getMapAddressNameAllDevices()
	{
		return _mapAddressNameAllDevices;
	}

	public List<String> getAllDeviceList()
	{
		return _allDeviceList;
	}

	public List<DnsListenerServiceAvailableItem> getAvailableServices()
	{
		return _serviceAvailables;
	}

	public List<DnsListenerTxtRecordItem> getAvailableTxtRecords()
	{
		return _txtRecordAvailables;
	}

	private String thisDeviceName = "";
	
	public void setThisDeviceName(String thisDeviceName)
	{
		this.thisDeviceName = thisDeviceName;
	}
	
	public static void setServerTempFileName(String name)
	{
		SocketHandler.setServerTempFileName(name);
	}

}
