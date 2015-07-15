// ForeMange.cpp : implementation file
//

#include "stdafx.h"
#include "HCDB.h"
#include "ForeMange.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CForeMange dialog
#include "PASSDLG.h"
extern CHCDBApp theApp;
CForeMange::CForeMange(CWnd* pParent /*=NULL*/)
	: CDialog(CForeMange::IDD, pParent)
{
	//{{AFX_DATA_INIT(CForeMange)
	m_id = _T("");
	m_num = 0.0;
	m_singlediscount = _T("");
	m_allmoney = _T("");
	m_backmoney = _T("");
	m_realmoney = _T("");
	m_editrealmoney = _T("");
	m_buynum = _T("");
	//}}AFX_DATA_INIT
	m_alreadynum=1;
	m_enter=false;
	m_allmoney="�ۼ�\r\n���:";
	m_realmoney="ʵ��\r\n���:";
	m_backmoney="�һ�\r\n���:";
	m_backenter=false;
	m_delitem=-1;
	m_delflag=false;
}


void CForeMange::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CForeMange)
	DDX_Control(pDX, IDC_LIST2, m_list2);
	DDX_Control(pDX, IDC_EDIT_REALMONEY, m_editrealmoneyctrl);
	DDX_Control(pDX, IDC_BACKMONEY, m_backmoneyctrl);
	DDX_Control(pDX, IDC_ANOTHER, m_another);
	DDX_Control(pDX, IDC_REALLMONEY, m_realmoneyctrl);
	DDX_Control(pDX, IDC_ALLMONEY, m_allmoneyctrl);
	DDX_Control(pDX, IDC_MERCNUM, m_mercnum);
	DDX_Control(pDX, IDC_MERCID, m_mercid);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_MERCID, m_id);
	DDX_Text(pDX, IDC_MERCNUM, m_num);
	DDX_Text(pDX, IDC_SINGLEDISCOUNT, m_singlediscount);
	DDX_Text(pDX, IDC_ALLMONEY, m_allmoney);
	DDX_Text(pDX, IDC_BACKMONEY, m_backmoney);
	DDX_Text(pDX, IDC_REALLMONEY, m_realmoney);
	DDX_Text(pDX, IDC_EDIT_REALMONEY, m_editrealmoney);
	DDX_Text(pDX, IDC_BUYNUM, m_buynum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CForeMange, CDialog)
	//{{AFX_MSG_MAP(CForeMange)
	ON_BN_CLICKED(IDC_BTN_LOCKSCREEN, OnBtnLockscreen)
	ON_BN_CLICKED(IDC_BTNOK, OnBtnok)
	ON_WM_CTLCOLOR()
	ON_WM_CHAR()
	ON_BN_CLICKED(IDC_BTN_DELMERC, OnBtnDelmerc)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	ON_BN_CLICKED(IDC_BTNCANCELSALES, OnBtncancelsales)
	ON_BN_CLICKED(IDC_BTNQUIT, OnBtnquit)
	ON_EN_KILLFOCUS(IDC_MERCID, OnKillfocusMercid)
	ON_EN_KILLFOCUS(IDC_MERCNUM, OnKillfocusMercnum)
	ON_EN_KILLFOCUS(IDC_EDIT_REALMONEY, OnKillfocusEditRealmoney)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CForeMange message handlers

void CForeMange::OnBtnLockscreen() 
{
	// TODO: Add your control notification handler code here

}


