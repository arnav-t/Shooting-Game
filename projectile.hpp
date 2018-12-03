#include "raycasting.hpp"

const int pStep = 12;

class Projectile
{
	private:
		int aim;
		Point location;
		float pStepX, pStepY;
	public:
		Projectile(Point l, int a)
		{
			location = l;
			aim = a;
			pStepX = pStep*cos(aim*CV_PI/180);
			pStepY = pStep*sin(aim*CV_PI/180);
		}
		~Projectile()
		{
			imgv = Scalar(0);
			for(float t = 0; t < 360; t+=0.1)
				castRay(location,t,3);
			imshow("Game", img);
		}
		void draw()
		{
			imgv = Scalar(0);
			for(float t = 0; t < 360; t+=0.1)
				castRay(location,t,2);
			circle(img, location, 4, Scalar(255,0,0),CV_FILLED);
			imshow("Game",img);
		}
		template <class T>
		bool update(vector<T> charVec, int creater)
		{
			location.x += pStepX;
			location.y += pStepY;
			for(int i=0; i<charVec.size();++i)
			{
				if(i == creater)
				{
					continue;
				}

				Point charLoc = charVec[i]->getLocation();
				if(abs(charLoc.x - location.x) + abs(charLoc.y - location.y) < 15)
				{
					charVec[i]->damage();
					location.x -= pStepX;
					location.y -= pStepY;
					return false;
				}
			}
			if(isValid(location.y,location.x) && imgg.at<uchar>(location.y,location.x) < 128)
			{
				draw();
				return true;
			}
			else 
			{
				location.x -= pStepX;
				location.y -= pStepY;
				return false;
			}
		}
};
