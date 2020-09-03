#include "handle.h"
#include <QDebug>

handle::handle()
{
}


handle::~handle()
{
}


/////////获取风向编码0 - 7
void handle::AngleCode(float* u, float* v, int &longitude_num, int &latitude_num, int* BinarySet, vector<float>& angleuv)
{
	//float temp1 = 0;///中间变量,风速大小
	float temp2 = 0;//中间变量,风速角度
	int temp4 = 0;//中间变量
	for (int j = 0; j < latitude_num; j++)
	{
		for (int i = 0; i < longitude_num; i++)
		{
			//风速角度
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
	//风速角度量化信息
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


//判断点周围风向码类型，若类型数较少，则标记为不检测
void handle::Judge(vector<int>& bianrySet, int latitude_num, int longitude_num)
{
	vector <int> bianarTemp;
	vector <int> NouseData;        // 定义一个整型数组
	bianarTemp.clear();
	///bianarTemp初始化
	for (int j = 0; j < latitude_num; j++)
	{
		for (int i = 0; i < longitude_num; i++)
		{
			bianarTemp.push_back(bianrySet[j*longitude_num + i]);
		}
	}
	////判断
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
			//去除重复的风向码
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
			//////////相邻八个位置不同的风向区域小于八个时，该像素标记为-2
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


//散乱区域判定
void handle::UVJudge(vector<float>& angleUV, vector <int> TempBianry, int latitude_num, int longitude_num, int n)
{
	vector <float> angleData;        // 定义一个容器
	angleData.clear();
	///angleData初始化
	for (int j = 0; j < latitude_num; j++)
	{
		for (int i = 0; i < longitude_num; i++)
		{
			angleData.push_back(angleUV[j*longitude_num + i]);
		}
	}
	//检测点
	for (int j = n + 1; j < latitude_num - n - 1; j++)
	{
		for (int i = n; i < longitude_num - n - 1; i++)
		{
			////标记点跳过
			//if (TempBianry[j*longitude_num + i] == -2)//不是边界处跳过
			//{
			//	angleData[j*longitude_num + i] = 255;
			//	continue;
			//}
			//////////模板(2n+1)*(2n+1)////////////
			float temp1 = 0;
			float temp2 = 0;//存放某点上方及右方的角度差值
			float temp = 0;//模板下所有值总和
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
	//	IplImage* imageG = cvCreateImage(cvSize(longitude_num, latitude_num), 8, 1);//分配图像空间
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


//统计确定每个区域风向；考虑连通区域>=1/3；没有特定风向则取空
int handle::jj(int *temp2, int datanum, int temp3)
{
	//// TODO:  在此添加命令处理程序代码
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
	int temp1[8];//中间变量一维数组，统计区域风向
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
	//对一维数组temp1排序,由大到小排列
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
	//排序结束
	//out << DD[0].num << "  ";
	//如果最大特征值数目小于1/3，返回值-1，表明没有特征风向
	if (DD[0].num <= (datanum + 1)*datanum / 6.0)
	{
		return -1;
	}
	//如果大于1/3，统计最大连通区域数目
	struct InputData
	{
		int type;
		int mark;
	};
	InputData* data = new InputData[(datanum + 2) * (datanum + 2)];
	//初始化数据信息
	for (int i = 0; i < datanum + 2; i++)
	{
		for (int j = 0; j < datanum + 2; j++)
		{
			data[i * (datanum + 2) + j].type = -1;
			data[i * (datanum + 2) + j].mark = -1;
		}
	}
	//其次为特定三角区域赋值
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
	InputData* Jdata = new InputData[DD[0].num];//存放聚类后的元组信息
	for (int k = 0; k <DD[0].num; k++)
	{
		Jdata[k].mark = 0;
		Jdata[k].type = 0;
	}//初始化结束
	////////////寻找最大连通区域//////////////////
	int dataN = 0;
	int datan = 0;
	int onceNum = 0;
	while (dataN != DD[0].num)
	{
		datan++;
		onceNum = dataN;
		//第一次循环：找到第一个种子点
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
		//第二次循环：对周围8邻域进行标记，统计数目
		for (int j = 1; j < (datanum + 1); j++)
		{
			for (int i = 1; i < (datanum + 1); i++)
			{
				if (data[j * (datanum + 2) + i].type == datan)
				{
					Jdata[dataN].mark = datan;
					Jdata[dataN].type = 1;
					dataN++;
					////邻域聚类点标记
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
	//统计每类数目
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
	//对clusterNum排序,由大到小排列
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

	if (areaS <= (datanum + 1)*datanum / 6.0)//如果连通区域面积小于1/3；返回值-1，表明没有特征风向
	{
		return -1;
	}
	else
	{
		temp3 = DD[0].data;
		return temp3;
	}

}


//输入为8区域组成的数组，返回风向值
int handle::PointWind(int *temp, float &centerFU)
{
	int returnData = -1;
	int windDtemp[8] = { 0 };//存储8个区域风向，中间变量
	for (int k = 0; k < 8; ++k)
	{
		windDtemp[k] = temp[k];
	}

	////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////
	//去除-1的风向码，然后判断单调性：递减为鞍型风场，递增为其他四种风场
	vector <int> NouseData;//存放去除-1的风向码
	for (int k = 0; k < 8; ++k)
	{
		if (windDtemp[k] != -1)
			NouseData.push_back(windDtemp[k]);
	}
	//判断单调性,差分的方法
	//vector <int> DecData;//存放差分风向码
	int tempDec = 0;//中间变量
	int BigNum = 0;
	int SmaNum = 0;//统计差分正数负数个数
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

	//判断，负数个数为1，正数个数大于2，则递增；
	//正数个数为1，负数个数大于2，则递减
	int FunctionMark = 1;//标记函数增减性
	if (SmaNum == 1 && BigNum > 2)
	{
		FunctionMark = 1;//增
	}
	else
	{
		return returnData;
	}

	///////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////
	//去除重复的风向码
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

	//NoRepeat里面存放的为无重复数组	//为8个值指定坐标位置
	//如果去处重复后元素个数大于等于4；则可能形成特征风
	//！！！！！全力处理此种情况！！！！！
	if (NoRepeat.size() <= 3)
	{
		NoRepeat.clear();
		return returnData;
	}
	////////////////////////////////////////////////////////////////////

	//对差分码进行排序////从小到大排序
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

	//实际CSWS均匀性波动Do
	vector <int> DecData;//存放差分风向码
	//NoRepeat递增存放
	int tempDecD = NoRepeat[0] - NoRepeat[NoRepeat.size() - 1] + 8;
	DecData.push_back(tempDecD);
	for (int k = 0; k < NoRepeat.size() - 1; ++k)
	{
		tempDecD = NoRepeat[k + 1] - NoRepeat[k];
		DecData.push_back(tempDecD);
	}
	//计算差分码最大值
	int DecM = 0;
	for (int k = 0; k < NoRepeat.size(); ++k)
	{
		if (DecData[k]>DecM)
		{
			DecM = DecData[k];
		}
	}

	//计算对称度指标
	double centerD = 0;
	centerD = (DecM - 1)*1.0 / 8;

	//实际CSWS的有效风向空缺率Po
	double centerP = 1 - NoRepeat.size() / 8.0;

	//CSWS综合形变指数U
	double centerU = 0.67*centerD + 0.33*centerP;

	//基于U的隶属度函数f(u)
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
	//风向码存放在temp中，八个区域依次判断//
	int typeTemp = 1;
	/////////////////////////与标准风向码做差判定/////////////////
	int type1[8];////气旋
	int type2[8];////反气旋
	int type3[8];////辐合
	int type4[8];////辐散
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
	//对一维数组temp1排序,由大到小排列
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
	//排序结束
	typeTemp = returnType[3].data;
	return typeTemp;

}



//////聚类
void handle::CLuster(vector<int>X, vector<int>Y, int typeN, int datanumN, int &longitude_num, int &latitude_num,
	vector<int>& wdX, vector<int>& wdY, vector <int>& wdXx, vector <int>& wdYy, vector<float> angleuv)
{
	// TODO:  在此添加命令处理程序代码
	struct InputData
	{
		int type;
		int mark;
		int dataX;
		int dataY;
	};
	InputData* Jdata = new InputData[datanumN];//存放聚类后的元组信息
	for (int k = 0; k <datanumN; k++)
	{
		Jdata[k].dataX = X[k];
		Jdata[k].dataY = Y[k];
		Jdata[k].mark = 0;
		Jdata[k].type = typeN;
	}//初始化结束

	int datanum = 0;
	int datan = 0;
	int onceNum = 0;
	while (datanum != datanumN)
	{
		datan++;
		onceNum = datanum;
		//第一次循环：找到第一个种子点
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


	////////计算各个聚类中心/////////////
	wdX.clear();
	wdY.clear();
	wdXx.clear();
	wdYy.clear();

	for (int m = 0; m < datan; m++)
	{
		vector <int> valueX;
		vector <int> valueY;//存放每类的坐标值

		valueX.clear();
		valueY.clear();

		int valueNum = 0;//统计每类数目
		int nouDataNum = 0;//统计无效区数目
		for (int n = 0; n <datanum; n++)
		{
			if (Jdata[n].mark == (m + 1))
			{
				valueNum++;
				valueX.push_back(Jdata[n].dataX);
				valueY.push_back(Jdata[n].dataY);
			}
		}

		//计算涡度
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



////插值

void handle::Interpolation(vector<int>centerX, vector<int>centerY, int longitude_num, int latitude_num,
	float* u, float* v, vector<float>& centerXX, vector<float>& centerYY, int modeL)
{
	struct SetPoint
	{
		float TpX;//点的坐标信息
		float TpY;
		float TpU;//点的速度信息
		float TpV;
	};
	///遍历每个点周围邻域
    for (int i = 0; i < centerX.size(); i++)
	{
		int tempX = centerX[i];
		int tempY = centerY[i];
        ////****here's a bug*****????
        if(tempX<7||tempY<7)
            continue;
		//得到新的（modeL*2+1）阶矩阵
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
		//判断新的6*6网格内是否存在零点，对每个小网格进行判断
		//可能有多个零点，存储下来进行选择
		vector<float> centerTempX;
		vector<float> centerTempY;
		centerTempX.clear();
		centerTempY.clear();

		for (int m = 0; m < modeN - 1; m++)
		{
			for (int n = 0; n < modeN - 1; n++)
			{

				//零点坐标初始值取为网格中心位置
				float tempWx = (pointSet[m*modeN + n].TpX + pointSet[m*modeN + n + 1].TpX) / 2.0;
				float tempWy = (pointSet[m*modeN + n].TpY + pointSet[(m + 1)*modeN + n + 1].TpY) / 2.0;

				//U、V分量分别判断
				int positiveU = 0;//速度U分量为正的个数
				int minusU = 0;//速度U分量为负的个数
				int positiveV = 0;//速度V分量为正的个数
				int minusV = 0;//速度V分量为负的个数

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
				//统计U结束

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


				//如果不存在零点，退出此次循环
				//否则插值寻找零点
				if ((positiveU > 0) && (minusU > 0) && (positiveV > 0) && (minusV > 0))
				{
					//U、V分别存在零点，判断是否有交点
					//划分为两个三角网格，U、V分别画等值线

					//第一个三角网格找到U、V线段

					//先找U
					//第一个网格
					float tempUX1 = 0; float tempUY1 = 0;
					float tempUX2 = 0; float tempUY2 = 0;
					float tempUX3 = 0; float tempUY3 = 0;
					vector<float> tempUX4; vector<float> tempUY4;
					vector<float> tempUX5; vector<float> tempUY5;
					tempUX4.clear(); tempUY4.clear();
					tempUX5.clear(); tempUY5.clear();

					//上下两点之间
					if ((pointSet[m*modeN + n].TpU)*(pointSet[(m + 1)*modeN + n].TpU) < 0)
					{
						//计算零点坐标
						tempUX1 = pointSet[m*modeN + n].TpX;
						tempUY1 = pointSet[m*modeN + n].TpY +
							fabs((pointSet[m*modeN + n].TpU) / (pointSet[(m + 1)*modeN + n].TpU - pointSet[m*modeN + n].TpU));
						tempUX4.push_back(tempUX1);
						tempUY4.push_back(tempUY1);
					}

					//对角之间
					if ((pointSet[m*modeN + n].TpU)*(pointSet[(m + 1)*modeN + n + 1].TpU) < 0)
					{
						//计算零点坐标
						tempUX2 = pointSet[m*modeN + n].TpX +
							fabs((pointSet[m*modeN + n].TpU) / (pointSet[(m + 1)*modeN + n + 1].TpU - pointSet[m*modeN + n].TpU));
						tempUY2 = pointSet[m*modeN + n].TpY +
							fabs((pointSet[m*modeN + n].TpU) / (pointSet[(m + 1)*modeN + n + 1].TpU - pointSet[m*modeN + n].TpU));
						tempUX4.push_back(tempUX2);
						tempUY4.push_back(tempUY2);
					}

					//下方左右之间
					if ((pointSet[(m + 1)*modeN + n].TpU)*(pointSet[(m + 1)*modeN + n + 1].TpU) < 0)
					{
						//计算零点坐标
						tempUX3 = pointSet[(m + 1)*modeN + n].TpX +
							fabs((pointSet[(m + 1)*modeN + n].TpU) / (pointSet[(m + 1)*modeN + n + 1].TpU - pointSet[(m + 1)*modeN + n].TpU));
						tempUY3 = pointSet[(m + 1)*modeN + n].TpY;
						tempUX4.push_back(tempUX3);
						tempUY4.push_back(tempUY3);
					}

					//第二个网格
					//上侧左右两点之间
					if ((pointSet[m*modeN + n].TpU)*(pointSet[m*modeN + n + 1].TpU) < 0)
					{
						//计算零点坐标
						tempUX1 = pointSet[m*modeN + n].TpX +
							fabs((pointSet[m*modeN + n].TpU) / (pointSet[m*modeN + n + 1].TpU - pointSet[m*modeN + n].TpU));
						tempUY1 = pointSet[m*modeN + n].TpY;
						tempUX5.push_back(tempUX1);
						tempUY5.push_back(tempUY1);
					}
					//对角之间
					if ((pointSet[m*modeN + n].TpU)*(pointSet[(m + 1)*modeN + n + 1].TpU) < 0)
					{
						//计算零点坐标
						tempUX2 = pointSet[m*modeN + n].TpX +
							fabs((pointSet[m*modeN + n].TpU) / (pointSet[(m + 1)*modeN + n + 1].TpU - pointSet[m*modeN + n].TpU));
						tempUY2 = pointSet[m*modeN + n].TpY +
							fabs((pointSet[m*modeN + n].TpU) / (pointSet[(m + 1)*modeN + n + 1].TpU - pointSet[m*modeN + n].TpU));
						tempUX5.push_back(tempUX2);
						tempUY5.push_back(tempUY2);
					}
					//上下之间
					if ((pointSet[m*modeN + n + 1].TpU)*(pointSet[(m + 1)*modeN + n + 1].TpU) < 0)
					{
						//计算零点坐标
						tempUX3 = pointSet[m*modeN + n + 1].TpX;
						tempUY3 = pointSet[m*modeN + n + 1].TpY +
							fabs((pointSet[m*modeN + n + 1].TpU) / (pointSet[(m + 1)*modeN + n + 1].TpU - pointSet[m*modeN + n + 1].TpU));
						tempUX5.push_back(tempUX3);
						tempUY5.push_back(tempUY3);
					}

					//求取U的零线，一个网格画一条线段
					//先考虑两个点的，有U的0点的暂时不考虑
					//根据两点坐标求直线方程
					float xu1 = 0;
					float yu1 = 0;
					if (tempUX4.size() == 2)
					{
						//两点坐标A1(tempUX4[0],tempUY4[0]);B1(tempUX4[1],tempUY4[1])
						yu1 = tempUY4[0] +
							((tempUY4[1] - tempUY4[0]) / (tempUX4[1] - tempUX4[0]))*(xu1 - tempUX4[0]);

					}
					float xu2 = 0;
					float yu2 = 0;
					if (tempUX5.size() == 2)
					{
						//两点坐标C1(tempUX5[0],tempUY5[0]);D1(tempUX5[1],tempUY5[1])
						yu2 = tempUY5[0] +
							((tempUY5[1] - tempUY5[0]) / (tempUX5[1] - tempUX5[0]))*(xu2 - tempUX5[0]);
					}
					//U的两条线段方程求解结束

					//再找V
					//第一个网格
					float tempVX1 = 0; float tempVY1 = 0;
					float tempVX2 = 0; float tempVY2 = 0;
					float tempVX3 = 0; float tempVY3 = 0;
					vector<float> tempVX4; vector<float> tempVY4;
					vector<float> tempVX5; vector<float> tempVY5;
					tempVX4.clear(); tempVY4.clear();
					tempVX5.clear(); tempVY5.clear();

					if ((pointSet[m*modeN + n].TpV)*(pointSet[(m + 1)*modeN + n].TpV) < 0)//上下两点之间
					{
						//计算零点坐标
						tempVX1 = pointSet[m*modeN + n].TpX;
						tempVY1 = pointSet[m*modeN + n].TpY +
							fabs((pointSet[m*modeN + n].TpV) / (pointSet[(m + 1)*modeN + n].TpV - pointSet[m*modeN + n].TpV));
						tempVX4.push_back(tempVX1);
						tempVY4.push_back(tempVY1);
					}
					if ((pointSet[m*modeN + n].TpV)*(pointSet[(m + 1)*modeN + n + 1].TpV) < 0)//对角之间
					{
						//计算零点坐标
						tempVX2 = pointSet[m*modeN + n].TpX +
							fabs((pointSet[m*modeN + n].TpV) / (pointSet[(m + 1)*modeN + n + 1].TpV - pointSet[m*modeN + n].TpV));
						tempVY2 = pointSet[m*modeN + n].TpY +
							fabs((pointSet[m*modeN + n].TpV) / (pointSet[(m + 1)*modeN + n + 1].TpV - pointSet[m*modeN + n].TpV));
						tempVX4.push_back(tempVX2);
						tempVY4.push_back(tempVY2);
					}
					if ((pointSet[(m + 1)*modeN + n].TpV)*(pointSet[(m + 1)*modeN + n + 1].TpV) < 0)//下方左右之间
					{
						//计算零点坐标
						tempVX3 = pointSet[(m + 1)*modeN + n].TpX +
							fabs((pointSet[(m + 1)*modeN + n].TpV) / (pointSet[(m + 1)*modeN + n + 1].TpV - pointSet[(m + 1)*modeN + n].TpV));
						tempVY3 = pointSet[(m + 1)*modeN + n].TpY;
						tempVX4.push_back(tempVX3);
						tempVY4.push_back(tempVY3);
					}

					//第二个网格
					if ((pointSet[m*modeN + n].TpV)*(pointSet[m*modeN + n + 1].TpV) < 0)//上侧左右两点之间
					{
						//计算零点坐标
						tempVX1 = pointSet[m*modeN + n].TpX +
							fabs((pointSet[m*modeN + n].TpV) / (pointSet[m*modeN + n + 1].TpV - pointSet[m*modeN + n].TpV));
						tempVY1 = pointSet[m*modeN + n].TpY;
						tempVX5.push_back(tempVX1);
						tempVY5.push_back(tempVY1);
					}
					if ((pointSet[m*modeN + n].TpV)*(pointSet[(m + 1)*modeN + n + 1].TpV) < 0)//对角之间
					{
						//计算零点坐标
						tempVX2 = pointSet[m*modeN + n].TpX +
							fabs((pointSet[m*modeN + n].TpV) / (pointSet[(m + 1)*modeN + n + 1].TpV - pointSet[m*modeN + n].TpV));
						tempVY2 = pointSet[m*modeN + n].TpY +
							fabs((pointSet[m*modeN + n].TpV) / (pointSet[(m + 1)*modeN + n + 1].TpV - pointSet[m*modeN + n].TpV));
						tempVX5.push_back(tempVX2);
						tempVY5.push_back(tempVY2);
					}
					if ((pointSet[m*modeN + n + 1].TpV)*(pointSet[(m + 1)*modeN + n + 1].TpV) < 0)//上下之间
					{
						//计算零点坐标
						tempVX3 = pointSet[m*modeN + n + 1].TpX;
						tempVY3 = pointSet[m*modeN + n + 1].TpY +
							fabs((pointSet[m*modeN + n + 1].TpV) / (pointSet[(m + 1)*modeN + n + 1].TpV - pointSet[m*modeN + n + 1].TpV));
						tempVX5.push_back(tempVX3);
						tempVY5.push_back(tempVY3);
					}

					//求取V的零线，一个网格画一条线段
					//先考虑两个点的，有V的0点的暂时不考虑
					//根据两点坐标求直线方程
					float xv1 = 0;
					float yv1 = 0;
					if (tempVX4.size() == 2)
					{
						//两点坐标A2(tempVX4[0],tempVY4[0]);B2(tempVX4[1],tempVY4[1])
						yv1 = tempVY4[0] +
							((tempVY4[1] - tempVY4[0]) / (tempVX4[1] - tempVX4[0]))*(xv1 - tempVX4[0]);

					}
					float xv2 = 0;
					float yv2 = 0;
					if (tempVX5.size() == 2)
					{
						//两点坐标C2(tempVX5[0],tempVY5[0]);D2(tempVX5[1],tempVY5[1])
						yv2 = tempVY5[0] +
							((tempVY5[1] - tempVY5[0]) / (tempVX5[1] - tempVX5[0]))*(xv2 - tempVX5[0]);
					}
					//V的两条线段方程求解结束

					//判断U、V线段是否有交点
					//第一种情况
					if (tempUX4.size() == 2 && tempVX4.size() == 2)
					{
						float s1 = (tempUX4[0] - tempVX4[0])*(tempUY4[1] - tempVY4[0]) -
							(tempUY4[0] - tempVY4[0])*(tempUX4[1] - tempVX4[0]);
						float s2 = (tempUX4[0] - tempVX4[1])*(tempUY4[1] - tempVY4[1]) -
							(tempUY4[0] - tempVY4[1])*(tempUX4[1] - tempVX4[1]);
						//判断s1*s2的正负

						float s3 = (tempVX4[0] - tempUX4[0])*(tempVY4[1] - tempUY4[0]) -
							(tempVY4[0] - tempUY4[0])*(tempVX4[1] - tempUX4[0]);
						float s4 = s3 + s1 - s2;
						//判断s3*s4的正负

						if ((s1*s2 <= 0) && (s3*s4 <= 0))
						{
							//计算交点
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

					//相交第二种情况
					else if (tempUX4.size() == 2 && tempVX5.size() == 2)
					{
						float s1 = (tempUX4[0] - tempVX5[0])*(tempUY4[1] - tempVY5[0]) -
							(tempUY4[0] - tempVY5[0])*(tempUX4[1] - tempVX5[0]);
						float s2 = (tempUX4[0] - tempVX5[1])*(tempUY4[1] - tempVY5[1]) -
							(tempUY4[0] - tempVY5[1])*(tempUX4[1] - tempVX5[1]);
						//判断s1*s2的正负

						float s3 = (tempVX5[0] - tempUX4[0])*(tempVY5[1] - tempUY4[0]) -
							(tempVY5[0] - tempUY4[0])*(tempVX5[1] - tempUX4[0]);
						float s4 = s3 + s1 - s2;
						//判断s3*s4的正负

						if ((s1*s2 <= 0) && (s3*s4 <= 0))
						{
							//计算交点
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
					//相交第三种情况
					else if (tempUX5.size() == 2 && tempVX4.size() == 2)
					{
						float s1 = (tempUX5[0] - tempVX4[0])*(tempUY5[1] - tempVY4[0]) -
							(tempUY5[0] - tempVY4[0])*(tempUX5[1] - tempVX4[0]);
						float s2 = (tempUX5[0] - tempVX4[1])*(tempUY5[1] - tempVY4[1]) -
							(tempUY5[0] - tempVY4[1])*(tempUX5[1] - tempVX4[1]);
						//判断s1*s2的正负

						float s3 = (tempVX4[0] - tempUX5[0])*(tempVY4[1] - tempUY5[0]) -
							(tempVY4[0] - tempUY5[0])*(tempVX4[1] - tempUX5[0]);
						float s4 = s3 + s1 - s2;
						//判断s3*s4的正负

						if ((s1*s2 <= 0) && (s3*s4 <= 0))
						{
							//计算交点
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
					//相交第四种情况
					else if (tempUX5.size() == 2 && tempVX5.size() == 2)
					{
						float s1 = (tempUX5[0] - tempVX5[0])*(tempUY5[1] - tempVY5[0]) -
							(tempUY5[0] - tempVY5[0])*(tempUX5[1] - tempVX5[0]);
						float s2 = (tempUX5[0] - tempVX5[1])*(tempUY5[1] - tempVY5[1]) -
							(tempUY5[0] - tempVY5[1])*(tempUX5[1] - tempVX5[1]);
						//判断s1*s2的正负

						float s3 = (tempVX5[0] - tempUX5[0])*(tempVY5[1] - tempUY5[0]) -
							(tempVY5[0] - tempUY5[0])*(tempVX5[1] - tempUX5[0]);
						float s4 = s3 + s1 - s2;
						//判断s3*s4的正负

						if ((s1*s2 <= 0) && (s3*s4 <= 0))
						{
							//计算交点
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
			centerYY.push_back(ytemp); //存放找到的零点坐标
		}



		//6*6网格内是否存在零点判断完成,对零点进行选择，取离中心最近的点
		if (centerTempX.size() <= 0)
		{
			//continue;
			//对新矩阵进行插值
			int insertN = 40;
			//对矩阵的行数据进行插值
			SetPoint* pointSetRow = new SetPoint[insertN * (modeN - 1)*modeN];

			int nn = 0;
			for (int m = 0; m < modeN; m++)
			{
				int kk = 0;
				for (nn = 0; nn < (modeN - 1); nn++)
				{
					//先对坐标插值
					//X坐标
					float tempXYValX = (pointSet[m*modeN + nn + 1].TpX - pointSet[m*modeN + nn].TpX)*1.0 / insertN;
					//Y坐标
					float tempXYValY = (pointSet[m*modeN + nn + 1].TpY - pointSet[m*modeN + nn].TpY)*1.0 / insertN;
					//再对速度插值
					//速度U
					float tempXYValU = (pointSet[m*modeN + nn + 1].TpU - pointSet[m*modeN + nn].TpU)*1.0 / insertN;
					//速度V
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

			//对矩阵的列数据进行插值
			SetPoint* pointSetCol = new SetPoint[insertN * (modeN - 1)*insertN * (modeN - 1)];
			int mm = 0;
			for (int m = 0; m < (modeN - 1); m++)
			{
				for (int n = 0; n < insertN *(modeN - 1); n++)
				{
					//先对坐标插值
					//X坐标
					float tempXYValX = (pointSetRow[(m + 1)*insertN *(modeN - 1) + n].TpX - pointSetRow[m *insertN *(modeN - 1) + n].TpX)*1.0 / insertN;
					//Y坐标
					float tempXYValY = (pointSetRow[(m + 1)*insertN *(modeN - 1) + n].TpY - pointSetRow[m *insertN *(modeN - 1) + n].TpY)*1.0 / insertN;
					//速度U
					float tempXYValU = (pointSetRow[(m + 1)*insertN *(modeN - 1) + n].TpU - pointSetRow[m *insertN *(modeN - 1) + n].TpU)*1.0 / insertN;
					//速度V
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

			//插值完成，得到新的矩阵，在此范围内搜索速度零点
			float tempU = 0;
			float tempV = 0;
			float tempUV = 0;
			float tempXX = 0;
			float tempYY = 0;
			float tempValue = 100;//给定初始最小值
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
