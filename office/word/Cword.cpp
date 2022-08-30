
#include "Cword.h"


CmyWord::CmyWord(){
	InitCOM();
}

CmyWord::~CmyWord(){
	//�ͷ���Դ��ô� С�����˳�����ͷš������c����һЩ�ͷ���Դ�ĵ�����һ����

	//��c+= ���������� ������������һ���ġ�
	CoUninitialize();
	font.ReleaseDispatch();
	range.ReleaseDispatch();
	tab.ReleaseDispatch();
	doc.ReleaseDispatch();
	docs.ReleaseDispatch();
	app.ReleaseDispatch();
	sel.ReleaseDispatch();
}

BOOL CmyWord::InitCOM(){
	if (!CoInitialize(NULL)){
		AfxMessageBox(_T("��ʼ��com��ʧ��"));
		return 0;
	}
	else{
		return TRUE;
	}
}
BOOL CmyWord::CreateAPP(){
	if (!app.CreateDispatch(_T("Word.Application"))){
		AfxMessageBox(_T("��û�а�װOFFICE"));
		return FALSE;
	}
	else{
		app.SetVisible(TRUE);
		return TRUE;
	}
}
//�ҵ���Ĭ���Ǵ򿪵ģ���Word ��Ĭ�Ͽ������ġ�

