#include "LSM.h"

LSM::LSM()
{

}

void LSM::manualInput()
{
	if (!data.empty())
	{
		cerr << "Данные уже введены";
	}
	else
	{
		data.resize(2);

		int N;
		cout << "Введите количество узлов" << endl;
		cin >> N;

		system("cls");
		cout << "Введите данные " << N << " узлов в формате <x y>" << endl;
		for (int i = 0; i < N; i++)
		{
			float x, y;
			cin >> x >> y;
			data[0].push_back(x);
			data[1].push_back(y);
		}

		system("cls");
		cout << "Данные успешно введены";
	}
}

void LSM::fileInput()
{
	if (!data.empty())
	{
		cerr << "Данные уже введены";
	}
	else
	{
		data.resize(2);

		ifstream ifs("dataInput.txt");
		if (!ifs.is_open())
		{
			cerr << "Ошибка открытия файла";
		}
		else
		{
			int N;
			ifs >> N;

			for (int i = 0; i < N; i++)
			{
				float x, y;
				ifs >> x >> y;
				data[0].push_back(x);
				data[1].push_back(y);
			}
		}

		system("cls");
		cout << "Данные успешно введены";
	}
}

void LSM::deleteData()
{
	if (data.empty())
	{
		cerr << "Данные отсутствуют";
	}
	else
	{
		data.clear();
		cout << "Данные удалены";
	}
}

void LSM::printData()
{
	if (data.empty())
	{
		cerr << "Данные отсутствуют";
	}
	else
	{
		int size = data[0].size();
		string table = "x";
		for (int i = 0; i < size; i++)
		{
			table += "\t" + to_string(data[0][i]);
		}
		table += "\ny";
		for (int i = 0; i < size; i++)
		{
			table += "\t" + to_string(data[1][i]);
		}
		cout << table;
	}
}

void LSM::printInterpData(vector<float>& dataInterp)
{
	int size = dataInterp.size();

	float sumSqDiff = 0;
	for (int i = 0; i < size; i++)
	{
		float diff = data[1][i] - dataInterp[i];
		sumSqDiff += pow(diff, 2);
	}

	string temp = "y*";
	for (int i = 0; i < size; i++)
	{
		temp += "\t" + to_string(dataInterp[i]);
	}

	cout << "Полученные значения" << endl;
	printData();
	cout << endl << temp << endl << endl;

	cout << "Сумма квадратов отклонений = " << sumSqDiff;
}

void LSM::printQuit()
{
	cout << endl << endl << "Backspace - возврат в меню";

	int answ = _getch();
	bool press = false;

	while (!press)
	{
		if (answ == 8)
		{
			press = true;
			break;
		}
		else
		{
			answ = _getch();
		}
	}

	system("cls");
}

void LSM::linearFunc()
{
	int size = data[0].size();
	float sumX = 0, sumY = 0, sumXY = 0, sumXSq = 0;
	for (int i = 0; i < size; i++)
	{
		sumX += data[0][i];
		sumY += data[1][i];
		sumXY += (data[0][i] * data[1][i]);
		sumXSq += (data[0][i] * data[0][i]);
	}

	float detMain = sumXSq * size - sumX * sumX;
	float detA = sumXY * size - sumX * sumY;
	float detB = sumXSq * sumY - sumXY * sumX;
	
	if (detMain == 0.0f)
	{
		cerr << "Определитель главной матрицы равен нулю";
		return;
	}

	float A = detA / detMain, B = detB / detMain;

	vector<float> dataInterp(size);
	for (int i = 0; i < size; i++)
	{
		dataInterp[i] = A * data[0][i] + B;		 
	}

	cout << "Полученное уравнение: y = " << A << "*x + " << B << endl << endl;

	printInterpData(dataInterp);
}

void LSM::squareFunc()
{
	int size = data[0].size();
	float sumX = 0, sumY = 0, sumXY = 0, sumXSq = 0, sumXCubed = 0, sumXQuadred = 0, sumXSqY = 0;
	for (int i = 0; i < size; i++)
	{
		sumX += data[0][i];
		sumY += data[1][i];
		sumXY += (data[0][i] * data[1][i]);
		sumXSq += (data[0][i] * data[0][i]);
		sumXCubed += (data[0][i] * data[0][i] * data[0][i]);
		sumXQuadred += (data[0][i] * data[0][i] * data[0][i] * data[0][i]);
		sumXSqY += (data[0][i] * data[0][i] * data[1][i]);
	}

	vector<vector<float>> matrix =
	{
		{(float)size, sumX, sumXSq},
		{sumX, sumXSq, sumXCubed},
		{sumXSq, sumXCubed, sumXQuadred}
	};
	vector<vector<float>> inverseMatrix = getInverseMatrix(matrix);

	vector<vector<float>> colY = 
	{
		{sumY},
		{sumXY},
		{sumXSqY}
	};

	vector<vector<float>> coefs = multiplyMat(inverseMatrix, colY);

	vector<float> dataInterp(size);
	for (int i = 0; i < size; i++)
	{
		dataInterp[i] = coefs[2][0] * data[0][i] * data[0][i] + coefs[1][0] * data[0][i] + coefs[0][0];
	}

	cout << "Полученное уравнение: y = " << coefs[2][0] << "*x^2 + " << coefs[1][0] << "*x + " << coefs[0][0] << endl << endl;

	printInterpData(dataInterp);
}

