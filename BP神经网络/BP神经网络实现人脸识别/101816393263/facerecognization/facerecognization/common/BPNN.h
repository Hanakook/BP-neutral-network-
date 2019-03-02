/*
This BPNN class is created by Vincent Gao(c_gao),2009-10-31.
you cat use it as any purposes as you want,but you must keep
this comment on these files,if you have any problems,please 
contact me at:

e-mail:	c_gao@163.net
blog:	http://cgao.csai.cn

this class can create a three lays BackPropogation Neural
Network,to create it you can call the InitBPNN() function,
to train the network,call Train().

if you want to use this code in your project,please tell me that 
through my e-mail. thanks.
*/

#pragma once

#include <vector>
#include <string>

#define DEFAULT_ETA			0.05		//Ĭ��ѧϰ��
#define DEFAULT_MOMENTUM	0			//Ĭ�ϳ���


#define BPNN_SAVEFILE_FLAG			"BPNN"	//BP���籣���ļ��ı��
#define BPNN_SAVEFILE_SEPCHAR		'\n'	//�����ļ��ķָ���

#define ABS(x)	(x)>0?(x):(-(x))

typedef std::vector<std::pair<std::string,std::vector<double> > >	target_type;

class CBPNN
{
public:
	CBPNN(void);
	~CBPNN(void);
public:
	bool InitBPNN(int nInput,int nOutput,int nHidden);
	bool LoadInputUnit(const double* data,int count,double scale);
	bool LoadTarget(const double* data,int count,double scale=1.0);
	double Train(double eta,double momentum);
	void Test(const double* data,int nDimentions,double scale,std::vector<double>& vecOutput);
	const double* GetOutput();
	bool SaveBPNNFile(const char* sSavePath,const char* sTargetName,int nIterateTime,double fStopError,target_type* pvecTarget=NULL);
	bool LoadBPNNFile(const char* sSavePath,target_type* pvecTarget);
protected:
	void ReleaseBPNN();
	double Sigmoid(double x);//�����
	void LayerForward();
	double OutputError();
	double HiddenError();
	void AdjustWeights();
protected:
	int m_nInput;
	int m_nOutput;
	int m_nHidden;

	double* m_InputUnit;
	double* m_HiddenUnit;
	double* m_OutputUnit;

	//����ͼ(�ȱ���o1��Ԫ������Ȩ,�ٱ���o2��Ȩ,�ϲ�ͬ��)
	//       output layer      o
	//                        / \
	//       hidden layer   o1  o2
	//			           /|\
	//       input layer  o o o
	//���²�Ϊ�����,���ϲ�Ϊ�����,����������:��������

	double* m_I2HWeight;

	double* m_H2OWeight;

	double* m_preI2HWeight;
	double* m_preH2OWeight;

	double* m_OutputError;
	double* m_HiddenError;

	double* m_Target;

	double m_eta;//learning rate
	double m_momentum;//momentum for updating the weight
};
