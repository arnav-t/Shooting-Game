#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>
#define IMAGE "robomaze.jpg"
using namespace cv;
using namespace std;

Mat img = imread(IMAGE,1);
Mat imgg = imread(IMAGE,0);
Mat imgv(img.rows,img.cols,CV_8UC1,Scalar(0));
const int fov = 90;
const int step = 4;
const int pStep = 20;
const int spread = 4;

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
	float r = 1; 
	while(isValid(y,x))
	{
		if(imgv.at<uchar>(y,x) == 0)
		{
			r = 1 + sqrt(pow(x-source.x,2)+pow(y-source.y,2))/40;
			if(imgg.at<uchar>(y,x) < 128)
			{
				if(type == 0)
					img.at<Vec3b>(y,x)[1] = min(255, (int)(img.at<Vec3b>(y,x)[1] + 255/pow(r,2)));
				if(type == 1)
					img.at<Vec3b>(y,x)[2] = min(255, (int)(img.at<Vec3b>(y,x)[2] + 63/pow(r,2)));
				if(type == 2)
					img.at<Vec3b>(y,x)[0] = min(255, (int)(img.at<Vec3b>(y,x)[0] + 255/pow(r,2)));
			}
			else
				break;
			imgv.at<uchar>(y,x) = 255;
		}
		x += dx;
		y += dy;
	}
}

class Projectile
{
	private:
		int aim;
		Point location;
	public:
		Projectile(Point l, int a)
		{
			location = l;
			aim = a;
		}
		void draw()
		{
			imgv = Scalar(0);
			for(float i = 0; i < 360; i+=0.1)
				castRay(location,i,2);
			circle(img, location, 4, Scalar(255,0,0),CV_FILLED);
			imshow("Game",img);
		}
		bool update()
		{
			location.x += pStep*cos(aim*CV_PI/180);
			location.y += pStep*sin(aim*CV_PI/180);
			if(isValid(location.y,location.x) && imgg.at<uchar>(location.y,location.x) < 128)
			{
				draw();
				return true;
			}
			else 
				return false;
		}
};

class Character
{
	protected:
		int type;
		int health;
		int aim;
		Point location;
		vector<Projectile *> activeProjectiles;
	public:
		void draw()
		{
			imgv = Scalar(0);
			img = imread(IMAGE,1);
			for(float i = aim - fov/2; i <= aim + fov/2;i+=0.1)
				castRay(location,i,type);
			if(type == 0)
				circle(img, location, 4, Scalar(0,255,0),CV_FILLED);
			else if(type == 1)
				circle(img, location, 4, Scalar(0,0,255),CV_FILLED);
			circle(img, location, 4, Scalar(0,0,0), 1);
			imshow("Game",img);
		}
		Point getLocation()
		{
			return location;
		}
		void shoot()
		{
			Projectile *newProjectile = new Projectile(location,aim + rand()%spread*pow(-1,rand()));
			activeProjectiles.push_back(newProjectile);
		}
		void updateProjectiles()
		{
			for(int i=0;i<activeProjectiles.size();++i)
			{
				if(!activeProjectiles[i]->update())
				{
					delete activeProjectiles[i];
					activeProjectiles.erase(activeProjectiles.begin() + i);
					i -= 1;
				}
			}
		}
};

class Player : public Character
{
	private:
	
	public:
		Player()
		{
			srand(time(NULL));
			type = 0;
			health = 100;
			aim = rand()%360;
			location = Point(rand()%img.cols,rand()%img.rows);
			while(imgg.at<uchar>(location.y,location.x) >= 128)
				location = Point(rand()%img.cols,rand()%img.rows);
			draw();
		}
		void setAim(float theta)
		{
			aim = 180*theta/CV_PI;
		}
		int keyInput(int keyCode)
		{
			if(keyCode == 27)
				return 0;
			if(keyCode == 100)
			{
				if(isValid(location.y,location.x + step + 4))
					if(imgg.at<uchar>(location.y,location.x + step + 4) < 128)
					{
						location.x += step;
						draw();
					}
			}
			else if(keyCode == 119)
			{
				if(isValid(location.y + step*sin(aim*CV_PI/180),location.x + step*cos(aim*CV_PI/180)))
					if(imgg.at<uchar>(location.y + step*sin(aim*CV_PI/180),location.x + step*cos(aim*CV_PI/180)) < 128)
					{
						location.y += step*sin(aim*CV_PI/180);
						location.x += step*cos(aim*CV_PI/180);
						draw();
					}
			}
			else if(keyCode == 97)
			{
				if(isValid(location.y,location.x - step - 4))
					if(imgg.at<uchar>(location.y,location.x - step - 4) < 128)
					{
						location.x -= step;
						draw();
					}
			}
			else if(keyCode == 115)
			{
				if(isValid(location.y - step*sin(aim*CV_PI/180),location.x - step*cos(aim*CV_PI/180)))
					if(imgg.at<uchar>(location.y - step*sin(aim*CV_PI/180),location.x - step*cos(aim*CV_PI/180)) < 128)
					{
						location.y -= step*sin(aim*CV_PI/180);
						location.x -= step*cos(aim*CV_PI/180);
						draw();
					}
			}
			return 1;
		}
};

Player p;

void upImg(int event, int x, int y, int flags, void* a)
{
	Point pLoc = p.getLocation();
	float angle = atan2(y - pLoc.y, x - pLoc.x);
	p.setAim(angle);
	if(event == EVENT_LBUTTONDOWN)
		p.shoot();
	p.draw();
}

int main()
{
	namedWindow("Game",CV_WINDOW_AUTOSIZE);
	imshow("Game",img);
	setMouseCallback("Game", upImg, NULL);
	while(p.keyInput(waitKey(50)))
	{
		p.draw();
		p.updateProjectiles();
	}
	return 0;
}


