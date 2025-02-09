#include "pch.h"
#include "CMenuScene.h"
#include "CKeyManager.h"
#include "CSceneManager.h"
#include "CBitManager.h"

CMenuScene::CMenuScene():m_iIndex(0)
{
}

void CMenuScene::Initialize()
{
}

int CMenuScene::Update()
{
    Key_Input();
    return 0;
}

void CMenuScene::Late_Update()
{
}

void CMenuScene::Render(HDC hDC)
{
    COLORREF color = RGB(0, 0, 0);
    HBRUSH hBrush = CreateSolidBrush(color);
    HPEN hPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
    HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
    Rectangle(hDC, -100, -100, 900, 700);
    SelectObject(hDC, hOldBrush);
    SelectObject(hDC, hOldPen);
    DeleteObject(hBrush);
    DeleteObject(hPen);

    SetTextColor(hDC, RGB(255, 255, 255));
    HFONT hFont1 = CreateFont(50, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, VARIABLE_PITCH, TEXT("Arial"));
    HFONT hOldFont = (HFONT)SelectObject(hDC, hFont1);
    TCHAR szLength[64];
    _stprintf_s(szLength, _T("홍동완 복습"));
    TextOut(hDC, 300, 100, szLength, _tcslen(szLength));
    SelectObject(hDC, hOldFont);
    DeleteObject(hFont1);


    color = RGB(255, 255, 255);
    hBrush = CreateSolidBrush(color);
    hPen = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
    hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
    hOldPen = (HPEN)SelectObject(hDC, hPen);

    Rectangle(hDC, 10 + (200 * m_iIndex), 190, 190 + (200 * m_iIndex), 410);

    SelectObject(hDC, hOldBrush);
    SelectObject(hDC, hOldPen);
    DeleteObject(hBrush);
    DeleteObject(hPen);


    SetTextColor(hDC, RGB(255, 255, 255)); //글자 색
    SetBkMode(hDC, TRANSPARENT); //글자 배경 투명

    TCHAR whoMake[64];
    _stprintf_s(whoMake, _T("에이스타"));
    RECT rect = { 20, 410, 180, 450 };
    DrawText(hDC, whoMake, _tcslen(whoMake), &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

    _stprintf_s(whoMake, _T("카메라"));
    rect = { 220, 410, 380, 450 };
    DrawText(hDC, whoMake, _tcslen(whoMake), &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

    _stprintf_s(whoMake, _T("미정1"));
    rect = { 420, 410, 580, 450 };
    DrawText(hDC, whoMake, _tcslen(whoMake), &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

    _stprintf_s(whoMake, _T("미정2"));
    rect = { 620, 410, 780, 450 };
    DrawText(hDC, whoMake, _tcslen(whoMake), &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

    HDC		hMemDC = CBitManager::Get_Instance()->Find_Image(L"YG");
    GdiTransparentBlt(hDC, 11, 191, 178, 218, hMemDC, 0, 0, 347, 424, SRCCOPY);

    hMemDC = CBitManager::Get_Instance()->Find_Image(L"CY");
    GdiTransparentBlt(hDC, 211, 191, 178, 218, hMemDC, 0, 0, 295, 395, SRCCOPY);

    hMemDC = CBitManager::Get_Instance()->Find_Image(L"DW");
    GdiTransparentBlt(hDC, 411, 191, 178, 218, hMemDC, 0, 0, 292, 430, SRCCOPY);

    hMemDC = CBitManager::Get_Instance()->Find_Image(L"JW");
    GdiTransparentBlt(hDC, 611, 191, 178, 218, hMemDC, 0, 0, 429, 563, SRCCOPY);
}

void CMenuScene::Release()
{
}

void CMenuScene::Key_Input()
{


    if (CKeyManager::Get_Instance()->Key_Down(VK_RIGHT)) {
        m_iIndex++;
        if (m_iIndex > 3) {
            m_iIndex = 0;
        }
    }

    if (CKeyManager::Get_Instance()->Key_Down(VK_LEFT)) {
        m_iIndex--;
        if (m_iIndex < 0) {
            m_iIndex = 3;
        }
    }

    if (CKeyManager::Get_Instance()->Key_Down(VK_SPACE)) {
        switch (m_iIndex)
        {
        case 0:
            CSceneManager::Get_Instance()->Set_Scene(SC_YG);
            break;
        case 1:
            CSceneManager::Get_Instance()->Set_Scene(SC_CY);
            break;
        case 2:
            CSceneManager::Get_Instance()->Set_Scene(SC_DW);
            break;
        case 3:
            CSceneManager::Get_Instance()->Set_Scene(SC_JW);
            break;
        default:
            break;
        }
    }
}

void CMenuScene::Create_MapObj()
{
}

void CMenuScene::Offset()
{
}