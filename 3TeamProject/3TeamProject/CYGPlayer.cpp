#include "pch.h"
#include "CYGPlayer.h"
#include "CKeyManager.h"
#include "CAbstractFactory.h"
#include "CObjectManager.h"
#include "CYGBullet.h"
#include "CScrollManager.h"
#include "CYGBulletItem.h"
#include "CYGGunItem.h"
#include "CYGItem.h"
#include "CYGBulletBox.h"
#include "CSoundManager.h"

CYGPlayer::CYGPlayer():m_bLeftPush(false), m_iShootTick(0), m_bHaveGun(false), m_iBulletNum(0), m_footStepTick(0)
{
}

void CYGPlayer::Initialize()
{
	m_eOBJID = OBJ_PLAYER;
	m_eRender = RENDER_GAMEOBJECT;

	m_fSpeed = 3.f;
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_PlayerState = PS_NOGUN;

	m_vLeftNoGunHandPos = { 65.f,270.f,0.f };
	m_vRightNoGunHandPos = { 95.f,270.f,0.f };
	m_vOriginLeftNoGunHand = m_vLeftNoGunHandPos; 
	m_vOriginRightNoGunHand = m_vRightNoGunHandPos;

	m_vLeftGunHandPos = {75.f, 240.f, 0.f};
	m_vRightGunHandPos = {85.f, 270.f, 0.f};
	m_vOriginLeftGunHand = m_vLeftGunHandPos;
	m_vOriginRightGunHand = m_vRightGunHandPos;

	m_vGunRectanglePoint[0] = {75.f, 210.f, 0.f};
	m_vGunRectanglePoint[1] = {85.f, 210.f, 0.f};
	m_vGunRectanglePoint[2] = {85.f, 280.f, 0.f};
	m_vGunRectanglePoint[3] = {75.f, 280.f, 0.f};

	m_vBulletSpawn = { 80.f, 210.f, 0.f };
	m_vOriginBulletSpawn = m_vBulletSpawn;

	for (int i = 0; i < 4; ++i) {
		m_vOriginGunRectanglePoint[i] = m_vGunRectanglePoint[i];
	}

	m_vOriginPos = m_tInfo.vPos;

	m_iHp = 100;
	m_iMaxHp = m_iHp;
	m_iShootTick = 0;

	m_CollisionBox = { 0,0,0,0};
}

int CYGPlayer::Update()
{
	m_footStepTick++;
	m_CollisionBox = { 0,0,0,0 };
	m_iShootTick++;
	m_tInfo.vLook = Get_Mouse() - m_tInfo.vPos;
	D3DXVec3Normalize(&m_tInfo.vLook, &m_tInfo.vLook);
	m_fAngle = D3DXToDegree(atan2f(m_tInfo.vLook.y, m_tInfo.vLook.x));

	if (m_fAngle < 0) {
		m_fAngle += 360;
	}
	m_fAngle += 100;

	D3DXMATRIX		matScale, matRotZ, matTrans;

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixRotationZ(&matRotZ, m_fAngle/60);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, m_tInfo.vPos.z);

	m_tInfo.matWorld = matScale * matRotZ * matTrans;


	m_vLeftNoGunHandPos = m_vOriginLeftNoGunHand;
	m_vLeftNoGunHandPos -= {80.f, 300.f, 0.f};
	D3DXVec3TransformCoord(&m_vLeftNoGunHandPos, &m_vLeftNoGunHandPos, &m_tInfo.matWorld);

	m_vRightNoGunHandPos = m_vOriginRightNoGunHand;
	m_vRightNoGunHandPos -= {80.f, 300.f, 0.f};
	D3DXVec3TransformCoord(&m_vRightNoGunHandPos, &m_vRightNoGunHandPos, &m_tInfo.matWorld);

	m_vLeftGunHandPos = m_vOriginLeftGunHand;
	m_vLeftGunHandPos -= {80.f, 300.f, 0.f};
	D3DXVec3TransformCoord(&m_vLeftGunHandPos, &m_vLeftGunHandPos, &m_tInfo.matWorld);

	m_vRightGunHandPos = m_vOriginRightGunHand;
	m_vRightGunHandPos -= {80.f, 300.f, 0.f};
	D3DXVec3TransformCoord(&m_vRightGunHandPos, &m_vRightGunHandPos, &m_tInfo.matWorld);

	for (int i = 0; i < 4; ++i) {
		m_vGunRectanglePoint[i] = m_vOriginGunRectanglePoint[i];
		m_vGunRectanglePoint[i] -= {80.f, 300.f, 0.f};
		D3DXVec3TransformCoord(&m_vGunRectanglePoint[i], &m_vGunRectanglePoint[i], &m_tInfo.matWorld);
	}

	m_vBulletSpawn = m_vOriginBulletSpawn;
	m_vBulletSpawn -= {80.f, 300.f, 0.f};
	D3DXVec3TransformCoord(&m_vBulletSpawn, &m_vBulletSpawn, &m_tInfo.matWorld);


	

	__super::Update_Rect();

	Key_Input();
	return 0;
}

