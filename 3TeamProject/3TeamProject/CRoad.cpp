#include "pch.h"
#include "CRoad.h"
#include "CKeyManager.h"
#include "CObjectManager.h"

bool CRoad::m_bLeft_Rotation = false;
bool CRoad::m_bRight_Rotation = false;
bool CRoad::m_bTop_Rotation = false;
bool CRoad::m_bBottom_Rotation = false;

CRoad::CRoad():
	m_bJumping(false),
	m_fJumpSpeed(0.f),
	m_fGravity(0.f),
 m_fJumpOffsetY(0.f),
	m_bFirst_Check(false),
	m_dwPrevSpawnTime(0),
	m_fTargetAngle(0.f),
	m_fTargetX(0.f),
	m_pPlayer(nullptr),
	m_fPlayerStartY(0.f),
	m_fCheck_Rotation_Angle(0.f),
	m_bRelease_Check(false)


{
	ZeroMemory(&hBmp, sizeof(hBmp));
	ZeroMemory(&hPatternBrush, sizeof(hPatternBrush));


}

void CRoad::Initialize()
{
	m_eOBJID = OBJ_DW_ROAD;
	m_eRender = RENDER_BACKGROUND;

	m_tInfo.vPos = { 400.f, -900.f, 0.f };
	m_fSpeed = 10.f;
	m_tInfo.vLook = { 1.f, 0.f, 0.f };
	m_fAngle = 0.f;
	m_fTargetAngle = 0.f; // �ʱ� ��ǥ ���� ����

	m_vCorner[0] = { -1500, -1500, 0 }; // ������
	m_vCorner[1] = { 1500, -1500, 0 };  // ��������
	m_vCorner[2] = { 1500, 1500, 0 };   // ������ �Ʒ�
	m_vCorner[3] = { -1500, 1500, 0 };  // ���� �Ʒ�

	Sosil[0] = { 350.f, 250.f, 0.f }; // ���� ��
	Sosil[1] = { 450.f, 250.f, 0.f }; // ������ ��
	Sosil[2] = { 450.f, 350.f, 0.f }; // ������ �Ʒ�
	Sosil[3] = { 350.f, 350.f, 0.f }; // ���� �Ʒ�

	m_vecPoints.push_back(new POINT[4]{}); // ���ʸ�
	m_vecPoints.push_back(new POINT[4]{}); // �����ʸ�
	m_vecPoints.push_back(new POINT[4]{}); // ����
	m_vecPoints.push_back(new POINT[4]{}); // �Ʒ���
	m_vecPoints.push_back(new POINT[4]{}); // �߾Ӹ�


	Wall_Update();

	m_pPlayer = CObjectManager::Get_Instance()->Get_ObjList_ByID(OBJ_PLAYER).front();

	m_bFirst_Check = false;

	float halfWidth = 50.f;

	m_dwPrevSpawnTime = GetTickCount64();
	g_ullPlayTime = GetTickCount64();
	
	m_bJumping = false;
	m_fJumpSpeed = 0.f;
	m_fGravity = 0.6f;     
	m_fJumpOffsetY = 0.f;

	hBmp = (HBITMAP)LoadImage(
		nullptr,
		L"../Assets/Back/Space.bmp",
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE
	);
m_fPlayerStartY = m_pPlayer->Get_Info().vPos.y;

	hPatternBrush = CreatePatternBrush(hBmp);

	m_bLeft_Rotation = false;
	m_bRight_Rotation = false;
	m_bTop_Rotation = false;
	m_bBottom_Rotation = false;

	ullSecond = 0.f;
}

