#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <cmath>
#define IMAGE "map.jpg"
using namespace cv;
using namespace std;

Mat img = imread(IMAGE,1);
Mat imgg = imread(IMAGE,0);
Mat imgv(img.rows,img.cols,CV_8UC1,Scalar(0));

template <class T>
bool isValid(T y, T x)
{
	if(y < 0 || y >= img.rows)
		return false;
	else if(x < 0 || x >= img.cols)
		return false;
	else
		return true;
}

void castRay(Point source, float theta, int type)
{
	float y = source.y;
	float x = source.x;
	float dx = cos(CV_PI*theta/180);
	float dy = sin(CV_PI*theta/180);
	float r; 
	while(isValid(y,x))
	{
		if(imgv.at<uchar>(y,x) == 0)
		{
			r = 1 + (sqrt(pow(x-source.x,2)+pow(y-source.y,2))/100);
			if(imgg.at<uchar>(y,x) < 128)
			{
				if(type == 0)
					img.at<Vec3b>(y,x)[1] = min(255, (int)(img.at<Vec3b>(y,x)[1] + 255/pow(r,2)));
				if(type == 1)
					img.at<Vec3b>(y,x)[2] = min(255, (int)(img.at<Vec3b>(y,x)[2] + 127/pow(r,2)));
				if(type == 2)
					img.at<Vec3b>(y,x)[0] = min(255, (int)(img.at<Vec3b>(y,x)[0] + 255/pow(r,2)));
				if(type == 3)
				{
					img.at<Vec3b>(y,x)[1] = min(255, (int)(img.at<Vec3b>(y,x)[1] + 200/pow(r,2)));
					img.at<Vec3b>(y,x)[2] = min(255, (int)(img.at<Vec3b>(y,x)[2] + 255/pow(r,2)));
				}
			}
			else
				break;
			imgv.at<uchar>(y,x) = 255;
		}
		x += dx;
		y += dy;
	}
}
