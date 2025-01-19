#include "pch.h"
#include "CDWPlayer.h"
#include "CKeyManager.h"
#include "pch.h"
#include "CCYObject.h"
#include "CCollisionManager.h"
#include "CRoad.h"
#include "CObjectManager.h"

CDWPlayer::CDWPlayer()
{
}


void CDWPlayer::Initialize()
{
	m_eOBJID = OBJ_PLAYER;
	m_eRender = RENDER_GAMEOBJECT;


	m_tInfo.vPos = { 400.f, 500.f, 0.f };
	m_fSpeed = 2.f;
	m_tInfo.vLook = { 1.f, 0.f, 0.f };
	CDWPlayer::Initialize_OriginPoint(30, 30);

	m_pPlayer_ScaleX=1.f;
	m_pPlayer_ScaleY=1.f;
	m_pPlayer_ScaleZ=1.f;
	
	m_bFalling = false; 
	m_fFallSpeed = 0.f; // ���� �ӵ�
	m_fFallDepth = 50.f; // �����̿� ������ ����
}


int CDWPlayer::Update()
{
 
    if (m_bFalling)
    {
      
        static float rotationSpeed = D3DXToRadian(10.f); // �ʴ� 10�� ȸ��
        m_fAngle += rotationSpeed;

        m_tInfo.vPos.y += m_fFallSpeed;
        m_fFallSpeed += 0.5f; // �߷� ȿ��

        m_pPlayer_ScaleX -= 0.02f; // ���� �۾���
        m_pPlayer_ScaleY -= 0.02f;
        m_pPlayer_ScaleZ -= 0.02f;
         

        // ������ �۾����� ����..
        if (m_pPlayer_ScaleX <= 0.1f || m_pPlayer_ScaleY <= 0.1f)
        {
            m_pPlayer_ScaleX = 0.f;
            m_pPlayer_ScaleY = 0.f;
            m_pPlayer_ScaleZ = 0.f;

       
            m_bFalling = false;
            m_fFallSpeed = 0.f;
            m_fAngle = 0.f;
            m_tInfo.vPos.y = 500.f; // ���� ��ġ�� ����

        }
    }
    if (!CObjectManager::Get_Instance()->Get_ObjList_ByID(OBJ_DW_ROAD).empty())
    {
        auto sOb = CObjectManager::Get_Instance()->Get_ObjList_ByID(OBJ_DW_ROAD).front();
        auto& sOb_vevStruct = dynamic_cast<CRoad*>(sOb)->Get_obs();

        for (auto& sOb_Struct : sOb_vevStruct)
        {
            if (CCollisionManager::DW_Check_Coll(this, &sOb_Struct))
            {
                if (!m_bFalling) 
                {
                    m_bFalling = true;
                    m_fFallSpeed = 0.f; 
                    m_fAngle = 0.f; 
                }
                break;
            }
        }
    }


    D3DXMatrixScaling(&matScale, m_pPlayer_ScaleX, m_pPlayer_ScaleY, m_pPlayer_ScaleZ);
    D3DXMatrixRotationZ(&matRotZ, m_fAngle); 
    D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
    m_tInfo.matWorld = matScale * matRotZ * matTrans; 

    for (int i = 0; i < m_vOriginPointvec.size(); ++i)
    {
        D3DXVec3TransformCoord(&m_vPointvec[i], &m_vOriginPointvec[i], &m_tInfo.matWorld);
        m_pRenderPoint[i].x = m_vPointvec[i].x;
        m_pRenderPoint[i].y = m_vPointvec[i].y;
    }

    // ȭ�� ��ǥ ��ȯ
    float cameraZ = 500.f;
    float centerX = 400.f;
    float centerY = 300.f;

    for (int i = 0; i < (int)m_vPointvec.size(); ++i)
    {
        float distance = cameraZ + m_vPointvec[i].z;
        float factor = (cameraZ / distance);

        float screenX = centerX + (m_vPointvec[i].x * factor);
        float screenY = centerY - (m_vPointvec[i].y * factor);

        m_screenCorner[i].x = (LONG)screenX;
        m_screenCorner[i].y = (LONG)screenY;
    }

    return 0;
}



void CDWPlayer::Late_Update()
{
	
}
void CDWPlayer::Render(HDC hDC)
{
	if (g_bDevmode) 
	{
		HitCircle(hDC, m_tHitRect, 0, 0);
	}
	HBRUSH PinkBrush = CreateSolidBrush(RGB(255, 220, 220));
	HBRUSH OldBrush = (HBRUSH)SelectObject(hDC, PinkBrush);

	Polygon(hDC, m_pRenderPoint, m_vOriginPointvec.size());
	SelectObject(hDC, OldBrush); DeleteObject(PinkBrush);

	Ellipse(hDC, m_pRenderPoint[20].x - 5, m_pRenderPoint[20].y - 5, m_pRenderPoint[20].x + 5, m_pRenderPoint[20].y + 5);

	Ellipse(hDC, m_pRenderPoint[25].x - 5, m_pRenderPoint[25].y - 5, m_pRenderPoint[25].x + 5, m_pRenderPoint[25].y + 5);

	//TCHAR m_szBuf[100] = {};
	//swprintf_s(m_szBuf, L"�÷��̾� x : %.f, �÷��̾� y : %.f", m_tInfo.vPos.x, m_tInfo.vPos.y);
	//TextOut(hDC, 300, 5, m_szBuf, lstrlen(m_szBuf));




}
void CDWPlayer::Release()
{
}

void CDWPlayer::OnCollision(CObject* _obj)
{
}

void CDWPlayer::Key_Input()
{
	
	//if (GetAsyncKeyState('A'))
	//{
	//	m_tInfo.vDir = { -m_fSpeed, 0.f, 0.f };

	//	D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vDir, &m_tInfo.matWorld);
	//	m_tInfo.vPos += m_tInfo.vDir;
	//}
	//if (GetAsyncKeyState('D'))
	//{
	//	m_tInfo.vDir = { m_fSpeed, 0.f, 0.f };

	//	D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vDir, &m_tInfo.matWorld);
	//	m_tInfo.vPos += m_tInfo.vDir;
	//}
}