BOOL CForeMange::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//////////////////////////////////////
    SetListStyle();
	this;
	///////////////////////////////////////
	m_mercid.InsertItem=m_alreadynum-1;
	///////////////////////////////////////////
	SetStaticStyle();
	SetEditStyle(16);
    SetSalesNum();
	if(rectlarge.IsRectNull())
	{
		CRect rectseparator;
        GetWindowRect(&rectlarge);
		GetDlgItem(IDC_SEPARATOR)->GetWindowRect(&rectseparator);
		rectsmall.left=rectlarge.left;
		rectsmall.top=rectlarge.top;
		rectsmall.right=rectseparator.right;
		rectsmall.bottom=rectseparator.bottom;
	}
	SetWindowPos(NULL,0,0,rectsmall.Width(),rectsmall.Height(),SWP_NOMOVE|SWP_NOZORDER);
	SetList2Style();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CForeMange::SetListStyle()
{
	m_list.InsertColumn(0,"���",LVCFMT_CENTER,80);
	m_list.InsertColumn(1,"��Ʒ���",LVCFMT_CENTER,80);
	m_list.InsertColumn(2,"��Ʒ����",LVCFMT_CENTER,140);
	m_list.InsertColumn(3,"����",LVCFMT_CENTER,80);
	m_list.InsertColumn(4,"����",LVCFMT_CENTER,80);
	m_list.InsertColumn(5,"�ۿ�",LVCFMT_CENTER,80);
	m_list.InsertColumn(6,"���",LVCFMT_CENTER,80);
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE);
}
BOOL CForeMange::PreTranslateMessage(MSG* pMsg) 
{	
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) //��ȡ�س���   
	{
		if(::GetFocus()==m_mercnum)
		{
			UpdateData();
			CString sqlstr;
			sqlstr.Format("select * from tb_merchangiseinfo where id='%s'",m_id);
			if(GetRecord(sqlstr)==0)
				return false;
			CString str=(LPCSTR)(_bstr_t)m_record->Fields->GetItem("number")->Value;
			if(double(atof(str))<m_num)
			{
				MessageBox("��������!");
				m_id="";
				m_singlediscount="";
				m_num=0;
				m_mercid.SetFocus();
				m_mercid.querystr="";
				m_mercnum.num=0;
				return false;
			}
			else
			{
				m_id="";
				m_singlediscount="";				
				m_mercid.SetFocus();
				m_mercid.querystr="";
				m_mercnum.num=0;
				m_alreadynum=m_list.GetItemCount();
				m_mercid.InsertItem=m_alreadynum;
				m_mercnum.SetWindowText("");
				m_mercid.m_querystr="";
				m_num=0;
				m_enter=true;
				GetAllMoney();
			}
			return true;
		}
		else if(::GetFocus()==m_editrealmoneyctrl)
		{
			SetBackMoney();
			m_backenter=true;
			return false;
		}
		else return false;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CForeMange::SetStaticStyle()
{
    m_allmoneyctrl.SetFontSize(16);
	m_allmoneyctrl.SetTextColor(RGB(0,255,0));
	m_allmoneyctrl.SetBkColor(RGB(0,0,0));
	m_allmoneyctrl.SetFontBold(TRUE);
	m_allmoneyctrl.SetText(m_allmoney);
	///////////////////////////////////////
	m_realmoneyctrl.SetFontSize(16);
	m_realmoneyctrl.SetTextColor(RGB(0,255,0));
	m_realmoneyctrl.SetBkColor(RGB(0,0,0));
	m_realmoneyctrl.SetFontBold(TRUE);
	m_realmoneyctrl.SetText(m_realmoney);
	/////////////////////////////////////////
	m_backmoneyctrl.SetFontSize(16);
	m_backmoneyctrl.SetTextColor(RGB(0,255,0));
	m_backmoneyctrl.SetBkColor(RGB(0,0,0));
	m_backmoneyctrl.SetFontBold(TRUE);
	m_backmoneyctrl.SetText(m_backmoney);
	//////////////////////////////////////
	m_another.SetFontSize(16);
	m_another.SetTextColor(RGB(0,255,0));
	m_another.SetBkColor(RGB(0,0,0));
	m_another.SetFontBold(TRUE);
	m_another.SetText("");
	//////////////////////////////////

}

void CForeMange::OnBtnok() 
{
	UpdateData();
	if(!m_list.GetItemCount())
	{
		MessageBox("ȷ�������Ʒ?","ϵͳ��ʾ!",MB_OK|MB_ICONSTOP);
		return;
	}
	if(m_editrealmoney=="")
	{
		MessageBox("ȷ������ʵ�����?","ϵͳ��ʾ!",MB_OK|MB_ICONSTOP);
		return;
	}
    if(!m_backenter)
	{
		MessageBox("��ȷ��������?","ϵͳ��ʾ!",MB_OK|MB_ICONSTOP);
		return;
	}
	m_backenter=false;
	m_salesrs->Close();
	CString sqlstr;
    for(int i=0;i<m_list.GetItemCount();i++)
	{ 
		m_salesrs=NULL;
	    m_salesrs.CreateInstance(__uuidof(Recordset));
		CString strmercid,strbuyid,strbuynum;
	    strmercid=m_list.GetItemText(i,1);
        strbuyid=m_buynum;
		strbuynum=m_list.GetItemText(i,3);
        sqlstr.Format("insert into tb_sale values('%s','%s',%s,'%s','no',0)",strbuyid,strmercid,strbuynum,operid);
		m_salesrs->Open((_variant_t )sqlstr,theApp.m_connection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
        sqlstr.Format("update tb_merchangiseinfo set number=number-%s where id='%s'", strbuynum,strmercid);
		GetRecord(sqlstr);		
	}	
	MessageBox("�ɹ�����!");
	InitValue();
}

void CForeMange::GetAllMoney()
{
  	     float allmoney=0;
	     CString strmoney;
		 int itemcount=m_list.GetItemCount();
		 if(itemcount==0)
		 {
			 ::AfxMessageBox("û�й�����Ʒ!");
			 GetDlgItem(IDC_MERCID)->SetFocus();
			 allmoney=0;
			 m_allmoney.Format("�ۼ�\r\n���:%.2f",allmoney);
			 m_floatallmoney=allmoney;
			 SetAllMoneyStyle();
			 return;
		 }
		 for(int i=0;i<itemcount;i++)
		 {
			 strmoney=m_list.GetItemText(i,6);
			 allmoney+=atof(strmoney);
		 }
		 m_allmoney.Format("�ۼ�\r\n���:%.2f",allmoney);
		 m_floatallmoney=allmoney;
		 SetAllMoneyStyle();
		 UpdateData(false);
}

void CForeMange::SetAllMoneyStyle()
{
	m_allmoneyctrl.SetText(m_allmoney);
}

void CForeMange::SetRealMoneyStyle()
{
	m_realmoneyctrl.SetText(m_realmoney); 
}

void CForeMange::SetBackMoneyStyle()
{
	m_backmoneyctrl.SetText(m_backmoney);
}

void CForeMange::SetEditStyle(int nSize)
{
      
  queryFont.CreateFont(   
  16,                                                 //   nHeight   
  16,                                                   //   nWidth   
  0,                                                   //   nEscapement   
  0,                                                   //   nOrientation   
  FW_NORMAL,                                   //   nWeight   
  FALSE,                                           //   bItalic   
  FALSE,                                           //   bUnderline   
  0,                                                   //   cStrikeOut   
  DEFAULT_CHARSET,                       //   nCharSet   
  OUT_DEFAULT_PRECIS,                 //   nOutPrecision   
  CLIP_DEFAULT_PRECIS,               //   nClipPrecision   
  DEFAULT_QUALITY,                       //   nQuality   
  DEFAULT_PITCH   |   FF_SWISS,     //   nPitchAndFamily   
  "����");                         //   lpszFacename   
    
  m_editrealmoneyctrl.SetFont(&queryFont);
 
  
}

HBRUSH CForeMange::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if(pWnd->GetDlgCtrlID()==IDC_EDIT_REALMONEY) 
	{ 
		  pDC->SetBkColor(RGB(0,0,0));
		  pDC-> SetTextColor(RGB(0,255,0)); 
          return ::CreateSolidBrush(RGB(0,0,0)); 
	}    

	// TODO: Return a different brush if the default is not desired
	return hbr;
}


void CForeMange::SetBackMoney()
{
    UpdateData();
	
	float backmoney=atof(m_editrealmoney)-m_floatallmoney;
	if(backmoney<0)
	{
		MessageBox("����","ϵͳ��ʾ",MB_OK|MB_ICONSTOP);
		return;
	}
	m_backmoney.Format("�һ�\r\n���:%.2f",backmoney);
	SetBackMoneyStyle();
	m_backmoneyctrl.SetFocus();
}

CString CForeMange::SetSalesNum()
{
   CString retstring; 
   CString sqlstr="Select Max(buyid) as maxid from tb_sale";
   m_salesrs=NULL;
   try
   {   
	   m_salesrs.CreateInstance(__uuidof(Recordset));
	   m_salesrs->Open((_variant_t )sqlstr,theApp.m_connection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
	   if(!m_salesrs->RecordCount)
	   {
		 retstring="1000000";
		 m_buynum=retstring;
	     UpdateData(false);
         return retstring;
	   }
	   else
	   {
         retstring=(LPCSTR)(_bstr_t)m_salesrs->Fields->GetItem("maxid")->Value;
		 int temp;
		 temp=atoi(retstring)+1;
         retstring.Format("%d",temp);
		 m_buynum=retstring;
	     UpdateData(false);
		 return retstring;
		
	   } 	   
	   m_salesrs->Close();
   }
   catch(_com_error e)
   {
	   CString strError;
	   strError.Format("CADOWrong is %s\nCode Meaning is %s\nDescription is %s",e.ErrorMessage(),
		   (LPCSTR)e.Source(),(LPCSTR)e.Description());
       ::AfxMessageBox(strError);
   }
}

void CForeMange::InitValue()
{
	if(!m_delflag)
     {   
		m_list.DeleteAllItems();
		m_allmoney="";
		m_alreadynum=1;
		m_enter=false;
		////////////////////////////////////
		m_mercnum.num=0;
		m_mercnum.t=1;
		m_mercnum.m_intdouflag=false;
		m_mercnum.lastnum=0;
		m_mercnum.m_allnum=0;
		m_id="";
		m_num=0;
		//////////////////////////////////////
		m_mercid.querystr="";
		m_mercid.m_backnum=0;
		m_mercid.m_querystr="";
		m_mercid.InsertItem=m_alreadynum-1;
		m_allmoney="�ۼ�\r\n���:";
		SetAllMoneyStyle();
		m_realmoney="ʵ��\r\n���:";
		m_backmoney="�һ�\r\n���:";
		SetStaticStyle();
		//SetEditStyle(16);
		SetSalesNum();
		m_editrealmoney="";
		m_singlediscount="";
	}
	else
	{
	    
		m_delflag=false;
	}
	UpdateData(false);

}

void CForeMange::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	MessageBox("ȷ���˳�");
	CDialog::OnChar(nChar, nRepCnt, nFlags);
}

void CForeMange::OnBtnDelmerc() 
{
     if(m_delitem==-1)
	 {
       MessageBox("��ѡ����Ʒ!");
	   return;
	 }
	 m_delflag=true;
     InitValue();
     DeleteMerc();
  	//MessageBox("ȷ��ɾ����Ʒ");
}

void CForeMange::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    m_delitem=pNMListView->iItem;
	*pResult = 0;
}

