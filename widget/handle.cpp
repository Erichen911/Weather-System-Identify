#include "handle.h"
#include <QDebug>

handle::handle()
{
}


handle::~handle()
{
}


/////////��ȡ�������0 - 7
void handle::AngleCode(float* u, float* v, int &longitude_num, int &latitude_num, int* BinarySet, vector<float>& angleuv)
{
	//float temp1 = 0;///�м����,���ٴ�С
	float temp2 = 0;//�м����,���ٽǶ�
	int temp4 = 0;//�м����
	for (int j = 0; j < latitude_num; j++)
	{
		for (int i = 0; i < longitude_num; i++)
		{
			//���ٽǶ�
			if (u[j*longitude_num + i] == 0)
			{
				if (v[j*longitude_num + i] > 0)
					temp2 = 90;
				else if (v[j*longitude_num + i] < 0)
					temp2 = 270;
				else
					temp2 = 0;
			}
			else
			{
				temp2 = atan(v[j*longitude_num + i] / u[j*longitude_num + i]) * 180 / 3.1415926;
				if (v[j*longitude_num + i] >= 0 && u[j*longitude_num + i] > 0)
					temp2 = temp2;
				else if (v[j*longitude_num + i] < 0 && u[j*longitude_num + i] > 0)
					temp2 = temp2 + 360;
				else if (v[j*longitude_num + i] >= 0 && u[j*longitude_num + i] < 0)
					temp2 = temp2 + 180;
				else if (v[j*longitude_num + i] < 0 && u[j*longitude_num + i] < 0)
					temp2 = temp2 + 180;
			}
			angleuv.push_back(temp2);
		}
	}
	//���ٽǶ�������Ϣ
	for (int j = 0; j < latitude_num; j++)
	{
		for (int i = 0; i < longitude_num; i++)
		{
			if (angleuv[j*longitude_num + i] >= 23 && angleuv[j*longitude_num + i] < 68)
			{
				temp4 = 5;// 101;
			}
			else if (angleuv[j*longitude_num + i] >= 68 && angleuv[j*longitude_num + i] < 113)
			{
				temp4 = 6;// 110;
			}
			else if (angleuv[j*longitude_num + i] >= 113 && angleuv[j*longitude_num + i] < 158)
			{
				temp4 = 7;// 111;
			}
			else if (angleuv[j*longitude_num + i] >= 158 && angleuv[j*longitude_num + i] < 203)
			{
				temp4 = 0;// 000;
			}
			else if (angleuv[j*longitude_num + i] >= 203 && angleuv[j*longitude_num + i] < 248)
			{
				temp4 = 1;// 001;
			}
			else if (angleuv[j*longitude_num + i] >= 248 && angleuv[j*longitude_num + i] < 293)
			{
				temp4 = 2;// 010;
			}
			else if (angleuv[j*longitude_num + i] >= 293 && angleuv[j*longitude_num + i] < 338)
			{
				temp4 = 3;// 011;
			}
			else// if ((angleuv[j*longitude_num + i] >= 338 && angleuv[j*longitude_num + i] < 360) | (angleuv[j*longitude_num + i] >= 0 && angleuv[j*longitude_num + i]<23))
			{
				temp4 = 4;// 100;
			}
			BinarySet[j*longitude_num + i] = temp4;
		}
	}

}


//�жϵ���Χ���������ͣ������������٣�����Ϊ�����
void handle::Judge(vector<int>& bianrySet, int latitude_num, int longitude_num)
{
	vector <int> bianarTemp;
	vector <int> NouseData;        // ����һ����������
	bianarTemp.clear();
	///bianarTemp��ʼ��
	for (int j = 0; j < latitude_num; j++)
	{
		for (int i = 0; i < longitude_num; i++)
		{
			bianarTemp.push_back(bianrySet[j*longitude_num + i]);
		}
	}
	////�ж�
	for (int j = 1; j < latitude_num - 1; j++)
	{
		for (int i = 1; i < longitude_num - 1; i++)
		{
			NouseData.clear();
			NouseData.push_back(bianrySet[(j - 1)*longitude_num + i - 1]);
			NouseData.push_back(bianrySet[(j - 1)*longitude_num + i - 0]);
			NouseData.push_back(bianrySet[(j - 1)*longitude_num + i + 1]);
			NouseData.push_back(bianrySet[(j - 0)*longitude_num + i - 1]);
			NouseData.push_back(bianrySet[(j - 0)*longitude_num + i + 1]);
			NouseData.push_back(bianrySet[(j + 1)*longitude_num + i - 1]);
			NouseData.push_back(bianrySet[(j + 1)*longitude_num + i - 0]);
			NouseData.push_back(bianrySet[(j + 1)*longitude_num + i + 1]);
			//ȥ���ظ��ķ�����
			vector <int> NoRepeat;
			NoRepeat.clear();
			NoRepeat.push_back(NouseData[0]);
			for (int m = 1; m < NouseData.size(); m++)
			{
				int numNR = 0;
				for (int n = 0; n < m; n++)
				{
					if (NouseData[m] != NouseData[n])
					{
						numNR++;
					}
				}
				if (numNR == m)
				{
					NoRepeat.push_back(NouseData[m]);
				}
			}
			//////////���ڰ˸�λ�ò�ͬ�ķ�������С�ڰ˸�ʱ�������ر��Ϊ-2
			if (NoRepeat.size() < 3)
			{
				bianarTemp[j*longitude_num + i] = -2;
			}
		}
	}
	for (int j = 0; j < latitude_num; j++)
	{
		for (int i = 0; i < longitude_num; i++)
		{
			bianrySet[j*longitude_num + i] = bianarTemp[j*longitude_num + i];
		}
	}

}


//ɢ�������ж�
void handle::UVJudge(vector<float>& angleUV, vector <int> TempBianry, int latitude_num, int longitude_num, int n)
{
	vector <float> angleData;        // ����һ������
	angleData.clear();
	///angleData��ʼ��
	for (int j = 0; j < latitude_num; j++)
	{
		for (int i = 0; i < longitude_num; i++)
		{
			angleData.push_back(angleUV[j*longitude_num + i]);
		}
	}
	//����
	for (int j = n + 1; j < latitude_num - n - 1; j++)
	{
		for (int i = n; i < longitude_num - n - 1; i++)
		{
			////��ǵ�����
			//if (TempBianry[j*longitude_num + i] == -2)//���Ǳ߽紦����
			//{
			//	angleData[j*longitude_num + i] = 255;
			//	continue;
			//}
			//////////ģ��(2n+1)*(2n+1)////////////
			float temp1 = 0;
			float temp2 = 0;//���ĳ���Ϸ����ҷ��ĽǶȲ�ֵ
			float temp = 0;//ģ��������ֵ�ܺ�
			for (int t = j - n; t < j + n + 1; t++)
			{
				for (int s = i - n; s < i + n + 1; s++)
				{
					temp1 = fabs(angleUV[t*longitude_num + s] - angleUV[t*longitude_num + s + 1]);
					if (temp1 > 180)
					{
						temp1 = fabs(temp1 - 360);
					}
					temp2 = fabs(angleUV[t*longitude_num + s] - angleUV[(t - 1)*longitude_num + s]);
					if (temp2 > 180)
					{
						temp2 = fabs(temp2 - 360);
					}
					float temp12 = temp1 + temp2;
					temp = temp + temp12;
				}
			}
			float tempJ = 0;
			tempJ = temp*1.0 / ((2 * n + 1)*(2 * n + 1));

			angleData[j*longitude_num + i] = tempJ;

		}
	}
	//	IplImage* imageG = cvCreateImage(cvSize(longitude_num, latitude_num), 8, 1);//����ͼ��ռ�
	//	cvNamedWindow("gray");

	for (int i = 0; i<latitude_num; i++)
	{
		for (int j = 0; j<longitude_num; j++)
		{
			if (angleData[i*longitude_num + j] > 70)
			{
				angleUV[i*longitude_num + j] = 0;
			}
			if (angleData[i*longitude_num + j] < 10)
			{
				angleUV[i*longitude_num + j] = 0;
			}
		}
	}
	//	cvShowImage("gray", imageG);


}


