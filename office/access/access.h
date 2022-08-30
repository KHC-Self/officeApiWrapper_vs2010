#pragma once
#import ".\msado15.dll" \
no_namespace \
rename("EOF", "adoEOF") rename("DataTypeEnum", "adoDataTypeEnum") \
rename("FieldAttributeEnum", "adoFielAttributeEnum") rename("EditModeEnum", "adoEditModeEnum") \
rename("LockTypeEnum", "adoLockTypeEnum") rename("RecordStatusEnum", "adoRecordStatusEnum") \
rename("ParameterDirectionEnum", "adoParameterDirectionEnum") \
rename("Field", "adoField") rename("Fields", "adoFields")

#include <string>
#include <vector>
using namespace std;
CString VariantToString(VARIANT var);

struct StuInf {
	CString Code;
	CString Name;
	CString PhoneCode;
	CString Native;
};
class ADOaccess {
public:
	ADOaccess() {}
	ADOaccess(string dataname):DataBaseName(dataname){}

	BOOL InitADOaccess(string DatabaseName);		//����access���ݿ�,��ʼ��m_pConnection

	BOOL InitADOaccess() {
		return InitADOaccess(DataBaseName);
	}

	void ExitAccess();								//�ر����ݿ�����

	_RecordsetPtr& GetRecordSet(_bstr_t bstrSQL);	//��ȡ��¼��

	BOOL ExecuteSQL(_bstr_t bstrSQL);				//ִ�����ݿ����

	bool addMessage(StuInf pt);                     //�����Ϣ

	void setbaseName(string name) {					//������Ҫ���ӵ����ݿ�����	
		DataBaseName = name;
	}					

	vector<StuInf> getAlldata();

private:
	_ConnectionPtr m_pConnection;                   //����access���ݿ�����Ӷ���
	_RecordsetPtr m_pRecordset;                     //���������
	string DataBaseName;							//���ݿ�����
};