int CRoad::Update()
{
	if (m_vecPoints.empty() == true)
		return 0;

	Key_Input();

	if (m_bJumping)
	{
		m_fJumpOffsetY += m_fJumpSpeed;  // Y�� ������ ����
		m_fJumpSpeed -= m_fGravity;    // �߷� ����

		if (m_fJumpOffsetY <= 0.f) // �ٴڿ� �������� ��
		{
			m_fJumpOffsetY = 0.f;
			m_fJumpSpeed = 0.f;
			m_bJumping = false;

			D3DXVECTOR3 playerPos = m_pPlayer->Get_Info().vPos;
			playerPos.y = m_fPlayerStartY;
			m_pPlayer->Set_Pos(playerPos.x, playerPos.y);
		}
		else
		{
			D3DXVECTOR3 playerPos = m_pPlayer->Get_Info().vPos;
			playerPos.y = m_fPlayerStartY - m_fJumpOffsetY;
			m_pPlayer->Set_Pos(playerPos.x, playerPos.y);
		}
	}





	if (abs(m_fTargetAngle - m_fAngle) > D3DXToRadian(1.f)) // ���밪 �����
	{
		if ((m_fTargetAngle > m_fAngle)) // ��ǥ���� 
			m_fAngle += D3DXToRadian(5.f);
		else if ((m_fTargetAngle < m_fAngle))
			m_fAngle -= D3DXToRadian(5.f);
	}

	float finalY = m_tInfo.vPos.y + m_fJumpOffsetY;

	D3DXMatrixRotationZ(&matRotZ, m_fAngle);


	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, finalY, m_tInfo.vPos.z);

	WorldMat = matRotZ * matTrans;

	for (int i = 0; i < 4; i++)
	{
		D3DXVec3TransformCoord(&transformedCorners[i], &m_vCorner[i], &WorldMat);
	}


	D3DXVECTOR3 sosilCenter =
	{
		(Sosil[0].x + Sosil[2].x) / 2.f,
		(Sosil[0].y + Sosil[2].y) / 2.f,
		0.f
	};

	float rotationAngle = m_fAngle; // ī�޶� �ޱ� �ְ�

	for (int i = 0; i < 4; i++)
	{
		D3DXVECTOR3 relativePoint = { Sosil[i].x - sosilCenter.x, Sosil[i].y - sosilCenter.y, 0.f };
		float rotatedX = relativePoint.x * cos(rotationAngle) - relativePoint.y * sin(rotationAngle);
		float rotatedY = relativePoint.x * sin(rotationAngle) + relativePoint.y * cos(rotationAngle);

		rotatedSosil[i].x = rotatedX + sosilCenter.x;
		rotatedSosil[i].y = rotatedY + sosilCenter.y;
		rotatedSosil[i].z = 0.f;
	}



	m_tInfo.vPos.z += m_fSpeed;

	//// ��ΰ� �������� ����� �ٽ� ������ �̵�
	//if (m_tInfo.vPos.z > 1000.f)
	//{
	//	m_tInfo.vPos.z = -1000.f;
	//}


	Wall_Update();
	Update_Obstacles();
	DWORD dwCurTime = GetTickCount64();
	if (dwCurTime - m_dwPrevSpawnTime >= 1000)
	{
		Spawn_Obstacle();
		m_dwPrevSpawnTime = dwCurTime;
	}
	//__super::Update_Rect();
	return 0;

}

