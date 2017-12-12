#include "astar.hpp"

const int aiPlayers = 2;
const float aiMoveRate = 0.05;
const float aiUpdateRate = 2;

class AI : public Character
{
	private:
		Character *target;
		clock_t prevUpdate;
		clock_t prevMove;
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
		stack<Point> path;
	public:
		AI()
		{
			type = 1;
			health = 100;
			aim = rand()%360;
			prevUpdate = clock();
			prevMove = clock();
			location = Point(rand()%img.cols,rand()%img.rows);
			while(imgg.at<uchar>(location.y,location.x) >= 128)
				location = Point(rand()%img.cols,rand()%img.rows);
			draw();
		}
		void think()
		{
			if(!path.empty())
			{
				if((double)(clock() - prevMove)/CLOCKS_PER_SEC >= aiMoveRate)
				{
					location = path.top();
					path.pop();
					prevMove = clock();
				}
			} 
			if((double)(clock() - prevUpdate)/CLOCKS_PER_SEC >= aiUpdateRate)
			{	
				setTarget();
				getPath(location, target->getLocation(), path);
				setAim(atan2(target->getLocation().y - location.y, target->getLocation().x - location.x));
				prevUpdate = clock();
			}
		}
};

