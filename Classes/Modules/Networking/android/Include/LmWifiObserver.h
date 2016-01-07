/*
 * LmWifiObserver.h
 *
 *  Created on: 7 oct. 2015
 *      Author: Gregoire
 */

#ifndef WIFIOBSERVER_H_
#define WIFIOBSERVER_H_

#include <vector>
#include <string>
#include "../Include/LmHelpers.h"
#include "LmWifiDirectFacade.h"
#include "LmJniCppFacade.h"
#include "LmBytesMessage.h"

class LmWifiObserver {

private:
	int _observerIndex;

	bytes lastMsg;

	protected:
	event _event;
		LmWifiDirectFacade* _wifiFacade;

	public:
		//attributs



		LmWifiObserver()
		{
			_wifiFacade = WIFIFACADE;
		}

		void printId()
		{
			CCLOG("index obs = %d",_observerIndex);
		}

		virtual void onGettingPeers(std::vector<std::string> peers)
		{
		}
		;

		virtual void onReceiving(std::string s)
		{
		}
		;

		virtual void onReceiving(int i)
		{
		}
		;

		virtual void onReceiving(bool b)
		{
		}
		;

		virtual void onReceiving(long l)
		{
		}
		;

		virtual void onReceivingFile(std::string path)
		{
		}
		;

		virtual void onReceiving(double d)
		{
		}
		;

		virtual void onReceiving(float f)
		{
		}
		;

		virtual void onReceiving(char c)
		{
		}
		;

		void onReceivingByte(event byte)
		{
			_event = byte;
			onReceivingEvent();
		}

		void listenWifiFacade()
		{
			CCLOG("<font color=\"green\"> ADD OBSERVER</font>");
			_observerIndex = _wifiFacade->addObserver(this);
		}

		void stopListenWifiFacade()
		{

			_wifiFacade->removeObserver(_observerIndex);
		}

		virtual void onReceivingEvent()
		{
		}
		;

		void clearLastMsg()
		{
			lastMsg = bytes();
		}

		void onReceiving(bytes msg)
		{

			CCLOG("msg len = %d, msg size = %d", msg.getLen(), msg.getSize());
			/*if(lastMsg == msg || msg.getLen() == 0)
			{
				lastMsg.rewind();
				event lastEvent = lastMsg.readByte();
				event currEvent = msg.readByte();
				CCLOG("Msg received two times (lastMsg = %d, msg = %d). Not forwaded.", lastEvent, currEvent);
				msg.rewind();
				return;
			}
			lastMsg = msg;*/
			onReceivingByte(msg.readByte());
			onReceivingMsg(msg);
		}
		;

		virtual void onReceivingMsg(bytes msg)
		{

		}
		;


		virtual void discoverPeers()
		{
		}
		;

		virtual ~LmWifiObserver()
		{

		}

};

#endif /* WIFIOBSERVER_H_ */
