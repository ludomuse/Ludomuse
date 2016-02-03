package org.cocos2dx.cpp.sockets;

public enum PACKET_TYPE {

	DEFAULT, ACCUSE, STRING, INT, BOOL, LONG, FILE, DOUBLE, FLOAT, CHAR, IP, BYTE, BYTE_ARRAY, ASYNC_ACCUSE;

	/**
	 * @return the integer representation of the enum, since enum is a special
	 *         type in java. Basically, we return the position of the enum into
	 *         the enum list.
	 */
	public int toInt()
	{
		PACKET_TYPE[] all = PACKET_TYPE.class.getEnumConstants();
		for (int i = 0; i < all.length; i++)
		{
			if (all[i] == this)
			{
				return i;
			}
		}
		return -1;
	}

	/**
	 * @param i
	 * @return the PACKET_TYPE at the position i into the enumeration.
	 */
	public static PACKET_TYPE get(int i)
	{
		PACKET_TYPE[] types = PACKET_TYPE.class.getEnumConstants();
		return i < 0 || i >= types.length ? PACKET_TYPE.DEFAULT : types[i];
	}

	public static void main(String[] args)
	{
		System.out.println(PACKET_TYPE.STRING.toInt());
	}
}
