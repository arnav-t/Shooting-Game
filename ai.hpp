#include "astar.hpp"

const int aiPlayers = 2;
const float aiMoveRate = 0.03;
const float aiUpdateRate = 0.8;

class AI : public Character
{
	private:
		Character *target;
		clock_t prevUpdate;
		clock_t prevMove;
		stack<Point> path;
		void setTarget()
		{
			if(activeChars.size())
				target = activeChars[0];
			for(int i=1;i<activeChars.size();++i)
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
					if(isValid(path.top().y,path.top().x))
						location = path.top();
					path.pop();
					prevMove = clock();
				}
			} 
			if((double)(clock() - prevUpdate)/CLOCKS_PER_SEC >= aiUpdateRate)
			{	
				setTarget();
				if(target != nullptr)
				{
					getPath(location, target->getLocation(), path);
					setAim(atan2(target->getLocation().y - location.y, target->getLocation().x - location.x));
				}
				prevUpdate = clock();
			}
		}
};

