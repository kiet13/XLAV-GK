#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <algorithm>
using namespace std;
using namespace cv;
/*
 Lớp base dùng để nội suy màu của 1 pixel
*/
class PixelInterpolate
{
public:
	/*
	Hàm tính giá trị màu của ảnh kết quả từ nội suy màu trong ảnh gốc và gán màu được nội suy trong ảnh kết quả
	Tham số
		- (tx,ty): tọa độ thực của ảnh gốc sau khi thực hiện phép biến đổi affine
		- pSrc: con trỏ ảnh gốc
		- srcWidthStep: widthstep của ảnh gốc
		- nChannels: số kênh màu của ảnh gốc
		- pDstRow: con trỏ của ảnh kết quả đến pixel đang muốn nội suy màu
	
	*/
	virtual void Interpolate(
		float tx, float ty, 
		uchar* pSrc, int srcWidthStep, int nChannels,
		uchar* pDstRow) = 0;
	PixelInterpolate();
	~PixelInterpolate();
};

PixelInterpolate::PixelInterpolate()
{}

PixelInterpolate::~PixelInterpolate()
{}

/*
Lớp nội suy màu theo phương pháp song tuyến tính
*/
class BilinearInterpolate : public PixelInterpolate
{
public:
	void Interpolate(
		float tx, float ty, 
		uchar* pSrc, int srcWidthStep, int nChannels,
		uchar* pDstRow);
	BilinearInterpolate();
	~BilinearInterpolate();
};

BilinearInterpolate::BilinearInterpolate()
{}

BilinearInterpolate::~BilinearInterpolate()
{}


/*
	Hàm tính giá trị màu của ảnh kết quả từ nội suy màu trong ảnh gốc và gán màu được nội suy trong ảnh kết quả
	Tham số
		- (tx,ty): tọa độ thực của ảnh gốc sau khi thực hiện phép biến đổi affine
		- pSrc: con trỏ ảnh gốc
		- srcWidthStep: widthstep của ảnh gốc
		- nChannels: số kênh màu của ảnh gốc
		- pDstRow: con trỏ của ảnh kết quả đến pixel đang muốn nội suy màu

	*/
void BilinearInterpolate::Interpolate(float tx, float ty, uchar* pSrc,
	int srcWidthStep, int nChannels, uchar* pDstRow)
{
	int l = (int)floor(tx);
	int k = (int)floor(ty);
	float a = tx - l;
	float b = ty - k;
	

	l *= nChannels;

	pSrc += (k*srcWidthStep);

	for (int i = 0; i < nChannels; i++)
	{
		*(pDstRow + i) = (uchar)((1 - a)*(1 - b)*pSrc[l + i] + a * (1 - b)*(pSrc + srcWidthStep)[l + i] +
			b * (1 - a)*pSrc[l + nChannels + i] + a * b*(pSrc + srcWidthStep)[l + nChannels + i]);
	}

}

/*
Lớp nội suy màu theo phương pháp láng giềng gần nhất
*/
class NearestNeighborInterpolate : public PixelInterpolate
{
public:
	void Interpolate(
		float tx, float ty, 
		uchar* pSrc, int srcWidthStep, int nChannels,
		uchar* pDstRow);
	NearestNeighborInterpolate();
	~NearestNeighborInterpolate();
};

NearestNeighborInterpolate::NearestNeighborInterpolate()
{}

NearestNeighborInterpolate::~NearestNeighborInterpolate()
{}

void NearestNeighborInterpolate::Interpolate(
	float tx, float ty,
	uchar* pSrc, int srcWidthStep, int nChannels,
	uchar* pDstRow)
{

}


/*
Lớp biểu diễn pháp biến đổi affine
*/
class AffineTransform
{
	Mat _matrixTransform;//ma trận 3x3 hiện hành biểu diễn phép biến đổi affine
public:
	// xây dựng matrix transform cho phép tịnh tiến theo vector (dx,dy) sau đó nhân với ma trận hiện hành
	void Translate(float dx, float dy);
	//xây dựng matrix transform cho phép xoay 1 góc angle quanh gốc tọa độ sau đó nhân với ma trận hiện hành
	void Rotate(float angle);
	//xây dựng matrix transform cho phép tỉ lệ theo hệ số sau đó nhân với ma trận hiện hành
	void Scale(float sx, float sy);		
	//transform 1 điểm (x,y) theo matrix transform hiện hành đã có
	void TransformPoint(float &x, float &y);
	// Xây dựng 1 matrix tranform cho phép xoay quanh trục Ox hay Oy sau đó nhân với ma trận hiện hành
	void Flip(int Ox, int Oy);
	// hàm nghịch đảo ma trận hiện hành
	void Inverse();
	
	AffineTransform();
	~AffineTransform();
};

AffineTransform::AffineTransform()
{
	_matrixTransform = Mat(3, 3, CV_32FC1, Scalar(0));
}

