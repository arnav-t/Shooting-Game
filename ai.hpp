#include "player.hpp"

const int aiPlayers = 2;

class AI : public Character
{
	private:
		Character *target;
		clock_t prevFire;
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
				//shoot();
				prevFire = clock();
			}
		}
};

