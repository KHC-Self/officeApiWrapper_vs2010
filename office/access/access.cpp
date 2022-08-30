#include "afxole.h"
#include "atlstr.h"
#include "atltime.h"
#include "access.h"

BOOL ADOaccess::InitADOaccess(string DatabaseName)
{
	CoInitialize(NULL);         //��ʼ��OLE/COM�⻷��  
	try {
		m_pConnection.CreateInstance("ADODB.Connection");  //�������Ӷ���ʵ��
		CString connect1 = "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=";
		CString Connect = connect1 + DatabaseName.c_str();
		_bstr_t strConnect = Connect;
		m_pConnection->Open(strConnect, "", "", adModeUnknown); //�����ݿ�
	}
	catch (_com_error e) {
		return FALSE;
	}
	return TRUE;
}

void ADOaccess::ExitAccess()
{
	if (m_pRecordset != NULL){
		m_pRecordset->Close();
	}
	m_pConnection->Close();
	//�ͷŻ���
	CoUninitialize();
}

_RecordsetPtr & ADOaccess::GetRecordSet(_bstr_t bstrSQL)
{
	try {
		if (m_pConnection == NULL) //�ж�Connection�����Ƿ�Ϊ��	
			InitADOaccess(DataBaseName); //���Ϊ���������������ݿ�		
		m_pRecordset.CreateInstance("ADODB.Recordset"); //������¼������		
		//��ȡ���ݱ��е�����		
		m_pRecordset->Open(bstrSQL, m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	}	
	catch (_com_error e) {	//�����쳣
		_RecordsetPtr pt;
		return pt;
	}	
	return m_pRecordset;
	// TODO: �ڴ˴����� return ���
}

BOOL ADOaccess::ExecuteSQL(_bstr_t bstrSQL)
{
	try {
		if(!m_pConnection)
			InitADOaccess(DataBaseName); //���Ϊ���������������ݿ�		
		m_pConnection->Execute(bstrSQL, NULL , adCmdText);//ִ��sql����
	}
	catch (_com_error e) {
		return FALSE;
	}
	return TRUE;
}

bool ADOaccess::addMessage(StuInf pt)
{
	if (!m_pConnection)
		InitADOaccess(DataBaseName); //���Ϊ���������������ݿ�	
	_variant_t RecordsAffected;                        //����������
	CString AddSql;
	CString median = "INSERT INTO student(code,name,phoneNumber,native) VALUES('";
	AddSql = median + pt.Code + "','" + pt.Name + "','" + pt.PhoneCode + "','" + pt.Native + "')";
	return ExecuteSQL(_bstr_t(AddSql));
}

vector<StuInf> ADOaccess::getAlldata()
{
	vector<StuInf> alldata;
	StuInf data;
	if (!m_pConnection)
		InitADOaccess(DataBaseName); //���Ϊ���������������ݿ�	
	_bstr_t bstrSQL = "select * from student";
	m_pRecordset = GetRecordSet(bstrSQL);
	if (m_pRecordset) {
		while (!m_pRecordset->adoEOF) {			
			data.Code = VariantToString(m_pRecordset->GetadoFields()->GetItem(long(0))->Value);
			data.Name = VariantToString(m_pRecordset->GetadoFields()->GetItem(long(1))->Value);
			data.PhoneCode = VariantToString(m_pRecordset->GetadoFields()->GetItem(long(2))->Value);
			data.Native = VariantToString(m_pRecordset->GetadoFields()->GetItem(long(3))->Value);
			
			alldata.push_back(data);
			m_pRecordset->MoveNext();
		}
	}
	m_pRecordset = _RecordsetPtr();
	return alldata;
}

CString VariantToString(VARIANT var) {
	CString strValue;
	_variant_t var_t;
	_bstr_t bstr_t1;
	time_t cur_time;
	CTime time_value;
	COleCurrency var_currency;
	switch (var.vt)
	{
	case VT_EMPTY:
	case VT_NULL:strValue = _T(""); break;
	case VT_UI1:strValue.Format(_T("%d"), var.bVal); break;
	case VT_I2:strValue.Format(_T("%d"), var.iVal); break;
	case VT_I4:strValue.Format(_T("%d"), var.lVal); break;
	case VT_R4:strValue.Format(_T("%f"), var.fltVal); break;
	case VT_R8:strValue.Format(_T("%f"), var.dblVal); break;
	case VT_CY:
		var_currency = var;
		strValue = var_currency.Format(0); break;
	case VT_BSTR:
		var_t = var;
		bstr_t1 = var_t;
		strValue.Format(_T("%s"), (const char *)bstr_t1); break;
	case VT_DATE:
		cur_time = var.date;
		time_value = cur_time;
		strValue.Format(_T("%A,%B,%d,%Y")); break;
	case VT_BOOL:strValue.Format(_T("%d"), var.boolVal); break;
	default:strValue = _T(""); break;
	}
	return strValue;
}
