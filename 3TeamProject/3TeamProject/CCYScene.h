#pragma once
#include "CScene.h"
#include "CObject.h"

class CCYScene :public CScene
{
public:
	CCYScene();
	virtual ~CCYScene() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void Key_Input() override;
	void Create_MapObj() override;
	void Offset() override;

	int Get_PlayerLength() { return m_iPlayerLength; }

private:
	ULONGLONG	m_ullFoodTimeTicker;
	ULONGLONG	m_ullMonsterTimeTicker;
	int			m_iPlayerLength;

	// CScene을(를) 통해 상속됨
	
};

