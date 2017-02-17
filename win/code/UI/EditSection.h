#pragma once

#include "TypeDefine.h"
#include "afxwin.h"
#include "SectionContent.h"

// CEditSection �Ի���



class CEditSection : public CDialog
{
	DECLARE_DYNAMIC(CEditSection)

public:
	CEditSection(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEditSection();

// �Ի�������
	enum { IDD = IDD_DIALOG_EDIT_SECTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual void OnOK();
      void AttachTree( CCommonTree *	pTT , CString & parent);
      afx_msg void OnBnClickedButtonPreCheck();
      afx_msg void OnEnKillfocusEditSection();

	DECLARE_MESSAGE_MAP()

public:
      CContentTree      *  pCntTree;//content tree
	CSectionContent     SectionContent;//content string
	
      u32                       LastEnd;
      u8 *                      NewSecData;
      u32                       NewSecLenth;


      CEdit                 	HintControl;
	CEdit 			SectionContentControl;
};
