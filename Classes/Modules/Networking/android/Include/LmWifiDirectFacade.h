/*
 * WifiDirectFacade.h
 *
 *  Created on: 7 oct. 2015
 *      Author: Gregoire
 */

#ifndef WIFIDIRECTFACADE_H_
#define WIFIDIRECTFACADE_H_

#include <vector>
#include <list>
#include "LmHelpers.h"
#include <string>
#include "LmBytesMessage.h"

class LmWifiObserver;

class LmWifiDirectFacade
{
	public:

		enum SEND_F
		{
			SEND_INT,
			SEND_FLOAT,
			SEND_LONG,
			SEND_DOUBLE,
			SEND_FILE,
			SEND_CHAR,
			SEND_BYTE,
			SEND_BYTES,
			SEND_STRING
		};


		LmWifiDirectFacade();

		void onGettingPeers(std::vector<std::string> peers);

		void onReceiving(std::string s);

		void onReceiving(int i);

		void onReceiving(bool b);

		void onReceiving(long l);

		void onReceivingFile(std::string path);

		void onReceiving(double d);

		void onReceiving(float f);

		void onReceiving(char c);

		void onReceivingByte(lmByte b);

		void onReceiving(bytes byteArray);

		void discoverPeers();

		void connectTo(std::string deviceName);

		void send(std::string s);

		void send(int i);

		void send(bool b);

		void send(long l);

		void sendFile(std::string filePath);

		void setServerTempFileName(std::string filePath);

		void clean();

		void send(double d);

		void send(float f);

		void send(char c);

		void sendByte(lmByte b);

		void sendBytes(bytes bytes);

		void group(int size, SEND_F* send_functions, void** params);

		void askTabletName();

		void setTabletName(std::string name);

		std::string getTabletName();

		int addObserver(LmWifiObserver* wo);

		void removeObserver(int index);

		void sendEvent(event e, LmWifiDirectFacade::SEND_F method, void* arg);

		void sendEvent(event e, int size,
				const LmWifiDirectFacade::SEND_F* method, const void** args);

	private:
		std::string _name;

		std::list<LmWifiObserver*> _observers;
};

#endif /* WIFIDIRECTFACADE_H_ */