//ͳ��ȷ��ÿ��������򣻿�����ͨ����>=1/3��û���ض�������ȡ��
int handle::jj(int *temp2, int datanum, int temp3)
{
	//// TODO:  �ڴ���������������
	//ofstream out;
	//out.open("outs1.txt",ios::app);
	//if (out.fail())
	//{
	//	cout << "open file error " << endl;
	//}

	struct Result
	{
		int data;
		int num;
	};
	int temp1[8];//�м����һά���飬ͳ���������
	for (int i = 0; i < 8; ++i)
	{
		temp1[i] = 0;
	}

	for (int i = 0; i < (datanum + 1)*datanum / 2; ++i)
	{
		if (temp2[i] == 0){ temp1[0] = temp1[0] + 1; }
		if (temp2[i] == 1){ temp1[1] = temp1[1] + 1; }
		if (temp2[i] == 2){ temp1[2] = temp1[2] + 1; }
		if (temp2[i] == 3){ temp1[3] = temp1[3] + 1; }
		if (temp2[i] == 4){ temp1[4] = temp1[4] + 1; }
		if (temp2[i] == 5){ temp1[5] = temp1[5] + 1; }
		if (temp2[i] == 6){ temp1[6] = temp1[6] + 1; }
		if (temp2[i] == 7){ temp1[7] = temp1[7] + 1; }
	}
	Result DD[8];
	DD[0].data = 0; DD[1].data = 1;
	DD[2].data = 2; DD[3].data = 3;
	DD[4].data = 4; DD[5].data = 5;
	DD[6].data = 6; DD[7].data = 7;
	for (int i = 0; i < 8; ++i)
	{
		DD[i].num = temp1[i];
	}
	//��һά����temp1����,�ɴ�С����
	for (int m = 0; m < 7; ++m)
	{
		for (int n = 0; n < 7 - m; ++n)
		{
			Result temp;
			temp.num = 0;
			temp.data = 0;
			if (DD[n].num < DD[n + 1].num)
			{
				temp = DD[n]; DD[n] = DD[n + 1]; DD[n + 1] = temp;
			}
		}
	}
	//�������
	//out << DD[0].num << "  ";
	//����������ֵ��ĿС��1/3������ֵ-1������û����������
	if (DD[0].num <= (datanum + 1)*datanum / 6.0)
	{
		return -1;
	}
	//�������1/3��ͳ�������ͨ������Ŀ
	struct InputData
	{
		int type;
		int mark;
	};
	InputData* data = new InputData[(datanum + 2) * (datanum + 2)];
	//��ʼ��������Ϣ
	for (int i = 0; i < datanum + 2; i++)
	{
		for (int j = 0; j < datanum + 2; j++)
		{
			data[i * (datanum + 2) + j].type = -1;
			data[i * (datanum + 2) + j].mark = -1;
		}
	}
	//���Ϊ�ض���������ֵ
	for (int i = 1; i < datanum + 1; ++i)
	{
		for (int j = 1; j < i + 1; ++j)
		{
			if (temp2[i*(i - 1) / 2 + j - 1] == DD[0].data)
			{
				data[i * (datanum + 2) + j].type = 0;
			}
		}
	}
	InputData* Jdata = new InputData[DD[0].num];//��ž�����Ԫ����Ϣ
	for (int k = 0; k <DD[0].num; k++)
	{
		Jdata[k].mark = 0;
		Jdata[k].type = 0;
	}//��ʼ������
	////////////Ѱ�������ͨ����//////////////////
	int dataN = 0;
	int datan = 0;
	int onceNum = 0;
	while (dataN != DD[0].num)
	{
		datan++;
		onceNum = dataN;
		//��һ��ѭ�����ҵ���һ�����ӵ�
		bool done = false;
		for (int j = 1; j < (datanum + 1) && !done; j++)
		{
			for (int i = 1; i < (datanum + 1) && !done; i++)
			{
				if (data[j * (datanum + 2) + i].type == 0)
				{
					data[j * (datanum + 2) + i].type = datan;
					done = true;
					break;
				}
			}
		}
		//�ڶ���ѭ��������Χ8������б�ǣ�ͳ����Ŀ
		for (int j = 1; j < (datanum + 1); j++)
		{
			for (int i = 1; i < (datanum + 1); i++)
			{
				if (data[j * (datanum + 2) + i].type == datan)
				{
					Jdata[dataN].mark = datan;
					Jdata[dataN].type = 1;
					dataN++;
					////����������
					if (data[j * (datanum + 2) + i + 1].type == 0)
					{
						data[j * (datanum + 2) + i + 1].type = datan;
					}
					if (data[(j + 1) * (datanum + 2) + i - 1].type == 0)
					{
						data[(j + 1) * (datanum + 2) + i - 1].type = datan;
					}
					if (data[(j + 1) * (datanum + 2) + i].type == 0)
					{
						data[(j + 1) * (datanum + 2) + i].type = datan;
					}
					if (data[(j + 1) * (datanum + 2) + i + 1].type == 0)
					{
						data[(j + 1) * (datanum + 2) + i + 1].type = datan;
					}

				}
			}
		}
		onceNum = dataN - onceNum;
		if (onceNum == 0)
		{
			datan--;
			break;
		}
	}
	//ͳ��ÿ����Ŀ
	vector<int> clusterNum;
	clusterNum.clear();
	for (int m = 0; m < datan; m++)
	{
		int valueNum = 0;
		for (int n = 0; n <dataN; n++)
		{
			if (Jdata[n].mark == (m + 1))
			{
				valueNum++;
			}
		}
		clusterNum.push_back(valueNum);
	}
	int areaS = 0;
	//��clusterNum����,�ɴ�С����
	for (int m = 0; m < (clusterNum.size() - 1); ++m)
	{
		for (int n = 0; n < (clusterNum.size() - 1 - m); ++n)
		{
			int temp;
			if (clusterNum[n] < clusterNum[n + 1])
			{
				temp = clusterNum[n]; clusterNum[n] = clusterNum[n + 1];
				clusterNum[n + 1] = temp;
			}
		}
	}
	areaS = clusterNum[0];

	delete data;
	delete Jdata;

	if (areaS <= (datanum + 1)*datanum / 6.0)//�����ͨ�������С��1/3������ֵ-1������û����������
	{
		return -1;
	}
	else
	{
		temp3 = DD[0].data;
		return temp3;
	}

}


