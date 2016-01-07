/*
 * BYTESMESSAGE_H_.h
 *
 *  Created on: 7 oct. 2015
 *      Author: Gregoire, IHMTEK
 */

#ifndef BYTESMESSAGE_H_
#define BYTESMESSAGE_H_

#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include "cocos2d.h"

#define WR_PRIM(X) 		void write(X v)\
						{\
							byte* rep = alloc(sizeof(X));\
							memcpy(rep, &v, sizeof(X));\
							write(rep, sizeof(X));\
							del(rep);\
						}

#define RD_PRIM(X, Y) 	X Y()\
						{\
							byte* rep = readBytes(sizeof(X));\
							X res = 0;\
							memcpy(&res, rep, sizeof(X));\
							return res;\
						}

#define OP_IN(X) 		bytes& operator<<(X value)\
						{\
							write(value);\
							return *this;\
						}

#define OP_OUT(X, Y) 	bytes& operator>>(X& value)\
						{\
							value = Y;\
							return *this;\
						}

typedef char byte;

class bytes;
class LmSerializable {
	public:
		virtual void writeOn(bytes* msg) = 0;
		virtual void readOn(bytes* msg) = 0;
};

class bytes {

	private:
		byte* data;
		int writeCursor;
		int readCursor;
		int size;

		bool error = false;

		void allocate(int size)
		{
			if (this->size >= size || size == 0)
				return;
			byte* newBytes = alloc(size);
			if (data != 0)
				memcpy(newBytes, data, getLen());
			del(data);
			data = newBytes;
			this->size = size;
		}

		void increase(int size)
		{
			allocate(getLen() + size);
		}

		void init()
		{
			data = 0;
			writeCursor = 0;
			readCursor = 0;
			size = 0;
		}

		void clearError()
		{
			error = false;
		}

		void setError()
		{
			error = true;
		}

		void del(byte* data)
		{
			//free(data);
		}

		byte* alloc(int size)
		{
			return (byte*) malloc(size * sizeof(byte));
		}

	public:

		void rewind()
		{
			readCursor = 0;
		}

		bool isThereError()
		{
			return error;
		}

		bytes()
		{
			init();
		}

		bytes(const bytes& other)
		{
			copy(other);
		}

		bytes& operator=(const bytes& other)
		{
			copy(other);
			return *this;
		}

		void copy(const bytes & other)
		{
			size = other.size;
			readCursor = other.readCursor;
			writeCursor = other.writeCursor;
			del(data);
			data = alloc(size);
			memcpy(data, other.data, size);
			error = other.error;
		}

		bytes(int size)
		{
			init();
			allocate(size);
		}

		bytes(bytes* dataBytes)
		{
			if (dataBytes != 0)
			{
				copy(*dataBytes);
			}
			else
			{
				init();
			}
		}

		bytes(byte* dataBytes, int len)
		{
			init();
			write(dataBytes, len);
		}

		int getReadCursor()
		{
			return readCursor;
		}

		int getLen()
		{
			return writeCursor;
		}

		int getSize()
		{
			return size;
		}

		int getFreeCount()
		{
			return getSize() - getLen();
		}

		void write(bytes array)
		{
			write(array.data, array.getLen());
		}

		void write(const byte* str, int len)
		{
			clearError();
			if (len == 0)
			{
				setError();
				return;
			}
			increase(len);
			memcpy((void*) &(data[writeCursor]), str, len);
			writeCursor += len;
		}

		void write(const char* str)
		{
			write(std::string(str));
		}

		void write(std::string str)
		{
			int len = str.length();
			std::vector<char> vect(str.begin(), str.end());
			byte* data = &vect[0];
			write(len);
			write(data, len);
		}

		WR_PRIM(int)

		WR_PRIM(long)

		WR_PRIM(double)

		WR_PRIM(float)

		void write(LmSerializable* obj)
		{
			obj->writeOn(this);
		}

		void writeChar(char c)
		{
			write((byte) c);
		}

		void write(byte b)
		{
			increase(1);
			data[writeCursor] = b;
			writeCursor++;
		}

		void write(bool b)
		{
			CCLOG("writting bool");
			write(b ? (byte) 1 : (byte) 0);
			CCLOG("writting bool ok");
		}

		byte* readBytes(int size)
		{
			clearError();
			if (getSize() == 0 || readCursor + size > getSize())
			{
				setError();
				return 0;
			}
			byte* res = alloc(size);
			memcpy(res, (void*) &(data[readCursor]), size);
			readCursor += size;
			return res;
		}

		byte readByte()
		{
			clearError();
			if (getSize() == 0 || readCursor > getSize())
			{
				setError();
				return 0;
			}
			byte res = data[readCursor];
			readCursor++;
			return res;
		}

		char readChar()
		{
			return (char) readByte();
		}

		bool readBool()
		{
			return readByte() == 0 ? false : true;
		}

		std::string readString()
		{
			int len = readInt();
			CCLOG("string size = %d", size);
			byte* array = readBytes(len);
			CCLOG("string read  correctly ! %s", array);
			return std::string(array, len);
		}

		RD_PRIM(int, readInt)

		RD_PRIM(long, readLong)

		RD_PRIM(double, readDouble)

		RD_PRIM(float, readFloat)

		template<typename Serializable>
		void read(Serializable* res)
		{

			res->readOn(this);

		}

		template<typename Serializable>
		Serializable* read()
		{
			Serializable* res = new Serializable();
			res->readOn(this);
			return res;
		}

		const byte* toByteArray()
		{
			return data;
		}

		std::string toString()
		{
			std::ostringstream convert;

			int len = getLen();
			for(int i = 0; i < len; i++)
			{
				int val = data[i];
				if(i < len - 1)
				{
					convert << val << "-";
				}
				else
				{
					convert << val;
				}
			}
			return convert.str();
		}

		const char* toCharSequence()
		{
			return toString().c_str();
		}

		OP_IN(long)

		OP_IN(int)

		OP_IN(std::string)

		OP_IN(bytes)

		OP_IN(float)

		OP_IN(double)

		OP_IN(byte)

		/*bytes& operator<<(byte value)
		{
			write(value);
			return *this;
		}*/

		bytes& operator<<(const char* value)
		{
			write(value);
			return *this;
		}

		bytes& operator<<(LmSerializable &value)
		{
			write(&value);
			return *this;
		}

		OP_OUT(int, readInt())

		OP_OUT(std::string, readString())

		OP_OUT(bool, readBool())

		OP_OUT(byte, readByte())

		OP_OUT(long, readLong())

		OP_OUT(double, readDouble())

		OP_OUT(float, readFloat())

		bytes& operator>>(bytes& value)
		{
			value.write(readBytes(value.getLen()), getLen());
			return *this;
		}

		template<typename Serializable>
		bytes& operator>>(Serializable** value)
		{
			*value = read<Serializable>();
			return *this;
		}

		//inline bool operator == (const PlMessageHeader &b) const
		bool operator==(bytes & other)
		{
			int len = getLen();
			int lenOther = other.getLen();
			if (len != lenOther)
				return false;
			for (int i = 0; i < len; i++)
			{
				if (data[i] != other.data[i])
					return false;
			}
			return true;
		}

		~bytes()
		{
			del(data);
		}

};

#endif /* BYTESMESSAGE_H_ */
