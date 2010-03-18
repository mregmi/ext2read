// ext2exploreDoc.h : interface of the Cext2exploreDoc class
//


#pragma once


class Cext2exploreDoc : public CDocument
{
protected: // create from serialization only
	Cext2exploreDoc();
	DECLARE_DYNCREATE(Cext2exploreDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~Cext2exploreDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


