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
	while(p->keyInput(waitKey((int) delay)) && !escpressed)
	{
		img = imread(IMAGE,1);
		timeSinceFire = (double)(clock() - prevFire)/CLOCKS_PER_SEC;
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
				activeChars[i]->updateProjectiles(activeChars);
			}
		}
		if(!dead)
		drawRechargeRect();
    drawHealthRect(p);
		delay = checkDelay(&start);
	}
	return 0;
}


