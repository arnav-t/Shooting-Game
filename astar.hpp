#include "player.hpp"
#include <stack>
#include <queue>

const int greed = 100;
const int deltaStep = 4;

Point aimPoint;

namespace astar
{
	class Node
	{
		private:
			Point location;
			Node *parent;
			int weight;
		public:
			Node(Point l = Point(0,0), Node *p = nullptr)
			{
				location = l;
				weight = 0;
				setParent(p);
			}
			int getHeuristicWeight() const
			{
				return greed*(abs(aimPoint.x - location.x) + abs(aimPoint.y - location.y)) + weight;
			}
			Point getLocation() const
			{
				return location;
			}
			int getWeight() const
			{
				return weight;
			}
			void setParent(astar::Node *p)
			{
				parent = p;
				if(parent == nullptr)
					weight = 0;
				else if((parent->getLocation().x - location.x) && (parent->getLocation().y - location.y))
					weight = 14 + parent->getWeight();
				else
					weight = 10 + parent->getWeight();
			}
			void addToStack(stack<Point> &s)
			{
				s.push(location); 
				if(parent != nullptr)
				{
					parent->addToStack(s);
				}
			}
	};

	inline bool operator<(const Node &l, const Node &r) 
	{
	    return l.getHeuristicWeight() > r.getHeuristicWeight();
	}

}


priority_queue<astar::Node, vector<astar::Node>> open;

void aStar(astar::Node n, stack<Point> &s)
{
	int y = n.getLocation().y;
	int x = n.getLocation().x;
	if(!open.empty())
		open.pop();
	if(!isValid(y,x))
	{
		aStar(open.top(), s);
		return;
	}
	if(imgg.at<uchar>(y,x) >= 128 || imgv.at<uchar>(y,x) >= 128)
	{
		aStar(open.top(), s);
		return;
	}
	imgv.at<uchar>(y,x) = 255;
	if(abs(y - aimPoint.y) + abs(x - aimPoint.x) <= 5*deltaStep)
	{
		n.addToStack(s);
		while(!open.empty())
			open.pop();
		return;
	}
	for(int j=-deltaStep; j<=deltaStep; j+=deltaStep)
	{
		for(int i=-deltaStep; i<=deltaStep; i+=deltaStep)
		{
			if(j || i)
				if(isValid(y+j,x+i))
					if((imgv.at<uchar>(y+j,x+i) == 0) && (imgg.at<uchar>(y+j,x+i) < 128))
					{
						astar::Node *newNode = new astar::Node(Point(x+i,y+j), &n);
						if(newNode != nullptr)
							open.push(*newNode);
					}
		}
	}
	if(!open.empty())
		aStar(open.top(), s);
}

void getPath(Point start, Point finish, stack<Point> &path)
{
	imgv = Scalar(0);
	while(!open.empty())
		open.pop();
	while(!path.empty())
		path.pop();
	if(abs(start.x - finish.x) + abs(start.y - finish.y) <= 5*deltaStep)
		return;
	aimPoint = finish;
	astar::Node *startNode = new astar::Node(start);
	open.push(*startNode);
	aStar(*startNode, path);
}
