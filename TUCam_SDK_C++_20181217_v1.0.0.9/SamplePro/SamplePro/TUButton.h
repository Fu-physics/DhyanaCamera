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
    CString     m_strText;          //按钮文字
    CFont*      p_Font;             //字体
    COLORREF    m_ForeColor;        //文本颜色
    COLORREF    m_BackColor;        //背景色

    void  DrawButton(CDC *pDC);     //画正常的按钮

    // 接口函数
public:

    void SetText(CString str);

protected:
    virtual void PreSubclassWindow();
public:
    virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};
