#include <iostream>
#include "ai.hpp"

Player *p;
clock_t prevFire;
clock_t start;

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

int main()
{
	p = new Player;
	int dead=0;
	activeChars.push_back(p);
	start = clock();
	for(int i=0; i<aiPlayers; ++i)
		activeChars.push_back(new AI);
	namedWindow("Game",CV_WINDOW_AUTOSIZE);
	imshow("Game",img);
	setMouseCallback("Game", upImg, NULL);
	prevFire = clock();
	double delay = 1;
	Mat trans = img;
	trans.setTo(Scalar(0, 0, 0));
	while(p->keyInput(waitKey((int) delay)) && !escpressed)
	{
		img = imread(IMAGE,1);
		timeSinceFire = (dead==1)?fireRate:(double)(clock() - prevFire)/CLOCKS_PER_SEC;
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
				activeChars[i]->updateProjectiles(activeChars, i);
			}
		}

		drawRechargeRect();
    	drawHealthRect(p);

		if(dead == 1 || activeChars.size() == 1)
		{			
			double alpha = 0.5;
			double beta = (double)1 - alpha;
			addWeighted(img, alpha, trans, beta, 0.0, img);
			if(dead == 1)
			{
				putText(img, "Game Over", Point(40, 250), FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 0, 255), 10, 2);
				putText(img, "You Lose", Point(90, 350), FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 0, 255), 10, 2);
			}
			else if(dead == 0 && activeChars.size() == 1)
			{
				putText(img, "Game Over", Point(40, 250), FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 255, 0), 10, 2);
				putText(img, "You Win", Point(100, 350), FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 255, 0), 10, 2);	
			}
			imshow("Game", img);			
		}

		delay = checkDelay(&start);
	}
	return 0;
}


