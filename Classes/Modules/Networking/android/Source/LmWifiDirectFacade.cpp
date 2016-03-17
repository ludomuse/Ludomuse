#include "../Include/LmWifiObserver.h"
#include "../Include/LmWifiDirectFacade.h"
#include "../Include/LmJniCppFacade.h"
#include "../Include/LmJniJavaFacade.h"

#define FORWARD(X) 	list<LmWifiObserver*>::const_iterator lit(_observers.begin());\
					list<LmWifiObserver*>::const_iterator lend(_observers.end());\
					for (; lit != lend; ++lit)\
					{\
						LmWifiObserver* tps = (*lit);\
						if(tps)\
							tps->X;\
					}

using namespace std;

LmWifiDirectFacade::LmWifiDirectFacade()
{
	LmJniCppFacade::setWifiFacade(this);
}


void LmWifiDirectFacade::onGettingPeers(vector<string> peers)
{
	FORWARD(onGettingPeers(peers));
}

void LmWifiDirectFacade::onReceiving(string s)
{
	FORWARD(onReceiving(s));
}

void LmWifiDirectFacade::onReceiving(int i)
{
	FORWARD(onReceiving(i));
}

void LmWifiDirectFacade::onReceiving(bool b)
{
	FORWARD(onReceiving(b));
}

void LmWifiDirectFacade::onReceiving(long l)
{
	FORWARD(onReceiving(l));
}

void LmWifiDirectFacade::onReceivingFile(string path)
{
	FORWARD(onReceivingFile(path));
}

void LmWifiDirectFacade::onReceiving(double d)
{
	FORWARD(onReceiving(d));
}

void LmWifiDirectFacade::onReceiving(float f)
{
	FORWARD(onReceiving(f));
}

void LmWifiDirectFacade::onReceiving(char c)
{
	FORWARD(onReceiving(c));
}

void LmWifiDirectFacade::onReceivingByte(lmByte b)
{
	FORWARD(onReceivingByte(b));
}

void LmWifiDirectFacade::onReceiving(bytes byteArray)
{
	FORWARD(onReceiving(byteArray));
}

void LmWifiDirectFacade::discoverPeers()
{

	LmJniJavaFacade::discoverPeers();
}

void LmWifiDirectFacade::connectTo(string deviceName)
{
	LmJniJavaFacade::connectTo(deviceName);
}

void LmWifiDirectFacade::send(string s)
{
	LmJniJavaFacade::send(s);
}

void LmWifiDirectFacade::send(int i)
{
	CCLOG("sending int");
	LmJniJavaFacade::send(i);
}

void LmWifiDirectFacade::send(bool b)
{
	LmJniJavaFacade::send(b);
}

void LmWifiDirectFacade::send(long l)
{
	LmJniJavaFacade::send(l);
}

void LmWifiDirectFacade::sendFile(string filePath)
{
	LmJniJavaFacade::sendFile(filePath);
}

void LmWifiDirectFacade::send(double d)
{
	LmJniJavaFacade::send(d);
}

void LmWifiDirectFacade::send(float f)
{
	LmJniJavaFacade::send(f);
}

void LmWifiDirectFacade::send(char c)
{
	LmJniJavaFacade::send(c);
}

void LmWifiDirectFacade::sendByte(lmByte b)
{
	CCLOG("sending byte");
	LmJniJavaFacade::sendByte(b);
}


void LmWifiDirectFacade::sendBytes(bytes bytes)
{

	//FORWARD(clearLastMsg());
	CCLOG("message send");
	LmJniJavaFacade::sendBytes(bytes);
}

void LmWifiDirectFacade::setServerTempFileName(std::string filePath)
{
	LmJniJavaFacade::setServerTempFileName(filePath);
}

int LmWifiDirectFacade::addObserver(LmWifiObserver* wo)
{
	_observers.push_back(wo);
	return _observers.size() - 1;
}

void LmWifiDirectFacade::removeObserver(int index)
{


	list<LmWifiObserver*>::iterator it = _observers.begin();
	advance(it, index);
	FORWARD(printId());
	CCLOG("list size %d",_observers.size() - 1);
	(*it)=nullptr;
	//_observers.erase(it);

	//FORWARD(printId());
}

void LmWifiDirectFacade::group(int size, SEND_F* send_functions, void** params)
{
	CCLOG("sending group");
	for (int i = 0; i < size; i++)
	{
		SEND_F function = send_functions[i];
		void* param = params[i];
		switch (function)
		{
		case SEND_INT:
			send(PTR_TO_OBJ(param, int));
			break;
			case SEND_FLOAT:
			send(PTR_TO_OBJ(param, float));
			break;
			case SEND_DOUBLE:
			send(PTR_TO_OBJ(param, int));
			break;
			case SEND_FILE:
			sendFile(PTR_TO_OBJ(param, std::string));
			break;
			case SEND_CHAR:
			send(PTR_TO_OBJ(param, char));
			break;
			case SEND_BYTE:
			sendByte(PTR_TO_OBJ(param, lmByte));
			break;
			case SEND_LONG:
			send(PTR_TO_OBJ(param, long));
			break;
			case SEND_BYTES:
			sendBytes(PTR_TO_OBJ(param, bytes));
			break;
			case SEND_STRING:
			send(PTR_TO_OBJ(param, std::string));
			break;
		}
	}
}

void LmWifiDirectFacade::sendEvent(event e, LmWifiDirectFacade::SEND_F method,
		void* arg)
{
	CCLOG("sending event");
	LmWifiDirectFacade::SEND_F* functions = new LmWifiDirectFacade::SEND_F[2]
	{ LmWifiDirectFacade::SEND_F::SEND_BYTE, method };
	void** params = new void*[2]
	{ &e, arg };
	return group(2, functions, params);
}

void LmWifiDirectFacade::sendEvent(event e, int size,
		const LmWifiDirectFacade::SEND_F* methods, const void** args)
{
	LmWifiDirectFacade::SEND_F* functions = new LmWifiDirectFacade::SEND_F[size + 1];
	functions[0] = LmWifiDirectFacade::SEND_F::SEND_BYTE;
	memcpy((void*) functions[1], (const void*) methods[0], size);

	void** params = new void*[size + 1];
	params[0] = &e;
	memcpy(params[1], args[0], size);
	return group(size + 1, functions, params);
}

void LmWifiDirectFacade::askTabletName()
{
	LmJniJavaFacade::getTabletName();
}

void LmWifiDirectFacade::setTabletName(std::string name)
{
	_name = name;
}

std::string LmWifiDirectFacade::getTabletName()
{
	return _name;
}

void LmWifiDirectFacade::clean()
{
	LmJniJavaFacade::clean();
}

