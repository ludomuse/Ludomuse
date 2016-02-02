package org.cocos2dx.cpp.wifiDirect;

import java.util.Map;

import android.net.wifi.p2p.WifiP2pDevice;

public class DnsListenerTxtRecordItem {

	private String fullDomainName;
	private Map<String, String> record;
	private WifiP2pDevice device;

	public DnsListenerTxtRecordItem(String fullDomainName,
			Map<String, String> record, WifiP2pDevice device)
	{
		super();
		this.fullDomainName = fullDomainName;
		this.record = record;
		this.device = device;
	}

	public String getFullDomainName()
	{
		return fullDomainName;
	}

	public Map<String, String> getRecord()
	{
		return record;
	}

	public WifiP2pDevice getDevice()
	{
		return device;
	}

}
