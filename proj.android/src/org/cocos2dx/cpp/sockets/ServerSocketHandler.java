package org.cocos2dx.cpp.sockets;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.SocketException;
import java.io.InputStream;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketAddress;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;

import org.cocos2dx.cpp.DebugManager;
import org.cocos2dx.cpp.wifiDirect.WifiDirectManager;

import android.annotation.SuppressLint;
import android.os.AsyncTask;
import android.os.Environment;

/**
 * An instance of this class is created when the server socket accept a
 * connection with a new client. This class concretely handle client message,
 * while the server socket wait for another connection.
 * 
 * @author Gregoire
 * 
 */
class Communication implements Runnable {
	private Socket client;
	private SocketHandler master;
	
	// Temp file name
	public static String tempFile = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES) + "/Screenshots/";

	// This array list contains the id of the message previously received
	private static ArrayList<Long> alreadyTreated = new ArrayList<Long>();
	// The id from the message being read
	private long id;

	// User can defien callback method that are called when receiving data
	private static CallBackMethod onReceiveString = null;
	private static CallBackMethod onReceiveInt = null;
	private static CallBackMethod onReceiveBool = null;
	private static CallBackMethod onReceiveFloat = null;
	private static CallBackMethod onReceiveDouble = null;
	private static CallBackMethod onReceiveByte = null;
	private static CallBackMethod onReceiveLong = null;
	private static CallBackMethod onReceiveFile = null;
	private static CallBackMethod onReceiveByteArray = null;
	private static CallBackMethod onReceiveChar = null;

	public Communication(Socket client, SocketHandler master)
	{
		super();
		this.client = client;
		this.master = master;
	}

	/**
	 * read a character into the inputstream 'is'
	 * 
	 * @param is
	 * @return
	 */
	private char readChar(InputStream is)
	{
		try
		{
			return (char) is.read();
		}
		catch (IOException e)
		{
			DebugManager.print(ServerSocketHandler.getTag()
					+ "error reading char in inputstream",
					WifiDirectManager.DEBUGGER_CHANNEL);
			return '#';
		}
	}

	/**
	 * read a string into the inputstream 'is'
	 * 
	 * @param is
	 * @return
	 */
	private String getStringFromInputStream(InputStream is)
	{

		String res = "";
		char tps;
		while ((tps = readChar(is)) != '\0')
		{
			res += tps;
		}
		return res;
	}

	/**
	 * read a float into the inputstream 'is'
	 * 
	 * @param is
	 * @return
	 */
	private Float getFloatFromInputStream(InputStream is)
	{
		byte[] bytes = new byte[] { (byte) read(is), (byte) read(is),
				(byte) read(is), (byte) read(is) };
		return ByteBuffer.wrap(bytes).getFloat();
	}

	/**
	 * read a double into the inputstream 'is'
	 * 
	 * @param is
	 * @return
	 */
	private Double getDoubleFromInputStream(InputStream is)
	{
		byte[] bytes = new byte[] { (byte) read(is), (byte) read(is),
				(byte) read(is), (byte) read(is), (byte) read(is),
				(byte) read(is), (byte) read(is), (byte) read(is) };
		return ByteBuffer.wrap(bytes).getDouble();
	}

	/**
	 * read a long into the inputstream 'is'
	 * 
	 * @param is
	 * @return
	 */
	private Long getLongFromInputStream(InputStream is)
	{
		byte[] bytes = new byte[Long.SIZE];
		for (int i = 0; i < Long.SIZE; i++)
		{
			bytes[i] = (byte) read(is);

		}
		return ByteBuffer.wrap(bytes).getLong();
	}

	/**
	 * read a byte array into the inputstream 'is'
	 * 
	 * @param is
	 * @return
	 */
	private byte[] getBytesFromInputStream(InputStream is)
	{
		ByteArrayOutputStream bos = new ByteArrayOutputStream();
		int tps;
		while ((tps = read(is)) != -1)
		{
			bos.write(tps);
		}

		return bos.toByteArray();
	}

	
	/**
	 * read a file into the inputstream 'is' The file will be created with the
	 * following path : "/sdcard/Screenshots/tps"
	 * 
	 * @param is
	 * @return
	 */
	private File getFileFromInputStream(InputStream is)
	{
	/*	File f = new File(tempFile);
		try
		{
			f.createNewFile();
		}
		catch (IOException e1)
		{
			DebugManager.print(ServerSocketHandler.getTag()
					+ "error while creating new file" + e1.getLocalizedMessage(),
					WifiDirectManager.DEBUGGER_CHANNEL);
		}*/

		BufferedOutputStream bos;
		try
		{
			bos = new BufferedOutputStream(new FileOutputStream(new File(tempFile)));
			int tps;
			while ((tps = read(is)) != -1)
			{
				bos.write(tps);
			}
			bos.close();
			return new File(tempFile);
		}
		catch (Exception e)
		{
			DebugManager.print(ServerSocketHandler.getTag()
					+ "Error while reading file. Default directory is: "
					+ tempFile + " " + e.getLocalizedMessage(),
					WifiDirectManager.DEBUGGER_CHANNEL);
			return null;
		}
	}

	/**
	 * 
	 * @param client
	 * @return client's inputstream
	 */
	private InputStream getClientData(Socket client)
	{
		InputStream is = null;
		try
		{
			is = client.getInputStream();
		}
		catch (IOException e)
		{
			DebugManager.print(ServerSocketHandler.getTag()
					+ "error while getting client data",
					WifiDirectManager.DEBUGGER_CHANNEL);
		}
		return is;
	}

	/**
	 * close the inputstream is (handle exception for you)
	 * 
	 * @param is
	 */
	private void closeInputStream(InputStream is)
	{
		try
		{
			if (is == null)
			{
				DebugManager.print(ServerSocketHandler.getTag()
						+ "Inputstream is null",
						WifiDirectManager.DEBUGGER_CHANNEL);
			}
			else
			{
				is.close();
			}
		}
		catch (IOException e)
		{
			DebugManager.print(ServerSocketHandler.getTag()
					+ "error while closing inputstream",
					WifiDirectManager.DEBUGGER_CHANNEL);
		}
	}

	/**
	 * Called when the message is a "IP" message.
	 * 
	 * @param stream
	 */
	private void receiveIP(InputStream stream)
	{
		// get the client's server ip and port in the string
		String clientIP = getStringFromInputStream(stream);

		DebugManager.print(ServerSocketHandler.getTag()
				+ "client address is : " + clientIP + ". i'm the host. cesar",
				WifiDirectManager.DEBUGGER_CHANNEL);

		// notify SocketHandler that this device client should
		// connect to the ip and port previously got.
		String[] splittedClientIP = clientIP.split("!");
		if (splittedClientIP.length >= 2)
		{
			master.setRemoteHost(splittedClientIP[0],
					Integer.parseInt(splittedClientIP[1]));
			master.send();
			// send an accuse message with the client newly created and
			// connected
			// to the remost server.
	
			master.sendAccuse(id);
		}
	}

	/**
	 * Called when the message is a "STRING" message.
	 * 
	 * @param stream
	 */
	private void receiveString(InputStream stream)
	{
		// get the string into the message
		String res = getStringFromInputStream(stream);
		DebugManager.print(ServerSocketHandler.getTag()
				+ "we receive string : " + res,
				WifiDirectManager.DEBUGGER_CHANNEL);
		// call the callback if set
		if (onReceiveString != null)
			onReceiveString.Do(res);
		// send accuse to client
		master.sendAccuse(id);
	}

	/**
	 * Called when the message is an "ACCUSE" (synchrone or asynchrone) message.
	 * 
	 * @param stream
	 */
	private void receiveAccuse()
	{
		DebugManager.print(ServerSocketHandler.getTag() + "Accuse received !",
				WifiDirectManager.DEBUGGER_CHANNEL);
		// Receiving an accuse means that previous message was received, so we
		// can allow
		// the client to send the next one
		master.sendNext();
	}

	/**
	 * Called when the message is a "BOOL" message.
	 * 
	 * @param stream
	 */
	private void receiveBool(InputStream is)
	{
		// read a bool from the stream
		boolean res = read(is) == 0 ? false : true;
		DebugManager.print(ServerSocketHandler.getTag() + "we received bool: "
				+ res, WifiDirectManager.DEBUGGER_CHANNEL);
		// call the appropriate callback if set
		if (onReceiveBool != null)
			onReceiveBool.Do(res);
		// send accuse to client
		master.sendAccuse(id);
	}

	/**
	 * Called when the message is a "INT" message.
	 * 
	 * @param stream
	 */
	private void receiveInt(InputStream is)
	{
		// get an int from the stream
		int res = read(is);
		DebugManager.print(ServerSocketHandler.getTag() + "we received int: "
				+ res, WifiDirectManager.DEBUGGER_CHANNEL);
		// call the appropriate callback if set
		if (onReceiveInt != null)
			onReceiveInt.Do(res);
		// send accuse to client
		master.sendAccuse(id);
	}

	/**
	 * Called when the message is a "BYTE" message.
	 * 
	 * @param stream
	 */
	private void receiveByte(InputStream is)
	{
		byte res = (byte) read(is);
		DebugManager.print(ServerSocketHandler.getTag() + "we received byte: "
				+ res, WifiDirectManager.DEBUGGER_CHANNEL);
		if (onReceiveByte != null)
			onReceiveByte.Do(res);
		master.sendAccuse(id);
	}

	/**
	 * Called when the message is a "CHAR" message.
	 * 
	 * @param stream
	 */
	private void receiveChar(InputStream is)
	{
		char res = (char) read(is);
		DebugManager.print(ServerSocketHandler.getTag() + "we received char: "
				+ res, WifiDirectManager.DEBUGGER_CHANNEL);
		if (onReceiveChar != null)
			onReceiveChar.Do(res);
		master.sendAccuse(id);
	}

	/**
	 * Called when the message is a "FLOAT" message.
	 * 
	 * @param stream
	 */
	private void receiveFloat(InputStream is)
	{
		float res = getFloatFromInputStream(is);
		DebugManager.print(ServerSocketHandler.getTag() + "we received float: "
				+ res, WifiDirectManager.DEBUGGER_CHANNEL);
		if (onReceiveFloat != null)
			onReceiveFloat.Do(res);
		master.sendAccuse(id);
	}

	/**
	 * Called when the message is a "DOUBLE" message.
	 * 
	 * @param stream
	 */
	private void receiveDouble(InputStream is)
	{
		double res = getDoubleFromInputStream(is);
		DebugManager.print(ServerSocketHandler.getTag()
				+ "we received double: " + res,
				WifiDirectManager.DEBUGGER_CHANNEL);
		if (onReceiveDouble != null)
			onReceiveDouble.Do(res);
		master.sendAccuse(id);
	}

	/**
	 * Called when the message is a "LONG" message.
	 * 
	 * @param stream
	 */
	private void receiveLong(InputStream is)
	{
		long res = getLongFromInputStream(is);
		DebugManager.print(ServerSocketHandler.getTag() + "we received long: "
				+ res, WifiDirectManager.DEBUGGER_CHANNEL);
		if (onReceiveLong != null)
			onReceiveLong.Do(res);
		master.sendAccuse(id);
	}

	/**
	 * Called if the message contains an array of byte
	 * 
	 * @param stream
	 */
	private void receiveBytes(InputStream is)
	{
		byte[] res = getBytesFromInputStream(is);
		/* debug */
		String bytesStr = "";
		for (int i = 0; i < res.length; i++)
		{
			bytesStr += res[i] + "-";
		}

		DebugManager.print(ServerSocketHandler.getTag() + "we received bytes: "
				+ bytesStr, WifiDirectManager.DEBUGGER_CHANNEL);
		/* end debug */
		if (onReceiveByteArray != null)
			onReceiveByteArray.Do(res);
		master.sendAccuse(id);
	}

	/**
	 * Called when the message is a "FILE" message.
	 * 
	 * @param stream
	 */
	private void receiveFile(InputStream is)
	{
		File res = getFileFromInputStream(is);
		/* debug */
		DebugManager.print(ServerSocketHandler.getTag() + "new file received "
				+ res, WifiDirectManager.DEBUGGER_CHANNEL);
		/* end debug */
		if (onReceiveFile != null)
			onReceiveFile.Do(res);
		master.sendAccuse(id);
	}

	/**
	 * 
	 * @param is
	 * @return an int read from the inpustream 'is'
	 */
	private int read(InputStream is)
	{
		if (is != null)
		{
			try
			{
				return is.read();
			}
			catch (IOException e)
			{
				DebugManager.print(
						ServerSocketHandler.getTag()
								+ "error when reading inputstream "
								+ e.getLocalizedMessage(),
						WifiDirectManager.DEBUGGER_CHANNEL);
				return -1;
			}
		}
		return -1;
	}

	/**
	 * Read an int from the inputstream is and cast it to a PACKET_TYPE
	 * 
	 * @param is
	 * @return the packet type tagging the inputstream
	 */
	private PACKET_TYPE getPacketType(InputStream is)
	{
		int value = -1;
		int available = -1;
		try
		{
			available = is.available();
			value = read(is);
			return PACKET_TYPE.get(value);
		}
		catch (IOException e)
		{
			DebugManager.print(ServerSocketHandler.getTag()
					+ "error while getting packet type " + available,
					WifiDirectManager.DEBUGGER_CHANNEL);
			return PACKET_TYPE.DEFAULT;
		}
		catch (Exception e)
		{
			DebugManager.print(ServerSocketHandler.getTag()
					+ "unknow exception : " + value,
					WifiDirectManager.DEBUGGER_CHANNEL);
			return PACKET_TYPE.DEFAULT;
		}
	}

	/**
	 * Set the "id" attribute with the id read from the current message
	 * represented by the inputstream "is". if the message was already received
	 * ( = stored in the already treated attribute), the method return true, if
	 * not, it return false and add the message to the list of the message
	 * already treated.
	 * 
	 * @param is
	 * @return true if the message was already received and false else.
	 */
	private boolean isMessageAlreadyReceived(InputStream is)
	{
		id = getLongFromInputStream(is);
		DebugManager.print(ServerSocketHandler.getTag() + "message id = " + id,
				WifiDirectManager.DEBUGGER_CHANNEL);
		if (alreadyTreated.contains(id))
		{
			return true;
		}
		else
		{
			alreadyTreated.add(id);
			return false;
		}
	}

	@Override
	public void run()
	{
		// Get the inputstream to receive the data from the client
		InputStream is = getClientData(client);

		// Get the packet type from the message
		PACKET_TYPE type = getPacketType(is);

		// If the packet type is ACCUSE, it means there is also an other message
		// after the accuse
		// because ACCUSE packet come always concatenated with other message.
		// This is done to avoid sending accuse and message in the same time,
		// but asynchronously so
		// there is a risk that of them mask the other
		if (type == PACKET_TYPE.ACCUSE)
		{
			if (!isMessageAlreadyReceived(is))
			{
				receiveAccuse();// synchrone Accuse was not already received, so
								// we treat it
			}
			type = getPacketType(is);// ...and then read the message
										// concatenated with this accuse
		}

		boolean msgAlreadyReceived = isMessageAlreadyReceived(is);

		DebugManager.print(ServerSocketHandler.getTag() + "We received a packet of type " + type  + ". was already received ? " + msgAlreadyReceived, WifiDirectManager.DEBUGGER_CHANNEL);

		// We want to ignore all messages received more than 1 time
		if (!msgAlreadyReceived)
		{
			switch (type)
			{
			// ASYNC ACCUSE are sent when there is no message in instance to be
			// sent
				case ASYNC_ACCUSE:
					DebugManager.print(ServerSocketHandler.getTag()
							+ "Client is alive !",
							WifiDirectManager.DEBUGGER_CHANNEL);
					receiveAccuse(); // We treat them as synchrone accuse (=
										// ACCUSE packet type)
					break;
				case DEFAULT:
					break;
				case BOOL:
					receiveBool(is);
					break;
				case BYTE:
					receiveByte(is);
					break;
				case BYTE_ARRAY:
					receiveBytes(is);
					break;
				case CHAR:
					receiveChar(is);
					break;
				case DOUBLE:
					receiveDouble(is);
					break;
				case FILE:
					receiveFile(is);
					DebugManager.print(ServerSocketHandler.getTag()
							+ "We finished receiving a file!",
							WifiDirectManager.DEBUGGER_CHANNEL);
					break;
				case FLOAT:
					receiveFloat(is);
					break;
				case INT:
					receiveInt(is);
					break;
				case IP:
					receiveIP(is);
					break;
				case LONG:
					receiveLong(is);
					break;
				case STRING:
					receiveString(is);
					break;
				case ACCUSE:
					// receiveAccuse(); // already treated
					break;
				default:
					break;
			}
		}
		else
		{
			// if the message was already receive, it means
			// remote pair should not have receive
			// accuse, so it keeps sending it -> so
			// we send the accuse again
			// (except if the message is an accuse, because if we do that
			// both will keep sending accuse to answer the previously received
			// accuse
			// and the communication will be broken)
			if (type != PACKET_TYPE.ASYNC_ACCUSE)
			{
				master.sendAccuse(id);
			}
		}

		closeInputStream(is);

		DebugManager.print(
				ServerSocketHandler.getTag() + "Request is answered",
				WifiDirectManager.DEBUGGER_CHANNEL);
	}

	/**
	 * Set callback to be call when receiving data
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
		Communication.onReceiveString = onReceiveString;
		Communication.onReceiveInt = onReceiveInt;
		Communication.onReceiveBool = onReceiveBool;
		Communication.onReceiveFloat = onReceiveFloat;
		Communication.onReceiveDouble = onReceiveDouble;
		Communication.onReceiveByte = onReceiveByte;
		Communication.onReceiveLong = onReceiveLong;
		Communication.onReceiveFile = onReceiveFile;
		Communication.onReceiveByteArray = onReceiveByteArray;
		Communication.onReceiveChar = onReceiveChar;
	}

}

/**
 * This class define a server socket for android, that can handle message as
 * int, float, long, file, ... All message type are declared in the PACKET_TYPE
 * interface.
 * 
 * @see PACKET_TYPE
 * 
 * @author Gregoire
 * 
 */