//����Ϊ8������ɵ����飬���ط���ֵ
int handle::PointWind(int *temp, float &centerFU)
{
	int returnData = -1;
	int windDtemp[8] = { 0 };//�洢8����������м����
	for (int k = 0; k < 8; ++k)
	{
		windDtemp[k] = temp[k];
	}

	////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////
	//ȥ��-1�ķ����룬Ȼ���жϵ����ԣ��ݼ�Ϊ���ͷ糡������Ϊ�������ַ糡
	vector <int> NouseData;//���ȥ��-1�ķ�����
	for (int k = 0; k < 8; ++k)
	{
		if (windDtemp[k] != -1)
			NouseData.push_back(windDtemp[k]);
	}
	//�жϵ�����,��ֵķ���
	//vector <int> DecData;//��Ų�ַ�����
	int tempDec = 0;//�м����
	int BigNum = 0;
	int SmaNum = 0;//ͳ�Ʋ��������������
	tempDec = NouseData[0] - NouseData[NouseData.size() - 1];
	//DecData.push_back(tempDec);
	if (tempDec > 0)
	{
		BigNum = BigNum + 1;
	}
	if (tempDec < 0)
	{
		SmaNum = SmaNum + 1;
	}
	for (int k = 0; k < NouseData.size() - 1; ++k)
	{
		tempDec = NouseData[k + 1] - NouseData[k];
		//DecData.push_back(NouseData[k + 1] - NouseData[k]);
		if (tempDec > 0)
		{
			BigNum = BigNum + 1;
		}
		if (tempDec < 0)
		{
			SmaNum = SmaNum + 1;
		}
	}

	//�жϣ���������Ϊ1��������������2���������
	//��������Ϊ1��������������2����ݼ�
	int FunctionMark = 1;//��Ǻ���������
	if (SmaNum == 1 && BigNum > 2)
	{
		FunctionMark = 1;//��
	}
	else
	{
		return returnData;
	}

	///////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////
	//ȥ���ظ��ķ�����
	vector <int> NoRepeat;
	NoRepeat.push_back(NouseData[0]);
	for (int m = 1; m < NouseData.size(); m++)
	{
		int numNR = 0;
		for (int n = 0; n < m; n++)
		{
			if (NouseData[m] != NouseData[n])
			{
				numNR++;
			}
		}
		if (numNR == m)
		{
			NoRepeat.push_back(NouseData[m]);
		}
	}

	//NoRepeat�����ŵ�Ϊ���ظ�����	//Ϊ8��ֵָ������λ��
	//���ȥ���ظ���Ԫ�ظ������ڵ���4��������γ�������
	//����������ȫ����������������������
	if (NoRepeat.size() <= 3)
	{
		NoRepeat.clear();
		return returnData;
	}
	////////////////////////////////////////////////////////////////////

	//�Բ�����������////��С��������
	int Ttemp = 0;
	int sizeN = NoRepeat.size();
	for (int m = 0; m < sizeN - 1; m++)
	{
		for (int n = 0; n < sizeN - 1 - m; n++)
		{
			if (NoRepeat[n]>NoRepeat[n + 1])
			{
				Ttemp = NoRepeat[n];
				NoRepeat[n] = NoRepeat[n + 1];
				NoRepeat[n + 1] = Ttemp;
			}
		}
	}

	//ʵ��CSWS�����Բ���Do
	vector <int> DecData;//��Ų�ַ�����
	//NoRepeat�������
	int tempDecD = NoRepeat[0] - NoRepeat[NoRepeat.size() - 1] + 8;
	DecData.push_back(tempDecD);
	for (int k = 0; k < NoRepeat.size() - 1; ++k)
	{
		tempDecD = NoRepeat[k + 1] - NoRepeat[k];
		DecData.push_back(tempDecD);
	}
	//�����������ֵ
	int DecM = 0;
	for (int k = 0; k < NoRepeat.size(); ++k)
	{
		if (DecData[k]>DecM)
		{
			DecM = DecData[k];
		}
	}

	//����Գƶ�ָ��
	double centerD = 0;
	centerD = (DecM - 1)*1.0 / 8;

	//ʵ��CSWS����Ч�����ȱ��Po
	double centerP = 1 - NoRepeat.size() / 8.0;

	//CSWS�ۺ��α�ָ��U
	double centerU = 0.67*centerD + 0.33*centerP;

	//����U�������Ⱥ���f(u)
	centerFU = exp(-4.3*centerU*centerU);

	//test2 << centerE << "  " << centerD << "  " << centerP << "  " << centerFU << endl;

	if (centerFU > 0.55)
	{
		returnData = FunctionMark;
	}

	NouseData.clear();
	NoRepeat.clear();
	return returnData;
}



int handle::typeJude(int *temp)
{
	//����������temp�У��˸����������ж�//
	int typeTemp = 1;
	/////////////////////////���׼�����������ж�/////////////////
	int type1[8];////����
	int type2[8];////������
	int type3[8];////����
	int type4[8];////��ɢ
	for (int i = 0; i < 8; ++i)
	{
		type1[i] = 0;
		type2[i] = 0;
		type3[i] = 0;
		type4[i] = 0;
	}
	///////////////////////////////////////////////////
	if (temp[0] != -1)
	{
		type1[0] = abs(temp[0] - 2);
		type2[0] = abs(temp[0] - 6);
		type3[0] = abs(temp[0] - 5);
		type4[0] = abs(temp[0] - 1);
	}
	if (temp[1] != -1)
	{
		type1[1] = abs(temp[1] - 3);
		type2[1] = abs(temp[1] - 7);
		type3[1] = abs(temp[1] - 6);
		type4[1] = abs(temp[1] - 2);
	}
	if (temp[2] != -1)
	{
		type1[2] = abs(temp[2] - 4);
		type2[2] = abs(temp[2] - 0);
		type3[2] = abs(temp[2] - 7);
		type4[2] = abs(temp[2] - 3);
	}
	if (temp[3] != -1)
	{
		type1[3] = abs(temp[3] - 5);
		type2[3] = abs(temp[3] - 1);
		type3[3] = abs(temp[3] - 0);
		type4[3] = abs(temp[3] - 4);
	}
	if (temp[4] != -1)
	{
		type1[4] = abs(temp[4] - 6);
		type2[4] = abs(temp[4] - 2);
		type3[4] = abs(temp[4] - 1);
		type4[4] = abs(temp[4] - 5);
	}
	if (temp[5] != -1)
	{
		type1[5] = abs(temp[5] - 7);
		type2[5] = abs(temp[5] - 3);
		type3[5] = abs(temp[5] - 2);
		type4[5] = abs(temp[5] - 6);
	}
	if (temp[6] != -1)
	{
		type1[6] = abs(temp[6] - 0);
		type2[6] = abs(temp[6] - 4);
		type3[6] = abs(temp[6] - 3);
		type4[6] = abs(temp[6] - 7);
	}
	if (temp[7] != -1)
	{
		type1[7] = abs(temp[7] - 1);
		type2[7] = abs(temp[7] - 5);
		type3[7] = abs(temp[7] - 4);
		type4[7] = abs(temp[7] - 0);
	}
	struct Result
	{
		int data;
		int num;
	};
	Result returnType[4];
	returnType[0].data = 1;
	returnType[1].data = 2;
	returnType[2].data = 3;
	returnType[3].data = 4;
	for (int i = 0; i < 4; ++i)
	{
		returnType[i].num = 0;
	}
	for (int k = 0; k < 8; ++k)
	{
		if (type1[k] > 4)
		{
			type1[k] = 8 - type1[k];
		}
		returnType[0].num = returnType[0].num + type1[k];
		if (type2[k] > 4)
		{
			type2[k] = 8 - type2[k];
		}
		returnType[1].num = returnType[1].num + type2[k];
		if (type3[k] > 4)
		{
			type3[k] = 8 - type3[k];
		}
		returnType[2].num = returnType[2].num + type3[k];
		if (type4[k] > 4)
		{
			type4[k] = 8 - type4[k];
		}
		returnType[3].num = returnType[3].num + type4[k];
	}
	//��һά����temp1����,�ɴ�С����
	for (int m = 0; m < 3; ++m)
	{
		for (int n = 0; n < 3 - m; ++n)
		{
			Result temp;
			temp.num = 0;
			temp.data = 0;
			if (returnType[n].num < returnType[n + 1].num)
			{
				temp = returnType[n];
				returnType[n] = returnType[n + 1];
				returnType[n + 1] = temp;
			}
		}
	}
	//�������
	typeTemp = returnType[3].data;
	return typeTemp;

}



