package org.cocos2dx.cpp.sockets;

import java.io.File;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.util.Collections;
import java.util.Enumeration;
import java.util.List;

import org.apache.http.conn.util.InetAddressUtils;
import org.cocos2dx.cpp.DebugManager;
import org.cocos2dx.cpp.wifiDirect.WifiDirectManager;

/**
 * Socket Handler is a wrapper class that allow to manager easily a TCP
 * connection, by exposing simple interface to access server and client method
 * on this device
 * 
 * @author Gregoire, IHMTEK
 * 
 */
public class SocketHandler {

	private ClientSocketHandler client;
	private ServerSocketHandler server;

	private int listenningPort;
	private MailBox mailBox;

	public SocketHandler(int bufferLen, int listenningPort)
	{
		client = new ClientSocketHandler(bufferLen, this);
		this.listenningPort = listenningPort;
		mailBox = new MailBox(client);
	}

	/**
	 * Register all methods you want to be call when server is receiving data
	 * 
	 * @param onReceiveString
	 * @param onReceiveInt
	 * @param onReceiveBool
	 * @param onReceiveFloat
	 * @param onReceiveDouble
	 * @param onReceiveByte
	 * @param onReceiveLong
	 * @param onReceiveFile
	 * @param onReceiveByteArray
	 * @param onReceiveChar
	 */
	public static void registerCallBackReceiver(CallBackMethod onReceiveString,
			CallBackMethod onReceiveInt, CallBackMethod onReceiveBool,
			CallBackMethod onReceiveFloat, CallBackMethod onReceiveDouble,
			CallBackMethod onReceiveByte, CallBackMethod onReceiveLong,
			CallBackMethod onReceiveFile, CallBackMethod onReceiveByteArray,
			CallBackMethod onReceiveChar)
	{
		ServerSocketHandler.registerCallBackReceiver(onReceiveString,
				onReceiveInt, onReceiveBool, onReceiveFloat, onReceiveDouble,
				onReceiveByte, onReceiveLong, onReceiveFile,
				onReceiveByteArray, onReceiveChar);
	}

	public boolean isConnected()
	{
		return client.isConnected();
	}

	/**
	 * ask server to stop listenning
	 */
	public void stop()
	{
		if (server != null)
			server.stop();
	}

	public boolean isDettachedFromRemoteHost()
	{
		return client.isDettachedFromRemoteHost();
	}

	public void dettachFromRemoteHost()
	{
		client.dettachFromRemoteHost();
	}
	
	public boolean wasPreviouslyAttached()
	{
		return client.wasPreviouslyAttached();
	}
	
	public void attachToRemoteHost()
	{
		client.attachToRemoteHost();
	}
	

	/**
	 * Stop server and client
	 */
	public void notifyIsDisconnectedFromNetwork()
	{
		//stop();
		//dettachFromRemoteHost();
	}

	/**
	 * Ask server to listen on port "port" and address "address"
	 * 
	 * @param port
	 * @param address
	 */
	public void listen(int port, String address)
	{
		server = new ServerSocketHandler(port, address, this);
		server.listen();
	}

	public void send(File f)
	{
		mailBox.post("send", f, File.class);
	}

	public void send(byte[] bytes)
	{
		mailBox.post("sendBytes", bytes, byte[].class);
	}

	public void send(double d)
	{
		mailBox.post("send", d, double.class);
	}

	public void send(long l)
	{
		mailBox.post("send", l, long.class);
	}

	public void send(float f)
	{
		mailBox.post("send", f, float.class);
	}

	public void send(byte b)
	{
		mailBox.post("send", b, byte.class);
	}

	public void send(char c)
	{
		mailBox.post("send", c, char.class);
	}

	public void send(int i)
	{
		mailBox.post("send", i, int.class);
	}

	public void send(boolean b)
	{
		mailBox.post("send", b, boolean.class);
	}

	public void send(String str)
	{
		mailBox.post("send", str, String.class);
	}

