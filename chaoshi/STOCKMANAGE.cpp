// STOCKMANAGE.cpp : implementation file
//

#include "stdafx.h"
#include "HCDB.h"
#include "STOCKMANAGE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSTOCKMANAGE dialog

extern CHCDBApp theApp;
CSTOCKMANAGE::CSTOCKMANAGE(CWnd* pParent /*=NULL*/)
	: CDialog(CSTOCKMANAGE::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSTOCKMANAGE)
	m_mercid = _T("");
	m_min = 0.0;
	m_max = 0.0;
	//}}AFX_DATA_INIT
	item=-1;
}


void CSTOCKMANAGE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSTOCKMANAGE)
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Control(pDX, IDC_CHECK2, m_check2);
	DDX_Control(pDX, IDC_CHECK1, m_check1);
	DDX_Text(pDX, IDC_EDIT_NAME, m_mercid);
	DDX_Text(pDX, IDC_EDIT_MIN, m_min);
	DDX_Text(pDX, IDC_EDIT_MAX, m_max);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSTOCKMANAGE, CDialog)
	//{{AFX_MSG_MAP(CSTOCKMANAGE)
	ON_BN_CLICKED(IDC_BTN_QUERY, OnBtnQuery)
	ON_BN_CLICKED(IDC_BTN_SET, OnBtnSet)
	ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, OnItemchangedList)
	ON_BN_CLICKED(IDC_BUTTON_SHOWSPECIAL, OnButtonShowspecial)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTOCKMANAGE message handlers

void CSTOCKMANAGE::OnBtnQuery() 
{
	// TODO: Add your control notification handler code here
    if(Justify()=="")
		return;
	GetRecord(Justify());

}

void CSTOCKMANAGE::OnBtnSet() 
{
	// TODO: Add your control notification handler code here
	CString sqlstr;
	UpdateData();
	sqlstr.Format("Update tb_storemanage set uplimit=%.2f ",m_max);
    if(Justify()=="")
		return;
	try{
		theApp.m_connection->Execute(_bstr_t(sqlstr),NULL,adModeUnknown);
		sqlstr.Format("Update tb_storemanage set  downlimit=%.2f",m_min);
		theApp.m_connection->Execute(_bstr_t(sqlstr),NULL,adModeUnknown);
		MessageBox("����ɹ�!");
		GetDlgItem(IDC_BTN_SET)->ShowWindow(false);
	}
	catch(...)
	{
		MessageBox("û�и��³ɹ�!");
	}
	
	ShowListValue(Justify());
	
}

void CSTOCKMANAGE::OnBtnClose() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

BOOL CSTOCKMANAGE::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_BTN_SET)->ShowWindow(false);

	SetListStyle();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CSTOCKMANAGE::SetListStyle()
{
    m_list.InsertColumn(0,"��Ʒ���",LVCFMT_LEFT,120);
	m_list.InsertColumn(1,"��Ʒ����",LVCFMT_LEFT,120);
	m_list.InsertColumn(2,"����",LVCFMT_LEFT,80);
	m_list.InsertColumn(3,"�������",LVCFMT_LEFT,120);
	m_list.InsertColumn(4,"�������",LVCFMT_LEFT,120);
	m_list.InsertColumn(5,"�����",LVCFMT_LEFT,120);
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
}

void CSTOCKMANAGE::GetRecord(CString sqlstr)
{ 
  ShowListValue(sqlstr);
  if(m_list.GetItemCount()==0)
	  return;
  if(MessageBox("�Ƿ��������ÿ��������?","ϵͳ��ʾ",MB_YESNO|MB_ICONQUESTION)==6)
  {
	  GetDlgItem(IDC_BTN_SET)->ShowWindow(true);
	  GetDlgItem(IDC_STATIC1)->SetWindowText("����������!");
  }
  else return;
}

void CSTOCKMANAGE::OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	item=pNMListView->iItem;
	m_min=atof(m_list.GetItemText(item,3));
    m_max=atof(m_list.GetItemText(item,4));
	UpdateData(false);
	*pResult = 0;
}

CString CSTOCKMANAGE::Justify()
{
	UpdateData();
    CString sqlstr;
	if((m_check1.GetCheck()==true)&&(m_check2.GetCheck()==true))
	{
		MessageBox("�벻Ҫͬʱѡ��!");
		return "";
	}
	else if((m_check1.GetCheck()==false)&&(m_check2.GetCheck()==true))	
	{
		if(m_mercid==""){MessageBox("������Ҫ��ѯ������");return "";}
		sqlstr.Format("select a.mercid,b.mercname,b.inprice,a.downlimit,a.uplimit,a.nowvalues from tb_storemanage a inner join tb_merchangiseinfo\
			b on a.mercid=b.id where a.mercid='%s'",m_mercid);
	}
	else if((m_check1.GetCheck()==true)&&(m_check2.GetCheck()==false))
	{
	sqlstr.Format("select a.mercid,b.mercname,b.inprice,a.downlimit,a.uplimit,a.nowvalues from tb_storemanage a inner join tb_merchangiseinfo\
		b on a.mercid=b.id where a.downlimit>=%.2f and a.uplimit<=%.2f",m_min,m_max);
	
	
	}
	else
	{
		MessageBox("������Ҫ��ѯ������");
		return "";
	}
	return sqlstr;
}

void CSTOCKMANAGE::ShowListValue(CString sqlstr)
{
     m_record=NULL;
   m_record.CreateInstance(__uuidof(Recordset));
   m_record=theApp.m_connection->Execute(_bstr_t(sqlstr),NULL,adModeUnknown);
   m_list.DeleteAllItems();
   int rows=0;
   try
   {
	   while(!m_record->_EOF)
	   {
		   for(int i=0;i<6;i++)
		   {
			   CString temp=(LPCSTR)(_bstr_t)m_record->Fields->GetItem(long(i))->Value;
			   if(i==0)m_list.InsertItem(rows,temp);
			   else m_list.SetItemText(rows,i,temp);
		   }
		   m_record->MoveNext();
		   rows++;
	   }
   }
   catch(_com_error e)
   {
	   MessageBox(e.ErrorMessage());
   }
   if(m_list.GetItemCount()==0)
   {
	   MessageBox("û���ҵ���¼");
	   return;
   }
}

void CSTOCKMANAGE::OnButtonShowspecial() 
{
	// TODO: Add your control notification handler code here
	
}