//////����
void handle::CLuster(vector<int>X, vector<int>Y, int typeN, int datanumN, int &longitude_num, int &latitude_num,
	vector<int>& wdX, vector<int>& wdY, vector <int>& wdXx, vector <int>& wdYy, vector<float> angleuv)
{
	// TODO:  �ڴ���������������
	struct InputData
	{
		int type;
		int mark;
		int dataX;
		int dataY;
	};
	InputData* Jdata = new InputData[datanumN];//��ž�����Ԫ����Ϣ
	for (int k = 0; k <datanumN; k++)
	{
		Jdata[k].dataX = X[k];
		Jdata[k].dataY = Y[k];
		Jdata[k].mark = 0;
		Jdata[k].type = typeN;
	}//��ʼ������

	int datanum = 0;
	int datan = 0;
	int onceNum = 0;
	while (datanum != datanumN)
	{
		datan++;
		onceNum = datanum;
		//��һ��ѭ�����ҵ���һ�����ӵ�
		vector<int> tempi;
		vector<int> tempj;
		tempi.clear();
		tempj.clear();
		bool done = false;
		for (int i = 0; i < datanumN && !done; i++)
		{
			if (Jdata[i].mark == 0)
			{
				Jdata[i].mark = datan;
				tempi.push_back(Jdata[i].dataX);
				tempj.push_back(Jdata[i].dataY);
				datanum++;
				done = true;
				break;
			}
		}

		for (int i = 0; i < datanumN; i++)
		{
			for (int j = 0; j < datanumN; j++)
			{
				if (Jdata[j].mark != datan)
				{
					int pointway = 20;
					for (int m = 0; m < tempi.size(); m++)
					{
						int temppointway = (Jdata[j].dataX - tempi[m])*(Jdata[j].dataX - tempi[m]) +
							(Jdata[j].dataY - tempj[m])*(Jdata[j].dataY - tempj[m]);
						if (temppointway < pointway)
						{
							pointway = temppointway;
						}
					}
					if (pointway < 19)
					{
						Jdata[j].mark = datan;
						tempi.push_back(Jdata[j].dataX);
						tempj.push_back(Jdata[j].dataY);
						datanum++;
					}
				}

			}
		}

		onceNum = datanum - onceNum;
		if (onceNum == 0)
		{
			datanum = datanum - onceNum;
			datan--;
			break;
		}
//        qDebug()<<"The"<<datan<<"time find center"<<"size="<<tempi.size();
	}


	////////���������������/////////////
	wdX.clear();
	wdY.clear();
	wdXx.clear();
	wdYy.clear();

	for (int m = 0; m < datan; m++)
	{
		vector <int> valueX;
		vector <int> valueY;//���ÿ�������ֵ

		valueX.clear();
		valueY.clear();

		int valueNum = 0;//ͳ��ÿ����Ŀ
		int nouDataNum = 0;//ͳ����Ч����Ŀ
		for (int n = 0; n <datanum; n++)
		{
			if (Jdata[n].mark == (m + 1))
			{
				valueNum++;
				valueX.push_back(Jdata[n].dataX);
				valueY.push_back(Jdata[n].dataY);
			}
		}

		//�����ж�
		int WtempX = 0;
		int WtempY = 0;
		int WtempXx = 0;
		int WtempYy = 0;
		double tempMax = -10;
		double tempMin = 10;
		for (int k = 0; k < valueX.size(); k++)
		{
			int t = valueX[k];
			int s = valueY[k];
			double tempWValue = 0;
			double tempV1 = sin(angleuv[s*longitude_num + t + 1] * 3.1415962 / 180) - sin(angleuv[s*longitude_num + t - 1] * 3.1415962 / 180);
			double tempV2 = cos(angleuv[(s - 1)*longitude_num + t] * 3.1415962 / 180) - cos(angleuv[(s + 1)*longitude_num + t] * 3.1415962 / 180);
			tempWValue = (tempV1 - tempV2) / 2;
			if (tempWValue > tempMax)
			{
				tempMax = tempWValue;
				WtempX = t;
				WtempY = s;
			}
			if (tempWValue < tempMin)
			{
				tempMin = tempWValue;
				WtempXx = t;
				WtempYy = s;
			}
		}
		wdX.push_back(WtempX);
		wdY.push_back(WtempY);
		wdXx.push_back(WtempXx);
		wdYy.push_back(WtempYy);
	}
	delete Jdata;
}



////��ֵ

