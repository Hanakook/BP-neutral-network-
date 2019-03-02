/*
This CPGMFile class is created by Vincent Gao(c_gao),2009-10-31.
you cat use it as any purposes as you want,but you must keep
this comment on these files,if you have any problems,please 
contact me at:

e-mail:	c_gao@163.net
blog:	http://cgao.csai.cn

this class can read a pgm file, and get the picture data as several
ways(from row, col, or a one dim array).

if you want to use this code in your project,please tell me that 
through my e-mail. thanks.
*/

#pragma once

#define _CRT_SECURE_NO_WARNINGS

#define MAX_NAME_SIZE	50

enum OPERATOR_CODE
{
	FILENAME_ERROR,
	FILECANTOPEN_ERROR,

	FILE_OK
};

class CPGMFile
{
public:
	CPGMFile(void);
	~CPGMFile(void);

	unsigned char getGrayscale();
	int getRows();
	int getCols();
	OPERATOR_CODE openPGMFile(const char* filepath);
	const unsigned char* getData();
protected:
	char name[MAX_NAME_SIZE];
	char magic_number[2];
	int rows, cols;
	unsigned char grayscale; 
	unsigned char* data;
};

