#ifndef HANDLE_H
#define HANDLE_H
#include <iostream>
#include <fstream>
#include<vector>
using namespace std;


class handle
{
public:
	handle();
	~handle();

	//获取风向编码0-7
	void AngleCode(float* u, float* v, int &longitude_num, int &latitude_num, int* BinarySet, vector<float>& angleuv);
	//判断区域特征风向码
	int jj(int *temp2, int datanum, int temp3);
	//判断是否为特征中心点
	int PointWind(int *temp, float &centerFU);
	//判断是哪类特征风
	int typeJude(int *temp);

	//判定点
	void Judge(vector<int>& bianrySet, int latitude_num, int longitude_num);

	//聚类
	void CLuster(vector<int>X, vector<int>Y, int typeN, int datanumN, int &longitude_num, int &latitude_num,
		vector<int>& wdX, vector<int>& wdY, vector <int>& wdXx, vector <int>& wdYy, vector<float> angleuv);
	//散乱区域判断
	void UVJudge(vector<float>& angleUV, vector <int> TempBianry, int latitude_num, int longitude_num, int n);

	//计算零点
	void Interpolation(vector<int>centerX, vector<int>centerY, int longitude_num, int latitude_num,
		float* u, float* v,
		vector<float>& centerXX, vector<float>& centerYY, int modeL);

};

#endif // HANDLE_H