void LSM::powFunc() // ln y(x) = ln(x) + m*ln(x) y < 0 ????
{
	int size = data[0].size();
	float sumLnX = 0, sumLnY = 0, sumLnXY = 0, sumLnXSq = 0;
	for (int i = 0; i < size; i++)
	{
		sumLnX += log(data[0][i]);
		sumLnY += log(data[1][i]);
		sumLnXY += (log(data[0][i]) * log(data[1][i]));
		sumLnXSq += pow(log(data[0][i]), 2);
	}

	float detMain = sumLnXSq * size - sumLnX * sumLnX;
	float detA = sumLnXY * size - sumLnX * sumLnY;
	float detM = sumLnXSq * sumLnY - sumLnXY * sumLnX;

	if (detMain == 0.0f)
	{
		cerr << "Определитель главной матрицы равен нулю";
		return;
	}

	float lnA = detA / detMain, M = detM / detMain, A = pow(exp(1.0f), lnA);

	vector<float> dataInterp(size);
	for (int i = 0; i < size; i++)
	{
		dataInterp[i] = pow(A * log(data[0][i]), M);
	}

	cout << "Полученное уравнение: y = " << A << "*x^" << M << endl << endl;

	printInterpData(dataInterp);
}

void LSM::expFunc() // ln y(x) = ln(a) + mx y < 0 ???
{
	int size = data[0].size();
	float sumX = 0, sumLnY = 0, sumXLnY = 0, sumXSq = 0;
	for (int i = 0; i < size; i++)
	{
		sumX += data[0][i];
		sumLnY += log(data[1][i]);
		sumXLnY += (data[0][i] * log(data[1][i]));
		sumXSq += pow(data[0][i], 2);
	}

	float detMain = sumXSq * size - sumX * sumX;
	float detM = sumXLnY * size - sumX * sumLnY;
	float detA = sumXSq * sumLnY - sumXLnY * sumX;

	if (detMain == 0.0f)
	{
		cerr << "Определитель главной матрицы равен нулю";
		return;
	}

	float lnM = detM / detMain, A = detA / detMain, M = pow(exp(1.0f), lnM);

	vector<float> dataInterp(size);
	for (int i = 0; i < size; i++)
	{
		dataInterp[i] = A * pow(exp(1.0f), M * data[0][i]);
	}

	cout << "Полученное уравнение: y = " << A << "*e^(" << M << "*x)" << endl << endl;

	printInterpData(dataInterp);
}

void LSM::lnFunc()
{
	int size = data[0].size();
	float sumLnX = 0, sumY = 0, sumLnXY = 0, sumLnXSq = 0;
	for (int i = 0; i < size; i++)
	{
		sumLnX += log(data[0][i]);
		sumY += data[1][i];
		sumLnXY += (log(data[0][i]) * data[1][i]);
		sumLnXSq += pow(log(data[0][i]), 2);
	}

	float detMain = sumLnXSq * size - sumLnX * sumLnX;
	float detA = sumLnXY * size - sumLnX * sumY;
	float detB = sumLnXSq * sumY - sumLnXY * sumLnX;

	if (detMain == 0.0f)
	{
		cerr << "Определитель главной матрицы равен нулю";
		return;
	}

	float A = detA / detMain, B = detB / detMain;

	vector<float> dataInterp(size);
	for (int i = 0; i < size; i++)
	{
		dataInterp[i] = A * log(data[0][i]) + B;
	}

	cout << "Полученное уравнение: y = " << A << "*ln(x) + " << B << endl << endl;

	printInterpData(dataInterp);
}

void LSM::fractLinFunc()
{
	int size = data[0].size();
	float sumX = 0, sumY = 0, sumXY = 0, sumXSq = 0;
	for (int i = 0; i < size; i++)
	{
		sumX += data[0][i];
		sumY += (1/data[1][i]);
		sumXY += (data[0][i] * (1/data[1][i]));
		sumXSq += (data[0][i] * data[0][i]);
	}

	float detMain = sumXSq * size - sumX * sumX;
	float detA = sumXY * size - sumX * sumY;
	float detB = sumXSq * sumY - sumXY * sumX;

	if (detMain == 0.0f)
	{
		cerr << "Определитель главной матрицы равен нулю";
		return;
	}

	float A = detA / detMain, B = detB / detMain;

	vector<float> dataInterp(size);
	for (int i = 0; i < size; i++)
	{
		dataInterp[i] = 1 / (A * data[0][i] + B);
	}

	cout << "Полученное уравнение: y = 1/(" << A << "x + " << B << ")" << endl << endl;

	printInterpData(dataInterp);
}

