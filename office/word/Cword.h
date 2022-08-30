#pragma once

#ifndef CMYWORD_H

#define CMYWORD_H

#include "msword.h"
#include <ATLBASE.H>
//������������
enum Alignment { wdAlignParagraphCenter = 1, wdAlignParagraphRight, wdAlignParagraphJustify };
enum SaveType {
	wdFormatDocument = 0,
	wdFormatWebArchive = 9,
	wdFormatHTML = 8,
	wdFormatFilteredHTML = 10,
	wdFormatTemplate = 1

};
class CmyWord{
	//һЩ��������
public:
	_Application app;//����word
	Documents docs;//word�ĵ�����
	_Document doc;//һ��word�ļ�
	_Font font;//�������
	Selection sel;//ѡ��༭���� û�ж����ʱ����ǲ����
	Table tab;//������
	Range range;

public:
	CmyWord();//���캯��
	virtual ~CmyWord();//��������
	void ShowApp(BOOL flag);
	void AppClose();
	BOOL InitCOM();//��COM���г�ʼ��
	BOOL CreateAPP();//����һ��word����
	BOOL CreateDocument();//����word�ĵ�
	BOOL Create();//����һ��word�����Word�ĵ�


	BOOL Open(CString FileName, BOOL ReadOnly = FALSE, BOOL  AddToRecentFiles = FALSE);//��һ��word�ĵ�;
	BOOL Close(BOOL SaveChange = FALSE);//�ر�һ��word�ĵ�
	BOOL Save();//�����ĵ�
	BOOL SaveAs(CString FileName, int SaveType = 0);//��������

	//////////////////////////�ļ�д��������/////////////////////////////////////////////

	void WriteText(CString Text);//д���ı�
	void NewLine(int nCount = 1);//�س���N��
	void WriteTextNewLineText(CString Text, int nCount = 1);//�ز⻻N��д������

	//////////////////////////////////////////////////////////////////////////
	   
	//////////////////////////��������////////////////////////////////////////
	void SetFont(CString FontName, int FontSize = 9, long FontColor = 0, long FontBackColor = 0);
	void SetFont(BOOL Blod, BOOL Italic = FALSE, BOOL UnderLine = FALSE);
	void SetTableFont(int Row, int Column, CString FontName, int FontSize = 9, long FontColor = 0, long FontBackColor = 0);
	//void SetTableFont();//ͳһ�Ա���������������.

		/////////////////////////������/////////////////////////////////////
	void CreateTable(int Row, int Column);
	void WriteCellText(int Row, int Column, CString Text);
	/////////////////////////////���ö�������///////////////////////////////////////
	void SetParaphformat(int Alignment);

	/////////////////////////////һЩ���ò���///////////////////////////////////////
	//�����ַ��� Ȼ��ȫ���滻
	void FindWord(CString FindW, CString RelWord);
	//��ȡWord ���ı�����
	void GetWordText(CString &Text);
	//Word ��ӡ
	void PrintWord();
};
#endif


