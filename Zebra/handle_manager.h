#ifndef _HANDLE_MANAGER_H_
#define _HANDLE_MANAGER_H_

#include <pin.H>
#include <vector>

#include "defines.h"
#include "lockable_handle_container.h"

class HandleManager
{
protected:
	LockableHandleContainer m_handles;
	LockableHandleContainer m_mapping_handles;
	PIN_LOCK m_lock;

public:
	HandleManager();
	~HandleManager();

	// �Ϲ� �ڵ� ���� �Լ�
	VOID SaveHandle(HANDLE handle);
	VOID DeleteHandle(HANDLE handle);
	BOOL ExistHandle(HANDLE handle);

	// ���� �ڵ� ���� �Լ�
	VOID SaveMappingHandle(HANDLE handle);
	VOID DeleteMappingHandle(HANDLE handle);
	BOOL ExistMappingHandle(HANDLE handle);
};


#endif 