AffineTransform::~AffineTransform()
{}

void AffineTransform::Translate(float dx, float dy)
{
	_matrixTransform.at<float>(0, 0) = 1;
	_matrixTransform.at<float>(1, 1) = 1;
	_matrixTransform.at<float>(2, 2) = 1;
	_matrixTransform.at<float>(0, 2) = dx;
	_matrixTransform.at<float>(1, 2) = dy;
}

void AffineTransform::Rotate(float angle)
{
	_matrixTransform.at<float>(0, 0) = cosf(angle);
	_matrixTransform.at<float>(0, 1) = -sinf(angle);
	_matrixTransform.at<float>(1, 0) = sinf(angle);
	_matrixTransform.at<float>(1, 1) = cosf(angle);
	_matrixTransform.at<float>(2, 2) = 1;
}

void AffineTransform::Scale(float sx, float sy)
{
	_matrixTransform.at<float>(0, 0) = sx;
	_matrixTransform.at<float>(1, 1) = sy;
	_matrixTransform.at<float>(2, 2) = 1;
}

void AffineTransform::TransformPoint(float &x, float &y)
{
	x = (_matrixTransform.at<float>(0, 0) * x) +
		(_matrixTransform.at<float>(0, 1) * y) +
		_matrixTransform.at<float>(0, 2);

	y = (_matrixTransform.at<float>(1, 0) * x) +
		(_matrixTransform.at<float>(1, 1) * y) +
		_matrixTransform.at<float>(1, 2);
}

void AffineTransform::Inverse()
{
	// Nếu là phép tịnh tiến thì đổi dấu giá trị tx, ty
	float tx = _matrixTransform.at<float>(0, 2);
	float ty = _matrixTransform.at<float>(1, 2);
	if (tx != 0 && ty != 0)
	{
		_matrixTransform.at<float>(0, 2) = -tx;
		_matrixTransform.at<float>(1, 2) = -ty;
	}
	else // Nếu là các phép biến đổi còn lại
	{
		// Thực hiện tìm ma trận khả nghịch 2x2 của ma trận con dòng 0, 1 và cột 0, 1
		float a = _matrixTransform.at<float>(0, 0);
		float b = _matrixTransform.at<float>(0, 1);
		float c = _matrixTransform.at<float>(1, 0);
		float d = _matrixTransform.at<float>(1, 1);

		float M = a * d - b * c;

		_matrixTransform.at<float>(0, 0) = d / M;
		_matrixTransform.at<float>(0, 1) = -b / M;
		_matrixTransform.at<float>(1, 0) = -c / M;
		_matrixTransform.at<float>(1, 1) = a / M;

	}
}
void AffineTransform::Flip(int Ox, int Oy)
{
	// Nếu quay quanh trục Ox thì Ox =1 và ngược lại
	if (Ox == 1 && Oy == 0)
	{
		_matrixTransform.at<float>(0, 0) = 1;
		_matrixTransform.at<float>(2, 2) = -1;
	}
	if (Oy == 1 && Ox == 0)
	{
		_matrixTransform.at<float>(0, 0) = -1;
		_matrixTransform.at<float>(2, 2) = 1;
	}
}



/*
Lớp thực hiện phép biến đổi hình học trên ảnh
*/

class GeometricTransformer
{
public:
	/*
	Hàm biến đổi ảnh theo 1 phép biến đổi affine đã có
	Tham số
	 - beforeImage: ảnh gốc trước khi transform
	 - afterImage: ảnh sau khi thực hiện phép biến đổi affine
	 - transformer: phép biến đổi affine
	 - interpolator: biến chỉ định phương pháp nội suy màu
	Trả về:
	 - 0: Nếu ảnh input ko tồn tại hay ko thực hiện được phép biến đổi
	 - 1: Nếu biến đổi thành công
	*/
	int Transform(
		const Mat &beforeImage, 
		Mat &afterImage, 
		AffineTransform* transformer, 
		PixelInterpolate* interpolator);

	/*
	Hàm xoay bảo toàn nội dung ảnh theo góc xoay cho trước
	Tham số
	- srcImage: ảnh input
	- dstImage: ảnh sau khi thực hiện phép xoay
	- angle: góc xoay (đơn vị: độ)
	- interpolator: biến chỉ định phương pháp nội suy màu
	Trả về:
	 - 0: Nếu ảnh input ko tồn tại hay ko thực hiện được phép biến đổi
	 - 1: Nếu biến đổi thành công
	*/
	int RotateKeepImage(
		const Mat &srcImage, Mat &dstImage, float angle, PixelInterpolate* interpolator);

