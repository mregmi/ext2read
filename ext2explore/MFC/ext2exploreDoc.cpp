// ext2exploreDoc.cpp : implementation of the Cext2exploreDoc class
//

#include "stdafx.h"
#include "ext2explore.h"

#include "ext2exploreDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cext2exploreDoc

IMPLEMENT_DYNCREATE(Cext2exploreDoc, CDocument)

BEGIN_MESSAGE_MAP(Cext2exploreDoc, CDocument)
END_MESSAGE_MAP()


// Cext2exploreDoc construction/destruction

Cext2exploreDoc::Cext2exploreDoc()
{
	// TODO: add one-time construction code here

}

Cext2exploreDoc::~Cext2exploreDoc()
{
}

BOOL Cext2exploreDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// Cext2exploreDoc serialization

void Cext2exploreDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// Cext2exploreDoc diagnostics

#ifdef _DEBUG
void Cext2exploreDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void Cext2exploreDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Cext2exploreDoc commands
