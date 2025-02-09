#pragma once

#include "CObject.h"
class CCYPlayer : public CObject
{
public:
	CCYPlayer();
	virtual ~CCYPlayer();
public:
	void Initialize() ;
	int Update() ;
	void Late_Update() ;
	void Render(HDC hDC) ;
	void Release() ;


};

