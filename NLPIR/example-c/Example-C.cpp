// win_cDemo.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "../../include/NLPIR.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef OS_LINUX
#pragma comment(lib, "../../bin/NLPIR.lib")
#endif

//Linux
#ifdef OS_LINUX
	#define _stricmp(X,Y) strcasecmp((X),(Y))
	#define _strnicmp(X,Y,Z) strncasecmp((X),(Y),(Z))
	#define strnicmp(X,Y,Z)	strncasecmp((X),(Y),(Z))
	#define _fstat(X,Y)     fstat((X),(Y))
	#define _fileno(X)     fileno((X))
	#define _stat           stat
	#define _getcwd         getcwd
	#define _off_t          off_t
	#define PATH_DELEMETER  "/"
#else
	#pragma warning(disable:4786)
	#define PATH_DELEMETER  "\\"
#endif
void SplitGBK(const char *sInput);
void SplitBIG5();
void SplitUTF8();
void testNewWord();

void testNewWord(int nCode)
{
	//��ʼ���ִ����
	
	if(!NLPIR_Init("..",nCode))//�����ڵ�ǰ·���£�Ĭ��ΪGBK����ķִ�
	{
		printf("ICTCLAS INIT FAILED!\n");
		return ;
	}
	char sInputFile[1024]="../test/test.TXT",sResultFile[1024];
	if (nCode==UTF8_CODE)
	{
		strcpy(sInputFile,"../test/test-utf8.TXT");
	}

	//NLPIR
	const char *sResult=NLPIR_GetFileKeyWords(sInputFile);
	FILE *fp=fopen("Result.txt","wb");
	fprintf(fp,sResult);
	fclose(fp);

	sResult=NLPIR_GetFileNewWords(sInputFile);
	fp=fopen("ResultNew.txt","wb");
	fprintf(fp,sResult);
	fclose(fp);

	NLPIR_NWI_Start();
	NLPIR_NWI_AddFile(sInputFile);
	NLPIR_NWI_Complete();
	const char *pNewWordlist=NLPIR_NWI_GetResult();
	printf("ʶ������´�Ϊ��%s\n",pNewWordlist);

	strcpy(sResultFile,sInputFile);
	strcat(sResultFile,"_result1.txt");
	NLPIR_FileProcess(sInputFile,sResultFile);

	NLPIR_NWI_Result2UserDict();//�´�ʶ����

	strcpy(sResultFile,sInputFile);
	strcat(sResultFile,"_result2.txt");
	NLPIR_FileProcess(sInputFile,sResultFile);

	NLPIR_Exit();
}
void SplitGBK(const char *sInput)
{//�ִ���ʾ

	//��ʼ���ִ����
	if(!ICTCLAS_Init())//�����ڵ�ǰ·���£�Ĭ��ΪGBK����ķִ�
	{
		printf("ICTCLAS INIT FAILED!\n");
		return ;
	}

	ICTCLAS_SetPOSmap(ICT_POS_MAP_SECOND);

	char sSentence[5000]="ICTCLAS�ڹ���973ר������֯�������л����˵�һ�����ڵ�һ��������Ĵ����о�����SigHan��֯�������ж�����˶����һ����";
	const char * sResult;


	int nCount;
	ICTCLAS_ParagraphProcessA(sSentence,&nCount);
	printf("nCount=%d\n",nCount);
	nCount = ICTCLAS_GetParagraphProcessAWordCount(sSentence);
	const result_t *pResult=ICTCLAS_ParagraphProcessA(sSentence,&nCount);

	int i=1;
	char *sWhichDic;
	for(i=0;i<nCount;i++)
	{
		sWhichDic="";
		switch (pResult[i].word_type)
		{
		case 0:
			sWhichDic = "���Ĵʵ�";
			break;
		case 1:
			sWhichDic = "�û��ʵ�";
			break;
		case 2:
			sWhichDic = "רҵ�ʵ�";
			break;
		default:
			break;
		}
		printf("No.%d:start:%d, length:%d,POS_ID:%d,Word_ID:%d, UserDefine:%s, Word:%s\n",
			i+1, pResult[i].start, pResult[i].length, pResult[i].iPOS, pResult[i].word_ID, sWhichDic,sSentence+pResult[i].start );
	}
	while(_stricmp(sSentence,"q")!=0)
	{
		sResult = ICTCLAS_ParagraphProcess(sSentence,1);
		printf("%s\nInput string now('q' to quit)!\n", sResult);// 
		gets(sSentence);
	}
	
	//�����û��ʵ�ǰ
	printf("δ�����û��ʵ䣺\n");
	sResult = ICTCLAS_ParagraphProcess(sInput, 1);
	printf("%s\n", sResult);

	//�����û��ʵ��
	printf("\n�����û��ʵ��\n");
	nCount = ICTCLAS_ImportUserDict("userdic.txt");//userdic.txt������ǰ���û��ʵ�
	//�����û��ʵ�
	ICTCLAS_SaveTheUsrDic();
	printf("����%d���û��ʡ�\n", nCount);
	
	sResult = ICTCLAS_ParagraphProcess(sInput, 1);
	printf("%s\n", sResult);

	//��̬����û���
	printf("\n��̬����û��ʺ�\n");
	ICTCLAS_AddUserWord("�����ѧԺ   xueyuan");
	ICTCLAS_SaveTheUsrDic();
	sResult = ICTCLAS_ParagraphProcess(sInput, 1);
	printf("%s\n", sResult);


	//���ļ����зִ�
	//ICTCLAS_FileProcess("test2.txt","test2_result.txt",1);
	//ICTCLAS_FileProcess("testGBK.txt","testGBK_result.txt",1);


	//�ͷŷִ������Դ
	ICTCLAS_Exit();
}

