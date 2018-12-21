#include "projectile.hpp"
#include <cstdlib>

const int fov = 90;
const int spread = 10;
const int hitDamage = 10;
const int hitDamagePlayer = 7;
clock_t timeDamage = clock();

class Character
{
	protected:
		int type;
		int health;
		int aim;
	    int score;
		Point location;
		vector<Projectile *> activeProjectiles;
	public:
		void draw()// Draws Player & Bot
		{
			imgv = Scalar(0);
			for(float t = aim - fov/2; t <= aim + fov/2;t+=0.1)
				castRay(location,t,type);
			if(type == 0)
				circle(img, location, 7, Scalar(0,255,0),CV_FILLED);// Player
			else if(type == 1)
				circle(img, location, 7, Scalar(0,0,255),CV_FILLED);// AI Bot
			circle(img, location, 7, Scalar(0,0,0), 1);
			imshow("Game",img);
		}
		Point getLocation()
		{
			return location;
		}
		void setAim(float theta)// Mouse sensitivity
		{
			aim = 180*theta/CV_PI;
		}
		void shoot()// New Projectile Created.
		{
			Projectile *newProjectile = new Projectile(location, aim + (rand()%spread)*pow(-1,rand()));
			activeProjectiles.push_back(newProjectile);
		}
		template <class T>
		void updateProjectiles(vector<T> charVec, int creater)// This Will Update Projectiles.
		{
			for(int i=activeProjectiles.size()-1;i>=0;--i)
			{

				if(!activeProjectiles[i]->update(charVec, creater))

				{
					Projectile *tempProj = activeProjectiles[i];
					activeProjectiles[i] = activeProjectiles.back();
					activeProjectiles.pop_back();
					delete tempProj;
				}
			}
		}
		void damage()// Shows Damage.
		{
			if(type == 1)
				health -= hitDamage;
			else
			{
				health -= hitDamagePlayer;
				timeDamage = clock();
			}
			if(health < 0)
			{
				health = 0;
			}
			circle(img, location, 7, Scalar(153,255,255), CV_FILLED);
			imshow("Game",img);
			waitKey(1);
		}
		virtual void think() 
		{
			return;
		}
		virtual int checkLife()
		{
			return true;
		}
	        virtual void givepoints()
		{ 
			return;
		}
		virtual void healthboost()
		{
		   return;
		}
		virtual int gethealth()
		{
		   return 0;
		}
};

vector<Character *> activeChars;
