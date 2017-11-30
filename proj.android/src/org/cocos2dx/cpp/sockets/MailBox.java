package org.cocos2dx.cpp.sockets;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import android.util.Log;
import org.cocos2dx.cpp.DebugManager;
import org.cocos2dx.cpp.wifiDirect.WifiDirectManager;

import android.os.Handler;

/**
 * private class that represent a Message = a reference to the sent method and
 * its arg, from the class ClientSocketHandler and to call when sending the
 * message
 * 
 * @author Gregoire, IHMTEK
 * 
 */
class Message {

	public String methodName;
	public Object[] args;
	public Class<?>[] parameterTypes;

	private ClientSocketHandler client;

	public Message(ClientSocketHandler client, String methodName,
			Object[] args, Class<?>[] parameterTypes)
	{
		super();
		this.client = client;
		this.methodName = methodName;
		this.args = args;
		this.parameterTypes = parameterTypes;
	}

	/**
	 * send the message
	 */
	public void send()
	{
		try
		{
			// message has to be send with the same id each time until there are
			// received by host
			// To avoid the id to increase for the next sending, we use the
			// method "setId"
			// that specify the id to use for the next sending
			ClientSocketHandler.setId(ClientSocketHandler.getId());
			// Then we retrieve the send method to call by reflection and call
			// it
			ClientSocketHandler.class.getMethod(methodName, parameterTypes)
					.invoke(client, args);
		
		}
		catch (Exception e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}

/**
 * The mailbox class stock message in a list and send the next message when
 * receiving an accuse, so we are sure that the previously message was received
 * before sending the next one
 * 
 * @author Gregoire, IHMTEK
 * 
 */
public class MailBox {

	private ClientSocketHandler client;
	private List<Message> messages = new ArrayList<Message>();

	private boolean _sendFirstMessageWhenPosted = false;

	// When this timer expire, last message send will be send again
	public static int timeBeforeConsideringMessageLost = 5000;

	private Handler timeout = new Handler();
	private Runnable worker = new Runnable() {

		@Override
		public void run()
		{
			send();
		}

	};

	public MailBox(ClientSocketHandler client)
	{
		this.client = client;
	}

	/**
	 * post a message (@see Message)
	 * 
	 * @param methodName
	 *            the method to call from the class ClientSocketHandler
	 * @param args
	 *            argument of the method
	 * @param argType
	 *            class type of the arguments "args"
	 * @param position
	 *            position in the sending order. The closer to zero, the sooner
	 *            to be send.
	 */
	public void post(String methodName, Object[] args, Class<?>[] argType,
			int position)
	{
		Message msg = new Message(client, methodName, args, argType);
		messages.add(position, msg);
		DebugManager.print(ClientSocketHandler.GetTag() + "Debug info MailBox = "
				+ messages.size() + "/" + position,
				WifiDirectManager.DEBUGGER_CHANNEL);
		if (_sendFirstMessageWhenPosted && messages.size() == 1)
		{
			_sendFirstMessageWhenPosted = false;
			send();
		}
	}

	/**
	 * Post a message at the end of the message (so it will be the last to be
	 * send) (@see other post method)
	 * 
	 * @param methodName
	 * @param arg
	 * @param argType
	 */
	public void post(String methodName, Object arg, Class<?> argType)
	{
		DebugManager.print(ClientSocketHandler.GetTag() + "Message.size() = "
				+ messages.size(), WifiDirectManager.DEBUGGER_CHANNEL);
		post(methodName, arg, argType, messages.size());
	}

	/**
	 * Do the same that the method "post(String methodName, Object[] args,
	 * Class<?>[] argType, int position)" but with just one argument to pass to
	 * the send method "methodName"
	 * 
	 * @param methodName
	 * @param arg
	 * @param argType
	 * @param position
	 */
	public void post(String methodName, Object arg, Class<?> argType,
			int position)
	{
		post(methodName, new Object[] { arg }, new Class<?>[] { argType },
				position);
	}

	private void armTimeOut()
	{
		timeout.postDelayed(worker, timeBeforeConsideringMessageLost);
	}

	private void unarmTimeOut()
	{
		timeout.removeCallbacks(worker);
	}

	public void send()
	{
		unarmTimeOut();
		// If the list is empty, wait for the next message to be post and send
		// it
		if (messages.size() == 0)
		{
			_sendFirstMessageWhenPosted = true;
		}
		else
		{
			// else send the first message in the list
			Message message = messages.get(0);
			message.send();
			// arm timer in order to send this message again if no accuse is
			// received
			if(message.parameterTypes[0] == File.class){
				Log.d("debug", "Ludomuse - LudoMuse - Message à base d'un file trop style c'est une grand ligne pour prendre de la place est resortir dans le caca qu'est le debug");
			}
			else{
				armTimeOut();
			}
		}
	}

	public boolean isEmpty()
	{
		return messages.size() == 0;
	}

	public void sendNext()
	{
		// Remove first element in the list
		if (!isEmpty())
			messages.remove(0);
		// and generate an id in order to differentiate all message
		// (We can send to time the same thing if wanted, but in different
		// message.
		// but if you received two time a message with the same id, it just
		// means, your
		// host's mailbox never received your accuse)F
		ClientSocketHandler.generateId();
		send();
	}

	public void stopHandlers()
	{
		unarmTimeOut();
	}
}
