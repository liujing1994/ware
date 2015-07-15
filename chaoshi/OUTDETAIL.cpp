// OUTDETAIL.cpp : implementation file
//

#include "stdafx.h"
#include "HCDB.h"
#include "OUTDETAIL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COUTDETAIL dialog

extern CHCDBApp theApp;
COUTDETAIL::COUTDETAIL(CWnd* pParent /*=NULL*/)
	: CDialog(COUTDETAIL::IDD, pParent)
{
	//{{AFX_DATA_INIT(COUTDETAIL)
	// NOTE: the ClassWizard will add member initialization here
	m_begintime = 0;
	m_endtime = 0;
	m_edit2 = _T("");
	//}}AFX_DATA_INIT
}


void COUTDETAIL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COUTDETAIL)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_LIST2, m_list2ctrl);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_endtimectrl);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_begintimectrl);
	DDX_Control(pDX, IDC_COMBO1, m_comquery);
	DDX_Control(pDX, IDC_CHECK2, m_check2);
	DDX_Control(pDX, IDC_CHECK1, m_check);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_begintime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_endtime);
	DDX_Text(pDX, IDC_EDIT2, m_edit2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COUTDETAIL, CDialog)
	//{{AFX_MSG_MAP(COUTDETAIL)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COUTDETAIL message handlers

