#include <iostream>
#include "ai.hpp"

Player *p;
clock_t prevFire;
clock_t start;
int health_pickuptaken=0;
Point health_location;
void upImg(int event, int x, int y, int flags, void* a)
{
	if(event == EVENT_MOUSEMOVE)
	{
		Point pLoc = p->getLocation();
		float angle = atan2(y - pLoc.y, x - pLoc.x);
		p->setAim(angle);
	}
	else if(event == EVENT_RBUTTONDOWN)
		p->setKeyState(true);
	else if(event == EVENT_RBUTTONUP)
		p->setKeyState(false);
	else if(event == EVENT_LBUTTONDOWN && timeSinceFire >= fireRate)
	{
		prevFire = clock();
		p->shoot();
	}
}

double checkDelay(clock_t *start_ref)
{
	clock_t now = clock();
	double delay = double(now- (*start_ref))/CLOCKS_PER_SEC;
	delay *= 1000;
	(*start_ref) = now;

	if(delay >= 16)
		return 1;
	else
		return (ceil((double)16 - delay));
}

void displayhealthpickup(Point location,Player *p)
{ if(abs(p->getLocation().x-location.x)+abs(p->getLocation().y-location.y)<=15)
  {p->healthboost();
  health_pickuptaken=1;
  imshow("Game", img);
  }
  else
  {rectangle(img,Point(location.x-5,location.y-5),Point(location.x+5,location.y+5),Scalar(255,0,255),CV_FILLED);
   imgv = Scalar(0);
  for(float t = 0; t < 360; t+=0.1)
				castRay(location,t,4);
  imshow("Game", img);
  }
}

int main(int argc, char *argv[])
{
	p = new Player;
	int dead=0;
	activeChars.push_back(p);
	start = clock();
	if(argc >= 2)
	{
		stringstream inp(argv[1]);
		inp>>aiPlayers;
	}
	if(argc >= 3)
	{
		stringstream inp(argv[2]);
		int x;
		inp>>x;
		Mat temp;
		switch(x)
		{
			case 1:
			temp=imread("Level_1.jpg",1);
			imwrite("map.jpg",temp);
			break;
			case 2:
			temp=imread("Level_2.jpg",1);
			imwrite("map.jpg",temp);
			break;
			default:
			temp=imread("Level_3.jpg",1);
			imwrite("map.jpg",temp);
			break;	
		}
	}
	for(int i=0; i<aiPlayers; ++i)
		activeChars.push_back(new AI);
	namedWindow("Game",CV_WINDOW_AUTOSIZE|CV_GUI_NORMAL);
	imshow("Game",img);
	setMouseCallback("Game", upImg, NULL);
	prevFire = clock();
	double delay = 1;
	Mat trans = img;
	trans.setTo(Scalar(0, 0, 0));
	health_location = Point(rand()%img.cols,rand()%img.rows);
    while(imgg.at<uchar>(health_location.y,health_location.x) >= 128)
				health_location = Point(rand()%img.cols,rand()%img.rows);
	while(p->keyInput(waitKey((int) delay)) && !escpressed)
	{        if(pause==1)
	    {putText(img, "pause", Point(150, 300), FONT_HERSHEY_SIMPLEX, 3, Scalar(255, 0, 0), 10, 2);
	     imshow("Game", img);
	     continue;
	    }
		img = imread(IMAGE,1);
		printscore(p);
		timeSinceFire = (dead==1)?fireRate:(double)(clock() - prevFire)/CLOCKS_PER_SEC;

		double alpha = 0.5;
		double beta = (double)1 - alpha;
		if(dead == 1)
		{
			addWeighted(img, alpha, trans, beta, 0.0, img);
			putText(img, "Game Over", Point(40, 250), FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 0, 255), 10, 2);
			putText(img, "You Lose", Point(90, 350), FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 0, 255), 10, 2);
		}
		else if(p->gethealth()<=50&&!health_pickuptaken)
		{displayhealthpickup(health_location,p);
		}

		for(int i=activeChars.size()-1;i>=0;--i)
		{
			if(activeChars[i]->checkLife() == 0)
			{
				Character *tempChar = activeChars[i];
				activeChars[i] = activeChars.back();
				activeChars.pop_back();
				delete tempChar;
			}
			else if(activeChars[i]->checkLife() == 2)
			{
				activeChars[i] = activeChars.back();
				activeChars.pop_back();
				dead=1;
			}
			else
			{
				activeChars[i]->think();
				activeChars[i]->draw();

				activeChars[i]->updateProjectiles(activeChars,i);

			}
		}

		double timeSinceDamage = double(clock() - timeDamage)/CLOCKS_PER_SEC;

		drawRechargeRect(timeSinceFire);
    	drawHealthRect(p, timeSinceDamage);

		if(dead == 0 && activeChars.size() == 1)
		{
			addWeighted(img, alpha, trans, beta, 0.0, img);
			putText(img, "Game Over", Point(40, 250), FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 255, 0), 10, 2);
			putText(img, "You Win", Point(100, 350), FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 255, 0), 10, 2);
		}
		imshow("Game", img);

		delay = checkDelay(&start);
	}
	return 0;
}