public class ServerSocketHandler extends AsyncTask<Void, String, Void> {

	private int port;
	private SocketHandler master;
	private String address;
	public static int ACCEPT_TIMEOUT = 5000;

	/**
	 * @return debug identifier when printing something relative to this class
	 */
	public static String getTag()
	{
		Calendar cal = Calendar.getInstance();
		SimpleDateFormat sdf = new SimpleDateFormat("HH:mm:ss");
		return "[LUDOSERVER][" + sdf.format(cal.getTime()) + "]";
	}

	/**
	 * @param port
	 *            the port you want to listen
	 * @param address
	 *            the address you want for your server
	 * @param master
	 *            the socket handler that know also the client of your local
	 *            device
	 */
	public ServerSocketHandler(int port, String address, SocketHandler master)
	{
		this.port = port;
		this.master = master;
		this.address = address;

	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void stop()
	{
		DebugManager.print(getTag() + "<font color='green'>stop async</font>", WifiDirectManager.DEBUGGER_CHANNEL);
		this.cancel(true);
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	public void listen()
	{
		execute();
	}

	//------------------------------------------------------------------------------------------------------------------
	// @return the address specified in the constructor
	//------------------------------------------------------------------------------------------------------------------
	public String getServerIpAddress()
	{
		return address;
	}

	//------------------------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------------------------
	private void openServerSocket()
	{
		 // 1. Create a server socket and wait for client connections. This call blocks until a connection is accepted
		 //    from a client
		try
		{
			if( serverSocket != null )
			{
				closeServerSocket();
			}

			serverSocket = new ServerSocket();
			System.setProperty("sun.net.useExclusiveBind","false");
			serverSocket.setReuseAddress(true);
			SocketAddress socketAddress = new InetSocketAddress(InetAddress.getByName(address), port);
			serverSocket.bind(socketAddress);
		}
		catch (Exception e)
		{
			DebugManager.print(ServerSocketHandler.getTag() + "socket error: " + e.getLocalizedMessage() + "\n" + e.getMessage(),
					WifiDirectManager.DEBUGGER_CHANNEL);
			
			try
			{
				serverSocket = new ServerSocket(port, 0, InetAddress.getByName(address));
			}
			catch (UnknownHostException e1)
			{
				DebugManager.print(ServerSocketHandler.getTag() + e1.getLocalizedMessage(), WifiDirectManager.DEBUGGER_CHANNEL);
			}
			catch (IOException e1)
			{
				DebugManager.print(ServerSocketHandler.getTag() + e1.getLocalizedMessage(), WifiDirectManager.DEBUGGER_CHANNEL);
			}

		}

		String debugText = "launching server on " + serverSocket.getLocalSocketAddress() + ":" + serverSocket.getLocalPort();
		DebugManager.print(ServerSocketHandler.getTag() + debugText,  WifiDirectManager.DEBUGGER_CHANNEL);
	}

	/**
	 * return from this function when a client connect to this server at the
	 * specified address and port
	 * 
	 * @return a newly connected client
	 */
	private Socket waitForClient()
	{
		Socket client = null;

		try
		{
			//set serversocket timeout
			serverSocket.setSoTimeout(ACCEPT_TIMEOUT);

			DebugManager.print(ServerSocketHandler.getTag()  + " 1.4 ServerSocketHandler::waitForClient: listen socket server", WifiDirectManager.DEBUGGER_CHANNEL);
			client = serverSocket.accept();
			DebugManager.print(ServerSocketHandler.getTag()  + " 1.5 ServerSocketHandler::waitForClient: read of socket", WifiDirectManager.DEBUGGER_CHANNEL);

		}
		catch(SocketException se)
		{
			DebugManager.print( ServerSocketHandler.getTag() + " 1.5 ServerSocketHandler::waitForClient:serverSocket.accept() failed because  " + se, WifiDirectManager.DEBUGGER_CHANNEL);
			//openServerSocket();
		}
		catch (IOException e1)
		{
			DebugManager.print(ServerSocketHandler.getTag() + " 1.5 ServerSocketHandler::waitForClient: serverSocket.accept() failed with error exception : " + e1, WifiDirectManager.DEBUGGER_CHANNEL);
		}



		return client;
	}

	private boolean closeServerSocket()
	{
		boolean bServerSocketClose = false;
		try
		{
			serverSocket.close();
			DebugManager.print(ServerSocketHandler.getTag() + " ServerSocketHandler::closeServerSocket ==> server is closed",  WifiDirectManager.DEBUGGER_CHANNEL);
			bServerSocketClose = true;
		}
		catch (IOException e)
		{
			DebugManager.print(ServerSocketHandler.getTag() + " server.close() failed",  WifiDirectManager.DEBUGGER_CHANNEL);
			bServerSocketClose = false;
		}

		return bServerSocketClose;
	}


	private ServerSocket serverSocket;

	@Override
	protected Void doInBackground(Void... params)
	{
		// start listenning
		openServerSocket();

		while ( !isCancelled() )
		{
			DebugManager.print(ServerSocketHandler.getTag() + " 1. ServerSocketHandler::doInBackground: server is waiting for client...", WifiDirectManager.DEBUGGER_CHANNEL);

			// Wait for client...
			Socket client = waitForClient();

			// and then open another thread to communicate with him
			// Treating communication in new thread allow to wait for other client
			// - and then answer to other communication in the same time
			if(client != null)
			{
				DebugManager.print(ServerSocketHandler.getTag() + " 2. ServerSocketHandler::doInBackground: Create a new communication object using the same client and master",  WifiDirectManager.DEBUGGER_CHANNEL);

				if ( master == null)
				{
					DebugManager.print("... but master is null", WifiDirectManager.DEBUGGER_CHANNEL);
				}

				new Thread(new Communication(client, master)).start();
			}
			else
			{
				DebugManager.print(ServerSocketHandler.getTag() + " 2. ServerSocketHandler::doInBackground: Client is NULL, (socketserver.accept did not work)",  WifiDirectManager.DEBUGGER_CHANNEL);
			}
		}

		// clean buffer after operations done
		if ( closeServerSocket() )
		{
			//DebugManager.printAllStackTraces(WifiDirectManager.DEBUGGER_CHANNEL);
			DebugManager.print(ServerSocketHandler.getTag() + "[FINISHING] Server is closed", WifiDirectManager.DEBUGGER_CHANNEL);

		}

		return null;
	}

	/**
	 * Set a callback to be call when data are received
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
		Communication
				.registerCallBackReceiver(onReceiveString, onReceiveInt,
						onReceiveBool, onReceiveFloat, onReceiveDouble,
						onReceiveByte, onReceiveLong, onReceiveFile,
						onReceiveByteArray, onReceiveChar);
	}

	public void stopHandlers()
	{
		// TODO Auto-generated method stub

	}

	public static void setServerTempFileName(String name)
	{
		Communication.tempFile = name;
	}

}