void LSM::inverseProportFunc()
{
	int size = data[0].size();
	float sumX = 0, sumY = 0, sumXY = 0, sumXSq = 0;
	for (int i = 0; i < size; i++)
	{
		sumX += (1/data[0][i]);
		sumY += data[1][i];
		sumXY += ((1/data[0][i]) * data[1][i]);
		sumXSq += ((1/data[0][i]) * (1/data[0][i]));
	}

	float detMain = sumXSq * size - sumX * sumX;
	float detA = sumXY * size - sumX * sumY;
	float detB = sumXSq * sumY - sumXY * sumX;

	if (detMain == 0.0f)
	{
		cerr << "Определитель главной матрицы равен нулю";
		return;
	}

	float A = detA / detMain, B = detB / detMain;

	vector<float> dataInterp(size);
	for (int i = 0; i < size; i++)
	{
		dataInterp[i] = A * (1/data[0][i]) + B;
	}

	cout << "Полученное уравнение: y = " << A << "/x + " << B << endl << endl;

	printInterpData(dataInterp);
}

void LSM::inverseProportLinFunc()
{
	int size = data[0].size();
	float sumX = 0, sumY = 0, sumXY = 0, sumXSq = 0;
	for (int i = 0; i < size; i++)
	{
		sumX += (1 / data[0][i]);
		sumY += (1 / data[1][i]);
		sumXY += ((1 / data[0][i]) * (1 / data[1][i]));
		sumXSq += ((1 / data[0][i]) * (1 / data[0][i]));
	}

	float detMain = sumXSq * size - sumX * sumX;
	float detA = sumXY * size - sumX * sumY;
	float detB = sumXSq * sumY - sumXY * sumX;

	if (detMain == 0.0f)
	{
		cerr << "Определитель главной матрицы равен нулю";
		return;
	}

	float A = detA / detMain, B = detB / detMain;

	vector<float> dataInterp(size);
	for (int i = 0; i < size; i++)
	{
		dataInterp[i] = 1/(A + (B/data[0][i]));
	}

	cout << "Полученное уравнение: y = x/(" << A << "*x + " << B << ")" << endl << endl;

	printInterpData(dataInterp);
}

float LSM::determinant_2x2(vector<vector<float>> mat)
{
	return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
}

float LSM::determinant(vector<vector<float>> mat)
{
	if (mat.empty())
	{
		throw exception();
	}

	int n = mat.size();

	if (n == 1)
	{
		return mat[0][0];
	}

	if (n == 2)
	{
		return determinant_2x2(mat);
	}

	float result = 0;
	for (int i = 0; i < n; i++)
	{
		vector<vector<float>> submat;
		for (int j = 1; j < n; j++)
		{
			vector<float> row;
			for (int k = 0; k < n; k++)
			{
				if (k != i)
				{
					row.push_back(mat[j][k]);
				}
			}
			if (!row.empty())
			{
				submat.push_back(row);
			}
		}
		int sign = (i % 2 == 0) ? 1 : -1;
		result += sign * mat[0][i] * determinant(submat);
	}

	return result;
}

float LSM::getDetMinor(vector<vector<float>> mat, int row, int col)
{
	int n = mat.size();
	vector<vector<float>> minor(n - 1, vector<float>(n - 1));

	int minor_row = 0, minor_col = 0;

	for (int i = 0; i < n; i++)
	{
		if (i != row)
		{
			minor_col = 0;
			for (int j = 0; j < n; j++)
			{
				if (j != col)
				{
					minor[minor_row][minor_col] = mat[i][j];
					minor_col++;
				}
			}
			minor_row++;
		}
	}

	return determinant(minor);
}

vector<vector<float>> LSM::getInverseMatrix(vector<vector<float>> mat)
{
	float det = determinant(mat);

	if (det == 0)
	{
		throw exception();
	}
	else
	{
		int n = mat.size();
		vector<vector<float>> result(n, vector<float>(n));

		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				int sign = pow(-1, i + j);
				result[j][i] = sign * getDetMinor(mat, i, j) / det;
			}
		}

		return result;
	}
}

vector<vector<float>> LSM::multiplyMat(vector<vector<float>>& mat1, vector<vector<float>>& mat2)
{
	int row1 = mat1.size();
	int row2 = mat2.size();
	int col1 = mat1[0].size();
	int col2 = mat2[0].size();

	if (col1 != row2)
	{
		cerr << "Невозможно перемножить матрицы";
		return vector<vector<float>>();
	}

	vector<vector<float>> result(row1, vector<float>(col2));

	for (int i = 0; i < row1; i++)
	{
		for (int j = 0; j < col2; j++)
		{
			for (int k = 0; k < col1; k++)
			{
				result[i][j] += mat1[i][k] * mat2[k][j];
			}
		}
	}

	return result;
}

LSM::~LSM()
{
	data.clear();
	data.~vector();
}