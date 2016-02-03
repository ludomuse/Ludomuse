package org.cocos2dx.cpp.wifiDirect;

import android.net.wifi.p2p.WifiP2pDevice;

public class DnsListenerServiceAvailableItem {

	private String instanceName;
	private String registrationType;
	private WifiP2pDevice srcDevice;

	public DnsListenerServiceAvailableItem(String instanceName,
			String registrationType, WifiP2pDevice srcDevice)
	{
		super();
		this.instanceName = instanceName;
		this.registrationType = registrationType;
		this.srcDevice = srcDevice;
	}

	public String getInstanceName()
	{
		return instanceName;
	}

	public String getRegistrationType()
	{
		return registrationType;
	}

	public WifiP2pDevice getSrcDevice()
	{
		return srcDevice;
	}

}