	/*
	Hàm xoay không bảo toàn nội dung ảnh theo góc xoay cho trước
	Tham số
	- srcImage: ảnh input
	- dstImage: ảnh sau khi thực hiện phép xoay
	- angle: góc xoay (đơn vị: độ)
	- interpolator: biến chỉ định phương pháp nội suy màu
	Trả về:
	 - 0: Nếu ảnh input ko tồn tại hay ko thực hiện được phép biến đổi
	 - 1: Nếu biến đổi thành công
	*/
	int RotateUnkeepImage(
		const Mat &srcImage, Mat &dstImage, float angle, PixelInterpolate* interpolator);

	/*
	Hàm phóng to, thu nhỏ ảnh theo tỉ lệ cho trước
	Tham số
	- srcImage: ảnh input
	- dstImage: ảnh sau khi thực hiện phép xoay	
	- sx, sy: tỉ lệ phóng to, thu nhỏ ảnh	
	- interpolator: biến chỉ định phương pháp nội suy màu
	Trả về:
	 - 0: Nếu ảnh input ko tồn tại hay ko thực hiện được phép biến đổi
	 - 1: Nếu biến đổi thành công
	*/
	int Scale(
		const Mat &srcImage, 
		Mat &dstImage, 
		float sx, float sy, 
		PixelInterpolate* interpolator);
	/*
	Hàm xoay quanh trục Ox, Oy
	Tham số: 
	 - srcImage: ảnh input
	 - desImage: ảnh out put
	 - interpolator : biến chỉ định phương pháp nội suy màu
	 - Ox, Oy : nếu xoay quanh trục Ox thì Ox bằng 1 và ngược lại
	Trả về :
	 - 0: Nếu ảnh input không tồn tại hoặc không thực hiện được phép biến đổi
	 - 1: Nếu biến đổi thành công
	*/
	int Flip(const Mat& srcImage, Mat& desImage,int Ox, int Oy,PixelInterpolate* interpolator);
	GeometricTransformer();
	~GeometricTransformer();
};

GeometricTransformer::GeometricTransformer()
{}

GeometricTransformer::~GeometricTransformer()
{}

int GeometricTransformer::Transform(const Mat &beforeImage, Mat &afterImage, 
	AffineTransform* transformer, PixelInterpolate* interpolator)
{

	if (beforeImage.empty() == true || beforeImage.isContinuous() == false)
		return 0;

	// Phương pháp: Từ ảnh đích nội suy giá trị màu từ ảnh gốc

	// Lấy ma trận nghịch đảo
	transformer->Inverse();
	Mat processedBeforeImage = beforeImage.clone();
	Mat newColumn = beforeImage(Range::all(), Range(beforeImage.cols - 1, beforeImage.cols));
	Mat newRow = beforeImage(Range(beforeImage.rows - 1, beforeImage.rows), Range::all());

	// Thêm vào hàng cuối 1 phần tử
	hconcat(newRow, Mat(1, 1, beforeImage.type(), Scalar(0)), newRow);

	// Thêm vào ảnh gốc cột thứ cols + 1 với giá trị của cột cols -1
	hconcat(processedBeforeImage, newColumn, processedBeforeImage);
	// Thêm vào ảnh gốc dòng thứ rows + 1 với giá trị của dòng rows - 1
	vconcat(processedBeforeImage, newRow, processedBeforeImage);

	uchar* data = processedBeforeImage.data;
	
	int stepWidth = (int)processedBeforeImage.step;
	int nChannels = processedBeforeImage.channels();
	for (int r = 0; r < afterImage.rows; r++)
	{
		for (int c = 0; c < afterImage.cols; c++)
		{
			float x = (float)c;
			float y = (float)r;

			// Thực hiện phép biến đổi affine ngược
			transformer->TransformPoint(x, y);

			// Nội suy giá trị màu
			interpolator->Interpolate(x, y, data, stepWidth,
				nChannels, afterImage.ptr<uchar>(r, c));
		}
	}
	return 1;
}

int GeometricTransformer::Scale(const Mat &srcImage,
	Mat &dstImage,
	float sx, float sy,
	PixelInterpolate* interpolator)
{
	if (srcImage.empty() == true)
		return 0;
	AffineTransform* transformer = new AffineTransform;
	transformer->Scale(sx, sy);
	this->Transform(srcImage, dstImage, transformer, interpolator);
	delete transformer;
	return 1;
}

inline int GeometricTransformer::RotateUnkeepImage(const Mat& srcImage, Mat& dstImage, float angle, PixelInterpolate* interpolator)
{
	AffineTransform* transformer = new AffineTransform;
	transformer->Rotate(angle);
	this->Transform(srcImage, dstImage, transformer, interpolator);
	delete transformer;
	return 1;
}

int GeometricTransformer::Flip(const Mat& srcImage, Mat& desImage,int Ox,int Oy, PixelInterpolate* interpolator)
{
	if (srcImage.empty() == true)
		return 0;
	AffineTransform* transformer = new AffineTransform;
	transformer->Flip(Ox, Oy);
	this->Transform(srcImage, desImage, transformer, interpolator);
	delete transformer;
	return 1;

}

