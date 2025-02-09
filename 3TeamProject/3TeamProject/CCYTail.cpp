#include "pch.h"
#include "CCYTail.h"
#include "Define.h"
#include "CCollisionManager.h"
#include "CCYPlayer.h"
#include "CObjectManager.h"
#include "CAbstractFactory.h"


void CCYTail::Initialize()
{
	m_eOBJID = OBJ_CYTAIL;
	m_eRender = RENDER_GAMEOBJECT;

	
}

int CCYTail::Update()
{
	

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CCYTail::Late_Update()
{
	
}

void CCYTail::Render(HDC hDC)
{
	

}

void CCYTail::Release()
{
	
}

void CCYTail::OnCollision(CObject* _obj)
{
	//if (_obj == m_targetHead)
	//{
	//	return;
	//}
	//else
	//{
	//	if (_obj == GET_PLAYER)
	//	{
	//		static_cast<CCYObject*>(GET_PLAYER)->Set_Dead();
	//	}
	//	else
	//	{
	//		static_cast<CCYObject*>(_obj)->Set_Dead();
	//	}
	//	//static_cast<CCYObject*>(m_targetHead)->Set_Dead();
	//	//return;
	//}
	////if (_obj == GET_PLAYER)
	////{
	////	if (m_targetHead != GET_PLAYER)
	////	{
	////		static_cast<CCYObject*>(GET_PLAYER)->Set_Dead();
	////	}
	////}
	////else
	////{
	////	if (m_targetHead == GET_PLAYER)
	////	{
	////		static_cast<CCYObject*>(m_targetHead)->Set_Dead();
	////	}
	////}
}
