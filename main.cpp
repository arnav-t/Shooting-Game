#include <iostream>
#include "ai.hpp"

Player *p;
clock_t prevFire;
clock_t start;
clock_t begining;
clock_t prevRelease;//time since new enemy is released
int released=0;
int health_pickuptaken=0;
int health_launched=0;
double timeSinceStart=0.0;
int maxtime=30;//for mode 2
int cycletime =5;// time to add new enemy
Point health_location;
int game_mode=1;//1,2  1 is default and 2  is the timelapse mode
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
  health_launched=0;
  if(game_mode==1)
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

void displaytime(double timeSinceStart,int maxtime)
{ int time =int(timeSinceStart/2);
 int countdown=(maxtime>=time)?(maxtime-time):0;
  ostringstream a;
  a<<countdown;
  string s=a.str();
  s="TIME:"+s;
  putText(img,s,Point(515,20),FONT_HERSHEY_COMPLEX_SMALL,0.8,Scalar(0,0,255),1,CV_AA);
}

int main(int argc, char *argv[])
{
	p = new Player;
	int dead=0;
	activeChars.push_back(p);
	start = clock();
	begining=clock();
	if(argc == 2)
	{
		stringstream inp(argv[1]);
		inp>>aiPlayers;
	}
	else if(argc == 3)
	{  
	   stringstream inp1(argv[1]);
	   stringstream inp2(argv[2]);
	   inp1>>aiPlayers;
	   inp2>>game_mode;
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
	while(p->keyInput(waitKey((int) delay)) && !escpressed)
	{        
		if(pause||stop)
	    {
			if(pause==1)
	    	putText(img, "pause", Point(150, 300), FONT_HERSHEY_SIMPLEX, 3, Scalar(255, 0, 0), 10, 2);
	    	imshow("Game", img);
	    	continue;
	    }
		img = imread(IMAGE,1);
		printscore(p);
	    timeSinceFire = (dead==1)?fireRate:(double)(clock() - prevFire)/CLOCKS_PER_SEC;
		timeSinceStart=(double)(clock()-begining)/CLOCKS_PER_SEC;
		if(game_mode==2)
		{displaytime(timeSinceStart,maxtime);
		 if(int(timeSinceStart/2)%cycletime==0)
		 { if(!released)
		   {activeChars.push_back(new AI);
		   released=1;
		   }
		 }
		 else
		 {released=0;
		 }
		  
		}
        double alpha = 0.5;
		double beta = (double)1 - alpha;
		if(dead == 1&&game_mode==1)
		{
			addWeighted(img, alpha, trans, beta, 0.0, img);
			putText(img, "Game Over", Point(40, 250), FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 0, 255), 10, 2);
			putText(img, "You Lose", Point(90, 350), FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 0, 255), 10, 2);
			
		}
		  
		  if(p->gethealth()<=50&&game_mode==1&&!health_pickuptaken||p->gethealth()<=50&&game_mode==2)
		{
		
		 if(!health_launched)
		  { health_location = Point(rand()%img.cols,rand()%img.rows);
		      while(imgg.at<uchar>(health_location.y,health_location.x) >= 128)
				 health_location = Point(rand()%img.cols,rand()%img.rows);
				 health_launched=1;
		  }
		 displayhealthpickup(health_location,p);
		}

		for(int i=activeChars.size()-1;i>=0;--i)
		{
			imgg = imread(IMAGE, 0);
			for(int j=activeChars.size()-1; j>=0; --j)
			{
				if(i!=j)
				{
					Mat temp;
					cvtColor(imgg, temp, COLOR_GRAY2BGR);

					circle(temp, activeChars[j]->getLocation(), 7, Scalar(255, 255, 255), CV_FILLED);
					
					cvtColor(temp, imgg, COLOR_BGR2GRAY);
				}
			}

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

		if(dead == 0 && activeChars.size() == 1&&game_mode==1)
		{
			addWeighted(img, alpha, trans, beta, 0.0, img);
			putText(img, "Game Over", Point(40, 250), FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 255, 0), 10, 2);
			putText(img, "You Win", Point(100, 350), FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 255, 0), 10, 2);
			
		}
		if(dead==0 &&int(timeSinceStart/2)>=maxtime&&game_mode==2||dead==1&&game_mode==2)
		{    ostringstream a;
	         a << p->getscore();
	         string s = a.str();
			addWeighted(img, alpha, trans, beta, 0.0, img);
			putText(img, "your score", Point(40, 250), FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 255, 0), 10, 2);
			putText(img, s, Point(200, 350), FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 255, 0), 10, 2);
		    stop=1;
		
		}
		imshow("Game", img);

		delay = checkDelay(&start);
	}
	return 0;
}
