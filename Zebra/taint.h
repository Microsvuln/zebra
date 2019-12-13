#ifndef _TAINT_H_
#define _TAINT_H_

#include "defines.h"

class Taint
{
public:
	static const int INVALID = 0xffffffff;
	
	ADDRINT addr;
	UINT32 size;
	UINT32 offset;		// �ҽ��� ������
	string symbol;

	Taint::Taint()
	{
		addr = INVALID;
		size = INVALID;
		offset = INVALID;
		symbol = "";
		
	}

	Taint::Taint(ADDRINT addr, UINT32 size)
	{
		this->addr = addr;
		this->size = size;
		this->offset = INVALID;
		symbol = "";
	}

	Taint::Taint(ADDRINT addr, UINT32 size, UINT32 offset)
	{
		this->addr = addr;
		this->size = size;
		this->offset = offset;
		symbol = "";
	}

	Taint::Taint(ADDRINT addr, UINT32 size, string symbol)
	{
		this->addr = addr;
		this->size = size;
		this->offset = INVALID;
		this->symbol = symbol;
	}

	VOID Taint::Invalidate()
	{
		addr = INVALID;
		size = INVALID;
		offset = INVALID;
		symbol = "";
	}

	BOOL Taint::IsValid()
	{
		return (offset != INVALID) || (symbol != "");
	}

	BOOL Taint::FromSource()
	{
		return offset != INVALID;
	}

	ADDRINT high_addr()
	{
		return addr + size;
	}

	VOID Dump()
	{
		cerr << "[*] Taint::Dump" << endl;
		cerr << "[**] addr : " << hex << addr << endl;
		cerr << "[**] size : " << hex << size << endl;
		cerr << "[**] offset : " << hex << offset << endl;
		cerr << "[**] symbol : " << hex << symbol << endl;
	}

	/**
	 * �ҽ��� �������� ��� ����Ʈ�� �����Ѵ�.
	 * �ּҿ� ������� �־��� ������ �����°� �ɺ��� �ҽ� ����Ʈ�� �����Ѵ�
	 * @param		addr		����Ʈ �ּ�
	 * @param		size		����Ʈ ������
	 * @param		dest		��� ����Ʈ
	 * @param		src			�ҽ� ����Ʈ
	 */
	VOID Copy(ADDRINT addr, UINT32 size, Taint& src)
	{
		this->addr = addr;
		this->size = size;
		this->symbol = src.symbol;

		if (src.FromSource())
		{
			this->offset = src.offset + addr - src.addr;
		}
	}
};

#endif // _TAINT_H_