void CRoad::Update_Obstacles()
{
	if (m_vecPoints.empty() == true)
		return;

	// ��ֹ��� z������ (ī�޶� ������) ��� �ٰ����� �����...
	float fSpeed = 10.f;  
	for (auto& obs : m_Obstacles)
	{
		obs.vPos.z -= fSpeed; // ī�޶� z=0�� �ִٰ� �����ؾߵǴµ�,,

		// ���� obs.vPos.z >= 0 �� �Ǹ�, ȭ�鿡 �����ߴٰ� ������ ���� ����.
		// �浹 ����, �Ҹ�, ����� �� ������ �߰��ϸ� �Ƿ���
		//z���� ���������


		if (obs.vPos.z < -500)
			m_Obstacles.erase(m_Obstacles.begin());
	}

}
void CRoad::Render_Obstacles(HDC hDC)
{
	if (m_vecPoints.empty() == true)
		return;

	float fCameraX = m_tInfo.vPos.x; // �갡 ī�޶��� x��

	float fScaleFactor = 0.002f;

	float cameraZ = 500.f;  // ī�޶��~ �̴ϼҽǳ׸���� �Ÿ�
	float centerX = 400.f;  
	float centerY = 300.f;  
	for (auto& obs : m_Obstacles)
	{
		//--------------------------------------------------
		// �簢�� ������ �����ֱ�(�߽� ���� ����)
		//--------------------------------------------------
		float halfW = obs.fWidth * 0.5f;
		float halfH = obs.fHeight * 0.5f;

		// (0,0) ���� �� ����, ����, ���, �»�
		/*D3DXVECTOR3 localCorner[4] =
		{
			{ -halfW, -halfH, 0.f },
			{ +halfW, -halfH, 0.f },
			{ +halfW, +halfH, 0.f },
			{ -halfW, +halfH, 0.f },
		}; ���簢��*/

		//��ٸ��� ��� ��°Ű�
		D3DXVECTOR3 localCorner[4] =
		{
			{ -60.f, -20.f, 0.f },  //������
			{ +60.f, -20.f, 0.f },  // ��������
			{ +60.f, +20.f, 0.f },  // ������ �Ʒ�
			{ -60.f, +20.f, 0.f },  // ���� �Ʒ�
		};
		D3DXMATRIX matScale, matRot, matTrans, matWorld;

		(fScaleFactor * fCameraX);

		obs.fScale = 4.0f;

		D3DXMatrixScaling(&matScale, obs.fScale, obs.fScale, 1.f);
		// Z�� ȸ��
		D3DXMatrixRotationZ(&matRot, obs.fAngle);

		float x = 0.f;
		float y = 0.f;
		if ((m_fCheck_Rotation_Angle = D3DXToDegree(m_fTargetAngle)==-90.f)) // ��ֹ��� ������ ������
		{
			x = -obs.vPos.y;
			y = obs.vPos.x;

			obs.fAngle = D3DXToRadian(90.f); // 90�� ȸ��
		}
		else if (m_fCheck_Rotation_Angle = D3DXToDegree(m_fTargetAngle)==-180.f) // ��ֹ��� ������ ������ �̵��ѻ��¿���
		{                                                                        // �� ���������� 
			x = -obs.vPos.x; 
			y = -obs.vPos.y;
			obs.fAngle = D3DXToRadian(180.f); // 180�� ȸ��
		}
		else if (m_fCheck_Rotation_Angle = D3DXToDegree(m_fTargetAngle) == -270.f) // ��ֹ��� ������ ������ �̵��ѻ��¿���
		{																		   // �����ʿ��� ������
			y = -obs.vPos.x;
			x = obs.vPos.y;
			obs.fAngle = D3DXToRadian(270.f);

		}
		else  // �ٽ� �ٴ�
		{
			x = obs.vPos.x;
			y = obs.vPos.y;
			obs.fAngle = D3DXToRadian(0.f);
	
			m_fTargetAngle = D3DXToRadian(0.f);
		}


		if ((m_fCheck_Rotation_Angle = D3DXToDegree(m_fTargetAngle) == 90.f)) // ��ֹ��� ������ ������
		{
			x = -obs.vPos.y;
			y = obs.vPos.x;

			obs.fAngle = D3DXToRadian(-90.f); // 90�� ȸ��
		}
		/*if (m_bRight_Rotation)
		{
			x = -obs.vPos.y;
			y = obs.vPos.x;

			obs.fAngle = D3DXToRadian(90.f);
		}
		else
		{

			x = obs.vPos.x;
			y = obs.vPos.y;
		}
		*/

		//TCHAR m_szBuf2[100] = {};
		//swprintf_s(m_szBuf2, L"��ֹ� ������ ũ�� : %f",obs.fScale);
		//TextOut(hDC, 300, 100, m_szBuf2, lstrlen(m_szBuf2));



		

		D3DXMatrixTranslation(&matTrans, x, y, obs.vPos.z);

		matWorld = matScale * matRot * matTrans;

		for (int i = 0; i < 4; ++i)
		{
			D3DXVec3TransformCoord(&obs.worldCorner[i], &localCorner[i], &matWorld);

		}

		float screenX=0.f;
		float screenY=0.f;


		POINT pt[4];
		for (int i = 0; i < 4; ++i)
		{
			float distance = cameraZ + obs.worldCorner[i].z;
			float factor = (cameraZ / distance);

			float screenX = centerX + obs.worldCorner[i].x * factor;
			float screenY = centerY - obs.worldCorner[i].y * factor;

			// ��ũ�� ��ǥ ����
			pt[i].x = (LONG)screenX;
			pt[i].y = (LONG)screenY;

			// �浹�� ��ũ�� ��ǥ�� ����
			obs.screenCorner[i].x = screenX;
			obs.screenCorner[i].y = screenY;

		}

		/*
		if (!m_Obstacles.empty())
		{
			// ù ��° ��ֹ�
			auto& firstObs = m_Obstacles.front(); // ù ��ֹ�
			TCHAR buf[100];
			swprintf_s(buf, L"ù ��ֹ� ���� �� X=%.2f", firstObs.screenCorner[0].x);
			TextOut(hDC, 300, 130, buf, lstrlen(buf));
			swprintf_s(buf, L"ù ��ֹ� ���� �� y=%.2f", firstObs.screenCorner[0].y);
			TextOut(hDC, 300, 150, buf, lstrlen(buf));

			swprintf_s(buf, L"ù ��ֹ� ������ �� X=%.2f", firstObs.screenCorner[1].x);
			TextOut(hDC, 300, 170, buf, lstrlen(buf));
			swprintf_s(buf, L"ù ��ֹ� ������ �� y=%.2f", firstObs.screenCorner[1].y);
			TextOut(hDC, 300, 190, buf, lstrlen(buf));

			swprintf_s(buf, L"ù ��ֹ� ������ �Ʒ� X=%.2f", firstObs.screenCorner[2].x);
			TextOut(hDC, 300, 210, buf, lstrlen(buf));
			swprintf_s(buf, L"ù ��ֹ� ������ �Ʒ� y=%.2f", firstObs.screenCorner[2].y);
			TextOut(hDC, 300, 230, buf, lstrlen(buf));

			swprintf_s(buf, L"ù ��ֹ� ���� �Ʒ� X=%.2f", firstObs.screenCorner[3].x);
			TextOut(hDC, 300, 250, buf, lstrlen(buf));
			swprintf_s(buf, L"ù ��ֹ� ���� �Ʒ� y=%.2f", firstObs.screenCorner[3].y);
			TextOut(hDC, 300, 270, buf, lstrlen(buf));
		}

		HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
		HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

		*/




		/*
		TCHAR m_szBuf6[100] = {};
		swprintf_s(m_szBuf6, L"��ֹ�  z ��ǥ : %f", m_Obstacles[0].vPos.z);
		TextOut(hDC, 100, 170, m_szBuf6, lstrlen(m_szBuf6));
		*/

		HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hPatternBrush);

		Polygon(hDC, pt, 4);
		SelectObject(hDC, hOldBrush);

		//SelectObject(hDC, hOldPen);
		//DeleteObject(hPen);


	
	}
}

