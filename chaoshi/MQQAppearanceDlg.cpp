// MQQAppearanceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HCDB.h"
#include "MQQAppearanceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMQQAppearanceDlg

IMPLEMENT_DYNCREATE(CMQQAppearanceDlg, CFormView)

CMQQAppearanceDlg::CMQQAppearanceDlg()
	: CFormView(CMQQAppearanceDlg::IDD)
{
	//{{AFX_DATA_INIT(CMQQAppearanceDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMQQAppearanceDlg::~CMQQAppearanceDlg()
{
}

void CMQQAppearanceDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMQQAppearanceDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMQQAppearanceDlg, CFormView)
	//{{AFX_MSG_MAP(CMQQAppearanceDlg)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMQQAppearanceDlg diagnostics

#ifdef _DEBUG
void CMQQAppearanceDlg::AssertValid() const
{
	CFormView::AssertValid();
}

void CMQQAppearanceDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMQQAppearanceDlg message handlers
#define IDC_LST_MENU	WM_USER + 1000
void CMQQAppearanceDlg::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
		//��������˵��ؼ�
	DWORD dwf = CGfxOutBarCtrl::fDragItems|CGfxOutBarCtrl::fEditGroups|CGfxOutBarCtrl::fEditItems|CGfxOutBarCtrl::fRemoveGroups|
		CGfxOutBarCtrl::fRemoveItems|CGfxOutBarCtrl::fAddGroups|CGfxOutBarCtrl::fAnimation
		|CGfxOutBarCtrl::fSelHighlight;
	CRect rc;
	GetClientRect(&rc);
	rc.bottom=rc.bottom-rc.bottom*0.1;
	wndBar.Create(WS_CHILD|WS_VISIBLE, rc, this,IDC_LST_MENU, dwf);
	wndBar.SetOwner(this);
	
	//���������ô�Сͼ������
	imaLarge.Create(IDB_IMAGELIST, 32, 0, RGB(0,130,132));
	imaSmall.Create(IDB_SMALL_IMAGELIST, 16, 0, RGB(0,128,128));
	wndBar.SetImageList(&imaLarge, CGfxOutBarCtrl::fLargeIcon);
	wndBar.SetImageList(&imaSmall, CGfxOutBarCtrl::fSmallIcon);

	//����4������
	wndBar.SetAnimationTickCount(1);
	wndBar.SetAnimSelHighlight(50);
	wndBar.AddFolder("ǰ̨����", 0);
	wndBar.AddFolder("��������",1);
	wndBar.AddFolder("������", 2);
	wndBar.AddFolder("���۹���", 3);
	wndBar.AddFolder("��̨����", 4);
	//����һ�������ͼ�갴ť
	wndBar.InsertItem(0, 0, "ǰ̨����", 0, 0);
	
	
	//���ڶ��������ͼ�갴ť
	wndBar.InsertItem(1, 0, "�Ǽ����", 0, 0);
	wndBar.InsertItem(1, 1, "��ⵥ��ѯ",0,0);
    wndBar.InsertItem(1, 2, "�����ϸ",0,0);
	wndBar.InsertItem(1, 3, "�Ǽǳ���",0,0);
	wndBar.InsertItem(1, 4, "���ⵥ��ѯ",0,0);
	wndBar.InsertItem(1, 5, "������ϸ",0,0);
	wndBar.InsertItem(1, 6, "������",0,0);
	wndBar.InsertItem(1, 7, "�������",0,0);
	//�������������ͼ�갴ť
	wndBar.InsertItem(2, 0, "������", 0, 0);
	
	//�����ĸ������ͼ�갴ť
	wndBar.InsertItem(3, 0, "���۹���", 3, 0);
    //�򿪵�һ������
	wndBar.InsertItem(4, 0, "���ݱ���", 1, 0);
	wndBar.InsertItem(4, 0, "���ݻ�ԭ", 1, 0);
	wndBar.SetSelFolder(0);
   
	// TODO: Add your specialized code here and/or call the base class
	
}
