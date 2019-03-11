#include "StdAfx.h"
#include "TUButton.h"

CTUButton::CTUButton(void)
{
    m_strText   = _T("");        
    m_ForeColor = RGB(0,0,0);            
    m_BackColor = RGB(138, 138, 138);      
    p_Font = NULL; 
}

CTUButton::~CTUButton(void)
{

}

void CTUButton::PreSubclassWindow()
{
    // TODO: 在此添加专用代码和/或调用基类

    ModifyStyle( 0, BS_OWNERDRAW );        

    CButton::PreSubclassWindow();
}

void CTUButton::SetText(CString str)
{
    m_strText = _T("");
    SetWindowText(str);
}

void CTUButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

    // TODO:  添加您的代码以绘制指定项

    CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

    m_rcBtn = lpDrawItemStruct->rcItem;   

    if( m_strText.IsEmpty() )
        GetWindowText( m_strText );          


    int nSavedDC = pDC->SaveDC();
    VERIFY( pDC );

    DrawButton( pDC );                // 绘制按钮

    pDC->RestoreDC( nSavedDC );
}

void CTUButton::DrawButton(CDC *pDC)
{
/*
    //绘制按钮背景
    CBrush Brush;
    Brush.CreateSolidBrush( m_BackColor );    
    pDC->SelectObject( &Brush );
*/
    //绘制按钮文字
    pDC->SetBkMode( TRANSPARENT );
    pDC->DrawText( m_strText, &m_rcBtn, DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS);
 
}