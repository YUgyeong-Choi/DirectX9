#pragma once
#include "CYGObject.h"
class CYGPlayer :public CYGObject
{
public:
	enum PLAYERSTATE { PS_NOGUN, PS_GUN, PS_END };
public:
	CYGPlayer();
	virtual ~CYGPlayer() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void OnCollision(CObject* _obj) override;
public:
	int Get_MaxHp() { return m_iMaxHp; }
	int Get_Hp() { return m_iHp; }
	RECT Get_CollisionBox() { return m_CollisionBox; }
	RECT Get_HitBox() { return m_tHitRect; }
	PLAYERSTATE Get_PS() { return m_PlayerState; }
	int Get_BulletNum() { return m_iBulletNum; }
public:
	void Set_Hp(int _i) { m_iHp += _i; }
private:
	void Key_Input();
private:
	//�� ���� ��
	D3DXVECTOR3 m_vLeftNoGunHandPos;
	D3DXVECTOR3 m_vRightNoGunHandPos;
	D3DXVECTOR3 m_vOriginLeftNoGunHand;
	D3DXVECTOR3 m_vOriginRightNoGunHand;

	//�� ���� ��
	D3DXVECTOR3 m_vLeftGunHandPos;
	D3DXVECTOR3 m_vRightGunHandPos;
	D3DXVECTOR3 m_vGunRectanglePoint[4];
	D3DXVECTOR3 m_vOriginLeftGunHand;
	D3DXVECTOR3 m_vOriginRightGunHand;
	D3DXVECTOR3 m_vOriginGunRectanglePoint[4];
	D3DXVECTOR3 m_vBulletSpawn;
	D3DXVECTOR3 m_vOriginBulletSpawn;

	//�÷��̾� ���� ����
	D3DXVECTOR3 m_vOriginPos;

	//���� ����
	PLAYERSTATE m_PlayerState;

	//���� ���� �� �� ���б�
	bool m_bLeftPush;
	RECT m_CollisionBox;

	//�� ����
	bool m_bHaveGun;
	int m_iShootTick;
	int m_iBulletNum;
	int m_footStepTick;
};