void SplitGBK_Fanti(const char *sInput)
{//�ִ���ʾ

	//��ʼ���ִ����
	if(!ICTCLAS_Init("",GBK_FANTI_CODE))//�����ڵ�ǰ·���£�Ĭ��ΪGBK����ķִ�
	{
		printf("ICTCLAS INIT FAILED!\n");
		return ;
	}

	ICTCLAS_SetPOSmap(ICT_POS_MAP_SECOND);

	char sSentence[5000]="ICTCLAS�ڹ���ר������֯�������л����˵�һ�����ڵ�һ��������Ĵ����о�����SigHan��֯�������ж�����˶����һ����ICTCLAS�ڇ��Ȍ��ҽM�M�����u�y�л�ӫ@���˵�һ�����ڵ�һ�Ç��H����̎���о��C��SigHan�M�����u�y�ж��@���˶�헵�һ����";
	const char * sResult;


	int nCount;
	ICTCLAS_ParagraphProcessA(sSentence,&nCount);
	printf("nCount=%d\n",nCount);
	int count = ICTCLAS_GetParagraphProcessAWordCount(sSentence);
	const result_t *pResult=ICTCLAS_ParagraphProcessA(sSentence,&nCount);

	int i=1;
	char *sWhichDic;
	for(i=0;i<nCount;i++)
	{
		sWhichDic="";
		switch (pResult[i].word_type)
		{
		case 0:
			sWhichDic = "���Ĵʵ�";
			break;
		case 1:
			sWhichDic = "�û��ʵ�";
			break;
		case 2:
			sWhichDic = "רҵ�ʵ�";
			break;
		default:
			break;
		}
		printf("No.%d:start:%d, length:%d,POS_ID:%d,Word_ID:%d, UserDefine:%s, Word:%s\n",
			i+1, pResult[i].start, pResult[i].length, pResult[i].iPOS, pResult[i].word_ID, sWhichDic,sSentence+pResult[i].start );
	}
	while(_stricmp(sSentence,"q")!=0)
	{
		sResult = ICTCLAS_ParagraphProcess(sSentence,0);
		printf("%s\nInput string now('q' to quit)!\n", sResult);// 
		scanf("%s",sSentence);
	}
	
	//�����û��ʵ�ǰ
	printf("δ�����û��ʵ䣺\n");
	sResult = ICTCLAS_ParagraphProcess(sInput, 0);
	printf("%s\n", sResult);

	//�����û��ʵ��
	printf("\n�����û��ʵ��\n");
	nCount = ICTCLAS_ImportUserDict("userdic.txt");//userdic.txt������ǰ���û��ʵ�
	//�����û��ʵ�
	ICTCLAS_SaveTheUsrDic();
	printf("����%d���û��ʡ�\n", nCount);
	
	sResult = ICTCLAS_ParagraphProcess(sInput, 1);
	printf("%s\n", sResult);

	//��̬����û���
	printf("\n��̬����û��ʺ�\n");
	ICTCLAS_AddUserWord("�����ѧԺ   xueyuan");
	ICTCLAS_SaveTheUsrDic();
	sResult = ICTCLAS_ParagraphProcess(sInput, 1);
	printf("%s\n", sResult);


	//���ļ����зִ�
	ICTCLAS_FileProcess("test2.txt","test2_result.txt",1);
	ICTCLAS_FileProcess("testGBK.txt","testGBK_result.txt",1);


	//�ͷŷִ������Դ
	ICTCLAS_Exit();
}
void SplitBIG5()
{
	//��ʼ���ִ����
	if(!ICTCLAS_Init("",BIG5_CODE))//�����ڵ�ǰ·���£�����ΪBIG5����ķִ�
	{
		printf("ICTCLAS INIT FAILED!\n");
		return ;
	}
	ICTCLAS_FileProcess("testBIG.txt","testBIG_result.txt");
	ICTCLAS_Exit();
}
void SplitUTF8()
{
	//��ʼ���ִ����
	if(!ICTCLAS_Init("",UTF8_CODE))//�����ڵ�ǰ·���£�����ΪUTF8����ķִ�
	{
		printf("ICTCLAS INIT FAILED!\n");
		return ;
	}
//	ICTCLAS_FileProcess("testUTF.txt","testUTF_result.txt");
	ICTCLAS_FileProcess("test.xml","testUTF_result.xml");


	FILE *fp=fopen("TestUTF8-bigfile.txt","rt");
	if (fp==NULL)
	{
		printf("Error Open TestUTF8-bigfile.txt\n");
		ICTCLAS_Exit();
		return ;
	}
	char sLine[10241];
	int i,nCount,nDocID=1;
	//result_t res[1000];
	//while (fgets(sLine,10240,fp))
	{
		CICTCLAS *pICTCLAS=new CICTCLAS;
	/*	
		int nCountA=pICTCLAS->GetParagraphProcessAWordCount(sLine);
		pICTCLAS->ParagraphProcessAW(nCountA,res);
		for(i=0;i<nCountA;i++)
		{
			printf("No.%d:start:%d, length:%d,POS_ID:%d,Word_ID:%d\n",
				i+1, res[i].start, res[i].length, res[i].iPOS, res[i].word_ID);
		}

	*/
		fseek(fp,0,SEEK_END);
		int nSize=ftell(fp);

		fseek(fp,0,SEEK_SET);
		fread(sLine,nSize,1,fp);
		sLine[nSize]=0;
		const result_t *pResult=pICTCLAS->ParagraphProcessA(sLine,&nCount);
	    i=1;

		for(i=0;i<500&&i<nCount;i++)
		{
 			printf("\nNo.%d:start:%d, length:%d,POS_ID:%d,Word_ID:%d\n",
 				i+1, pResult[i].start, pResult[i].length, pResult[i].iPOS, pResult[i].word_ID);
			fwrite(sLine+pResult[i].start,sizeof(char),pResult[i].length,stdout);
		}
		delete pICTCLAS;
		printf("Processed docs %d\r",nDocID++);
	}

	ICTCLAS_Exit();
}
void testMultiThread()
{
	//��ʼ���ִ����
	if(!ICTCLAS_Init("",UTF8_CODE))//�����ڵ�ǰ·���£�����ΪUTF8����ķִ�
	{
		printf("ICTCLAS INIT FAILED!\n");
		return ;
	}
	ICTCLAS_FileProcess("E:\\ictclas2010\\����\\�п�Ժ�ִʳ�����\\long_utf8.txt","E:\\ictclas2010\\����\\�п�Ժ�ִʳ�����\\long_utf8_result.txt");
	ICTCLAS_FileProcess("E:\\ictclas2010\\����\\�п�Ժ�ִʳ�����\\long.txt","E:\\ictclas2010\\����\\�п�Ժ�ִʳ�����\\long_result.txt");	
	ICTCLAS_Exit();
}
int main()
{
	//testNewWord(GBK_CODE);
	testNewWord(UTF8_CODE);

	const char *sInput = "@����δ�����ֲ� �� @ICTCLAS�Ż�ƽ��ʿ �Ǽ���������ר��ѧ�ߣ�רע��������Ȼ���Դ�����Ϣ��������Ϣ��ȫ��ѧ���������ҵӦ�á���ԣ��@������ ������ѧϰ���죬��ѧϰ�����ж���̡�Ҳ��@������ ��ע�� ";
	//�ִ�
	//SplitGBK(sInput);
	return 1;
}

