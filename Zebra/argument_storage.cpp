#include "argument_storage.h"

#include <assert.h>
#include <stdio.h>

// ������
ArgumentStorage::ArgumentStorage()
{
	memset(m_arg, 0, sizeof(m_arg));
}

// �Ҹ���
ArgumentStorage::~ArgumentStorage()
{
	int i = 0;

	for (i = 0 ; i < FUNCTION_TYPE_MAX; i++)
	{
		if (m_arg[i] != NULL)
			delete m_arg[i];
	}
}

/**
 * �ش� �Լ��� ������ ���� ���θ� Ȯ��
 * @param	func_type	�Լ��� Ÿ��
 * @return	/DTARGET_IA32 		�Լ��� ���ڰ� ����Ǿ������� TRUE, �ƴ� ��� FALSE
 */
BOOL ArgumentStorage::Exist(FUNCTION_TYPE func_type)
{
	BOOL result = (m_arg[func_type] != NULL);

	return result;
}

/**
 * �ش� �Լ��� ���ڸ� ����
 * @param	func_type	�Լ��� Ÿ��
 */
VOID ArgumentStorage::Delete(FUNCTION_TYPE func_type)
{
	if (m_arg[func_type] != NULL)
	{
		delete m_arg[func_type];
		m_arg[func_type] = NULL;
	}
}

/**
 * �ش� �Լ��� ���ڸ� ����
 * @param	func_type	�Լ��� Ÿ��
 * @param	arg			�Լ��� ����
 */
VOID ArgumentStorage::Save(FUNCTION_TYPE func_type, Argument* arg)
{
	if (m_arg[func_type] != NULL)
		Delete(func_type);

	m_arg[func_type] = arg;
}

/**
 * �ش� �Լ��� ���ڸ� ����
 * @param	func_type	�Լ��� Ÿ��
 * @return	Argument*	�Լ��� ���� 
 */
Argument* ArgumentStorage::Read(FUNCTION_TYPE func_type)
{
	return m_arg[func_type];
}