	private void sendIP()
	{
		mailBox.post("sendIP", listenningPort, int.class, 0);
	}

	public String getIP()
	{
		return server.getServerIpAddress();
	}

	public void sendAccuse(long idReceived)
	{

		if (mailBox.isEmpty())
		{
			/*
			 * Accuse id are the negatif of the id from the message they accuse.
			 * The reason that accuse do not generate id, is that another
			 * message can arrive and be send by the time this accuse is send
			 * (because it's asynchronous). Exemple : id = 1 maiBox.isEmpty
			 * ->msgArrive ->msgGenerateId (static id = id ++ = 2)
			 * ->AccuseGenerateID (static id = id ++ = 3) ->Accuse sent with
			 * previously generated id (3) ->msg sent with previously generated
			 * id (3) --> msg and accuse are send with same id, so one of them
			 * will be ignored (and for all time, because considered like a
			 * doublon)
			 */
			ClientSocketHandler.setId(-idReceived);
			/*
			 * In order to not mask other message, accuse are concatenate with
			 * next message. But, if there is no next message to be send ( =
			 * mailbox is empty and can stay empty) we will send an asynchronous
			 * accuse
			 */
			client.sendAsyncAccuse();
		}
		else
		{
			/*
			 * if mailbox is not empty, we need to concatenate accuse to the
			 * next sending (so send it in the same time that the next message)
			 * in order to not make interferate both message
			 */
			client.concatAccuseInNextSend = true;
			client.accuseId = -idReceived;
		}
	}

	public static void printAllNetworkInterfaceName()
	{
		try
		{
			List<NetworkInterface> interfaces = Collections
					.list(NetworkInterface.getNetworkInterfaces());
			for (NetworkInterface intf : interfaces)
			{
				List<InetAddress> addrs = Collections.list(intf
						.getInetAddresses());
				for (InetAddress addr : addrs)
				{
					String sAddr = addr.getHostAddress().toUpperCase();
					DebugManager.print("sAddr = " + sAddr,
							WifiDirectManager.DEBUGGER_CHANNEL);
				}
			}
		}
		catch (Exception ex)
		{
		} // for now eat exceptions
	}

	/**
	 * Get IP address from first non-localhost interface
	 * 
	 * @param ipv4
	 *            true=return ipv4, false=return ipv6
	 * @return address or empty string
	 */
	public static String getIPAddress(boolean useIPv4)
	{
		try
		{
			List<NetworkInterface> interfaces = Collections
					.list(NetworkInterface.getNetworkInterfaces());
			for (NetworkInterface intf : interfaces)
			{
				if (intf.getName().contains("p2p"))
				{
					List<InetAddress> addrs = Collections.list(intf
							.getInetAddresses());
					for (InetAddress addr : addrs)
					{
						if (!addr.isLoopbackAddress())
						{
							String sAddr = addr.getHostAddress().toUpperCase();
							boolean isIPv4 = InetAddressUtils
									.isIPv4Address(sAddr);
							if (useIPv4)
							{
								if (isIPv4)
									return sAddr;
							}
							else
							{
								if (!isIPv4)
								{
									int delim = sAddr.indexOf('%'); // drop ip6
																	// port
																	// suffix
									return delim < 0 ? sAddr : sAddr.substring(
											0, delim);
								}
							}
						}
					}
				}
			}
		}
		catch (Exception ex)
		{
		} // for now eat exceptions
		return "";
	}

	public static String getAnIpAddresForThisDevice()
	{
		return getStringLocalIPAddress();
		// return getDottedDecimalIP(getLocalIPAddress());
	}

	public static String getMyIpv6Address()
	{
		return SocketHandler.generateIpv6FromMacAddress(getMACAddress("wlan0"),
				false);
	}

	public static String getDistantIpv6Address(String peerMacAddress)
	{
		return SocketHandler.generateIpv6FromMacAddress(peerMacAddress, true);
	}

	private static String getMyIpv6AddressValue()
	{
		return getValue(getMyIpv6Address());
	}

