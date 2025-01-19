#pragma once

#include "CDWObject.h"
#include <d3dx9math.h>  
class CRoad : public CDWObject
{
public:

	struct SObstacle
	{
		D3DXVECTOR3 vPos;   // ��ֹ��� ���� ��ġ (z�� ����)
		float       fWidth; // ��ֹ��� ����ũ��
		float       fHeight;// ��ֹ��� ����ũ��
		D3DXVECTOR3 worldCorner[4];
		D3DXVECTOR3 screenCorner[4];
		float       fAngle; 
		float       fScale; // ������(����)
	};

public:
	CRoad();
	virtual ~CRoad() { Release(); }
	void Initialize() override;
	int Update() override;
	void Key_Input();
	void Late_Update() override;
	void Render(HDC hDC) override;
	void DrawOutlinedText(HDC hDC, int x, int y, const TCHAR* text, COLORREF outlineColor, COLORREF textColor, HFONT hFont);
	void Release() override;
	void OnCollision(CObject* _obj) override;
    void Wall_Update();
	//////////////////////////////////////
	void Update_Obstacles();
	void Spawn_Obstacle();
	void Render_Obstacles(HDC hDC);
    /////////////////////////////////////

	auto& Get_obs()
	{
		return  m_Obstacles;
	}
	

private:
	D3DXVECTOR3 Sosil[4];
	D3DXVECTOR3 rotatedSosil[4];
	D3DXVECTOR3 m_vCorner[4];
	D3DXVECTOR3 transformedCorners[4]; // ��İ����ع��� ����
	D3DXMATRIX        matScale_Sosil;  // ũ��
	D3DXMATRIX        matRotZ_Sosil; // ����
	D3DXMATRIX        matTrans_Sosil; // ��ġ
	D3DXMATRIX WorldMat_Sosil; // �ҽ��� ���� ���
	static bool m_bLeft_Rotation;
	static bool m_bRight_Rotation;
	static bool m_bTop_Rotation;
	static bool m_bBottom_Rotation;

	bool m_bFirst_Check;
	bool  m_bJumping;
	vector<POINT*> m_vecPoints; // ���;ȿ� ����Ʈ��� 4��¥���迭��`	
	vector<SObstacle> m_Obstacles; // ��ֹ� ����ü ����
	CObject* m_pPlayer;
	DWORD m_dwPrevSpawnTime; // ���� ������ Ÿ�̸�
	HBITMAP hBmp;
	HBRUSH  hPatternBrush;
private:    
	float m_fJumpSpeed;       
	float m_fGravity;         
	float m_fJumpOffsetY;   
	float m_fTargetAngle;
	float m_fTargetX;
	float m_fPlayerStartY;
	float m_fCheck_Rotation_Angle;
	

	bool m_bRelease_Check;

	ULONGLONG g_ullPlayTime;
	ULONG64 ullSecond;

};