void CForeMange::DeleteMerc()
{
    m_list.DeleteItem(m_delitem);
	int i=0;
	int itemcount=m_list.GetItemCount();
	LVCOLUMN;
	
	if(itemcount==0)
	{
		m_alreadynum=1;
		m_mercid.InsertItem=m_alreadynum-1;
		m_delitem=-1;InitValue();
		GetAllMoney();return;
	}
	else {m_mercid.InsertItem=m_mercid.InsertItem-1;m_delitem=-1;}
	float allmoney=0;
	for(i=m_delitem;i<=itemcount;i++)
	{
		CString str;
		str.Format("%d",i);
		m_list.SetItemText(i-1,0,str);
		
	}
	GetAllMoney();
	m_delitem=-1;
	//InitValue();
}

void CForeMange::OnBtncancelsales() 
{
	if(m_list.GetItemCount()==0)
	{
		MessageBox("��ǰû�н���");
		return;
	}
	if(MessageBox("ȷ��ȡ��?","ϵͳ��ʾ",MB_YESNO|MB_ICONSTOP))
	{
		
		InitValue();
	}
}

void CForeMange::OnBtnquit() 
{
	PASSDLG dlg1;
	if(dlg1.DoModal()==IDOK)
	{
		CString sqlstr;
		
		sqlstr.Format("select * from tb_operator where operid='%s' and operpassword='%s'",dlg1.m_user,dlg1.m_pass);
        if(GetRecord(sqlstr)==0){MessageBox("û�и��û�!");return;}
        operid=dlg1.m_user;
	}
	//else
	//	return;
	CString sqlstr;	CString str;
	sqlstr.Format("select * from tb_operator where operid='%s'",operid);
	if(GetRecord(sqlstr)==0)return;

	else
	{
	str=(LPCSTR)(_bstr_t)m_record->Fields->GetItem("operlevel")->Value;
		str.TrimLeft();
		str.TrimRight();
	}
    if(str=="����Ա")
	{
		CDialog::OnCancel();
	}
	else
	{
	::PostQuitMessage(0);
	}
}

