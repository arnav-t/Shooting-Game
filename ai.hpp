#include "astar.hpp"

int aiPlayers = 4;
const float aiMoveRate = 0.07;
const float aiUpdateRate = 0.8;
const int shootThreshold = 100;

class AI : public Character
{
	private:
		Character *target;
		clock_t prevUpdate;
		clock_t prevMove;
		clock_t prevFire;
		stack<Point> path;
		void setTarget()
		{
			if(activeChars.size())
				target = NULL;
			for(int i=0;i<activeChars.size();++i)
			{
				if(activeChars[i]->getLocation() != location)
				{
					if(target == NULL)
						target = activeChars[i];
					else if(abs(location.x - activeChars[i]->getLocation().x) + abs(location.y - activeChars[i]->getLocation().y) < 
						abs(location.x - target->getLocation().x) + abs(location.y - target->getLocation().y))
						target = activeChars[i];
				}
			}
		}
		bool checkLoS(float y, float x)
		{
			
		}
	public:
		AI()
		{
			type = 1;
			health = 100;
			aim = rand()%360;
			prevUpdate = clock();
			prevMove = clock();
			prevFire = clock();
			location = Point(rand()%img.cols,rand()%img.rows);
			while(imgg.at<uchar>(location.y,location.x) >= 128)
				location = Point(rand()%img.cols,rand()%img.rows);
			target = NULL;
			draw();
		}
		~AI()
		{
			imgv = Scalar(0);
			for(float t = 0; t < 360; t+=0.1)
				castRay(location,t,3);
			imshow("Game", img);
		}
		void think()
		{
			if((double)(clock() - prevMove)/CLOCKS_PER_SEC >= aiMoveRate)
			{
				if(!path.empty())
				{
					if(isValid(path.top().y,path.top().x))
						location = path.top();
					path.pop();
				}
				prevMove = clock();
			} 
			if((double)(clock() - prevFire)/CLOCKS_PER_SEC >= fireRate)
			{
				if(target != NULL)	
					if(abs(target->getLocation().x - location.x) + abs(target->getLocation().y - location.y) <= shootThreshold)
					{	
						shoot();
						prevFire = clock();
					}
			}
			if((double)(clock() - prevUpdate)/CLOCKS_PER_SEC >= aiUpdateRate)
			{	
				setTarget();
				if(target != NULL)
				{
					getPath(location, target->getLocation(), path);
					setAim(atan2(target->getLocation().y - location.y, target->getLocation().x - location.x));
				}
				prevUpdate = clock();
			}
		}
		int checkLife()
		{
			return health > 0 ? 1:0;
		}
};

