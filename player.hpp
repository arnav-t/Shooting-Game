#include "character.hpp"

const int step = 4;
const double fireRate = 0.5;
const int bPadding = 2;
Point rBarCorner(20,575);
Size rBarSize(100,15);

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
				if(isValid(location.y + step*cos(aim*CV_PI/180),location.x - step*sin(aim*CV_PI/180)))
					if(imgg.at<uchar>(location.y + step*cos(aim*CV_PI/180),location.x - step*sin(aim*CV_PI/180)) < 128)
					{
						location.x -= step*sin(aim*CV_PI/180);
						location.y += step*cos(aim*CV_PI/180);
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
				if(isValid(location.y - step*cos(aim*CV_PI/180),location.x + step*sin(aim*CV_PI/180)))
					if(imgg.at<uchar>(location.y - step*cos(aim*CV_PI/180),location.x + step*sin(aim*CV_PI/180)) < 128)
					{
						location.x += step*sin(aim*CV_PI/180);
						location.y -= step*cos(aim*CV_PI/180);
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

double timeSinceFire = 0.0;

void drawRechargeRect()
{
	rectangle(img, Point(rBarCorner.x - bPadding, rBarCorner.y - bPadding), Point(rBarCorner.x + rBarSize.width + bPadding, rBarCorner.y + rBarSize.height + bPadding), Scalar(255,255,255), 1);
	rectangle(img, rBarCorner, Point(rBarCorner.x + rBarSize.width*min(1.d,(double)timeSinceFire/fireRate), rBarCorner.y + rBarSize.height), Scalar(255,255,255), CV_FILLED);
	imshow("Game",img);
}

