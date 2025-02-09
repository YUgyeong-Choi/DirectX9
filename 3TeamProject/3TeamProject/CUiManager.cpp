#include "pch.h"
#include "CUiManager.h"
#include "CObjectManager.h"
#include "CSceneManager.h"
#include "CKeyManager.h"
#include "CYGPlayer.h"
#include "CJWScene.h"
#include "CCYPlayer.h"
#include "CCYScene.h"

CUiManager* CUiManager::m_pInstance = nullptr;

CUiManager::CUiManager():m_eCurUi(UI_END)
{
}

CUiManager::~CUiManager()
{
}

void CUiManager::Render(HDC hDC)
{
	switch (m_eCurUi)
	{
	case UI_YG:
		RenderUi_YG(hDC);
		break;
	case UI_CY:
		RenderUi_CY(hDC);
		break;
	case UI_DW:
		RenderUi_DW(hDC);
		break;
	case UI_JW:
		RenderUi_JW(hDC);
		break;
	case UI_END:
		break;
	default:
		break;
	}
}

void CUiManager::RenderUi_YG(HDC hDC)
{
	//COLORREF color = RGB(255, 255, 255);
	//HBRUSH hBrush = CreateSolidBrush(color);
	//HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	//HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	//HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
	//RoundRect(hDC, WINCX / 2 - 200, 510, WINCX / 2 - 200 + 400, 550, 10, 10);

	//SelectObject(hDC, hOldBrush);
	//SelectObject(hDC, hOldPen);
	//DeleteObject(hBrush);
	//DeleteObject(hPen);

	//int hpWidth = 0;
	//CYGPlayer* _copyYGPlayer = static_cast<CYGPlayer*>(CObjectManager::Get_Instance()->Get_Player());
	//if (_copyYGPlayer->Get_Hp() > 0) {
	//	hpWidth = (400 * _copyYGPlayer->Get_Hp()) / _copyYGPlayer->Get_MaxHp();
	//}
	//

	//color = RGB(237, 52, 52);
	//hBrush = CreateSolidBrush(color);
	//hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	//hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	//hOldPen = (HPEN)SelectObject(hDC, hPen);
	//RoundRect(hDC, WINCX/2-200,510, WINCX / 2 - 200 + hpWidth, 550, 10, 10);

	//SelectObject(hDC, hOldBrush);
	//SelectObject(hDC, hOldPen);
	//DeleteObject(hBrush);
	//DeleteObject(hPen);

	//switch (_copyYGPlayer->Get_PS()) {
	//case CYGPlayer::PS_NOGUN:
	//	color = RGB(0, 200, 230);
	//	hBrush = CreateSolidBrush(color);
	//	hPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 255));
	//	hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	//	hOldPen = (HPEN)SelectObject(hDC, hPen);
	//	RoundRect(hDC, 690, 490, 790, 560, 10, 10);

	//	SelectObject(hDC, hOldBrush);
	//	SelectObject(hDC, hOldPen);
	//	DeleteObject(hBrush);
	//	DeleteObject(hPen);
	//	ColorCircle(hDC, 730- 15, 515, 750-15, 535,252,194,114,2);
	//	ColorCircle(hDC, 730+15, 515, 750+15, 535,252,194,114,2);

	//	break;
	//case CYGPlayer::PS_GUN:
	//	color = RGB(0, 200, 230);
	//	hBrush = CreateSolidBrush(color);
	//	hPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 255));
	//	hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	//	hOldPen = (HPEN)SelectObject(hDC, hPen);
	//	RoundRect(hDC, 690, 490, 790, 560, 10, 10);

	//	SelectObject(hDC, hOldBrush);
	//	SelectObject(hDC, hOldPen);
	//	DeleteObject(hBrush);
	//	DeleteObject(hPen);

	//	ColorRect(hDC, 740 - 25 , 525 - 15 , 740 + 25 ,525, 0,0,0,0);
	//	ColorRect(hDC, 740 +10 , 525, 740 + 25, 525 +15, 0,0,0,0);
	//	MoveToEx(hDC, 740, 525, nullptr);
	//	LineTo(hDC, 740 +3, 525+7);
	//	LineTo(hDC, 740+10, 525+7);

	//	SetTextColor(hDC, RGB(255, 255, 255)); //글자 색
	//	SetBkMode(hDC, TRANSPARENT); //글자 배경 투명

	//	TCHAR bulletNum[64];
	//	_stprintf_s(bulletNum, _T("%d"), _copyYGPlayer->Get_BulletNum());
	//	RECT rect2 = { 740, 540, 780, 550 };
	//	DrawText(hDC, bulletNum, _tcslen(bulletNum), &rect2, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	//	break;
	//}

}

