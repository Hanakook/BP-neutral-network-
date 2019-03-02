#include "stdafx.h"
#include "BPNN.h"
#include "time.h"
#include "math.h"


CBPNN::CBPNN(void)
{
	m_nInput=0;
	m_nOutput=0;
	m_nHidden=0;
	m_InputUnit=NULL;
	m_HiddenUnit=NULL;
	m_OutputUnit=NULL;
	m_I2HWeight=NULL;
	m_H2OWeight=NULL;
	m_OutputError=NULL;
	m_HiddenError=NULL;
	m_Target=NULL;
	m_preI2HWeight=NULL;
	m_preH2OWeight=NULL;

	m_eta=m_momentum=0.0;
}

CBPNN::~CBPNN(void)
{
	ReleaseBPNN();
}

bool CBPNN::InitBPNN(int nInput,int nOutput,int nHidden)
{
	ReleaseBPNN();

	m_nInput=nInput+1;
	m_nOutput=nOutput;
	m_nHidden=nHidden+1;

	m_InputUnit = new double[m_nInput];
	if(!m_InputUnit)
		return false;
	m_OutputUnit= new double[m_nOutput];
	if(!m_OutputUnit)
		return false;
	m_HiddenUnit= new double[m_nHidden];
	if(!m_HiddenUnit)
		return false;

	m_OutputError= new double[m_nOutput];
	if(!m_OutputError)
		return false;
	m_HiddenError= new double[m_nHidden];
	if(!m_HiddenError)
		return false;

	m_Target= new double[m_nOutput];
	if(!m_Target)
		return false;


	memset(m_InputUnit,0,sizeof(double)*(m_nInput));
	memset(m_OutputUnit,0,sizeof(double)*(m_nOutput));
	memset(m_HiddenUnit,0,sizeof(double)*(m_nHidden));
	memset(m_OutputError,0,sizeof(double)*(m_nOutput));
	memset(m_HiddenError,0,sizeof(double)*(m_nHidden));
	memset(m_Target,0,sizeof(double)*(m_nOutput));

	int nI2HW=(m_nHidden-1)*(m_nInput);
	int nH2OW=(m_nHidden)*(m_nOutput);

	m_I2HWeight= new double[nI2HW];
	if(!m_I2HWeight)
		return false;
	m_H2OWeight= new double[nH2OW];
	if(!m_H2OWeight)
		return false;

	srand( (unsigned)time( NULL ) );

	double r=0.0;
	int i=0;
	for(i=0; i<nI2HW; i++)
		m_I2HWeight[i]=0;//rand()*1.0/RAND_MAX;//0.000005;//-0.005+rand()*1.0/RAND_MAX;//-0.05到0.05之间
	for(i=0; i<nH2OW; i++)
		m_H2OWeight[i]=rand()*1.0/RAND_MAX/100;//rand()*1.0/RAND_MAX/10000;//-0.5+rand()*1.0/RAND_MAX;

	m_preI2HWeight= new double[nI2HW];
	m_preH2OWeight= new double[nH2OW];
	memset(m_preI2HWeight,0,sizeof(double)*nI2HW);
	memset(m_preH2OWeight,0,sizeof(double)*nH2OW);

//	memset(m_I2HWeight,0,sizeof(double)*nInput*nHidden);
//	memset(m_H2OWeight,0,sizeof(double)*nHidden*nOutput);

	m_eta=DEFAULT_ETA;
	m_momentum=DEFAULT_MOMENTUM;

	return true;
}

void CBPNN::ReleaseBPNN()
{
	if(m_InputUnit)
		delete [] m_InputUnit;
	if(m_HiddenUnit)
		delete [] m_HiddenUnit;
	if(m_OutputUnit)
		delete [] m_OutputUnit;
	if(m_I2HWeight)
		delete [] m_I2HWeight;
	if(m_H2OWeight)
		delete [] m_H2OWeight;
	if(m_OutputError)
		delete [] m_OutputError;
	if(m_HiddenError)
		delete [] m_HiddenError;
	if(m_Target)
		delete[] m_Target;
	if(m_preH2OWeight)
		delete m_preH2OWeight;
	if(m_preI2HWeight)
		delete m_preI2HWeight;

	m_InputUnit=NULL;
	m_HiddenUnit=NULL;
	m_OutputUnit=NULL;
	m_I2HWeight=NULL;
	m_H2OWeight=NULL;
	m_OutputError=NULL;
	m_HiddenError=NULL;
	m_Target=NULL;
	m_preH2OWeight=NULL;
	m_preI2HWeight=NULL;
}

bool CBPNN::LoadInputUnit(const double* data,int count,double scale)
{
	if(count != m_nInput-1)
		return false;

	//m_InputUnit[0] 即 x0 始终为 1
	for(int i=0; i<count; i++)
		m_InputUnit[i+1]=data[i]/scale;

	m_InputUnit[0]=1.0;
	
	return true;
}

bool CBPNN::LoadTarget(const double* data,int count,double scale)
{
	if(count != m_nOutput)
		return false;

	//m_Target[0] 即 x0 始终为 1
	for(int i=0; i<count; i++)
		m_Target[i]=data[i]/scale;

	return true;
}