void CForeMange::OnKillfocusMercid() 
{
	CString sqlstr;
	UpdateData();
	if(m_id=="")return;
	sqlstr.Format("select * from tb_merchangiseinfo where id='%s'",m_id);
	if(GetRecord(sqlstr)==0)
	{
		MessageBox("û�и���Ʒ");
		GetDlgItem(IDC_MERCID)->SetFocus();
		return;
	}
}

int CForeMange::GetRecord(CString sqlstr)
{
   m_record=NULL;
   m_record.CreateInstance(__uuidof(Recordset));
   m_record->Open((_variant_t )sqlstr,theApp.m_connection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
   int count=0;
   if(m_record!=NULL)
   {
	   try
	   {
		   m_record->MoveFirst();
	   }
	   catch(...)
	   {
		   return 0;
	   }
	   if(m_record->_EOF)return 0;
	   while(!m_record->_EOF)
	   {
		  m_record->MoveNext();
		   count++;
	   }
   }
   m_record->MoveFirst();
   return count;
}

void CForeMange::OnKillfocusMercnum() 
{
	// TODO: Add your control notification handler code here


	//m_num="";
	CString sqlstr;
	sqlstr.Format("select * from tb_merchangiseinfo where id='%s'",m_id);
	if(GetRecord(sqlstr)==0)return ;
	CString str=(LPCSTR)(_bstr_t)m_record->Fields->GetItem("number")->Value;
	if(double(atof(str))<m_num)
	{
		MessageBox("��������!");
		m_id="";
		//m_num="";
		m_singlediscount="";
		
		m_mercid.SetFocus();
		m_mercid.querystr="";
		m_mercnum.num=0;
		m_list.DeleteItem(m_list.GetItemCount()-1);
		UpdateData(false);
		return ;
	}
	m_singlediscount="";
	m_id="";
	m_mercid.SetFocus();
	m_mercid.querystr="";
	m_mercnum.num=0;
	m_alreadynum=m_list.GetItemCount();
	m_mercid.InsertItem=m_alreadynum;
	m_mercnum.SetWindowText("0");
	m_mercid.m_querystr="";
	m_num=0;
	m_enter=true;
	UpdateData(false);
	return ;

}

void CForeMange::OnKillfocusEditRealmoney() 
{
	// TODO: Add your control notification handler code here
	SetBackMoney();
	m_backenter=true;
	return;
}

void CForeMange::OnButton7() 
{
	CString str;
	if(GetDlgItemText(IDC_BUTTON7,str),str=="��ѯ��Ʒ")
	{
		SetDlgItemText(IDC_BUTTON7,"�˳���ѯ");
	}
    else
	{
		SetDlgItemText(IDC_BUTTON7,"��ѯ��Ʒ");
	}
	
	if(rectlarge.IsRectNull())
	{
		CRect rectseparator;
		GetDlgItem(IDC_SEPARATOR)->GetWindowRect(&rectseparator);
		rectsmall.left=rectlarge.left;
		rectsmall.top=rectlarge.top;
		rectsmall.right=rectseparator.right;
		rectsmall.bottom=rectseparator.bottom;
	}
    if(str=="�˳���ѯ")
	{
		SetWindowPos(NULL,0,0,rectsmall.Width(),rectsmall.Height(),SWP_NOMOVE|SWP_NOZORDER);
		
	}
	else
	{
		SetWindowPos(NULL,0,0,rectlarge.Width(),rectlarge.Height(),SWP_NOMOVE|SWP_NOZORDER);
	}
	CString sqlstr;
	UpdateData();
	sqlstr.Format("select id,mercname,number,outprice,discount,spec from tb_merchangiseinfo where id='%s'",m_id);
	SetList2Value(sqlstr);
}

void CForeMange::SetList2Style()
{
	m_list2.InsertColumn(0,"��Ʒ���",LVCFMT_CENTER,80);
	m_list2.InsertColumn(1,"��Ʒ����",LVCFMT_CENTER,140);
	m_list2.InsertColumn(2,"����",LVCFMT_CENTER,80);
	m_list2.InsertColumn(3,"����",LVCFMT_CENTER,80);
	m_list2.InsertColumn(4,"�ۿ�",LVCFMT_CENTER,80);
	m_list2.InsertColumn(5,"���",LVCFMT_CENTER,80);
	m_list2.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE);
}

void CForeMange::SetList2Value(CString sqlstr)
{
	GetRecord(sqlstr);
	m_list2.DeleteAllItems();
	int rows=0;
	try
	{
		for(int i=0;i<6;i++)
		{
			CString str;
			str=(LPCSTR)(_bstr_t)m_record->Fields->GetItem(long(i))->Value;
			if(i==0)m_list2.InsertItem(rows,str);
			else
			m_list2.SetItemText(rows,i,str);
		}
		UpdateData(false);
	}
	catch(...)
	{
		return;
	}
}

void CForeMange::OnButton6() 
{
	PASSDLG dlg1;
	if(dlg1.DoModal()==IDOK)
	{
		CString sqlstr;		
		sqlstr.Format("select * from tb_operator where operid='%s' and operpassword='%s'",dlg1.m_user,dlg1.m_pass);
        if(GetRecord(sqlstr)==0){MessageBox("û�и��û�!");return;}
        operid=dlg1.m_user;
	}
}

void CForeMange::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	MessageBox("�벻Ҫ�˳�");
//	CDialog::OnClose();
}
