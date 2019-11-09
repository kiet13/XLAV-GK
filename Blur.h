#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <algorithm>
using namespace std;
using namespace cv;

class Blur
{
public:
	/*
	Hàm làm trơn ảnh xám
	sourceImage: ảnh input
	destinationImage: ảnh output
	method: phương pháp detect
	kWidth, kHeight: kích thước kernel
	0: Trung bình
	1: Trung vị
	2: Gaussian	
	Hàm trả về
	0: nếu detect thành công
	1: nếu detect thất bại (không đọc được ảnh input,...)
	*/
	int BlurImage(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight, int method);	
	Blur();
	~Blur();
};

Blur::Blur()
{}

Blur::~Blur()
{}

int Blur::BlurImage(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight, int method)
{
	// lam mo bang toan tu Gaussian
	if (method == 2)
	{
		double size = (double)kWidth * (double)kHeight;
		vector<float> kernel;
		float sigma = 1;

		// tao kernel
		for (int i = 0; i < size; i++)
		{
			int row = i / kWidth - kWidth / 2;
			int col = i % kHeight - kHeight / 2;
			float h = exp(-(float)(col * col + row * row) / (float)(2 * sigma * sigma)) / (sqrt(2 * 3.1415) * sigma);
			kernel.push_back(h);
		}

		// thuc hien tich chap
		Convolution gauss;
		gauss.SetKernel(kernel, kWidth, kHeight);

		gauss.DoConvolution(sourceImage, destinationImage);

		if (sourceImage.empty() || destinationImage.empty())
			return 0;
		else
			return 1;
	}
}