	private static String getDistantIpv6AddressValue(String peerMacAddress)
	{
		return getValue(getDistantIpv6Address(peerMacAddress));
	}

	private static int compareTo(String i1, String i2)
	{
		int a = i1.compareTo(i2);
		int b = i2.compareTo(i1);
		return a > b ? 1 : b > a ? -1 : 0;
		// int i1Len = i1.length();
		// int i2Len = i2.length();
		// if(i1Len > i2Len)
		// return 1;
		// if(i2Len > i1Len)
		// return -1;
		// for(int i = 0; i < i1Len; i++)
		// {
		// int ci1 = Integer.parseInt(i1.charAt(i) + "");
		// int ci2 = Integer.parseInt(i2.charAt(i) + "");
		// if(ci1 > ci2)
		// return 1;
		// if(ci2 > ci1)
		// return -1;
		// }
		// return 0;
	}

	public static boolean isOwner(String peerMacAddress)
	{

		int res = compareTo(getMyIpv6AddressValue(),
				getDistantIpv6AddressValue(peerMacAddress));
		DebugManager.print("<font color='green'>" + res + "</font>",
				WifiDirectManager.DEBUGGER_CHANNEL);
		return res < 0;
	}

	public static String getMyIpAddress(String peerMacAddress)
	{
		if (!isOwner(peerMacAddress))
		{
			return "192.168.49.3";
		}
		else
		{
			return "192.168.49.2";
		}

	}

	private static String getValue(String s)
	{
		s.replace(":", "");
		String res = "";
		for (int i = 0; i < s.length(); i++)
		{
			res += (int) s.charAt(i);
		}
		DebugManager.print("s/v = " + s + " " + res,
				WifiDirectManager.DEBUGGER_CHANNEL);
		return res;
	}

	public static String getPeerIpAddress(String peerMacAddress)
	{
		DebugManager.print("peerMacAddres = " + peerMacAddress,
				WifiDirectManager.DEBUGGER_CHANNEL);
		if (!isOwner(peerMacAddress))
		{
			return "192.168.49.2";
		}
		else
		{
			return "192.168.49.3";
		}
	}

	public static String generateIpv6FromMacAddress(String macAddress,
			boolean distantAddress)
	{
		macAddress = macAddress.toLowerCase();
		String[] splitted = macAddress.split(":");

		// take the mac address: for example 52:74:f2:b1:a8:7f
		// throw ff:fe in the middle: 52:74:f2:ff:fe:b1:a8:7f
		// reformat to IPv6 notation 5274:f2ff:feb1:a87f
		// convert the first octet from hexadecimal to binary: 52 -> 01010010

		// invert the bit at index 6 (counting from 0): 01010010 -> 01010000

		// convert octet back to hexadecimal: 01010000 -> 50
		// replace first octet with newly calculated one: 5074:f2ff:feb1:a87f
		// prepend the link-local prefix: fe80::5074:f2ff:feb1:a87f

		if (distantAddress)
		{
			int first = Integer.parseInt(splitted[0], 16);

			// force I/G byte from mac address to localy
			// administred
			first = first == (first & 253) ? first | 2 : first & 253;

			splitted[0] = Integer.toHexString(first);
		}

		macAddress = splitted[0] + splitted[1] + ":" + splitted[2] + "ff:";
		macAddress += "fe" + splitted[3] + ":" + splitted[4] + splitted[5];
		String res = "fe80:" + macAddress + "%" + getNetworkScope("p2p");
		res = res.replace(":0000", ":");
		res = res.replace(":000", ":");
		res = res.replace(":00", ":");
		res = res.replace(":0", "");
		return res;

	}

	public static String generateIpv4FromMacAddress(String macAddress)
	{
		String[] splitted = macAddress.split(":");
		int third = Integer.parseInt(splitted[3], 16);
		int fourth = Integer.parseInt(splitted[4], 16);
		int fiveth = Integer.parseInt(splitted[5], 16);
		return "192." + third + "." + fourth + "." + fiveth;
	}

