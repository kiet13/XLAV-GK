#pragma once
#include "Convolution.h"
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
int partition(vector<int>& values, int left, int right) {
	int pivotIndex = left + (right - left) / 2;
	int pivotValue = values[pivotIndex];
	int i = left, j = right;
	int temp;
	while (i <= j) {
		while (values[i] < pivotValue) {
			i++;
		}
		while (values[j] > pivotValue) {
			j--;
		}
		if (i <= j) {
			temp = values[i];
			values[i] = values[j];
			values[j] = temp;
			i++;
			j--;
		}
	}
	return i;
}
void quicksort(vector<int>& values, int left, int right) {
	if (left < right) {
		int pivotIndex = partition(values, left, right);
		quicksort(values, left, pivotIndex - 1);
		quicksort(values, pivotIndex, right);
	}
}

int Blur::BlurImage(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight, int method)
{
	// Lọc trung bình
	if (method == 0)
	{
		//Ktra ảnh input
		if (sourceImage.empty())
			return 1;
		// Kiểm tra ảnh input phải là ảnh xám ko?
		assert(sourceImage.type() != CV_8UC1);
		// Kiểm tra kWidth và kHeight có lẻ không
		if ((kWidth % 2 == 0) || (kHeight % 2 == 0))
			return 1;
		// Kiểm tra KWidth và kHeight > 1
		if ((kWidth < 1) || (kHeight < 1))
			return 1;

		Mat img_temp;

		//Thêm cột, dòng trống vào để tính biên
		int paddingWidth = int((kWidth - 1) / 2);
		int paddingHeight = int((kHeight - 1) / 2);
		copyMakeBorder(sourceImage, img_temp, paddingHeight, paddingHeight, paddingWidth, paddingWidth, BORDER_CONSTANT, 0);

		int mean = 0.0;
		int final = 0;
		int nrows, ncols;

		nrows = sourceImage.size().height;
		ncols = sourceImage.size().width;

		//Sửa ảnh kết quả
		destinationImage = Mat(nrows, ncols, CV_8UC1);

		//Duyệt qua từng pixel ảnh
		for (unsigned int j = paddingWidth; j < nrows + paddingWidth; j++) {
			for (unsigned int z = paddingHeight; z < ncols + paddingHeight; z++) {
				mean = 0.0;
				//Duyệt qua vùng kernel
				for (int x = -paddingWidth; x <= paddingWidth; x++) {
					for (int y = -paddingHeight; y <= paddingHeight; y++) {
						mean += img_temp.at<uchar>(j + x, z + y);
					}
				}

				mean = mean / (kWidth * kHeight);
				final = round(mean);

				//Trả kết quả
				destinationImage.at<uchar>(j - paddingWidth, z - paddingHeight) = (uchar)final;
			}
		}

		if (destinationImage.empty())
			return 1;

		return 0;
	}
	//Lọc trung vị
	if (method == 1)
	{
		//Ktra ảnh input
		if (sourceImage.empty())
			return 1;
		// Kiểm tra ảnh input phải là ảnh xám ko?
		assert(sourceImage.type() != CV_8UC1);
		// Kiểm tra kWidth và kHeight có lẻ không
		if ((kWidth % 2 == 0) || (kHeight % 2 == 0))
			return 1;
		// Kiểm tra KWidth và kHeight > 1
		if ((kWidth < 1) || (kHeight < 1))
			return 1;

		Mat img_temp;

		//Thêm cột, dòng trống vào để tính biên
		int paddingWidth = int((kWidth - 1) / 2);
		int paddingHeight = int((kHeight - 1) / 2);
		copyMakeBorder(sourceImage, img_temp, paddingHeight, paddingHeight, paddingWidth, paddingWidth, BORDER_CONSTANT, 0);

		//Tạo kernel
		int size = kWidth * kHeight;
		vector<int> kernel;
		for (int i = 0; i < size; i++)
		{
			kernel.push_back(0);
		}
		int k;
		int final = int(size / 2 + 1);
		int nrows, ncols;

		nrows = sourceImage.size().height;
		ncols = sourceImage.size().width;

		//Sửa ảnh kết quả
		destinationImage = Mat(nrows, ncols, CV_8UC1);

		//Duyệt qua từng pixel ảnh
		for (unsigned int j = paddingWidth; j < nrows + paddingWidth; j++) {
			for (unsigned int z = paddingHeight; z < ncols + paddingHeight; z++) {
				k = 0;
				//Duyệt qua vùng kernel
				for (int x = -paddingWidth; x <= paddingWidth; x++) {
					for (int y = -paddingHeight; y <= paddingHeight; y++) {
						kernel[k] = img_temp.at<uchar>(j + x, z + y);
						k++;
					}
				}
				quicksort(kernel, 0, size - 1);
				//Trả kết quả
				destinationImage.at<uchar>(j - paddingWidth, z - paddingHeight) = (uchar)kernel[final];
			}
		}

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
