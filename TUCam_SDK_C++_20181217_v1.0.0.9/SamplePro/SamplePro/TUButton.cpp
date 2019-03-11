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
    // TODO: �ڴ����ר�ô����/����û���

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

    // TODO:  ������Ĵ����Ի���ָ����

    CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

    m_rcBtn = lpDrawItemStruct->rcItem;   

    if( m_strText.IsEmpty() )
        GetWindowText( m_strText );          


    int nSavedDC = pDC->SaveDC();
    VERIFY( pDC );

    DrawButton( pDC );                // ���ư�ť

    pDC->RestoreDC( nSavedDC );
}

void CTUButton::DrawButton(CDC *pDC)
{
/*
    //���ư�ť����
    CBrush Brush;
    Brush.CreateSolidBrush( m_BackColor );    
    pDC->SelectObject( &Brush );
*/
    //���ư�ť����
    pDC->SetBkMode( TRANSPARENT );
    pDC->DrawText( m_strText, &m_rcBtn, DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS);
 
}