#pragma once
#include "Convolution.h"

class EdgeDetector
{
public:
	/*
	Hàm phát hiện biên cạnh của ảnh xám
	sourceImage: ảnh input
	destinationImage: ảnh output
	method: phương pháp detect
	kWidth, kHeight: kích thước kernel	
		1: Sobel
		2: Prewitt
		3: Laplace		
	Hàm trả về
		0: nếu detect thành công
		1: nếu detect thất bại (không đọc được ảnh input,...)
	*/
	int DetectEdge(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight, int method);	
	EdgeDetector();
	~EdgeDetector();
};

EdgeDetector::EdgeDetector()
{}

EdgeDetector::~EdgeDetector()
{}

int EdgeDetector::DetectEdge(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight, int method)
{
	uint8_t threshold = 50;
	if (method == 1)
	{
		// Sobel
	}
	else if (method == 2)
	{
		// Prewitt
		vector<float> Gx = { 1, 0, -1, 1, 0, -1, 1, 0, -1 };
		Convolution prewitt;
		prewitt.SetKernel(Gx, kWidth, kHeight);

		Mat horizontal, vertical;
		prewitt.DoConvolution(sourceImage, vertical);

		vector<float> Gy = { 1, 1, 1, 0, 0, 0, -1, -1, -1 };
		prewitt.SetKernel(Gy, kWidth, kHeight);
		prewitt.DoConvolution(sourceImage, horizontal);

		destinationImage = Mat(sourceImage.rows, sourceImage.cols, CV_8UC1);
		for (int r = 0; r < destinationImage.rows; r++)
		{
			for (int c = 0; c < destinationImage.cols; c++)
			{
				float X = vertical.at<uint8_t>(r, c);
				float Y = horizontal.at<uint8_t>(r, c);
				uint8_t mag = (uint8_t)sqrtf(X * X + Y * Y);
				if (mag > threshold)
					destinationImage.at<uint8_t>(r, c) = 255;
				else
					destinationImage.at< uint8_t>(r, c) = 0;
			}
		}
	}
	else if (method == 3)
	{
		vector<float> G = { 1,1,1,1,-8,1,1,1,1 };
	}

	return 0;
}

