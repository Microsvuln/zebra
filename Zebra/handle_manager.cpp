#include "handle_manager.h"

/**
 * ������ 
 */
HandleManager::HandleManager()
{
}

/**
 * �Ҹ���
 */
HandleManager::~HandleManager()
{

}

/**
 * �ش� �ڵ� ���
 * @param		handle		����� �ڵ�
 */
VOID HandleManager::SaveHandle(HANDLE handle)
{
	m_handles.Save(handle);
}

/**
 * �ڵ� ���� 
 * @param		handle		������ �ڵ�
 */
VOID HandleManager::DeleteHandle(HANDLE handle)
{
	m_handles.Delete(handle);
}

/**
 * �ڵ��� ���� ���θ� �Ǵ�
 * @param		handle		üũ�� �ڵ�
 * @return		�ڵ��� ���� ����
 */
BOOL HandleManager::ExistHandle(HANDLE handle)
{
	return m_handles.Exist(handle);
}

// SaveHandle�� ��
VOID HandleManager::SaveMappingHandle(HANDLE handle)
{
	m_mapping_handles.Save(handle);
}

// DeleteHandle�� ��
VOID HandleManager::DeleteMappingHandle(HANDLE handle)
{
	m_mapping_handles.Delete(handle);
}

// ExistHandle�� ��
BOOL HandleManager::ExistMappingHandle(HANDLE handle)
{
	return m_mapping_handles.Exist(handle);
}