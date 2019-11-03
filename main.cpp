#include "Converter.h"
#include "ColorTransformer.h"
#include "EdgeDetector.h"

int main(int argc, char *argv[])
{
	try
	{
		 // Get input path
		char* fileinput = argv[2];
		Mat origin = imread(fileinput, CV_LOAD_IMAGE_ANYCOLOR); // Original image
		Converter converter;
		ColorTransformer tranform;
		if (strcmp(argv[1], "--rgb2gray") == 0)
		{
			
		}
		else if (strcmp(argv[1], "--rgb2hsv") == 0)
		{

		}
		else if (strcmp(argv[1], "--bright") == 0)
		{
			Mat changedBrightness;
			short b = (short)atoi(argv[3]);
			tranform.ChangeBrighness(origin, changedBrightness, b);
			imshow("Origin", origin);
			imshow("Changed brightness", changedBrightness);
		}
		else if (strcmp(argv[1], "--contrast") == 0)
		{
			Mat changedContrast;
			float c = (float)atof(argv[3]);
			tranform.ChangeContrast(origin, changedContrast, c);
			imshow("Origin", origin);
			imshow("Changed contrast", changedContrast);
		}
		else if (strcmp(argv[1], "--drawHist") == 0)
		{
			int numBins = 256;
			if (origin.channels() == 3)
			{
				Mat splitChannels[3];
				split(origin, splitChannels);
				Mat b_hist, g_hist, r_hist;
				CalcHistogram(splitChannels[0], b_hist);
				CalcHistogram(splitChannels[1], g_hist);
				CalcHistogram(splitChannels[2], r_hist);

				// draw the histogram for R, G and B
				int hist_w = 768; int hist_h = 600;
				int bin_w = hist_w / numBins;

				Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

				DrawHistogram(b_hist, histImage, 255, 0, 0, 0);
				DrawHistogram(g_hist, histImage, 0, 255, 0, 1);
				DrawHistogram(r_hist, histImage, 0, 0, 255, 2);
				namedWindow("Histogram of RGB image", CV_WINDOW_AUTOSIZE);
				imshow("Histogram", histImage);
			}
			else if (origin.channels() == 1)
			{
				Mat hist;
				CalcHistogram(origin, hist);
				// draw the histogram for R, G and B
				int hist_w = 768; int hist_h = 600;
				int bin_w = hist_w / numBins;

				Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

				DrawHistogram(hist, histImage, 255, 0, 0, 0);
				namedWindow("Histogram of gray scale image", CV_WINDOW_AUTOSIZE);
				imshow("Histogram", histImage);
			}
		}
		else if (strcmp(argv[1], "--prewitt") == 0)
		{
			if (origin.channels() == 1)
			{
				// Edge dectection by Prewitt
				EdgeDetector prewitt;
				Mat destinationImage;
				prewitt.DetectEdge(origin, destinationImage, 3, 3, 2);
				imshow("Original", origin);
				imshow("Detect edge by prewitt", destinationImage);

			}
			else
			{

			}
		}
		else
		{
			cout << "Invalid command!!!" << endl;
		}
		waitKey(0);
	}
	catch (cv::Exception &e)
	{
		cerr << e.msg << endl;
	}
	return 0;
}

