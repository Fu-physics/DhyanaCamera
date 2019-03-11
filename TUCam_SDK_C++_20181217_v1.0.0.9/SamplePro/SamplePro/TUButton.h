#pragma once
#include "afxwin.h"

class CTUButton :
    public CButton
{
public:
    CTUButton(void);
    ~CTUButton(void);

private:
 
    CRect       m_rcBtn;
    CString     m_strText;          //��ť����
    CFont*      p_Font;             //����
    COLORREF    m_ForeColor;        //�ı���ɫ
    COLORREF    m_BackColor;        //����ɫ

    void  DrawButton(CDC *pDC);     //�������İ�ť

    // �ӿں���
public:

    void SetText(CString str);

protected:
    virtual void PreSubclassWindow();
public:
    virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};