double CBPNN::Sigmoid(double x)
{
	return (1.0 / (1.0 + exp(-x)));
}


//////////////////////////////////////////////////////////////////////////
//测试时注意
//////////////////////////////////////////////////////////////////////////
void CBPNN::LayerForward()
{
	double sum;
	int i=0,j=0;
	
	m_InputUnit[0] = 1.0;

	for (i = 1; i < m_nHidden; i++) 
	{
		sum = 0.0;
		for (j = 0; j < m_nInput; j++)
			sum += m_I2HWeight[(i-1)*m_nInput+j] * m_InputUnit[j];//按.h文件中的顺序格式来访问weight[i][j]

		m_HiddenUnit[i] = Sigmoid(sum);
	}

	//////////////////////////////////////////////////////////////////////////

	m_HiddenUnit[0]=1.0;
	for(i=0; i < m_nOutput; i++)
	{
		sum=0.0;
		for(j=0; j < m_nHidden; j++)
			sum+=m_H2OWeight[i*m_nHidden+j] * m_HiddenUnit[j];
		
		m_OutputUnit[i]=Sigmoid(sum);
	}

}

double CBPNN::OutputError()
{
	double o, t, errsum;
	errsum = 0.0;

	for (int i = 0; i < m_nOutput; i++) 
	{
		o = m_OutputUnit[i];
		t = m_Target[i];
		m_OutputError[i] = o * (1.0 - o) * (t - o);
		errsum += ABS(m_OutputError[i]);
	}
	
	return errsum;
}

/***********************容易出错***************************/
double CBPNN::HiddenError()
{
	double h, sum, errsum;

	errsum = 0.0;
	//对网络的每个隐藏单元计算它的误差项
	for (int i = 1; i < m_nHidden; i++)
	{
		h = m_HiddenUnit[i];
		sum = 0.0;
		for (int j= 0; j < m_nOutput; j++) 
		{
			sum += m_OutputError[j] * m_H2OWeight[j*m_nOutput+i];//按.h文件中的顺序格式来访问m_H2OWeight[i][j]
		}
		m_HiddenError[i] = h * (1.0 - h) * sum;
		errsum += ABS(m_HiddenError[i]);
	}
	
	return errsum;
}

void CBPNN::AdjustWeights()//double* delta, int ndelta, double* ly, int nly, double** w, double** oldw, double eta, double momentum)
{
	int i=0,j=0;
	double delta_w=0.0;
	for(j=0; j<m_nOutput; j++)
	{
		for(i=0; i<m_nHidden; i++)
		{
			delta_w = ( ( m_eta * m_OutputError[j] * m_HiddenUnit[i])+(m_momentum*m_preH2OWeight[j*m_nHidden+i]) );
			m_H2OWeight[j*m_nHidden+i]+=delta_w;
			m_preH2OWeight[j*m_nHidden+i]=delta_w;
		}
	}

	//////////////////////////////////////////////////////////////////////////

	for(j=1; j<m_nHidden; j++)
	{
		for(i=0; i<m_nInput; i++)
		{
			delta_w=( ( m_eta * m_HiddenError[j] * m_InputUnit[i])+(m_momentum*m_preI2HWeight[(j-1)*m_nInput+i]) );
			m_I2HWeight[(j-1)*m_nInput+i]+=delta_w;
			m_preI2HWeight[(j-1)*m_nInput+i]=delta_w;
		}
	}
}


double CBPNN::Train(double eta,double momentum)
{
	m_eta=eta;
	m_momentum=momentum;
	
	double err_o=0.0;
	double err_h=0.0;
	double err=0.0;
	//1.
	LayerForward();

	//2.
	err_o=OutputError();
	err_h=HiddenError();

	//3.
	AdjustWeights();

	err=err_o+err_h;

	return err;//
}

const double* CBPNN::GetOutput()
{
	return m_OutputUnit;
}

void CBPNN::Test(const double* data,int nDimentions,double scale,std::vector<double>& vecOutput)
{
	LoadInputUnit(data,nDimentions,scale);
	LayerForward();

	vecOutput.clear();

	for(int i=0; i<m_nOutput; i++)
	{
		vecOutput.push_back(m_OutputUnit[i]);
	}
}