void CUiManager::RenderUi_CY(HDC hDC)
{
	
}

void CUiManager::RenderUi_DW(HDC hDC)
{
}

void CUiManager::RenderUi_JW(HDC hDC)
{
	// 색깔 크기 반지름
	CJWScene* pScene = dynamic_cast<CJWScene*>(CSceneManager::Get_Instance()->Get_Scene());
	if (pScene)
	{
		auto& info =  pScene->Get_NextFruit_Info();

		if (info.vecRenderPoints.empty()) return;
		HBRUSH hBrush = CreateSolidBrush(pScene->Get_NextFruit_Info().tColor);
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		vector<D3DXVECTOR3> m_vecRenderPoints = info.vecRenderPoints;
		vector<POINT> m_vecPolyPoints;
		D3DXVECTOR3 vScale(1.f, 1.f, 1.f);

		D3DXMATRIX matScale, matRotZ, matTrans, matWorld;
		switch (info.eFruitType)
		{
		case FRUIT_TYPE::CHERRY:     // 3 * 2.0 = 6 단위
			vScale = {3.5f,3.5f,3.5f };  // 기준점
			break;
		case FRUIT_TYPE::ORANGE:     // 10 * 1.8 = 18 단위
			vScale = { 1.5f, 1.5f, 1.5f };
			break;
		case FRUIT_TYPE::LEMON:      // 15 * 1.6 = 24 단위
			vScale = { 1.5f, 1.5f, 1.5f };
			break;
		case FRUIT_TYPE::APPLE:      // 30 * 1.4 = 42 단위
			vScale = { 1.2f, 1.2f, 1.2f };
			break;
		case FRUIT_TYPE::PEACH:      // 35 * 1.2 = 42 단위
			vScale = { 1.2f, 1.2f, 1.2f };
			break;
		case FRUIT_TYPE::PINEAPPLE:  // 40 * 1.0 = 40 단위
			vScale = { 1.0f, 1.0f, 1.0f };
			break;
		case FRUIT_TYPE::MELON:      // 50 * 0.8 = 40 단위
			vScale = { 0.8f,0.8f,0.8f };
			break;
		case FRUIT_TYPE::PUMPKIN:    // 60 * 0.6 = 36 단위
			vScale = { 0.6f, 0.6f, 0.6f };
			break;
		case FRUIT_TYPE::WATERMELON: // 70 * 0.4 = 28 단위
			vScale = { 0.4f, 0.4f, 0.4f };
			break;
		default:
			vScale = { 1.0f, 1.0f, 1.0f };
			break;
		}
		
		D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);
		D3DXMatrixRotationZ(&matRotZ, 0.f);
		D3DXMatrixTranslation(&matTrans, 750.f, 50.f, 0.f);

		for (int i = 0; i < (int)m_vecRenderPoints.size(); i++)
		{
			matWorld = matScale * matRotZ* matTrans;

			// 해당 점 변환
			D3DXVec3TransformCoord(&m_vecRenderPoints[i], &m_vecRenderPoints[i], &matWorld);
		}

		// 폴리곤 
		for (const auto& p : m_vecRenderPoints)
		{
			m_vecPolyPoints.push_back({ (LONG)p.x,(LONG)p.y});
		}
	
		Polygon(hDC, m_vecPolyPoints.data(), (int)m_vecPolyPoints.size());

		D3DXVECTOR3 vDir = (m_vecRenderPoints.front() - D3DXVECTOR3(750.f, 50.f, 0.f));
		float fRadius =D3DXVec3Length(&vDir);
		Ellipse(hDC,
			int(m_vecRenderPoints[50].x - fRadius / 3.f),
			int(m_vecRenderPoints[50].y - fRadius / 3.f),
			int(m_vecRenderPoints[50].x + fRadius / 3.f),
			int(m_vecRenderPoints[50].y + fRadius / 3.f));


	    SetTextColor(hDC, RGB(50, 68, 45));
		SetBkMode(hDC, TRANSPARENT); //글자 배경 투명

		HFONT hFont1 = CreateFont(
			30, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Eulyoo1945-SemiBold"
		); //m3x6이 폰트

		HFONT OldFont = (HFONT)SelectObject(hDC, hFont1);

		TCHAR szInfo[64];
		_stprintf_s(szInfo, _T("SCORE"));
		RECT rect = { 20, 60, 120, 80 };
		DrawText(hDC, szInfo, _tcslen(szInfo), &rect, DT_RIGHT | DT_SINGLELINE | DT_VCENTER); //가운데 중앙 정렬

		TCHAR szScore[64];
		_stprintf_s(szScore, _T("%d"), pScene->Get_Score());
		RECT rect2 = { 30, 80, 80, 110 };
		DrawText(hDC, szScore, _tcslen(szScore), &rect2, DT_RIGHT | DT_SINGLELINE | DT_VCENTER); //가운데 중앙 정렬

		TCHAR szLevelText[64];
		_stprintf_s(szLevelText, _T("현재 레벨 %d"), pScene->Get_Level()+1);
		RECT rect3 = { 0, 10, 150, 50 };
		DrawText(hDC, szLevelText, _tcslen(szLevelText), &rect3, DT_RIGHT | DT_SINGLELINE | DT_VCENTER); //가운데 중앙 정렬
	

		if (pScene->Get_GameOver()||pScene->Get_GameClear())
		{

		HFONT hGameOverFont = CreateFont(
			100,            // 폰트 크기
			0, 0, 0,
			FW_BLACK,      // 굵기
			FALSE, FALSE, FALSE,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_SWISS,
			L"Eulyoo1945-SemiBold");

		// GAME OVER 텍스트 중앙 배치를 위한 RECT
		RECT gameOverRect = { 0, 0, 800, 600 };
		gameOverRect.top = 200;     // 상단에서 200픽셀 아래
		gameOverRect.bottom = 300;  // 영역 높이 100픽셀

		// GAME OVER 텍스트 그리기
		SelectObject(hDC, hGameOverFont);
		TCHAR szGameOver[64];
		_tcscpy_s(szGameOver, pScene->Get_GameClear() ? _T("GAME CLEAR") : _T("GAME OVER"));
		DrawText(hDC, szGameOver, _tcslen(szGameOver), &gameOverRect,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		// 최종 점수 표시용 폰트
		HFONT hFinalScoreFont = CreateFont(
			50,             // 폰트 크기
			0, 0, 0,
			FW_BOLD,
			FALSE, FALSE, FALSE,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_SWISS,
			L"Eulyoo1945-SemiBold"
		);

		// 최종 점수 텍스트
		SelectObject(hDC, hFinalScoreFont);
		TCHAR szFinalScore[64];
		_stprintf_s(szFinalScore, _T("FINAL SCORE: %d"), pScene->Get_Score());

		// 최종 점수 표시 영역 (GAME OVER 텍스트 아래)
		RECT finalScoreRect = { 0, 0, 800, 600 };
		finalScoreRect.top = 320;     
		finalScoreRect.bottom = 370;  

		DrawText(hDC, szFinalScore, _tcslen(szFinalScore), &finalScoreRect,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		// 폰트 리소스 정리
		DeleteObject(hGameOverFont);
		DeleteObject(hFinalScoreFont);
		}


		SelectObject(hDC, OldFont);
		DeleteObject(hFont1);


		SelectObject(hDC,hOldBrush);
		DeleteObject(hBrush);
	}
}