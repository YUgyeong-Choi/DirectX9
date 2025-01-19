#pragma once
#include "CDWObject.h"


class CDWPlayer :public CDWObject
{
public:
	CDWPlayer();
	virtual ~CDWPlayer() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Key_Input();
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void OnCollision(CObject* _obj) override;
	auto Get_ScreenCorner()
	{
		return m_screenCorner;
	}

private:
	
	D3DXVECTOR3 transformedCorners[2]; // ��İ����ع��� ����
	D3DXVECTOR3 m_vCorner[2];
	POINT m_screenCorner[30];

	float m_pPlayer_ScaleX;
	float m_pPlayer_ScaleY;
	float m_pPlayer_ScaleZ;

	float m_fPlayer_Scale;
	

	bool m_bFalling;
	float m_fFallSpeed; // ���� �ӵ�
	float m_fFallDepth; // �����̿� ������ ����

};


