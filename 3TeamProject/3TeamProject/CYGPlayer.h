#pragma once
#include "CYGObject.h"
class CYGPlayer :public CYGObject
{
public:
	enum PLAYERSTATE { PS_NOGUN, PS_GUN, PS_END };
public:
	CYGPlayer();
	virtual ~CYGPlayer() { Release(); }

	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void OnCollision(CObject* _obj) override;
};