void CYGPlayer::Late_Update()
{
}

void CYGPlayer::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	if (m_PlayerState == PS_NOGUN) {
		ColorCircle(hDC, m_vLeftNoGunHandPos.x - 10 + iScrollX, m_vLeftNoGunHandPos.y - 10+ iScrollY, m_vLeftNoGunHandPos.x + 10+ iScrollX, m_vLeftNoGunHandPos.y + 10+ iScrollY, 252,194,114,2); // �޼�
		ColorCircle(hDC, m_vRightNoGunHandPos.x - 10+ iScrollX, m_vRightNoGunHandPos.y - 10+ iScrollY, m_vRightNoGunHandPos.x + 10+ iScrollX, m_vRightNoGunHandPos.y + 10+ iScrollY, 252, 194, 114,2); // ������
	}
	else {
		POINT point[4];
		point[0] = { (long)m_vGunRectanglePoint[0].x, (long)m_vGunRectanglePoint[0].y };
		point[1] = { (long)m_vGunRectanglePoint[1].x, (long)m_vGunRectanglePoint[1].y };
		point[2] = { (long)m_vGunRectanglePoint[2].x, (long)m_vGunRectanglePoint[2].y };
		point[3] = { (long)m_vGunRectanglePoint[3].x, (long)m_vGunRectanglePoint[3].y };
		ColorPolygon(hDC, point, 4, 111,70,49, 1);

		ColorCircle(hDC, m_vLeftGunHandPos.x - 10+ iScrollX, m_vLeftGunHandPos.y - 10+ iScrollY, m_vLeftGunHandPos.x + 10+ iScrollX, m_vLeftGunHandPos.y + 10+ iScrollY, 252, 194, 114, 2); // �޼�
		ColorCircle(hDC, m_vRightGunHandPos.x - 10+ iScrollX, m_vRightGunHandPos.y - 10+ iScrollY, m_vRightGunHandPos.x + 10+ iScrollX, m_vRightGunHandPos.y + 10+ iScrollY, 252, 194, 114, 2); // ������
	}

	ColorCircle(hDC, m_tHitRect.left+ iScrollX, m_tHitRect.top+ iScrollY, m_tHitRect.right+ iScrollX, m_tHitRect.bottom+ iScrollY, 252, 194, 114,2);


	if (g_bDevmode) {
		HitCircle(hDC, m_tHitRect, iScrollX, iScrollY);
		HitCircle(hDC, m_CollisionBox, iScrollX, iScrollY);
		if (g_bDevmode) {
			TCHAR szWhoScene[64];
			_stprintf_s(szWhoScene, _T("%d %d %d"), iScrollX, iScrollY, m_iBulletNum);
			SetTextColor(hDC, RGB(0, 0, 0));
			TextOut(hDC, 350, 10, szWhoScene, _tcslen(szWhoScene));
		}
	}
}

void CYGPlayer::Release()
{
}

void CYGPlayer::OnCollision(CObject* _obj)
{
}

