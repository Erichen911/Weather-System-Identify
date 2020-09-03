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

	//��ȡ�������0-7
	void AngleCode(float* u, float* v, int &longitude_num, int &latitude_num, int* BinarySet, vector<float>& angleuv);
	//�ж���������������
	int jj(int *temp2, int datanum, int temp3);
	//�ж��Ƿ�Ϊ�������ĵ�
	int PointWind(int *temp, float &centerFU);
	//�ж�������������
	int typeJude(int *temp);

	//�ж���
	void Judge(vector<int>& bianrySet, int latitude_num, int longitude_num);

	//����
	void CLuster(vector<int>X, vector<int>Y, int typeN, int datanumN, int &longitude_num, int &latitude_num,
		vector<int>& wdX, vector<int>& wdY, vector <int>& wdXx, vector <int>& wdYy, vector<float> angleuv);
	//ɢ�������ж�
	void UVJudge(vector<float>& angleUV, vector <int> TempBianry, int latitude_num, int longitude_num, int n);

	//�������
	void Interpolation(vector<int>centerX, vector<int>centerY, int longitude_num, int latitude_num,
		float* u, float* v,
		vector<float>& centerXX, vector<float>& centerYY, int modeL);

};

#endif // HANDLE_H