	final protected static char[] hexArray = "0123456789ABCDEF".toCharArray();



	/**
	 * Returns MAC address of the given interface name.
	 * 
	 * @param interfaceName
	 *            eth0, wlan0 or NULL=use first interface
	 * @return mac address or empty string
	 */
	public static String getMACAddress(String interfaceName)
	{
		try
		{
			List<NetworkInterface> interfaces = Collections
					.list(NetworkInterface.getNetworkInterfaces());
			for (NetworkInterface intf : interfaces)
			{
				if (interfaceName != null)
				{
					if (!intf.getName().equalsIgnoreCase(interfaceName))
						continue;
				}
				DebugManager.print(intf.getName(),
						WifiDirectManager.DEBUGGER_CHANNEL);
				byte[] mac = intf.getHardwareAddress();
				if (mac == null)
					return "";
				StringBuilder buf = new StringBuilder();
				for (int idx = 0; idx < mac.length; idx++)
					buf.append(String.format("%02X:", mac[idx]));
				if (buf.length() > 0)
					buf.deleteCharAt(buf.length() - 1);
				return buf.toString();
			}
		}
		catch (Exception ex)
		{
		} // for now eat exceptions
		return "";
	}

	private static String getNetworkScope(String containing)
	{
		try
		{
			String res = null;
			for (Enumeration<NetworkInterface> en = NetworkInterface
					.getNetworkInterfaces(); en.hasMoreElements();)
			{
				NetworkInterface intf = en.nextElement();
				if (intf.getName().contains(containing))
				{
					res = intf.getName();
					DebugManager.print(res, WifiDirectManager.DEBUGGER_CHANNEL);
				}
			}
			return res;
		}
		catch (Exception ex)
		{

		}
		return null;
	}

	private static String getStringLocalIPAddress()
	{
		try
		{
			for (Enumeration<NetworkInterface> en = NetworkInterface
					.getNetworkInterfaces(); en.hasMoreElements();)
			{
				NetworkInterface intf = en.nextElement();
				for (Enumeration<InetAddress> enumIpAddr = intf
						.getInetAddresses(); enumIpAddr.hasMoreElements();)
				{
					InetAddress inetAddress = enumIpAddr.nextElement();
					if (!inetAddress.isLoopbackAddress())
					{
						return inetAddress.getHostAddress();
					}
				}
			}
		}
		catch (Exception ex)
		{

		}
		return null;
	}

	// private static String getDottedDecimalIP(byte[] ipAddr)
	// {
	// // convert to dotted decimal notation:
	// String ipAddrStr = "";
	// for (int i = 0; i < ipAddr.length; i++)
	// {
	// if (i > 0)
	// {
	// ipAddrStr += ".";
	// }
	// ipAddrStr += ipAddr[i] & 0xFF;
	// }
	// return ipAddrStr;
	// }

	/**
	 * Set the client remote host = the address and port of the remote server
	 * which the client will try to contact
	 * 
	 * @param hostAddress
	 * @param listenningPort
	 */
	void setRemoteHost(String hostAddress, int listenningPort)
	{
		this.listenningPort = listenningPort;
		client.setRemoteHost(hostAddress, listenningPort);
	}

	public static void setServerTempFileName(String name)
	{
		ServerSocketHandler.setServerTempFileName(name);
	}
	
	public void connectTo(String hostAddress)
	{
		setRemoteHost(hostAddress, listenningPort);
		sendIP();
		send();
	}

	/**
	 * clean properly / terminate all thread and timer
	 */
	public void stopHandlers()
	{
		if (server != null)
			server.stopHandlers();
		if (client != null)
			client.stopHandlers();
		mailBox.stopHandlers();
	}

	/**
	 * send actual message in mailbox
	 */
	void send()
	{
		mailBox.send();
	}

	/**
	 * send next message in mailbox
	 */
	void sendNext()
	{
		mailBox.sendNext();
	}
}
