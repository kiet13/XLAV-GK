#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;
using namespace cv;
class Converter
{

public:
	Converter();
	~Converter();
	/*
	Hàm chuyển đổi không gian màu của ảnh từ RGB sang GrayScale
	sourceImage: ảnh input
	destinationImage: ảnh output
	Hàm trả về
	0: nếu chuyển thành công
	1: nếu chuyển thất bại (không đọc được ảnh input,...)
	*/
	int RGB2GrayScale(const Mat& sourceImage, Mat& destinationImage);

	/*
	Hàm chuyển đổi không gian màu của ảnh từ GrayScale sang RGB
	sourceImage: ảnh input
	destinationImage: ảnh output
	Hàm trả về
	0: nếu chuyển thành công
	1: nếu chuyển thất bại (không đọc được ảnh input,...)
	*/
	int GrayScale2RGB(const Mat& sourceImage, Mat& destinationImage);


	/*
	Hàm chuyển đổi không gian màu của ảnh từ RGB sang HSV
	sourceImage: ảnh input
	destinationImage: ảnh output
	Hàm trả về
	0: nếu chuyển thành công
	1: nếu chuyển thất bại (không đọc được ảnh input,...)
	*/
	int RGB2HSV(const Mat& sourceImage, Mat& destinationImage);



	/*
	Hàm chuyển đổi không gian màu của ảnh từ HSV sang RGB
	sourceImage: ảnh input
	destinationImage: ảnh output
	Hàm trả về
	0: nếu chuyển thành công
	1: nếu chuyển thất bại (không đọc được ảnh input,...)
	*/
	int HSV2RGB(const Mat& sourceImage, Mat& destinationImage);




	/*
	Hàm chuyển đổi không gian màu của ảnh
	sourceImage: ảnh input
	destinationImage: ảnh output cùng kích thước, cùng loại với ảnh input
	type: loại chuyển đổi
		0,1: chuyển từ RGB sang GrayScale và ngược lại
		2,3: chuyển từ RGB sang HSV và ngược lại
	Hàm trả về
		0: nếu chuyển thành công
		1: nếu chuyển thất bại (không đọc được ảnh input hay type không chính xác,...)
	*/
	int Convert(Mat& sourceImage, Mat& destinationImage, int type);
	int Convert(IplImage* sourceImage, IplImage* destinationImage, int type);

};

Converter::Converter()
{}

Converter::~Converter()
{}


int Converter::RGB2GrayScale(const Mat& sourceImage, Mat& destinationImage)
{
	//Kiểm tra ảnh input có rỗng ko?
	if (sourceImage.empty())
		return 1;

	//Kiểm tra ảnh input phải là ảnh RGB ko?
	CV_Assert(sourceImage.type() == CV_8UC3);

	//Tạo ảnh KQ
	destinationImage.create(sourceImage.size(), CV_8UC1);

	//Xử lý từng pixel
	for (int j = 0; j < sourceImage.rows; j++)
	{
		const uchar* src_ptr = sourceImage.ptr<uchar>(j);
		uchar* dst_ptr = destinationImage.ptr<uchar>(j);

		//Gray = 0.082*Blue + 0.6095*Green + 0.3086*Red 
		for (int i = 0; i < sourceImage.cols; i++)
		{
			dst_ptr[i] = (uchar)(src_ptr[0] * 0.0820f + src_ptr[1] * 0.6094f + src_ptr[2] * 0.3086f);
			src_ptr += 3;
		}

	}

	return 0;
}

inline int Converter::GrayScale2RGB(const Mat& sourceImage, Mat& destinationImage)
{
	return 0;
}

int Converter::RGB2HSV(const Mat& sourceImage, Mat& destinationImage)
{
	if (sourceImage.channels() != 3)
		return 1;

	for (int r = 0; r < sourceImage.rows; r++)
	{
		for (int c = 0; c < sourceImage.cols; c++)
		{
			uint8_t blue = sourceImage.at<Vec3b>(r, c)[0];
			uint8_t green = sourceImage.at<Vec3b>(r, c)[1];
			uint8_t red = sourceImage.at<Vec3b>(r, c)[2];

			uint8_t V = max(max(blue, green), red);
			float S = 0;
			if (V != 0)
				S = (float)(V - min(min(blue, green), red)) / V;
			int H;
			if (V == red)
				H = 60 * (green - blue) / (V - min(min(blue, green), red));
			else if (V == green)
				H = 120 + 60 * (blue - red) / (V - min(min(blue, green), red));
			else if (V == blue)
				H = 240 + 60 * (red - green) / (V - min(min(blue, green), red));

			if (H < 0)
				H += 360;

			S *= 255;
			H /= 2;
			destinationImage.at<Vec3b>(r, c)[0] = H;
			destinationImage.at<Vec3b>(r, c)[1] = (uint8_t)S;
			destinationImage.at<Vec3b>(r, c)[2] = V;
		}
	}
	return 0;
}

inline int Converter::HSV2RGB(const Mat& sourceImage, Mat& destinationImage)
{
	return 0;
}

inline int Converter::Convert(Mat& sourceImage, Mat& destinationImage, int type)
{
	switch (type) {
	case 0:
		return RGB2GrayScale(sourceImage, destinationImage);
	case 1:
		return GrayScale2RGB(sourceImage, destinationImage);
	case 2:
		return RGB2HSV(sourceImage, destinationImage);
	case 3:
		return HSV2RGB(sourceImage, destinationImage);
	default:
		return 1;
	}
	return 0;
}

