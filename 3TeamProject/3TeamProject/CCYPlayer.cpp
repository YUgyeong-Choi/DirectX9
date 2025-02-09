#include "pch.h"
#include "CCYPlayer.h"
#include "CCYTail.h"
#include "Define.h"
#include "CKeyManager.h"
#include "CCollisionManager.h"
#include "CObjectManager.h"
#include "CSoundManager.h"

CCYPlayer::CCYPlayer() 
{
}

CCYPlayer::~CCYPlayer()
{
    Release();
}

void CCYPlayer::Initialize()
{
	m_eOBJID = OBJ_PLAYER;
	m_eRender = RENDER_GAMEOBJECT;
	
}

int CCYPlayer::Update()
{
	
    return 0;
}

void CCYPlayer::Late_Update()
{
	
}

void CCYPlayer::Render(HDC hDC)
{
	
}

void CCYPlayer::Release()
{
	
}





