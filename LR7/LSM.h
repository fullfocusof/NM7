#pragma once
#include <iostream>
#include <fstream>

#include <Windows.h>
#include <conio.h>

#include <cmath>

#include <vector>
#include <string>

using namespace std;

class LSM
{
	vector<vector<float>> data;

public:

	LSM();

	void manualInput();
	void fileInput();

	void deleteData();

	void printData();
	void printInterpData(vector<float>& dataInterp);
	void printQuit();

	void linearFunc();
	void squareFunc();
	void powFunc();
	void expFunc();
	void lnFunc();
	void fractLinFunc();
	void inverseProportFunc();
	void inverseProportLinFunc();

	float determinant_2x2(vector<vector<float>> mat);
	float determinant(vector<vector<float>> mat);
	float getDetMinor(vector<vector<float>> mat, int row, int col);
	vector<vector<float>> getInverseMatrix(vector<vector<float>> mat);
	vector<vector<float>> multiplyMat(vector<vector<float>>& mat1, vector<vector<float>>& mat2);

	~LSM();
};