void handle::Interpolation(vector<int>centerX, vector<int>centerY, int longitude_num, int latitude_num,
	float* u, float* v, vector<float>& centerXX, vector<float>& centerYY, int modeL)
{
	struct SetPoint
	{
		float TpX;//���������Ϣ
		float TpY;
		float TpU;//����ٶ���Ϣ
		float TpV;
	};
	///����ÿ������Χ����
    for (int i = 0; i < centerX.size(); i++)
	{
		int tempX = centerX[i];
		int tempY = centerY[i];
        ////****here's a bug*****????
        if(tempX<7||tempY<7)
            continue;
		//�õ��µģ�modeL*2+1���׾���
		int modeN = 2 * modeL + 1;
		SetPoint* pointSet = new SetPoint[modeN*modeN];
		for (int m = 0; m < modeN; m++)
		{
			for (int n = 0; n < modeN; n++)
			{
				int tempx1 = tempX - modeL + n;
				int tempy1 = tempY - modeL + m;
				float tempu = u[longitude_num*tempy1 + tempx1];
				float tempv = v[longitude_num*tempy1 + tempx1];
				pointSet[m*modeN + n].TpX = tempx1;
				pointSet[m*modeN + n].TpY = tempy1;
				pointSet[m*modeN + n].TpU = tempu;
				pointSet[m*modeN + n].TpV = tempv;
			}
		}
		//�ж��µ�6*6�������Ƿ������㣬��ÿ��С��������ж�
		//�����ж����㣬�洢��������ѡ��
		vector<float> centerTempX;
		vector<float> centerTempY;
		centerTempX.clear();
		centerTempY.clear();

		for (int m = 0; m < modeN - 1; m++)
		{
			for (int n = 0; n < modeN - 1; n++)
			{

				//��������ʼֵȡΪ��������λ��
				float tempWx = (pointSet[m*modeN + n].TpX + pointSet[m*modeN + n + 1].TpX) / 2.0;
				float tempWy = (pointSet[m*modeN + n].TpY + pointSet[(m + 1)*modeN + n + 1].TpY) / 2.0;

				//U��V�����ֱ��ж�
				int positiveU = 0;//�ٶ�U����Ϊ���ĸ���
				int minusU = 0;//�ٶ�U����Ϊ���ĸ���
				int positiveV = 0;//�ٶ�V����Ϊ���ĸ���
				int minusV = 0;//�ٶ�V����Ϊ���ĸ���

				if (pointSet[m*modeN + n].TpU > 0)
				{
					positiveU = positiveU + 1;
				}
				else
				{
					minusU = minusU + 1;
				}

				if (pointSet[m*modeN + n + 1].TpU > 0)
				{
					positiveU = positiveU + 1;
				}
				else
				{
					minusU = minusU + 1;
				}
				if (pointSet[(m + 1)*modeN + n].TpU > 0)
				{
					positiveU = positiveU + 1;
				}
				else
				{
					minusU = minusU + 1;
				}
				if (pointSet[(m + 1)*modeN + n + 1].TpU > 0)
				{
					positiveU = positiveU + 1;
				}
				else
				{
					minusU = minusU + 1;
				}
				//ͳ��U����

				if (pointSet[m*modeN + n].TpV > 0)
				{
					positiveV = positiveV + 1;
				}
				else
				{
					minusV = minusV + 1;
				}
				if (pointSet[m*modeN + n + 1].TpV > 0)
				{
					positiveV = positiveV + 1;
				}
				else
				{
					minusV = minusV + 1;
				}
				if (pointSet[(m + 1)*modeN + n].TpV > 0)
				{
					positiveV = positiveV + 1;
				}
				else
				{
					minusV = minusV + 1;
				}
				if (pointSet[(m + 1)*modeN + n + 1].TpV > 0)
				{
					positiveV = positiveV + 1;
				}
				else
				{
					minusV = minusV + 1;
				}


				//�����������㣬�˳��˴�ѭ��
				//�����ֵѰ�����
				if ((positiveU > 0) && (minusU > 0) && (positiveV > 0) && (minusV > 0))
				{
					//U��V�ֱ������㣬�ж��Ƿ��н���
					//����Ϊ������������U��V�ֱ𻭵�ֵ��

					//��һ�����������ҵ�U��V�߶�

					//����U
					//��һ������
					float tempUX1 = 0; float tempUY1 = 0;
					float tempUX2 = 0; float tempUY2 = 0;
					float tempUX3 = 0; float tempUY3 = 0;
					vector<float> tempUX4; vector<float> tempUY4;
					vector<float> tempUX5; vector<float> tempUY5;
					tempUX4.clear(); tempUY4.clear();
					tempUX5.clear(); tempUY5.clear();

					//��������֮��
					if ((pointSet[m*modeN + n].TpU)*(pointSet[(m + 1)*modeN + n].TpU) < 0)
					{
						//�����������
						tempUX1 = pointSet[m*modeN + n].TpX;
						tempUY1 = pointSet[m*modeN + n].TpY +
							fabs((pointSet[m*modeN + n].TpU) / (pointSet[(m + 1)*modeN + n].TpU - pointSet[m*modeN + n].TpU));
						tempUX4.push_back(tempUX1);
						tempUY4.push_back(tempUY1);
					}

					//�Խ�֮��
					if ((pointSet[m*modeN + n].TpU)*(pointSet[(m + 1)*modeN + n + 1].TpU) < 0)
					{
						//�����������
						tempUX2 = pointSet[m*modeN + n].TpX +
							fabs((pointSet[m*modeN + n].TpU) / (pointSet[(m + 1)*modeN + n + 1].TpU - pointSet[m*modeN + n].TpU));
						tempUY2 = pointSet[m*modeN + n].TpY +
							fabs((pointSet[m*modeN + n].TpU) / (pointSet[(m + 1)*modeN + n + 1].TpU - pointSet[m*modeN + n].TpU));
						tempUX4.push_back(tempUX2);
						tempUY4.push_back(tempUY2);
					}

					//�·�����֮��
					if ((pointSet[(m + 1)*modeN + n].TpU)*(pointSet[(m + 1)*modeN + n + 1].TpU) < 0)
					{
						//�����������
						tempUX3 = pointSet[(m + 1)*modeN + n].TpX +
							fabs((pointSet[(m + 1)*modeN + n].TpU) / (pointSet[(m + 1)*modeN + n + 1].TpU - pointSet[(m + 1)*modeN + n].TpU));
						tempUY3 = pointSet[(m + 1)*modeN + n].TpY;
						tempUX4.push_back(tempUX3);
						tempUY4.push_back(tempUY3);
					}

					//�ڶ�������
					//�ϲ���������֮��
					if ((pointSet[m*modeN + n].TpU)*(pointSet[m*modeN + n + 1].TpU) < 0)
					{
						//�����������
						tempUX1 = pointSet[m*modeN + n].TpX +
							fabs((pointSet[m*modeN + n].TpU) / (pointSet[m*modeN + n + 1].TpU - pointSet[m*modeN + n].TpU));
						tempUY1 = pointSet[m*modeN + n].TpY;
						tempUX5.push_back(tempUX1);
						tempUY5.push_back(tempUY1);
					}
					//�Խ�֮��
					if ((pointSet[m*modeN + n].TpU)*(pointSet[(m + 1)*modeN + n + 1].TpU) < 0)
					{
						//�����������
						tempUX2 = pointSet[m*modeN + n].TpX +
							fabs((pointSet[m*modeN + n].TpU) / (pointSet[(m + 1)*modeN + n + 1].TpU - pointSet[m*modeN + n].TpU));
						tempUY2 = pointSet[m*modeN + n].TpY +
							fabs((pointSet[m*modeN + n].TpU) / (pointSet[(m + 1)*modeN + n + 1].TpU - pointSet[m*modeN + n].TpU));
						tempUX5.push_back(tempUX2);
						tempUY5.push_back(tempUY2);
					}
					//����֮��
					if ((pointSet[m*modeN + n + 1].TpU)*(pointSet[(m + 1)*modeN + n + 1].TpU) < 0)
					{
						//�����������
						tempUX3 = pointSet[m*modeN + n + 1].TpX;
						tempUY3 = pointSet[m*modeN + n + 1].TpY +
							fabs((pointSet[m*modeN + n + 1].TpU) / (pointSet[(m + 1)*modeN + n + 1].TpU - pointSet[m*modeN + n + 1].TpU));
						tempUX5.push_back(tempUX3);
						tempUY5.push_back(tempUY3);
					}

					//��ȡU�����ߣ�һ������һ���߶�
					//�ȿ���������ģ���U��0�����ʱ������
					//��������������ֱ�߷���
					float xu1 = 0;
					float yu1 = 0;
					if (tempUX4.size() == 2)
					{
						//��������A1(tempUX4[0],tempUY4[0]);B1(tempUX4[1],tempUY4[1])
						yu1 = tempUY4[0] +
							((tempUY4[1] - tempUY4[0]) / (tempUX4[1] - tempUX4[0]))*(xu1 - tempUX4[0]);

					}
					float xu2 = 0;
					float yu2 = 0;
					if (tempUX5.size() == 2)
					{
						//��������C1(tempUX5[0],tempUY5[0]);D1(tempUX5[1],tempUY5[1])
						yu2 = tempUY5[0] +
							((tempUY5[1] - tempUY5[0]) / (tempUX5[1] - tempUX5[0]))*(xu2 - tempUX5[0]);
					}
					//U�������߶η���������

					//����V
					//��һ������
					float tempVX1 = 0; float tempVY1 = 0;
					float tempVX2 = 0; float tempVY2 = 0;
					float tempVX3 = 0; float tempVY3 = 0;
					vector<float> tempVX4; vector<float> tempVY4;
					vector<float> tempVX5; vector<float> tempVY5;
					tempVX4.clear(); tempVY4.clear();
					tempVX5.clear(); tempVY5.clear();

					if ((pointSet[m*modeN + n].TpV)*(pointSet[(m + 1)*modeN + n].TpV) < 0)//��������֮��
					{
						//�����������
						tempVX1 = pointSet[m*modeN + n].TpX;
						tempVY1 = pointSet[m*modeN + n].TpY +
							fabs((pointSet[m*modeN + n].TpV) / (pointSet[(m + 1)*modeN + n].TpV - pointSet[m*modeN + n].TpV));
						tempVX4.push_back(tempVX1);
						tempVY4.push_back(tempVY1);
					}
					if ((pointSet[m*modeN + n].TpV)*(pointSet[(m + 1)*modeN + n + 1].TpV) < 0)//�Խ�֮��
					{
						//�����������
						tempVX2 = pointSet[m*modeN + n].TpX +
							fabs((pointSet[m*modeN + n].TpV) / (pointSet[(m + 1)*modeN + n + 1].TpV - pointSet[m*modeN + n].TpV));
						tempVY2 = pointSet[m*modeN + n].TpY +
							fabs((pointSet[m*modeN + n].TpV) / (pointSet[(m + 1)*modeN + n + 1].TpV - pointSet[m*modeN + n].TpV));
						tempVX4.push_back(tempVX2);
						tempVY4.push_back(tempVY2);
					}
					if ((pointSet[(m + 1)*modeN + n].TpV)*(pointSet[(m + 1)*modeN + n + 1].TpV) < 0)//�·�����֮��
					{
						//�����������
						tempVX3 = pointSet[(m + 1)*modeN + n].TpX +
							fabs((pointSet[(m + 1)*modeN + n].TpV) / (pointSet[(m + 1)*modeN + n + 1].TpV - pointSet[(m + 1)*modeN + n].TpV));
						tempVY3 = pointSet[(m + 1)*modeN + n].TpY;
						tempVX4.push_back(tempVX3);
						tempVY4.push_back(tempVY3);
					}

					//�ڶ�������
					if ((pointSet[m*modeN + n].TpV)*(pointSet[m*modeN + n + 1].TpV) < 0)//�ϲ���������֮��
					{
						//�����������
						tempVX1 = pointSet[m*modeN + n].TpX +
							fabs((pointSet[m*modeN + n].TpV) / (pointSet[m*modeN + n + 1].TpV - pointSet[m*modeN + n].TpV));
						tempVY1 = pointSet[m*modeN + n].TpY;
						tempVX5.push_back(tempVX1);
						tempVY5.push_back(tempVY1);
					}
					if ((pointSet[m*modeN + n].TpV)*(pointSet[(m + 1)*modeN + n + 1].TpV) < 0)//�Խ�֮��
					{
						//�����������
						tempVX2 = pointSet[m*modeN + n].TpX +
							fabs((pointSet[m*modeN + n].TpV) / (pointSet[(m + 1)*modeN + n + 1].TpV - pointSet[m*modeN + n].TpV));
						tempVY2 = pointSet[m*modeN + n].TpY +
							fabs((pointSet[m*modeN + n].TpV) / (pointSet[(m + 1)*modeN + n + 1].TpV - pointSet[m*modeN + n].TpV));
						tempVX5.push_back(tempVX2);
						tempVY5.push_back(tempVY2);
					}
					if ((pointSet[m*modeN + n + 1].TpV)*(pointSet[(m + 1)*modeN + n + 1].TpV) < 0)//����֮��
					{
						//�����������
						tempVX3 = pointSet[m*modeN + n + 1].TpX;
						tempVY3 = pointSet[m*modeN + n + 1].TpY +
							fabs((pointSet[m*modeN + n + 1].TpV) / (pointSet[(m + 1)*modeN + n + 1].TpV - pointSet[m*modeN + n + 1].TpV));
						tempVX5.push_back(tempVX3);
						tempVY5.push_back(tempVY3);
					}

					//��ȡV�����ߣ�һ������һ���߶�
					//�ȿ���������ģ���V��0�����ʱ������
					//��������������ֱ�߷���
					float xv1 = 0;
					float yv1 = 0;
					if (tempVX4.size() == 2)
					{
						//��������A2(tempVX4[0],tempVY4[0]);B2(tempVX4[1],tempVY4[1])
						yv1 = tempVY4[0] +
							((tempVY4[1] - tempVY4[0]) / (tempVX4[1] - tempVX4[0]))*(xv1 - tempVX4[0]);

					}
					float xv2 = 0;
					float yv2 = 0;
					if (tempVX5.size() == 2)
					{
						//��������C2(tempVX5[0],tempVY5[0]);D2(tempVX5[1],tempVY5[1])
						yv2 = tempVY5[0] +
							((tempVY5[1] - tempVY5[0]) / (tempVX5[1] - tempVX5[0]))*(xv2 - tempVX5[0]);
					}
					//V�������߶η���������

					//�ж�U��V�߶��Ƿ��н���
					//��һ�����
					if (tempUX4.size() == 2 && tempVX4.size() == 2)
					{
						float s1 = (tempUX4[0] - tempVX4[0])*(tempUY4[1] - tempVY4[0]) -
							(tempUY4[0] - tempVY4[0])*(tempUX4[1] - tempVX4[0]);
						float s2 = (tempUX4[0] - tempVX4[1])*(tempUY4[1] - tempVY4[1]) -
							(tempUY4[0] - tempVY4[1])*(tempUX4[1] - tempVX4[1]);
						//�ж�s1*s2������

						float s3 = (tempVX4[0] - tempUX4[0])*(tempVY4[1] - tempUY4[0]) -
							(tempVY4[0] - tempUY4[0])*(tempVX4[1] - tempUX4[0]);
						float s4 = s3 + s1 - s2;
						//�ж�s3*s4������

						if ((s1*s2 <= 0) && (s3*s4 <= 0))
						{
							//���㽻��
							float dt = (tempUY4[1] - tempUY4[0])*(tempVX4[1] - tempVX4[0]) -
								(tempVY4[1] - tempVY4[0])*(tempUX4[1] - tempUX4[0]);

							float Tdx = (tempUX4[1] - tempUX4[0])*(tempVX4[1] - tempVX4[0])*(tempVY4[0] - tempUY4[0]) +
								(tempUY4[1] - tempUY4[0])*(tempVX4[1] - tempVX4[0])*tempUX4[0] -
								(tempVY4[1] - tempVY4[0])*(tempUX4[1] - tempUX4[0])*tempVX4[0];

							float Tdy = (tempUY4[1] - tempUY4[0])*(tempVY4[1] - tempVY4[0])*(tempVX4[0] - tempUX4[0]) +
								(tempUX4[1] - tempUX4[0])*(tempVY4[1] - tempVY4[0])*tempUY4[0] -
								(tempVX4[1] - tempVX4[0])*(tempUY4[1] - tempUY4[0])*tempVY4[0];

							if (dt != 0)
							{
								tempWx = Tdx / dt;
								tempWy = -Tdy / dt;
							}




						}

					}

					//�ཻ�ڶ������
					else if (tempUX4.size() == 2 && tempVX5.size() == 2)
					{
						float s1 = (tempUX4[0] - tempVX5[0])*(tempUY4[1] - tempVY5[0]) -
							(tempUY4[0] - tempVY5[0])*(tempUX4[1] - tempVX5[0]);
						float s2 = (tempUX4[0] - tempVX5[1])*(tempUY4[1] - tempVY5[1]) -
							(tempUY4[0] - tempVY5[1])*(tempUX4[1] - tempVX5[1]);
						//�ж�s1*s2������

						float s3 = (tempVX5[0] - tempUX4[0])*(tempVY5[1] - tempUY4[0]) -
							(tempVY5[0] - tempUY4[0])*(tempVX5[1] - tempUX4[0]);
						float s4 = s3 + s1 - s2;
						//�ж�s3*s4������

						if ((s1*s2 <= 0) && (s3*s4 <= 0))
						{
							//���㽻��
							float dt = (tempUY4[1] - tempUY4[0])*(tempVX5[1] - tempVX5[0]) -
								(tempVY5[1] - tempVY5[0])*(tempUX4[1] - tempUX4[0]);

							float Tdx = (tempUX4[1] - tempUX4[0])*(tempVX5[1] - tempVX5[0])*(tempVY5[0] - tempUY4[0]) +
								(tempUY4[1] - tempUY4[0])*(tempVX5[1] - tempVX5[0])*tempUX4[0] -
								(tempVY5[1] - tempVY5[0])*(tempUX4[1] - tempUX4[0])*tempVX5[0];

							float Tdy = (tempUY4[1] - tempUY4[0])*(tempVY5[1] - tempVY5[0])*(tempVX5[0] - tempUX4[0]) +
								(tempUX4[1] - tempUX4[0])*(tempVY5[1] - tempVY5[0])*tempUY4[0] -
								(tempVX5[1] - tempVX5[0])*(tempUY4[1] - tempUY4[0])*tempVY5[0];

							if (dt != 0)
							{
								tempWx = Tdx / dt;
								tempWy = -Tdy / dt;
							}


						}
					}
					//�ཻ���������
					else if (tempUX5.size() == 2 && tempVX4.size() == 2)
					{
						float s1 = (tempUX5[0] - tempVX4[0])*(tempUY5[1] - tempVY4[0]) -
							(tempUY5[0] - tempVY4[0])*(tempUX5[1] - tempVX4[0]);
						float s2 = (tempUX5[0] - tempVX4[1])*(tempUY5[1] - tempVY4[1]) -
							(tempUY5[0] - tempVY4[1])*(tempUX5[1] - tempVX4[1]);
						//�ж�s1*s2������

						float s3 = (tempVX4[0] - tempUX5[0])*(tempVY4[1] - tempUY5[0]) -
							(tempVY4[0] - tempUY5[0])*(tempVX4[1] - tempUX5[0]);
						float s4 = s3 + s1 - s2;
						//�ж�s3*s4������

						if ((s1*s2 <= 0) && (s3*s4 <= 0))
						{
							//���㽻��
							float dt = (tempUY5[1] - tempUY5[0])*(tempVX4[1] - tempVX4[0]) -
								(tempVY4[1] - tempVY4[0])*(tempUX5[1] - tempUX5[0]);

							float Tdx = (tempUX5[1] - tempUX5[0])*(tempVX4[1] - tempVX4[0])*(tempVY4[0] - tempUY5[0]) +
								(tempUY5[1] - tempUY5[0])*(tempVX4[1] - tempVX4[0])*tempUX5[0] -
								(tempVY4[1] - tempVY4[0])*(tempUX5[1] - tempUX5[0])*tempVX4[0];

							float Tdy = (tempUY5[1] - tempUY5[0])*(tempVY4[1] - tempVY4[0])*(tempVX4[0] - tempUX5[0]) +
								(tempUX5[1] - tempUX5[0])*(tempVY4[1] - tempVY4[0])*tempUY5[0] -
								(tempVX4[1] - tempVX4[0])*(tempUY5[1] - tempUY5[0])*tempVY4[0];

							if (dt != 0)
							{
								tempWx = Tdx / dt;
								tempWy = -Tdy / dt;
							}


						}
					}
					//�ཻ���������
					else if (tempUX5.size() == 2 && tempVX5.size() == 2)
					{
						float s1 = (tempUX5[0] - tempVX5[0])*(tempUY5[1] - tempVY5[0]) -
							(tempUY5[0] - tempVY5[0])*(tempUX5[1] - tempVX5[0]);
						float s2 = (tempUX5[0] - tempVX5[1])*(tempUY5[1] - tempVY5[1]) -
							(tempUY5[0] - tempVY5[1])*(tempUX5[1] - tempVX5[1]);
						//�ж�s1*s2������

						float s3 = (tempVX5[0] - tempUX5[0])*(tempVY5[1] - tempUY5[0]) -
							(tempVY5[0] - tempUY5[0])*(tempVX5[1] - tempUX5[0]);
						float s4 = s3 + s1 - s2;
						//�ж�s3*s4������

						if ((s1*s2 <= 0) && (s3*s4 <= 0))
						{
							//���㽻��
							float dt = (tempUY5[1] - tempUY5[0])*(tempVX5[1] - tempVX5[0]) -
								(tempVY5[1] - tempVY5[0])*(tempUX5[1] - tempUX5[0]);

							float Tdx = (tempUX5[1] - tempUX5[0])*(tempVX5[1] - tempVX5[0])*(tempVY5[0] - tempUY5[0]) +
								(tempUY5[1] - tempUY5[0])*(tempVX5[1] - tempVX5[0])*tempUX5[0] -
								(tempVY5[1] - tempVY5[0])*(tempUX5[1] - tempUX5[0])*tempVX5[0];

							float Tdy = (tempUY5[1] - tempUY5[0])*(tempVY5[1] - tempVY5[0])*(tempVX5[0] - tempUX5[0]) +
								(tempUX5[1] - tempUX5[0])*(tempVY5[1] - tempVY5[0])*tempUY5[0] -
								(tempVX5[1] - tempVX5[0])*(tempUY5[1] - tempUY5[0])*tempVY5[0];

							if (dt != 0)
							{
								tempWx = Tdx / dt;
								tempWy = -Tdy / dt;
							}
						}
					}

					centerTempX.push_back(tempWx);
					centerTempY.push_back(tempWy);
				}
				else
				{
					continue;
				}

			}
		}

		if (centerTempX.size()>0)
		{
			float tempCom = (centerTempX[0] - centerX[i])*(centerTempX[0] - centerX[i]) +
				(centerTempY[0] - centerY[i])*(centerTempY[0] - centerY[i]);
			float xtemp = centerTempX[0];
			float ytemp = centerTempY[0];
			for (int k = 0; k < centerTempX.size(); k++)
			{
				if (((centerTempX[k] - centerX[i])*(centerTempX[k] - centerX[i]) +
					(centerTempY[k] - centerY[i])*(centerTempY[k] - centerY[i])) < tempCom)
				{
					tempCom = (centerTempX[k] - centerX[i])*(centerTempX[k] - centerX[i]) +
						(centerTempY[k] - centerY[i])*(centerTempY[k] - centerY[i]);
					xtemp = centerTempX[k];
					ytemp = centerTempY[k];
				}
			}
			centerXX.push_back(xtemp);
			centerYY.push_back(ytemp); //����ҵ����������
		}



		//6*6�������Ƿ��������ж����,��������ѡ��ȡ����������ĵ�
		if (centerTempX.size() <= 0)
		{
			//continue;
			//���¾�����в�ֵ
			int insertN = 40;
			//�Ծ���������ݽ��в�ֵ
			SetPoint* pointSetRow = new SetPoint[insertN * (modeN - 1)*modeN];

			int nn = 0;
			for (int m = 0; m < modeN; m++)
			{
				int kk = 0;
				for (nn = 0; nn < (modeN - 1); nn++)
				{
					//�ȶ������ֵ
					//X����
					float tempXYValX = (pointSet[m*modeN + nn + 1].TpX - pointSet[m*modeN + nn].TpX)*1.0 / insertN;
					//Y����
					float tempXYValY = (pointSet[m*modeN + nn + 1].TpY - pointSet[m*modeN + nn].TpY)*1.0 / insertN;
					//�ٶ��ٶȲ�ֵ
					//�ٶ�U
					float tempXYValU = (pointSet[m*modeN + nn + 1].TpU - pointSet[m*modeN + nn].TpU)*1.0 / insertN;
					//�ٶ�V
					float tempXYValV = (pointSet[m*modeN + nn + 1].TpV - pointSet[m*modeN + nn].TpV)*1.0 / insertN;

					for (int iiN = 0; iiN < insertN; iiN++)
					{
						pointSetRow[m * insertN * (modeN - 1) + kk + iiN].TpX = pointSet[m*modeN + nn].TpX + iiN * tempXYValX;
						pointSetRow[m * insertN * (modeN - 1) + kk + iiN].TpY = pointSet[m*modeN + nn].TpY + iiN * tempXYValY;
						pointSetRow[m * insertN * (modeN - 1) + kk + iiN].TpU = pointSet[m*modeN + nn].TpU + iiN * tempXYValU;
						pointSetRow[m * insertN * (modeN - 1) + kk + iiN].TpV = pointSet[m*modeN + nn].TpV + iiN * tempXYValV;
					}
					kk = kk + insertN;

				}
			}

			//�Ծ���������ݽ��в�ֵ
			SetPoint* pointSetCol = new SetPoint[insertN * (modeN - 1)*insertN * (modeN - 1)];
			int mm = 0;
			for (int m = 0; m < (modeN - 1); m++)
			{
				for (int n = 0; n < insertN *(modeN - 1); n++)
				{
					//�ȶ������ֵ
					//X����
					float tempXYValX = (pointSetRow[(m + 1)*insertN *(modeN - 1) + n].TpX - pointSetRow[m *insertN *(modeN - 1) + n].TpX)*1.0 / insertN;
					//Y����
					float tempXYValY = (pointSetRow[(m + 1)*insertN *(modeN - 1) + n].TpY - pointSetRow[m *insertN *(modeN - 1) + n].TpY)*1.0 / insertN;
					//�ٶ�U
					float tempXYValU = (pointSetRow[(m + 1)*insertN *(modeN - 1) + n].TpU - pointSetRow[m *insertN *(modeN - 1) + n].TpU)*1.0 / insertN;
					//�ٶ�V
					float tempXYValV = (pointSetRow[(m + 1)*insertN *(modeN - 1) + n].TpV - pointSetRow[m *insertN *(modeN - 1) + n].TpV)*1.0 / insertN;

					for (int iiN = 0; iiN < insertN; iiN++)
					{
						pointSetCol[(mm + iiN)*insertN * (modeN - 1) + n].TpX = pointSetRow[m *insertN * (modeN - 1) + n].TpX + iiN * tempXYValX;
						pointSetCol[(mm + iiN)*insertN * (modeN - 1) + n].TpY = pointSetRow[m *insertN * (modeN - 1) + n].TpY + iiN * tempXYValY;
						pointSetCol[(mm + iiN)*insertN * (modeN - 1) + n].TpU = pointSetRow[m *insertN * (modeN - 1) + n].TpU + iiN * tempXYValU;
						pointSetCol[(mm + iiN)*insertN * (modeN - 1) + n].TpV = pointSetRow[m *insertN * (modeN - 1) + n].TpV + iiN * tempXYValV;
					}
				}
				mm = mm + insertN;
			}

			//��ֵ��ɣ��õ��µľ����ڴ˷�Χ�������ٶ����
			float tempU = 0;
			float tempV = 0;
			float tempUV = 0;
			float tempXX = 0;
			float tempYY = 0;
			float tempValue = 100;//������ʼ��Сֵ
			for (int m = 0; m < insertN * (modeN - 1); m++)
			{
				for (int n = 0; n < insertN * (modeN - 1); n++)
				{
					tempU = pointSetCol[m*insertN * (modeN - 1) + n].TpU;
					tempV = pointSetCol[m*insertN * (modeN - 1) + n].TpV;
                    tempUV = sqrt(tempU*tempU + tempV*tempV);
					if (tempUV < tempValue)
					{
						tempValue = tempUV;
						tempXX = pointSetCol[m*insertN * (modeN - 1) + n].TpX;
						tempYY = pointSetCol[m*insertN * (modeN - 1) + n].TpY;
					}
				}
			}

			centerXX.push_back(tempXX);
			centerYY.push_back(tempYY);
		}
    }

}
