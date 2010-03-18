// ext2exploreView.h : interface of the Cext2exploreView class
//


#pragma once


class Cext2exploreView : public CListView
{
protected: // create from serialization only
	Cext2exploreView();
	DECLARE_DYNCREATE(Cext2exploreView)

// Attributes
public:
	Cext2exploreDoc* GetDocument() const;
	
// Operations
public:
	void Browse();

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnRButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonDblClk(UINT nFlags, CPoint point);

// Implementation
public:
	virtual ~Cext2exploreView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

private:
	DWORD_PTR GetSelected(CPoint& pt, CString& str);	
// Generated message map functions
protected:
	afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ext2exploreView.cpp
inline Cext2exploreDoc* Cext2exploreView::GetDocument() const
   { return reinterpret_cast<Cext2exploreDoc*>(m_pDocument); }
#endif

