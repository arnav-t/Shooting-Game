#include "character.hpp"
int escpressed=0;
int pause=0;
const int step = 4;
const double fireRate = 0.5;
const int bPadding = 2;
Point rBarCorner(20,575);
Point hBarCorner(480,575);
Size rBarSize(100,15);

class Player : public Character
{
	private:
		bool rKeyDown;
	public:
		Player()
		{       score=0;
			srand(time(NULL));
			type = 0;
			health = 100;
			aim = rand()%360;
			rKeyDown = false;
			location = Point(rand()%img.cols,rand()%img.rows);
			while(imgg.at<uchar>(location.y,location.x) >= 128)
				location = Point(rand()%img.cols,rand()%img.rows);
			draw();
		}
		bool setKeyState(bool state)
		{
			rKeyDown = state;
		}
		int keyInput(int keyCode)
		{
			if(keyCode == 27)
				escpressed=1;
			if(keyCode==112)
			    { pause+=1;
			     pause%=2;
			     }
			if(keyCode == 100&&!pause)
			{
				if(isValid(location.y + step*cos(aim*CV_PI/180),location.x - step*sin(aim*CV_PI/180)))
					if(imgg.at<uchar>(location.y + step*cos(aim*CV_PI/180),location.x - step*sin(aim*CV_PI/180)) < 128)
					{
						location.x -= step*sin(aim*CV_PI/180);
						location.y += step*cos(aim*CV_PI/180);
						draw();
					}
			}
			else if((keyCode == 119 || rKeyDown)&&!pause)
			{
				if(isValid(location.y + step*sin(aim*CV_PI/180),location.x + step*cos(aim*CV_PI/180)))
					if(imgg.at<uchar>(location.y + step*sin(aim*CV_PI/180),location.x + step*cos(aim*CV_PI/180)) < 128)
					{
						location.y += step*sin(aim*CV_PI/180);
						location.x += step*cos(aim*CV_PI/180);
						draw();
					}
			}
			else if(keyCode == 97&&!pause)
			{
				if(isValid(location.y - step*cos(aim*CV_PI/180),location.x + step*sin(aim*CV_PI/180)))
					if(imgg.at<uchar>(location.y - step*cos(aim*CV_PI/180),location.x + step*sin(aim*CV_PI/180)) < 128)
					{
						location.x += step*sin(aim*CV_PI/180);
						location.y -= step*cos(aim*CV_PI/180);
						draw();
					}
			}
			else if(keyCode == 115&&!pause)
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
		void think()
		{
			return;
		}
		int checkLife()
		{
			return health > 0 ? 1:2;
		}
		void givepoints()
		{
		  score+=15;
		}
		friend void drawHealthRect(Player* p);
		friend void printscore(Player* p);

};

double timeSinceFire = 0.0;

void drawRechargeRect()
{
	rectangle(img, Point(rBarCorner.x - bPadding, rBarCorner.y - bPadding), Point(rBarCorner.x + rBarSize.width + bPadding, rBarCorner.y + rBarSize.height + bPadding), Scalar(255,255,255), 1);
	cvtColor(img, img,CV_BGR2HSV);
	rectangle(img, rBarCorner, Point(rBarCorner.x + rBarSize.width*min(1.0,(double)timeSinceFire/fireRate), rBarCorner.y + rBarSize.height), Scalar(min(1.0,(double)timeSinceFire/fireRate)*60,255,255), CV_FILLED);
	cvtColor(img, img,CV_HSV2BGR);
	imshow("Game",img);
}
void drawHealthRect(Player * p)
{
	rectangle(img, Point(hBarCorner.x - bPadding, hBarCorner.y - bPadding), Point(hBarCorner.x + rBarSize.width + bPadding, hBarCorner.y + rBarSize.height + bPadding), Scalar(255,255,255), 1);//outerborder
	rectangle(img, hBarCorner, Point(hBarCorner.x + rBarSize.width*min(1.0,(double)(p->health)/100), rBarCorner.y + rBarSize.height), Scalar(0,255,0), CV_FILLED);
	imshow("Game",img);

		}
 void printscore(Player* p)
 { ostringstream a;
   a<<p->score;
  string s=a.str();
   s="SCORE :"+s;

   putText(img,s,Point(5,20),FONT_HERSHEY_COMPLEX_SMALL,0.8,Scalar(0,0,255),1,CV_AA);


}
