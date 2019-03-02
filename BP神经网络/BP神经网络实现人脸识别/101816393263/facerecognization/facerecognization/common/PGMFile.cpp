#include "StdAfx.h"
#include "PGMFile.h"

#include "io.h"
#include "stdio.h"

CPGMFile::CPGMFile(void)
{
	memset(name,0,MAX_NAME_SIZE);
	memset(magic_number,0,2);
	rows=cols=0;
	grayscale=255;
	data=NULL;
}

CPGMFile::~CPGMFile(void)
{
	if(data)
		delete[] data;
	data=NULL;
}

unsigned char CPGMFile::getGrayscale()
{
	return grayscale;
}

int CPGMFile::getRows()
{
	return rows;
}

int CPGMFile::getCols()
{
	return cols;
}

OPERATOR_CODE CPGMFile::openPGMFile(const char* filepath)
{
	if(filepath == NULL)
		return FILENAME_ERROR;

	FILE* fp=fopen(filepath,"rb");
	if(!fp)
		return FILECANTOPEN_ERROR;

	//1.读取文件标识符,应为"P5"或"P2"
	fread(magic_number,2,1,fp);

	//int offset=0;

	if(magic_number[0]=='P' && magic_number[1]=='5')//binrary format
	{
		char temp[10];
		//offset+=3;
		//2.读取列像素个数		
		memset(temp,0,10);
		int i=0;
		char ch=(unsigned char)fgetc(fp);
		if(ch == 0x0a)
			ch=(unsigned char)fgetc(fp);
		while(ch != 0x20/*空格*/ && ch != 0x0a /*回车*/)
		{
			temp[i]=ch;
			i++;
			ch=(unsigned char)fgetc(fp);
		}
		cols=atoi(temp);
		//offset+=i+1;
		//3.读取行像素个数
		memset(temp,0,10);
		i=0;
		ch=(unsigned char)fgetc(fp);
		while(ch != 0x20/*空格*/ && ch != 0x0a /*回车*/)
		{
			temp[i]=ch;
			i++;
			ch=(unsigned char)fgetc(fp);
		}
		rows=atoi(temp);
		//offset+=i+1;
		//4.读取灰度级数
		memset(temp,0,10);
		i=0;
		ch=(unsigned char)fgetc(fp);
		while(ch != 0x20/*空格*/ && ch != 0x0a /*回车*/)
		{
			temp[i]=ch;
			i++;
			ch=(unsigned char)fgetc(fp);
		}
		grayscale=(unsigned char)atoi(temp);
		//offset+=i+1;
		//5.读取像素数据
		int nlen=filelength(fileno(fp));

		if(data)
			delete data;
		data=new unsigned char[rows*cols/*nlen-offset*/];
		fread(data,/*nlen-offset*/rows*cols,1,fp);
		/*
		i=0;
		while(!feof(fp))
		{
			data[i]=(unsigned int)fgetc(fp);
			i++;
		}
		int jj=i;
*/
	}
	else if(magic_number[0]=='P' && magic_number[1] == '2')//ascii format
	{
// #define MAX_LINE_CHAR	1000
// 		char temp[MAX_LINE_CHAR];
// 		memset(temp,0,MAX_LINE_CHAR);
// 		fgets(temp,MAX_LINE_CHAR,fp);
// 
// 		char ch=(unsigned char)fgetc(fp);
// 		if(ch == 0x0a)
// 			ch=(unsigned char)fgetc(fp);

		
	}

	fclose(fp);

	return FILE_OK;

}

const unsigned char* CPGMFile::getData()
{
	return data;
}