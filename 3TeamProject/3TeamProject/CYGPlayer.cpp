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
#include "CSoundManager.h"

CYGPlayer::CYGPlayer()
{

}

void CYGPlayer::Initialize()
{
	m_eOBJID = OBJ_PLAYER;
	m_eRender = RENDER_GAMEOBJECT;

}

int CYGPlayer::Update()
{
	return 0;
}

void CYGPlayer::Late_Update()
{
}

void CYGPlayer::Render(HDC hDC)
{
}

void CYGPlayer::Release()
{
}

void CYGPlayer::OnCollision(CObject* _obj)
{
}
