#include "pch.h"
#include "CYGItem.h"
#include "CYGPlayer.h"
#include "CObjectManager.h"
#include "CScrollManager.h"

CYGItem::CYGItem():m_bCanPick(false), m_bDead(false)
{
}

void CYGItem::Initialize()
{
    m_eOBJID = OBJ_ITEM;
    m_eRender = RENDER_BACKGROUND;
}

int CYGItem::Update()
{

    return 0;
}

void CYGItem::Late_Update()
{
	m_bCanPick = false;
    OnCollision();
}

void CYGItem::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	if (m_bCanPick) {
		Rectangle(hDC, m_tInfo.vPos.x - 10 +iScrollX, m_tInfo.vPos.y - 10 + iScrollY, m_tInfo.vPos.x + 10 + iScrollX, m_tInfo.vPos.y + 10+iScrollY);
		//F 글자 출력
	}
}

void CYGItem::Release()
{
}

void CYGItem::OnCollision(CObject* _obj)
{
}

void CYGItem::OnCollision()
{
	/*RECT _copyRect = static_cast<CYGPlayer*>(CObjectManager::Get_Instance()->Get_Player())->Get_HitBox();

	float fRadius = (50 + m_tInfo.fSizeX) * 0.5f;

	float x = (_copyRect.right + _copyRect.left) * 0.5f;
	float y = (_copyRect.bottom + _copyRect.top) * 0.5f;

	float fWidth = abs(x - m_tInfo.vPos.x);
	float fHeight = abs(y - m_tInfo.vPos.y);

	float fDiagonal = powf((fWidth * fWidth + fHeight * fHeight), 0.5f);

	bool check = fRadius >= fDiagonal;

	if (check) {
		m_bCanPick = true;
	}*/
}
