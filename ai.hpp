#include "player.hpp"

const int aiPlayers = 2;

class AI : public Character
{
	private:
		Character *target;
		clock_t prevFire;
		void setTarget()
		{
			if(activeChars.size())
				target = activeChars[0];
			for(int i=0;i<activeChars.size();++i)
			{
				if(activeChars[i]->getLocation() != location)
				{
					if(abs(location.x - activeChars[i]->getLocation().x) + abs(location.y - activeChars[i]->getLocation().y) < 
						abs(location.x - target->getLocation().x) + abs(location.y - target->getLocation().y))
						target = activeChars[i];
				}
			}
		}
	public:
		AI()
		{
			type = 1;
			health = 100;
			aim = rand()%360;
			prevFire = clock();
			location = Point(rand()%img.cols,rand()%img.rows);
			while(imgg.at<uchar>(location.y,location.x) >= 128)
				location = Point(rand()%img.cols,rand()%img.rows);
			draw();
		}
		void think()
		{
			if((double)(clock() - prevFire)/CLOCKS_PER_SEC >= fireRate)
			{	
				setTarget();
				setAim(atan2(target->getLocation().y - location.y, target->getLocation().x - location.x));
				prevFire = clock();
			}
		}
};