void CRoad::Render(HDC hDC)
{
	// ī�޶� ȭ��
	if (m_vecPoints.empty() == true)
		return;
	MoveToEx(hDC, transformedCorners[0].x, transformedCorners[0].y, nullptr);
	LineTo(hDC, transformedCorners[1].x, transformedCorners[1].y);
	LineTo(hDC, transformedCorners[2].x, transformedCorners[2].y);
	LineTo(hDC, transformedCorners[3].x, transformedCorners[3].y);
	LineTo(hDC, transformedCorners[0].x, transformedCorners[0].y);
	// �ҽ��� �׸� 
	MoveToEx(hDC, rotatedSosil[0].x, rotatedSosil[0].y, nullptr);
	LineTo(hDC, rotatedSosil[1].x, rotatedSosil[1].y);
	LineTo(hDC, rotatedSosil[2].x, rotatedSosil[2].y);
	LineTo(hDC, rotatedSosil[3].x, rotatedSosil[3].y);
	LineTo(hDC, rotatedSosil[0].x, rotatedSosil[0].y);



	HPEN hPen = CreatePen(PS_SOLID, 3, RGB(128, 128, 128));
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);



	// ó�� ���� ���� ��
	HBRUSH GrayBrush = CreateSolidBrush(RGB(128, 128, 128));
	HBRUSH OldBrush = (HBRUSH)SelectObject(hDC, GrayBrush);


	Polygon(hDC, m_vecPoints[0], 4);

	SelectObject(hDC, OldBrush);
	DeleteObject(GrayBrush);

	// ó�� ���� ������ ��
	HBRUSH GrayBrush1 = CreateSolidBrush(RGB(128, 128, 128));
	HBRUSH OldBrush1 = (HBRUSH)SelectObject(hDC, GrayBrush1);

	Polygon(hDC, m_vecPoints[1], 4);

	SelectObject(hDC, OldBrush1);
	DeleteObject(GrayBrush1);

	// ó�� ���� �� ��
	HBRUSH GrayBrush2 = CreateSolidBrush(RGB(128, 128, 128));
	HBRUSH OldBrush2 = (HBRUSH)SelectObject(hDC, GrayBrush2);

	Polygon(hDC, m_vecPoints[2], 4);

	SelectObject(hDC, OldBrush2);
	DeleteObject(GrayBrush2);

	HBRUSH GrayBrush3 = CreateSolidBrush(RGB(128, 128, 128));
	HBRUSH OldBrush3 = (HBRUSH)SelectObject(hDC, GrayBrush3);

	Polygon(hDC, m_vecPoints[3], 4);

	SelectObject(hDC, OldBrush3);
	DeleteObject(GrayBrush3);

	HBRUSH GrayBrush4 = CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH OldBrush4 = (HBRUSH)SelectObject(hDC, GrayBrush4);

	Polygon(hDC, m_vecPoints[4], 4);

	SelectObject(hDC, OldBrush4);
	DeleteObject(GrayBrush4);


	HBRUSH GrayBrush5 = CreateSolidBrush(RGB(128, 128, 128));
	HBRUSH OldBrush5 = (HBRUSH)SelectObject(hDC, GrayBrush5);


	SelectObject(hDC, hOldPen); DeleteObject(hPen);
	SelectObject(hDC, OldBrush5);
	DeleteObject(GrayBrush5);


	if (g_bDevmode)
	{
		HitCircle(hDC, m_tHitRect, 0, 0);                      
	}


	hPen = CreatePen(PS_SOLID, 3, RGB(128, 128, 128));
	hOldPen = (HPEN)SelectObject(hDC, hPen);

	SelectObject(hDC, hOldPen); DeleteObject(hPen);

	for (int i = 0; i < 4; ++i)
	{
		MoveToEx(hDC, transformedCorners[i].x, transformedCorners[i].y, nullptr);
		LineTo(hDC, rotatedSosil[i].x, rotatedSosil[i].y);
	}
	Render_Obstacles(hDC);

	/*
	TCHAR m_szBuf[100] = {};
	swprintf_s(m_szBuf, L"��� x : %.f, ��� y : %.f", m_tInfo.vPos.x, m_tInfo.vPos.y);
	TextOut(hDC, 300, 30, m_szBuf, lstrlen(m_szBuf));
	*/

	TCHAR m_szGameTime[100] = {};
	ULONG64 ullCurTime = GetTickCount64() - g_ullPlayTime;
	ULONG64 ullTime = ullCurTime / 1000;
	 ullSecond = ullTime % 60;
	swprintf_s(m_szGameTime, L" Time: %d ", (int)ullSecond);
	TextOut(hDC, 350, 550, m_szGameTime, lstrlen(m_szGameTime));

	HFONT hFont2 = CreateFont(
		72, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"SoyaPixel"
	);


	if (ullSecond > 30)
	{
		DrawOutlinedText(hDC, 205, 100, L"����Ŭ����!!!!!!!!!", RGB(0, 0, 0), RGB(255, 255, 255), hFont2);
		m_Obstacles.clear();
	}
	DeleteObject(hFont2);
}
void CRoad::DrawOutlinedText(HDC hDC, int x, int y, const TCHAR* text, COLORREF outlineColor, COLORREF textColor, HFONT hFont)
{
	HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);


	SetBkMode(hDC, TRANSPARENT);


	SetTextColor(hDC, outlineColor);
	for (int offsetX = -1; offsetX <= 1; ++offsetX)
	{
		for (int offsetY = -1; offsetY <= 1; ++offsetY)
		{
			if (offsetX != 0 || offsetY != 0)
			{
				TextOut(hDC, x + offsetX, y + offsetY, text, (int)_tcslen(text));
			}
		}
	}


	SetTextColor(hDC, textColor);

	TextOut(hDC, x, y, text, (int)_tcslen(text));


	SelectObject(hDC, hOldFont);
}
void CRoad::Spawn_Obstacle()
{
	if (m_vecPoints.empty() == true)
		return;
	SObstacle obs;

	// �������� �� ���� (0: ����, 1: ������, 2: ����, 3: �Ʒ���)
	int randomSide = rand() % 4;

	// Z�� ���� (��ֹ��� �÷��̾� ������ �ٰ����� ����)
	obs.vPos.z = 5000.f;

	// ��ֹ��� �⺻ ũ�� �� ȸ�� �ʱ�ȭ
	obs.fWidth = 60.f;
	obs.fHeight = 80.f;
	obs.fAngle = D3DXToRadian(0.f);
	obs.fScale = 1.0f;

	// �� �鿡 ���� ��ġ �� ȸ�� ����
	switch (randomSide)
	{
	case 0: // ����
		obs.vPos.x = -400.f;                   // ���� �� X ��ġ
		obs.vPos.y = static_cast<float>(rand() % 600 - 300); // -300 ~ +300 �������� Y ��ġ ����
		obs.fAngle = D3DXToRadian(90.f);       // ȸ�� ���� (Z�� ����)
		break;

	case 1: // ������
		obs.vPos.x = 400.f;                    // ������ �� X ��ġ
		obs.vPos.y = static_cast<float>(rand() % 600 - 300); // -300 ~ +300 �������� Y ��ġ ����
		obs.fAngle = D3DXToRadian(-90.f);      // ȸ�� ����
		break;

	case 2: // ����
		obs.vPos.x = static_cast<float>(rand() % 600 - 300); // -300 ~ +300 �������� X ��ġ ����
		obs.vPos.y = -400.f;                  // ���� �� Y ��ġ
		obs.fAngle = D3DXToRadian(180.f);     // ȸ�� ����
		break;

	case 3: // �Ʒ���
		obs.vPos.x = static_cast<float>(rand() % 600 - 300); // -300 ~ +300 �������� X ��ġ ����
		obs.vPos.y = 400.f;                   // �Ʒ��� �� Y ��ġ
		obs.fAngle = D3DXToRadian(0.f);       // ȸ�� ����
		break;
	}

	// ������ ��ֹ� �߰�
	m_Obstacles.push_back(obs);
}

