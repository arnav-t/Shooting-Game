#include "player.hpp"

const int aiPlayers = 2;

class AI : public Character
{
	private:
		Character *target;
		
	public:
		AI()
		{
			type = 1;
			health = 100;
			aim = rand()%360;
			location = Point(rand()%img.cols,rand()%img.rows);
			while(imgg.at<uchar>(location.y,location.x) >= 128)
				location = Point(rand()%img.cols,rand()%img.rows);
			draw();
		}
};

