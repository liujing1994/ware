// RESTOREDLG.cpp : implementation file
//

#include "stdafx.h"
#include "HCDB.h"
#include "RESTOREDLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRESTOREDLG dialog


CRESTOREDLG::CRESTOREDLG(CWnd* pParent /*=NULL*/)
	: CDialog(CRESTOREDLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRESTOREDLG)
	m_edit = _T("");
	//}}AFX_DATA_INIT
}


void CRESTOREDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRESTOREDLG)
	DDX_Control(pDX, IDC_LIST2, m_list);
	DDX_Text(pDX, IDC_EDIT1, m_edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRESTOREDLG, CDialog)
	//{{AFX_MSG_MAP(CRESTOREDLG)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, OnItemchangedList2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRESTOREDLG message handlers
extern CHCDBApp theApp;
void CRESTOREDLG::Connect()
{
 
}

BOOL CRESTOREDLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	Connect();
	SetListStyle();
	m_edit="F:\\HCDB.org";
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRESTOREDLG::SetListStyle()
{
    m_list.InsertColumn(0,"���",LVCFMT_LEFT,100);
	m_list.InsertColumn(1,"ʱ��",LVCFMT_LEFT,140);
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
    m_record=NULL;
	m_record.CreateInstance(__uuidof(Recordset));
	CString sqlstr;
	sqlstr.Format("select * from tb_backup");
	m_record->Open((_variant_t )sqlstr,theApp.m_connection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
	try
	{
		int i=0;
		while(!m_record->_EOF)
		{
			CString str=(LPCSTR)(_bstr_t)m_record->Fields->GetItem("id")->Value;
			m_list.InsertItem(i,str);
			COleDateTime tm;
			tm=(COleDateTime)m_record->Fields->GetItem("backuptime")->Value;
		    m_list.SetItemText(i,1,tm.Format("%Y-%m-%d-%H:%M:%S"));
			i++;
			m_record->MoveNext();
		}
	}
	catch(...)
	{
		MessageBox("û�л�ԭ��¼��");
		CDialog::OnCancel();
	}
}

void CRESTOREDLG::OnButton4() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

void CRESTOREDLG::OnButton1() 
{
	// TODO: Add your control notification handler code here
	 CFileDialog cfile(TRUE,"org",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"�����ļ�(*.org)|*.org|All Files (*.*)|*.*||");
     
	if(cfile.DoModal())
	{
	   m_edit="";
	   CString str;
	   str=cfile.GetPathName();	

       m_edit=str;
	   UpdateData(false);
	}
    else return;
}
UINT restore( LPVOID pParam )
{
	AfxMessageBox("���ڻ�ԭ��");
	return 0;
}
void CRESTOREDLG::OnButton3() 
{
	AfxBeginThread(restore,this);
	UpdateData();
   	try
	{
	   theApp.m_connection->Close();
       CString sqlstr;
	   sqlstr="select spid from master..sysprocesses where dbid=db_id('Sales')";
	   //�����"���ǵ����ݿ�"�����ӵĽ��������ִ�
	   _RecordsetPtr recordset;
	   recordset.CreateInstance(__uuidof(Recordset));
	   recordset=theApp.m_connection1->Execute((_bstr_t)sqlstr,NULL,adModeUnknown);
	   //theApp.m_connection1�����ӵ�master�ϵ�
	   while(!recordset->_EOF)
	   {
		   CString temp=(LPCSTR)(_bstr_t)recordset->Fields->GetItem("spid")->Value;
		   sqlstr.Format("kill %i",atoi(temp));//�Ͽ���theApp.m_connection���ӵĽ���
		   theApp.m_connection1->Execute((_bstr_t)sqlstr,NULL,adModeUnknown);
		   recordset->MoveNext();
	   }
	   sqlstr.Format(" restore database Sales from disk='%s'",m_edit);//���ݻ�ԭ
	  
	   theApp.m_connection1->Execute((_bstr_t)sqlstr,NULL,adModeUnknown);
       MessageBox("��ԭ�ɹ�!");
	   CDialog::OnCancel();
	   theApp.ConnectSql("Provider=SQLOLEDB.1;Persist Security Info=False;User ID=sa;Password=;Initial Catalog=Sales");
	}
	catch(...)
	{
		MessageBox("��ԭʧ��");
		CDialog::OnCancel();
	}
}

void CRESTOREDLG::OnItemchangedList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString id=m_list.GetItemText(pNMListView->iItem,0);
	CString sqlstr;
	sqlstr.Format("select * from tb_backup where id=%s",id);
	m_record->Close();
	m_record=theApp.m_connection->Execute((_bstr_t)sqlstr,NULL,adModeUnknown);
    m_edit=(LPCSTR)(_bstr_t)m_record->Fields->GetItem("filepath")->Value;
	UpdateData(false);
	*pResult = 0;
}
