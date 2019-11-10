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
	// Lọc trung bình
	if (method == 0)
	{
		//Ktra ảnh input
		if (sourceImage.empty())
			return 1;
		if (kWidth % 2 != 0 && kHeight % 2 != 0)
		{
			double size = (double)kWidth * (double)kHeight;
			vector<float> kernel;


			// Tạo Kernel
			float h = 1 / size;
			for (int i = 0; i < size; i++)
			{
				kernel.push_back(h);
			}

			//Set Kernel và lọc ảnh
			Convolution x;
			x.SetKernel(kernel, kWidth, kHeight);
			x.DoConvolution(sourceImage, destinationImage);
		}
		else return 1;


		if (destinationImage.empty())
			return 1;

		return 0;
	}
	// lam mo bang toan tu Gaussian
	if (method == 2)
	{
		double size = (double)kWidth * (double)kHeight;
		vector<float> kernel;
		float sigma = 1.03;

		// tao kernel
		for (int i = 0; i < size; i++)
		{
			int row = i / kWidth - kWidth / 2;  // row = ..., -2, -1, 0, 1, 2, ...
			int col = i % kHeight - kHeight / 2;
			float h = exp(-(float)(col * col + row * row) / (float)(2 * sigma * sigma)) / ((2 * 3.1415) * sigma * sigma);
			kernel.push_back(h);
		}

		vector<int> cIndices = getIndices(kWidth); // {...,-k,...-2,-1,0,1,2,...,k,...}
		vector<int> rIndices = getIndices(kHeight);
		for (int r = 0; r < destinationImage.rows; r++)
		{
			for (int c = 0; c < destinationImage.cols; c++)
			{
				float gx = 0;
				for (int i = 0; i < size; i++)
				{
					int kc = cIndices[i / kWidth]; // index in the column of kernel
					int kr = rIndices[i % kHeight]; // index in the row of kernel

					int sc = c - kc; // index in the column of source image
					int sr = r - kr; // index in the row of source image

					// Ignore image points out of the boundary
					if (sr < 0 || sr >= sourceImage.rows)
						continue;

					if (sc < 0 || sc >= sourceImage.cols)
						continue;

					gx += kernel[i] * sourceImage.at<uint8_t>(sr, sc);
				}
				// Update destination image
				gx = gx > 255 ? 255 : gx;
				gx = gx < 0 ? 0 : gx;
				destinationImage.at<uint8_t>(r, c) = gx;
			}
		}

		if (sourceImage.empty() || destinationImage.empty())
			return 0;
		else
			return 1;
	}
}