//将训练好的神经网络保存至文件
bool CBPNN::SaveBPNNFile(const char* sSavePath,const char* sTargetName,int nIterateTime,double fStopError,target_type* pvecTarget)
{
	FILE* fp=fopen(sSavePath,"w");
	if(fp == NULL)
		return false;

	if(pvecTarget && pvecTarget->size() <=0 )
		return false;
	if(pvecTarget &&  ((*pvecTarget)[0].second).size() <= 0 )
		return false;
	if(pvecTarget &&  ((*pvecTarget)[0].second).size() != m_nOutput)
		return false;

	int i=0;
	
	fprintf(fp,"%s%c",BPNN_SAVEFILE_FLAG,BPNN_SAVEFILE_SEPCHAR);
	fprintf(fp,"%s%c",/*strlen(sTargetName),*/sTargetName,BPNN_SAVEFILE_SEPCHAR);//"Direction" or other Target
	fprintf(fp,"%d%c",nIterateTime,BPNN_SAVEFILE_SEPCHAR);//Iterate Time
	fprintf(fp,"%lf%c",fStopError,BPNN_SAVEFILE_SEPCHAR);//fStopError,注意lf符号
	fprintf(fp,"%lf%c",m_eta,BPNN_SAVEFILE_SEPCHAR);//eta, the learning rate
	fprintf(fp,"%lf%c",m_momentum,BPNN_SAVEFILE_SEPCHAR);//momentum
	fprintf(fp,"%d%c",m_nInput,BPNN_SAVEFILE_SEPCHAR);//count of input unit
	fprintf(fp,"%d%c",m_nHidden,BPNN_SAVEFILE_SEPCHAR);//count of input unit
	fprintf(fp,"%d%c",m_nOutput,BPNN_SAVEFILE_SEPCHAR);//count of input unit
	
	int nI2HW=(m_nHidden-1)*(m_nInput);
	int nH2OW=(m_nHidden)*(m_nOutput);

	fprintf(fp,"%d%c",nI2HW,BPNN_SAVEFILE_SEPCHAR);
	fprintf(fp,"%d%c",nH2OW,BPNN_SAVEFILE_SEPCHAR);
	
	if(pvecTarget != NULL)
	{
		for(target_type::iterator it=pvecTarget->begin(); it!=pvecTarget->end(); it++)
		{
			fprintf(fp,"%s%c",it->first.c_str(),BPNN_SAVEFILE_SEPCHAR);
			for(i=0; i<m_nOutput; i++)
				fprintf(fp,"%lf%c",(it->second)[i],BPNN_SAVEFILE_SEPCHAR);
		}
	}
	

	//保存三层单元值xij
// 	for(i=0; i<m_nInput; i++)
// 		fprintf(fp,"%lf%c",m_InputUnit[i],BPNN_SAVEFILE_SEPCHAR);
// 	for(i=0; i<m_nHidden; i++)
// 		fprintf(fp,"%lf%c",m_HiddenUnit[i],BPNN_SAVEFILE_SEPCHAR);
// 	for(i=0; i<m_nOutput; i++)
// 		fprintf(fp,"%lf%c",m_OutputUnit[i],BPNN_SAVEFILE_SEPCHAR);

	//保存两层权值wij
	for(i=0; i<nI2HW; i++)
		fprintf(fp,"%lf%c",m_I2HWeight[i],BPNN_SAVEFILE_SEPCHAR);

	for(i=0; i<nH2OW; i++)
		fprintf(fp,"%lf%c",m_H2OWeight[i],BPNN_SAVEFILE_SEPCHAR);



	fclose(fp);
	return true;
}

//从文件读取网络的各参数值
bool CBPNN::LoadBPNNFile(const char* sSavePath,target_type* pvecTarget)
{
	FILE* fp=fopen(sSavePath,"r");
	if(fp == NULL)
		return false;

	char temp[100];
	fscanf(fp,"%s",temp);
	if(strcmp(temp,BPNN_SAVEFILE_FLAG) != 0)
	{
		fclose(fp);
		return false;
	}

	fscanf(fp,"%s",temp);

	int nIterateTime;
	double fStopError;
	double eta;
	double momentum;
	int nInput;
	int nHidden;
	int nOutput;
	int nI2HW;
	int nH2OW;
	fscanf(fp,"%d",&nIterateTime);
	fscanf(fp,"%lf",&fStopError);
	fscanf(fp,"%lf",&eta);
	fscanf(fp,"%lf",&momentum);
	fscanf(fp,"%d",&nInput);
	fscanf(fp,"%d",&nHidden);
	fscanf(fp,"%d",&nOutput);

	fscanf(fp,"%d",&nI2HW);
	fscanf(fp,"%d",&nH2OW);


	//根据读到的参数创建BP神经网络
	InitBPNN(nInput-1,nOutput,nHidden-1);

	int i=0;

// 		char a[100];
// 		USES_CONVERSION;

	std::string target_name;
	double ftemp;
	std::vector<double> target_vector;
	if(pvecTarget != NULL)
	{
		pvecTarget->clear();
		for(i=0; i<nOutput; i++)
		{
			fscanf(fp,"%s",temp);
			target_name=temp;
			target_vector.clear();
			for(int j=0; j<m_nOutput; j++)
			{
				fscanf(fp,"%lf",&ftemp);
				target_vector.push_back(ftemp);
			}
			pvecTarget->push_back(std::pair<std::string,std::vector<double> > (target_name,target_vector));
		}
	}


 	for(i=0; i<nI2HW; i++)
	{
		fscanf(fp,"%lf",&(m_I2HWeight[i]));
// 		sprintf(a,"%lf",m_I2HWeight[i]);
// 		MessageBox(NULL,A2T(a),_T(""),MB_OK);
	}
	for(i=0; i<nH2OW; i++)
		fscanf(fp,"%lf%c",&(m_H2OWeight[i]));

	


	fclose(fp);

	return true;
}