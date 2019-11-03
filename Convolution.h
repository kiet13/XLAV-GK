#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <algorithm>
using namespace std;
using namespace cv;

class Convolution
{
	vector<float> _kernel;// lưu giá trị của ma trận kernel dùng để tích chập
	int _kernelWidth;//chiều rộng kernel
	int _kernelHeight;//chiều dài kernel
public:
	//trả về kernel 
	vector<float> GetKernel();
	//set kernel, chiều rộng, chiều dài kernel phải là số lẻ
	void SetKernel(vector<float> kernel, int kWidth, int kHeight);

	/*
	Hàm tính convolution của 1 ảnh xám với kernel được xác định trước
	sourceImage: ảnh input
	destinationImage: ảnh output
	Hàm trả về
	0: nếu tính thành công
	1: nếu tính thất bại (không đọc được ảnh input,...)
	*/
	int DoConvolution(const Mat& sourceImage, Mat& destinationImage);	
	Convolution();
	~Convolution();
};

Convolution::Convolution()
{
	_kernelHeight = _kernelWidth = 0;
}

Convolution::~Convolution()
{

}

void Convolution::SetKernel(vector<float> kernel, int kWidth, int kHeight)
{
	if (kWidth % 2 != 0 && kHeight % 2 != 0)
	{
		_kernel = kernel;
		_kernelWidth = kWidth;
		_kernelHeight = kHeight;
	}
}

vector<int> getIndices(int oddNumber)
{
	int rbound = oddNumber / 2;
	vector<int> indices;
	for (int i = -rbound; i <= rbound; i++)
		indices.push_back(i);
	return indices;
}

int Convolution::DoConvolution(const Mat& sourceImage, Mat& destinationImage)
{
	destinationImage = sourceImage.clone();
	vector<int> cIndices = getIndices(_kernelWidth); // {...,-k,...-2,-1,0,1,2,...,k,...}
	vector<int> rIndices = getIndices(_kernelHeight);
	for (int r = 0; r < destinationImage.rows; r++)
	{
		for (int c = 0; c < destinationImage.cols; c++)
		{
			int gx = 0;
			for (int i = 0; i < _kernel.size(); i++)
			{
				int kc = cIndices[i / _kernelWidth]; // index in the column of kernel
				int kr = rIndices[i % _kernelHeight]; // index in the row of kernel

				int sc = c - kc; // index in the column of source image
				int sr = r - kr; // index in the row of source image

				// Ignore image points out of the boundary
				if (sr < 0 || sr >= sourceImage.rows)
					continue;

				if (sc < 0 || sc >= sourceImage.cols)
					continue;
			
				gx += (int)_kernel[i] * sourceImage.at<uint8_t>(sr, sc);
			}
			// Update destination image
			gx = gx > 255 ? 255 : gx;
			gx = gx < 0 ? 0 : gx;
			destinationImage.at<uint8_t>(r, c) = gx;
		}
	}
	return 0;
}