void CYGPlayer::Key_Input()
{
	if (CKeyManager::Get_Instance()->Key_Pressing('W')) {
		if (m_footStepTick > 10) {
			CSoundManager::GetInstance()->PlayEffect("YGfootstep");
			m_footStepTick = 0;
		}
		m_tInfo.vDir = { 0.f, -1.f, 0.f };
		m_tInfo.vPos += m_tInfo.vDir * m_fSpeed;
		m_vOriginPos += m_tInfo.vDir * m_fSpeed;
	}

	if (CKeyManager::Get_Instance()->Key_Pressing('S')) {
		if (m_footStepTick > 10) {
			CSoundManager::GetInstance()->PlayEffect("YGfootstep");
			m_footStepTick = 0;
		}
		m_tInfo.vDir = { 0.f, 1.f, 0.f };
		m_tInfo.vPos += m_tInfo.vDir * m_fSpeed;
		m_vOriginPos += m_tInfo.vDir * m_fSpeed;
	}

	if (CKeyManager::Get_Instance()->Key_Pressing('A')) {
		if (m_footStepTick > 10) {
			CSoundManager::GetInstance()->PlayEffect("YGfootstep");
			m_footStepTick = 0;
		}
		m_tInfo.vDir = { -1.f, 0.f, 0.f };
		m_tInfo.vPos += m_tInfo.vDir * m_fSpeed;
		m_vOriginPos += m_tInfo.vDir * m_fSpeed;
	}

	if (CKeyManager::Get_Instance()->Key_Pressing('D')) {
		if (m_footStepTick > 10) {
			CSoundManager::GetInstance()->PlayEffect("YGfootstep");
			m_footStepTick = 0;
		}
		m_tInfo.vDir = { 1.f, 0.f, 0.f };
		m_tInfo.vPos += m_tInfo.vDir * m_fSpeed;
		m_vOriginPos += m_tInfo.vDir * m_fSpeed;
	}

	if (CKeyManager::Get_Instance()->Key_Down('1')) {
		m_PlayerState = PS_NOGUN;
	}

	if (CKeyManager::Get_Instance()->Key_Down('2')) {
		if (m_bHaveGun) {
			m_PlayerState = PS_GUN;
		}
	}

	if (CKeyManager::Get_Instance()->Key_Down(VK_LBUTTON)) {
		if (m_PlayerState == PS_NOGUN) {
			float radian = D3DXToRadian(m_fAngle);
			if (m_bLeftPush) {
				m_vLeftNoGunHandPos += m_tInfo.vLook * 10;
				m_bLeftPush = !m_bLeftPush;
				m_CollisionBox = { (int)m_vLeftNoGunHandPos.x - 10, (int)m_vLeftNoGunHandPos.y - 10,(int)m_vLeftNoGunHandPos.x + 10, (int)m_vLeftNoGunHandPos.y + 10 };
			}
			else {
				m_vRightNoGunHandPos += m_tInfo.vLook * 10;
				m_bLeftPush = !m_bLeftPush;
				m_CollisionBox = { (int)m_vRightNoGunHandPos.x - 10, (int)m_vRightNoGunHandPos.y - 10,(int)m_vRightNoGunHandPos.x + 10, (int)m_vRightNoGunHandPos.y + 10 };
			}
		}
		else {
			if (m_iShootTick > 10) {
				if (m_iBulletNum > 0) {
					m_iBulletNum--;
					CObjectManager::Get_Instance()->Add_Object(OBJ_PLAYERBULLET, CAbstractFactory<CYGBullet>::Create(m_vBulletSpawn.x, m_vBulletSpawn.y));
					static_cast<CYGBullet*>(CObjectManager::Get_Instance()->Get_ObjList_ByID(OBJ_PLAYERBULLET).back())->Set_Dir(m_tInfo.vLook);
					m_iShootTick = 0;
					CSoundManager::GetInstance()->PlayEffect("YGFire");
				}
				else {
					CSoundManager::GetInstance()->PlayEffect("YGEmptyfire");
				}
			}
		}
	}

	if (CKeyManager::Get_Instance()->Key_Down('R')) {
		CObjectManager::Get_Instance()->Add_Object(OBJ_MAP, CAbstractFactory<CYGBulletBox>::Create(300, 300, 50, 50));
	}

	if (CKeyManager::Get_Instance()->Key_Down('F')) {
		list<CObject*> _itemList = CObjectManager::Get_Instance()->Get_ObjList_ByID(OBJ_ITEM);
		for (auto& _obj : _itemList) {
			switch (static_cast<CYGItem*>(_obj)->Get_ItemType())
			{
			case ITEM_GUN:
				if (static_cast<CYGItem*>(_obj)->Get_CanPick()) {
					static_cast<CYGItem*>(_obj)->Set_Dead();
					m_bHaveGun = true;
				}
				break;
			case ITEM_BULLET:
				if (static_cast<CYGItem*>(_obj)->Get_CanPick()) {
					static_cast<CYGItem*>(_obj)->Set_Dead();
					m_iBulletNum += 5;
				}
				break;
			}
		}

	}

}
