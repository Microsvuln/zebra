#ifndef _TAINT_MANAGER_H_
#define _TAINT_MANAGER_H_
#include <pin.H>

#include "taint.h"

class TaintManager
{
protected:
	// General purpose registers
	//
	// EAX, EBX, ECX, EDX, ESI, EDI
	//
	#define REGISTER_MAX 10

	vector<Taint> m_tainted_memories;
	Taint m_tainted_registers[REGISTER_MAX];

	UINT32 RegToInternalIndex(REG reg);
	UINT32 RegAccessSize(REG reg);

public:
	TaintManager();
	~TaintManager();

	// �������� ���� �Լ� 
	VOID DoTaintRegister(REG reg, Taint& taint);
	VOID UnTaintRegister(REG reg);
	BOOL IsTaintedRegister(REG reg, Taint& taint);

	// �޸� ���� �Լ�
	VOID DoTaintMemory(ADDRINT addr, UINT32 size, Taint& taint);
	VOID UnTaintMemory(ADDRINT addr, UINT32 size);
	BOOL IsTaintedMemory(ADDRINT addr, UINT32 size, Taint& taint);

	// ����� ���� �Լ�
	VOID Dump();

};

#endif // _TAINT_MANAGER_H_