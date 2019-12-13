#include "function_callback.h"
#include "process_context.h"

#include <iostream>
#include <fstream>

namespace WINDOWS
{
#include <Windows.h>
};

extern ProcessContext g_process_context;

/**
 * CreateFileW ȣ�� �� �ݹ�
 * @param		thread_id		������ ���̵�
 * @param		file_name		�Էµ� ���� �̸� 
 */
VOID PreCreateFileW(THREADID thread_id, 
					LPCWSTR file_name)
{
	wcerr << L"[*] PreCreateFile : " << file_name << endl;

	if (!wcscmp(USER_DEFINED_FILE, file_name))
	{
		// ���� ����
		wcerr << L"[*] PreCreateFile : matched - " << file_name << endl;
		g_process_context.SaveArgument(thread_id, CREATE_FILE, new CreateFileArgument());
	}
}

/**
 * CreateFile ȣ�� �� �ݹ�
 * @param		thread_id		������ ���̵�
 * @param		return_value	CreateFileW�� ���� ���� �ڵ� 
 */
VOID PostCreateFileW(THREADID thread_id, 
						HANDLE return_value)
{
	cerr << "[*] PostCreateFileW : " << hex << return_value << endl;
	if (g_process_context.ExistArgument(thread_id, CREATE_FILE))
	{
		//cerr << "[*] PostCreateFileW : " << hex << return_value << endl;
		// ������ ����� ���� ������ ���
		g_process_context.SaveHandle(return_value);
		g_process_context.DeleteArgument(thread_id, CREATE_FILE);
	}
}

/**
 * ReadFile ȣ�� �� �ݹ�
 * @param		thread_id				������ ���̵�
 * @param		handle					ReadFile ����
 * @param		lpBuffer				ReadFile ����
 * @param		nNumberOfBytesToRead	ReadFile ���� 
 * @param		lpNumberOfBytesToRead	ReadFile ����
 */
VOID PreReadFile(THREADID thread_id, 
					HANDLE hFile, 
					LPVOID lpBuffer, 
					DWORD nNumberOfBytesToRead, 
					LPDWORD lpNumberOfBytesToRead)
{
	if (g_process_context.ExistHandle(hFile))
	{
		cerr << "[*] PreReadFile : lpBuffer - " << hex << lpBuffer << ", nNumberOfBytesToRead - " << hex << nNumberOfBytesToRead << endl;
		// ��ϵ� �ڵ��� ��� �ƱԸ�Ʈ�� ����
		g_process_context.SaveArgument(thread_id, READ_FILE, new ReadFileArgument(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesToRead));
	}
}

/**
 * ReadFile ȣ�� �� �ݹ�
 * @param		thread_id		������ ���̵�
 */
VOID PostReadFile(THREADID thread_id)
{
	if (g_process_context.ExistArgument(thread_id, READ_FILE))
	{
		ReadFileArgument* arg = (ReadFileArgument*)(g_process_context.ReadArgument(thread_id, READ_FILE));

		DWORD size;
		PIN_SafeCopy(&size, arg->lpNumberOfBytesToRead(), sizeof(size));

		DWORD offset = WINDOWS::SetFilePointer(arg->hFile(), 0, NULL, FILE_CURRENT) - size;

		g_process_context.DefineTaintSource(thread_id, (ADDRINT)(arg->lpBuffer()), size, offset);

		g_process_context.DeleteArgument(thread_id, READ_FILE);

		// ����Ʈ ����
		cerr << "[*] PostReadFile : " << hex << arg->lpBuffer() << ", " << hex << arg->nNumberOfBytesToRead() << ", " << hex << arg->lpNumberOfBytesToRead() << endl;
	}
}

/**
 * CreateFileMapping ȣ�� �� �ݹ�
 * @param		
 */
VOID PreCreateFileMapping(THREADID thread_id, 
							HANDLE handle)
{
	if (g_process_context.ExistHandle(handle))
	{
		g_process_context.SaveArgument(thread_id, CREATE_FILE_MAPPING, new CreateFileMappingArgument());
	}
}

VOID PostCreateFileMapping(THREADID thread_id, 
							HANDLE return_value)
{
	if (g_process_context.ExistArgument(thread_id, CREATE_FILE_MAPPING))
	{
		g_process_context.SaveMappingHandle(return_value);

		cerr << "[*] PostCreateFileMapping : " << hex << return_value << endl;

		g_process_context.DeleteArgument(thread_id, CREATE_FILE_MAPPING);
	}
}

VOID PreZwMapViewOfSection(THREADID thread_id, 
							HANDLE mapping_handle,
							PVOID* base_address, 
							PLARGE_INTEGER section_offset, 
							PSIZE_T view_size)
{
	if (g_process_context.ExistMappingHandle(mapping_handle))
	{
		g_process_context.SaveArgument(thread_id, ZW_MAP_VIEW_OF_SECTION, new ZwMapViewOfSectionArgument(base_address, section_offset, view_size));
	}
}

VOID PostZwMapViewOfSection(THREADID thread_id)
{
	if (g_process_context.ExistArgument(thread_id, ZW_MAP_VIEW_OF_SECTION))
	{
		ZwMapViewOfSectionArgument* arg = (ZwMapViewOfSectionArgument*) (g_process_context.ReadArgument(thread_id, ZW_MAP_VIEW_OF_SECTION));
		
		LARGE_INTEGER offset;
		PIN_SafeCopy(&offset, arg->SectionOffset(), sizeof(LARGE_INTEGER));

		PVOID base_address;
		PIN_SafeCopy(&base_address, arg->BaseAddress(), sizeof(PVOID));

		SIZE_T view_size;
		PIN_SafeCopy(&view_size, arg->ViewSize(), sizeof(SIZE_T));

		g_process_context.DefineTaintSource(thread_id, (ADDRINT)base_address, view_size, offset.LowPart);
		g_process_context.DeleteArgument(thread_id, ZW_MAP_VIEW_OF_SECTION);

		cerr << "[*] PostZwMapViewOfSection : " << hex << arg->BaseAddress() << ", " << hex << arg->SectionOffset() << ", " << hex << arg->ViewSize() << endl;
	}
}

VOID CheckArgumentTainted(THREADID thread_id,
							ADDRINT return_address,
							CONTEXT* ctxt,
							char* func_name,
							UINT32 arg_position)
{
	Taint taint;
	ADDRINT esp = PIN_GetContextReg(ctxt, REG_ESP);
	BOOL is_tainted = g_process_context.IsTaintedMemory(thread_id, esp + arg_position * 4, 4, taint);

	if (is_tainted)
	{
		cerr << hexstr(return_address) << " : " << func_name << "(" << taint.symbol << ")" << endl;
	}
}

VOID CheckRegisterTainted(THREADID thread_id,
						ADDRINT return_address,
						char* func_name,
						REG reg)
{
	Taint taint;
	BOOL is_tainted = g_process_context.IsTaintedRegister(thread_id, reg, taint);

	if (is_tainted)
	{
		cerr << hexstr(return_address) << " : " << func_name << "(" << taint.symbol << ")" << endl;
	}
}