CString COUTDETAIL::GetMerc(CString sqlstr)
{
    if( m_mercRecord!=NULL)
  {
	  m_mercRecord=NULL;
	  m_mercRecord.CreateInstance(__uuidof(Recordset));
  }
   for (int i=0;i < m_list2ctrl.GetCount();i++)
   {
	   m_list2ctrl.DeleteString(i);
   }

    m_list2ctrl.ResetContent();
   UpdateData(false);
   m_mercRecord->Open((_variant_t )sqlstr,theApp.m_connection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
   CString str[10],returnstr;
   str[0]="��Ʒ��ţ�";
   str[1]="��Ʒ���ƣ�";
   str[2]="�ͺŹ��";
   str[3]="��λ�� ";
   str[4]="����:";
   str[5]="��Ӧ��:";
   str[6]="�ۿۣ�";
   str[7]="������:";
   str[8]="��������:";
   str[9]="��ע:";
   CString temp;
   int rows=0;
   int a=GetRecordCount(m_mercRecord);
   for(int j=0;j<a;j++)
   {
	   for(int i=0;i<10;i++)
	   {
		   temp=(LPCSTR)(_bstr_t)m_mercRecord->Fields->GetItem((long)i)->Value;
		   returnstr+=str[i]+temp+"\r\n   ";
	   }
	   
	   
	   m_list2ctrl.InsertString(rows,returnstr);
       returnstr="";
       rows++;
	   m_mercRecord->MoveNext();
   }
   UpdateData(false);
   return returnstr;
}

void COUTDETAIL::GetMercRecord(CString sqlstr)
{
    m_mercRecord=NULL;
	m_mercRecord.CreateInstance(__uuidof(Recordset));
	CString strgetprovname;
	m_mercRecord->Open((_variant_t )sqlstr,theApp.m_connection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
	if(m_mercRecord->RecordCount==0)
	{ 
		MessageBox("û���ҵ�");
		return;
	}
}

void COUTDETAIL::GetRecord(CString sqlstr)
{
     m_record=NULL;
	 m_record.CreateInstance(__uuidof(Recordset));
	 CString strgetprovname;
	 m_record->Open((_variant_t )sqlstr,theApp.m_connection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
}

int COUTDETAIL::GetRecordCount(_RecordsetPtr ptr)
{
     int count=0;
	if(ptr!=NULL)
	{
		try
		{
			ptr->MoveFirst();
		}
		catch(...)
		{
			return 0;
		}
		if(ptr->_EOF)return 0;
		while(!ptr->_EOF)
		{
			ptr->MoveNext();
			count++;
		}
	}
	ptr->MoveFirst();
	return count;
}

void COUTDETAIL::InitDate()
{

	CTime tm=CTime::GetCurrentTime();
	m_begintime=tm;
    m_endtime=tm;
	UpdateData(false);
}

void COUTDETAIL::SetListStyle()
{
   	m_list.InsertColumn(0,"��Ʒ���",LVCFMT_LEFT,80);
	m_list.InsertColumn(1,"��Ʒ����",LVCFMT_LEFT,120);
	m_list.InsertColumn(2,"����",LVCFMT_LEFT,80);
	m_list.InsertColumn(3,"����",LVCFMT_LEFT,80);
	m_list.InsertColumn(4,"�ۿ�",LVCFMT_LEFT,80);
	m_list.InsertColumn(5,"���",LVCFMT_LEFT,80);
	m_list.InsertColumn(6,"����Ա",LVCFMT_LEFT,120);
	m_list.InsertColumn(7,"������",LVCFMT_LEFT,120);
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
}

void COUTDETAIL::OnButton1() 
{
	if((m_check.GetCheck()==false)&&(m_check2.GetCheck()==false))
	{
		MessageBox("�����ò�ѯ����","ϵͳ��ʾ",64);
		return ;
	}
	if(m_check.GetCheck()==true&&(m_check2.GetCheck()==false))
	{
		UpdateData();
		if(m_edit2=="")
		{
         MessageBox("�����ò�ѯ����","ϵͳ��ʾ",64);
		 return ;
		}
		CString sqlstr;
		m_list.DeleteAllItems();
		CString fileds;
        switch(m_comquery.GetCurSel())
		{
		case 0:fileds="operid";break;
		case 1:fileds="provid";break;
		case 2:fileds="storid";break;
		case 3:fileds="merchandiseid";break;
		}

        sqlstr.Format("select * from tb_storage where %s='%s' and storflag='1'",fileds,m_edit2);
        GetRecord(sqlstr);
        int row=0;
		while(!m_record->_EOF)
		{
			CString temp;
		
			temp=(LPCSTR)(_bstr_t)m_record->GetFields()->GetItem("merchandiseid")->Value;
            m_list.InsertItem(row,temp);
			sqlstr.Format("select * from tb_merchangiseinfo where id='%s'",temp);
			GetMercRecord(sqlstr);
			temp=(LPCSTR)(_bstr_t)m_mercRecord->GetFields()->GetItem("mercname")->Value;
			m_list.SetItemText(row,1,temp);
            temp=(LPCSTR)(_bstr_t)m_mercRecord->GetFields()->GetItem("inprice")->Value;
			m_list.SetItemText(row,2,temp);
			temp=(LPCSTR)(_bstr_t)m_record->GetFields()->GetItem("numbers")->Value;
			m_list.SetItemText(row,3,temp);
			temp=(LPCSTR)(_bstr_t)m_record->GetFields()->GetItem("paymoney")->Value;
			m_list.SetItemText(row,4,temp);
			temp=(LPCSTR)(_bstr_t)m_mercRecord->GetFields()->GetItem("discount")->Value;
			m_list.SetItemText(row,5,temp);
			temp=(LPCSTR)(_bstr_t)m_record->GetFields()->GetItem("operid")->Value;
			
			m_list.SetItemText(row,6,temp);
			temp=(LPCSTR)(_bstr_t)m_record->GetFields()->GetItem("storid")->Value;
			m_list.SetItemText(row,7,temp);
			m_record->MoveNext();
			row++;
		}
		CString str;
		
		str.Format("�ҵ���%d����¼",row);
		MessageBox(str);
	}


	if(m_check.GetCheck()==true&&(m_check2.GetCheck()==true))
	{
		UpdateData();
		if(m_edit2=="")
		{
         MessageBox("�����ò�ѯ����","ϵͳ��ʾ",64);
		 return ;
		}
		CString sqlstr;
		m_list.DeleteAllItems();
		CString fileds;
        switch(m_comquery.GetCurSel())
		{
		case 0:fileds="operid";break;
		case 1:fileds="provid";break;
		case 2:fileds="storid";break;
		case 3:fileds="merchandiseid";break;
		}

        sqlstr.Format("select * from tb_storage where %s='%s' and storflag='1' and stordate between '%s' and '%s'",fileds,m_edit2,m_begintime.Format("%Y-%m-%d"),m_endtime.Format("%Y-%m-%d"));
        GetRecord(sqlstr);
        int row=0;
		while(!m_record->_EOF)
		{
			CString temp;
		
			temp=(LPCSTR)(_bstr_t)m_record->GetFields()->GetItem("merchandiseid")->Value;
            m_list.InsertItem(row,temp);
			sqlstr.Format("select * from tb_merchangiseinfo where id='%s'",temp);
			GetMercRecord(sqlstr);
			temp=(LPCSTR)(_bstr_t)m_mercRecord->GetFields()->GetItem("mercname")->Value;
			m_list.SetItemText(row,1,temp);
            temp=(LPCSTR)(_bstr_t)m_mercRecord->GetFields()->GetItem("inprice")->Value;
			m_list.SetItemText(row,2,temp);
			temp=(LPCSTR)(_bstr_t)m_record->GetFields()->GetItem("numbers")->Value;
			m_list.SetItemText(row,3,temp);
			temp=(LPCSTR)(_bstr_t)m_record->GetFields()->GetItem("paymoney")->Value;
			m_list.SetItemText(row,4,temp);
			temp=(LPCSTR)(_bstr_t)m_mercRecord->GetFields()->GetItem("discount")->Value;
			m_list.SetItemText(row,5,temp);
			temp=(LPCSTR)(_bstr_t)m_record->GetFields()->GetItem("operid")->Value;
			
			m_list.SetItemText(row,6,temp);
			temp=(LPCSTR)(_bstr_t)m_record->GetFields()->GetItem("storid")->Value;
			m_list.SetItemText(row,7,temp);
			m_record->MoveNext();
			row++;
		}
		CString str;
		
		str.Format("�ҵ���%d����¼",row);
		MessageBox(str);
	}
		if(m_check.GetCheck()==false&&(m_check2.GetCheck()==true))
	{
		UpdateData();
        CString sqlstr;
        sqlstr.Format("select * from tb_storage where stordate between '%s' and '%s'",m_begintime.Format("%Y-%m-%d"),m_endtime.Format("%Y-%m-%d"));
        GetRecord(sqlstr);
        int row=0;
		m_list.DeleteAllItems();
		while(!m_record->_EOF)
		{
			CString temp;
		
			temp=(LPCSTR)(_bstr_t)m_record->GetFields()->GetItem("merchandiseid")->Value;
            m_list.InsertItem(row,temp);
			sqlstr.Format("select * from tb_merchangiseinfo where id='%s'",temp);
			GetMercRecord(sqlstr);
			temp=(LPCSTR)(_bstr_t)m_mercRecord->GetFields()->GetItem("mercname")->Value;
			m_list.SetItemText(row,1,temp);
            temp=(LPCSTR)(_bstr_t)m_mercRecord->GetFields()->GetItem("inprice")->Value;
			m_list.SetItemText(row,2,temp);
			temp=(LPCSTR)(_bstr_t)m_record->GetFields()->GetItem("numbers")->Value;
			m_list.SetItemText(row,3,temp);
			temp=(LPCSTR)(_bstr_t)m_record->GetFields()->GetItem("paymoney")->Value;
			m_list.SetItemText(row,4,temp);
			temp=(LPCSTR)(_bstr_t)m_mercRecord->GetFields()->GetItem("discount")->Value;
			m_list.SetItemText(row,5,temp);
			temp=(LPCSTR)(_bstr_t)m_record->GetFields()->GetItem("operid")->Value;
			
			m_list.SetItemText(row,6,temp);
			temp=(LPCSTR)(_bstr_t)m_record->GetFields()->GetItem("storid")->Value;
			m_list.SetItemText(row,7,temp);
			m_record->MoveNext();
			row++;
		}
		CString str;
		
		str.Format("�ҵ���%d����¼",row);
		MessageBox(str);
}	
}

void COUTDETAIL::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int iItem=pNMListView->iItem;
   	m_list2ctrl.ShowWindow(true);
	m_list2ctrl.ResetContent();
	CString sqlstr,temp;
	temp=m_list.GetItemText(iItem,0);
    m_mercid=temp;
	sqlstr.Format("select distinct a.id,a.mercname,a.spec,a.mercunit,a.inprice,c.provname,\
		a.discount,b.storid,b.stortype,b.storinfo from tb_merchangiseinfo a inner join tb_storage b on a.id='%s' and \
		b.merchandiseid='%s' and storflag='0' inner join tb_provider c on a.provid=c.provid",m_mercid,m_mercid);
	GetMerc(sqlstr);	
	*pResult = 0;
}

void COUTDETAIL::OnCheck1() 
{
	if(!m_check.GetCheck())
   	InitDate();	
}

void COUTDETAIL::OnCheck2() 
{
	if(!m_check2.GetCheck())
   	InitDate();	
}

BOOL COUTDETAIL::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetListStyle();
	InitDate();
	m_comquery.SetCurSel(0);
	m_list2ctrl.ShowWindow(false);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
