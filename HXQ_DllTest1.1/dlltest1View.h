
// dlltest1View.h : Cdlltest1View ��Ľӿ�
//

#pragma once


class Cdlltest1View : public CFormView
{
protected: // �������л�����
	Cdlltest1View();
	DECLARE_DYNCREATE(Cdlltest1View)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_DLLTEST1_FORM };
#endif

// ����
public:
	Cdlltest1Doc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~Cdlltest1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
private:





// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg void OnLoadTop();
	afx_msg void OnLoadSide();
	afx_msg void OnLoadTop2();
	afx_msg void OnDeleteAll();
	afx_msg void OnLogin();
	afx_msg void OnOpenOsk();
	afx_msg void OnExcMode();
	afx_msg void OnID_EDIT_OKNG();
};

#ifndef _DEBUG  // dlltest1View.cpp �еĵ��԰汾
inline Cdlltest1Doc* Cdlltest1View::GetDocument() const
   { return reinterpret_cast<Cdlltest1Doc*>(m_pDocument); }
#endif