void CmyWord::ShowApp(BOOL flag){
	if (!app.m_lpDispatch){
		AfxMessageBox(_T("�㻹û�л��Word����"));
		return;
	}
	else{
		app.SetVisible(flag);
	}
}
BOOL CmyWord::CreateDocument(){
	if (!app.m_lpDispatch){
		AfxMessageBox(_T("ApplicationΪ��,Documents����ʧ��!"), MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	else{
		docs = app.GetDocuments();
		if (docs.m_lpDispatch == NULL){
			AfxMessageBox(_T("����DOCUMENTS ʧ��"));
			return FALSE;
		}
		else{
			CComVariant Template(_T(""));//����һ���յ�ģ��
			CComVariant NewTemplate(false);
			CComVariant DocumentType(0);
			CComVariant Visible;//������ ��Ĭ��ֵ
			doc = docs.Add(&Template, &NewTemplate, &DocumentType, &Visible);
			if (!doc.m_lpDispatch){
				AfxMessageBox(_T("����wordʧ��"));
				return FALSE;
			}
			else{
				sel = app.GetSelection();//��õ�ǰWord��������ʼ��Ϊ����doc���selection����ϸ��һ��ȷʵӦ����Word�Ľӿڵ�
				if (!sel.m_lpDispatch){
					AfxMessageBox(_T("selection ��ȡʧ��"));
					return FALSE;
				}
				else{
					return TRUE;
				}
			}
		}
	}
}

BOOL CmyWord::Create(){
	if (CreateAPP()){
		if (CreateDocument()){
			return TRUE;
		}
		else
			return FALSE;
	}
	else
		return FALSE;
}

BOOL CmyWord::Open(CString FileName, BOOL ReadOnly /* = FALSE */, BOOL AddToRecentFiles /* = FALSE */){
	CComVariant Read(ReadOnly);
	CComVariant AddToR(AddToRecentFiles);
	CComVariant Name(FileName);
	COleVariant vTrue((short)TRUE), vFalse((short)FALSE);
	COleVariant varstrNull(_T(""));
	COleVariant varZero((short)0);
	COleVariant varTrue(short(1), VT_BOOL);
	COleVariant varFalse(short(0), VT_BOOL);
	COleVariant vOpt((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	if (!app.m_lpDispatch){
		if (CreateAPP() == FALSE){
			return FALSE;
		}
	}
	if (!docs.m_lpDispatch){
		docs = app.GetDocuments();
		if (!docs.m_lpDispatch){

			AfxMessageBox(_T("DocuMent ���󴴽�ʧ��"));
			return FALSE;
		}
	}
	CComVariant format(0);//�򿪷�ʽ 0 Ϊdoc�Ĵ򿪷�ʽ
	doc = docs.Open(&Name, varFalse, &Read, &AddToR, vOpt, vOpt,
		vFalse, vOpt, vOpt, &format, vOpt, vTrue, vOpt, vOpt, vOpt, vOpt);
	if (!doc.m_lpDispatch){
		AfxMessageBox(_T("�ļ���ʧ��"));
		return FALSE;
	}
	else{
		sel = app.GetSelection();
		if (!sel.m_lpDispatch){
			AfxMessageBox(_T("��ʧ��"));
			return FALSE;
		}
		return TRUE;
	}
}
BOOL CmyWord::Save(){
	if (!doc.m_lpDispatch){
		AfxMessageBox(_T("Documents ����û�н��� ����ʧ��"));
		return FALSE;
	}
	else{
		doc.Save();
		return TRUE;
	}
}
BOOL CmyWord::SaveAs(CString FileName, int SaveType/* =0 */){
	CComVariant vTrue(TRUE);
	CComVariant vFalse(FALSE);
	CComVariant vOpt;
	CComVariant cFileName(FileName);
	CComVariant FileFormat(SaveType);
	doc = app.GetActiveDocument();
	if (!doc.m_lpDispatch){
		AfxMessageBox(_T("Document ����û�н��� ���Ϊʧ��"));
		return FALSE;
	}
	else{
		//��ð��պ���д ��Ȼ���ܳ������⡢ �Ͼ������΢��д��
		/*ActiveDocument.SaveAs FileName:="xiaoyuer.doc", FileFormat:= _

	wdFormatDocument, LockComments:=False, Password:="", AddToRecentFiles:= _

	True, WritePassword:="", ReadOnlyRecommended:=False, EmbedTrueTypeFonts:= _

	False, SaveNativePictureFormat:=False, SaveFormsData:=False, _

		SaveAsAOCELetter:=False*/
		doc.SaveAs(&cFileName, &FileFormat, &vFalse, COleVariant(_T("")), &vTrue,
			COleVariant(_T("")), &vFalse, &vFalse, &vFalse, &vFalse, &vFalse, &vOpt, &vOpt, &vOpt, &vOpt, &vOpt);
	}
	return TRUE;
}
BOOL CmyWord::Close(BOOL SaveChange/* =FALSE */){
	CComVariant vTrue(TRUE);
	CComVariant vFalse(FALSE);
	CComVariant vOpt;
	CComVariant cSavechage(SaveChange);
	if (!doc.m_lpDispatch){
		AfxMessageBox(_T("_Document �����ȡʧ��,�رղ���ʧ��"));
		return FALSE;
	}
	else{
		if (TRUE == SaveChange){
			Save();
		}
		//�����һ��������vTrue ����ִ��󣬿����Ǻ���Ĳ���ҲҪ��Ӧ�ı仯
		//��vba û�и���Ӧ���� �Ҿ������ַ���������
		doc.Close(&vFalse, &vOpt, &vOpt);
	}
	return TRUE;
}
void CmyWord::WriteText(CString Text){
	sel.TypeText(Text);
}
void CmyWord::NewLine(int nCount/* =1 */){
	if (nCount <= 0){
		nCount = 0;
	}
	else{
		for (int i = 0; i < nCount; i++){
			sel.TypeParagraph();//�½�һ��
		}
	}
}
void CmyWord::WriteTextNewLineText(CString Text, int nCount/* =1 */){
	NewLine(nCount);
	WriteText(Text);
}
void CmyWord::SetFont(BOOL Blod, BOOL Italic/* =FALSE */, BOOL UnderLine/* =FALSE */){
	if (!sel.m_lpDispatch){
		AfxMessageBox(_T("�༭����ʧ��,�������岻������"));
		return;
	}
	else{
		sel.SetText(_T("F"));
		font = sel.GetFont();//�������༭����;
		font.SetBold(Blod);
		font.SetItalic(Italic);
		font.SetUnderline(UnderLine);
		sel.SetFont(font);
	}
}
void CmyWord::SetFont(CString FontName, int FontSize/* =9 */, long FontColor/* =0 */, long FontBackColor/* =0 */){
	if (!sel.m_lpDispatch){
		AfxMessageBox(_T("Select Ϊ��,��������ʧ��!"));
		return;
	}
	//����ֻ��Ϊ�˻��һ��������Ϊû�ж�������������������.
	//��Ϊ����GetFont����ȡ�Ķ���ġ�
	//������SetText�������������
	sel.SetText(_T("a"));
	font = sel.GetFont();//��ȡ�������
	font.SetSize(20);
	font.SetName(FontName);
	font.SetColor(FontColor);
	sel.SetFont(font);//ѡ�����
}
void CmyWord::SetTableFont(int Row, int Column, CString FontName, int FontSize/* =9 */, long FontColor/* =0 */, long FontBackColor/* =0 */){
	Cell c = tab.Cell(Row, Column);
	c.Select();
	_Font ft = sel.GetFont();
	ft.SetName(FontName);
	ft.SetSize(FontSize);
	ft.SetColor(FontColor);
	Range r = sel.GetRange();
	r.SetHighlightColorIndex(FontBackColor);
}
void CmyWord::CreateTable(int Row, int Column)
{
	doc = app.GetActiveDocument();
	Tables tbs = doc.GetTables();
	CComVariant Vtrue(short(TRUE)), Vfalse(short(FALSE));
	if (!tbs.m_lpDispatch){
		AfxMessageBox(_T("����������ʧ��"));
		return;
	}
	else{
		tbs.Add(sel.GetRange(), Row, Column, &Vtrue, &Vfalse);
		tab = tbs.Item(1);//����ж��������ͨ��������ҵ�������
	}
}
void CmyWord::WriteCellText(int Row, int Column, CString Text){
	Cell c = tab.Cell(Row, Column);
	c.Select();//ѡ�����еĵ�Ԫ��
	sel.TypeText(Text);
}
void CmyWord::SetParaphformat(int Alignment){
	_ParagraphFormat p = sel.GetParagraphFormat();
	p.SetAlignment(Alignment);
	sel.SetParagraphFormat(p);
}
void CmyWord::FindWord(CString FindW, CString RelWord){
	sel = app.GetSelection();
	Find myFind = sel.GetFind();
	if (!myFind.m_lpDispatch){
		AfxMessageBox(_T("��ȡFind ����ʧ��"));
		return;
	}
	else{
		//���������ǰ���vba д��
		myFind.ClearFormatting();
		Replacement repla = myFind.GetReplacement();
		repla.ClearFormatting();
		COleVariant Text(FindW);
		COleVariant re(RelWord);
		COleVariant vTrue((short)TRUE), vFalse((short)FALSE);
		COleVariant vOpt((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
		CComVariant v(1);
		CComVariant v2(2);
		CComVariant v3(_T(""));
		//�����Replace ��Ӧ���滻�ķ�Χ������.
		// 1 ����һ�� 2 ���������ĵ�
		//myFind.Execute(Text,vFalse,vFalse,vFalse,vFalse,vFalse,vTrue,&v,vFalse,re,&v2,vOpt,vOpt,vOpt,vOpt);
		myFind.Execute(Text, vFalse, vFalse, vFalse, vFalse, vFalse,
			vTrue, &v, vFalse, &re, &v2, vOpt, vOpt, vOpt, vOpt);
	}
}
void CmyWord::GetWordText(CString &Text){
	//CComVariant vOpt;
	COleVariant vOpt((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	doc = app.GetActiveDocument();//��õ�ǰ�����ĵ� ���ǵ�ǰ���ڱ༭�ĵ�
	if (!doc.m_lpDispatch){
		AfxMessageBox(_T("��ȡ�����ĵ�����ʧ��"));
		return;
	}
	else{
		range = doc.Range(vOpt, vOpt);
		Text = range.GetText();
		AfxMessageBox(Text);
	}
}
//��ӡ������ֱ��Cppy ���˵� ��Ϊ��û�д�ӡ�����Բ���������
//����ֻ��Ϊ�˷�����
void CmyWord::PrintWord(){
	doc = app.GetActiveDocument();
	if (!doc.m_lpDispatch){
		AfxMessageBox(_T("��ȡ�����ĵ�����ʧ��"));
		return;
	}
	else{
		COleVariant covTrue((short)TRUE),
			covFalse((short)FALSE),
			covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
		doc.PrintOut(covFalse,              // Background.
			covOptional,           // Append.
			covOptional,           // Range.
			covOptional,           // OutputFileName.
			covOptional,           // From.
			covOptional,           // To.
			covOptional,           // Item.
			COleVariant((long)1),  // Copies.
			covOptional,           // Pages.
			covOptional,           // PageType.
			covOptional,           // PrintToFile.
			covOptional,           // Collate.
			covOptional,           // ActivePrinterMacGX.
			covOptional,           // ManualDuplexPrint.
			covOptional,           // PrintZoomColumn  New with Word 2002
			covOptional,           // PrintZoomRow          ditto
			covOptional,           // PrintZoomPaperWidth   ditto
			covOptional);          // PrintZoomPaperHeight  ditto*/
	}
}
void CmyWord::AppClose(){
	COleVariant vOpt((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	if (!app.m_lpDispatch){
		AfxMessageBox(_T("��ȡWord ����ʧ��,�رղ���ʧ��"));
		return;
	}
	else{
		app.Quit(vOpt, vOpt, vOpt);
		//�����ͷ���Դ�����Ǻܺã������Ҿ�����������ȥ�����ˡ�
	}
}