void CRoad::Key_Input()
{
	if (m_vecPoints.empty() == true)
		return;
	static float fPlayer_Y = 0.f;
	if (GetAsyncKeyState('A'))
	{
		// ��θ� ��������
		D3DXVECTOR3 fixedLeftDir = { +20.f, 0.f, 0.f };
		m_tInfo.vPos += fixedLeftDir; 
	}
	if (CKeyManager::Get_Instance()->Key_Down(VK_SPACE) && !m_bJumping)
	{
		m_bJumping = true;
		m_fJumpSpeed = 20.f;  // ���� �ʱ� �ӵ�
	}
	else if (GetAsyncKeyState('D'))
	{
	    // ��θ� ����������
		D3DXVECTOR3 fixedRightDir = { -20.f, 0.f, 0.f };
		m_tInfo.vPos += fixedRightDir; 
	}
	if (GetAsyncKeyState(VK_UP))
	{	
		D3DXVECTOR3 fixedDownDir = { 0.f, 40.f, 0.f };
		m_tInfo.vPos += fixedDownDir;
	}
	if (GetAsyncKeyState('P'))
	{
		D3DXVECTOR3 fixedDownDir = { 400.f, -900.f, 0.f };
		m_tInfo.vPos = fixedDownDir;
	}
	else if (GetAsyncKeyState(VK_DOWN))
	{
		D3DXVECTOR3 fixedUpDir = { 0.f, -40.f, 0.f };
		m_tInfo.vPos += fixedUpDir;
	}

	// ���� �������� 90�� ȸ��
	if (CKeyManager::Get_Instance()->Key_Down(VK_LEFT)) // ���� ȸ��
	{

		m_fTargetAngle -= D3DXToRadian(90.f);
	}
	if (m_tInfo.vPos.x > 1900) // ���� �� ���� ������
	{
		m_fTargetAngle -= D3DXToRadian(90.f);
		//float m_fdgree = D3DXToDegree(m_fTargetAngle); ���� Ȯ�ο�
		m_tInfo.vPos.x = -1140;
		//if (!m_bLeft_Rotation) // ó�� �������� ȸ��
		//{
		//	m_bLeft_Rotation = true;
		//}
		//else if (!m_bTop_Rotation) // ���ʿ��� �ٽ� ���� -> õ��
		//{
		//	m_bTop_Rotation = true;
		//}
		//else if (!m_bRight_Rotation&&!m_bTop_Rotation) // õ�忡�� �ٽ� ���� -> �����ʺ�
		//{

		//	m_bRight_Rotation = true;
		//
		//}
		//else if (m_bLeft_Rotation&&m_bTop_Rotation&&m_bRight_Rotation) // ���ʺ�, õ�� , ������ �� ��� ��������
		//{
		//	
		//	m_bTop_Rotation = false;
		//	m_bLeft_Rotation = false;
		//	m_bBottom_Rotation = false;
		//}
	}
	// ������ �������� 90�� ȸ��
	if (CKeyManager::Get_Instance()->Key_Down(VK_RIGHT))
	{
		m_fTargetAngle += D3DXToRadian(90.f);
	}
	if (m_tInfo.vPos.x < -1140) // ������ �� ���� ������
	{
		m_fTargetAngle -= D3DXToRadian(-90.f);
		m_tInfo.vPos.x = 1900;
	}

	if (CKeyManager::Get_Instance()->Key_Down(VK_SPACE) && !m_bJumping)
	{
		m_bJumping = true;
		m_fJumpSpeed = 20.f;  // �ʱ� ���� �÷��� �ӵ� (���ϴ� ������ ����)
	}
}
void CRoad::Wall_Update()
{

	if (m_vecPoints.empty() == true)
		return;
	m_vecPoints[0][0] = { (long)transformedCorners[0].x, (long)transformedCorners[0].y };
	m_vecPoints[0][1] = { (long)rotatedSosil[0].x, (long)rotatedSosil[0].y };
	m_vecPoints[0][2] = { (long)rotatedSosil[3].x, (long)rotatedSosil[3].y };
	m_vecPoints[0][3] = { (long)transformedCorners[3].x, (long)transformedCorners[3].y };


	m_vecPoints[1][0] = { (long)transformedCorners[1].x, (long)transformedCorners[1].y };
	m_vecPoints[1][1] = { (long)rotatedSosil[1].x, (long)rotatedSosil[1].y };
	m_vecPoints[1][2] = { (long)rotatedSosil[2].x, (long)rotatedSosil[2].y };
	m_vecPoints[1][3] = { (long)transformedCorners[2].x, (long)transformedCorners[2].y };

	m_vecPoints[2][0] = { (long)transformedCorners[0].x, (long)transformedCorners[0].y };
	m_vecPoints[2][1] = { (long)rotatedSosil[0].x, (long)rotatedSosil[0].y };
	m_vecPoints[2][2] = { (long)rotatedSosil[1].x, (long)rotatedSosil[1].y };
	m_vecPoints[2][3] = { (long)transformedCorners[1].x, (long)transformedCorners[1].y };

	m_vecPoints[3][0] = { (long)transformedCorners[3].x, (long)transformedCorners[3].y };
	m_vecPoints[3][1] = { (long)rotatedSosil[3].x, (long)rotatedSosil[3].y };
	m_vecPoints[3][2] = { (long)rotatedSosil[2].x, (long)rotatedSosil[2].y };
	m_vecPoints[3][3] = { (long)transformedCorners[2].x, (long)transformedCorners[2].y };

	m_vecPoints[4][0] = { (long)rotatedSosil[0].x, (long)rotatedSosil[0].y };
	m_vecPoints[4][1] = { (long)rotatedSosil[1].x, (long)rotatedSosil[1].y };
	m_vecPoints[4][2] = { (long)rotatedSosil[2].x, (long)rotatedSosil[2].y };
	m_vecPoints[4][3] = { (long)rotatedSosil[3].x, (long)rotatedSosil[3].y };

}

//
//HPEN hPen = CreatePen(PS_SOLID, 3, RGB(255, 220, 220));
//HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
//SelectObject(hDC, hOldPen); DeleteObject(hPen);


void CRoad::Late_Update()
{
}

void CRoad::Release()
{
	if (m_vecPoints.empty() == true)
		return;
	for (auto& pointArray : m_vecPoints)
	{
		delete[] pointArray;
	}
	m_vecPoints.clear(); 
	m_Obstacles.clear();
}

void CRoad::OnCollision(CObject* _obj)
{
}



