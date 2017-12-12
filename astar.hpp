#include "player.hpp"
#include <stack>
#include <queue>

const int greed = 50;
const int delta = 5;

void roundToDelta(Point &a)
{
	a.x -= a.x%delta;
	a.y -= a.y%delta;
}

Point aim;

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
			return greed*(abs(aim.x - location.x) + abs(aim.y - location.y)) + weight;
		}
		Point getLocation() const
		{
			return location;
		}
		int getWeight() const
		{
			return weight;
		}
		void setParent(Node *p)
		{
			parent = p;
			if(parent == nullptr)
				weight = 0;
			else if((parent->getLocation() - location).x && (parent->getLocation() - location).y)
				weight = 14 + parent->getWeight();
			else
				weight = 10 + parent->getWeight();
		}
		void addToStack(stack<Point> &s)
		{
			s.push(location); 
			if(parent != nullptr)
			{
				line(img, location, parent->getLocation(), Scalar(0,0,255), 1, CV_AA);
				parent->addToStack(s);
			}
		}
};

inline bool operator<(const Node &l, const Node &r) 
{
    return l.getHeuristicWeight() > r.getHeuristicWeight();
}

priority_queue<Node, vector<Node>> open;

void aStar(Node n, stack<Point> &s)
{
	if(!open.empty())
		open.pop();
	int y = n.getLocation().y;
	int x = n.getLocation().x;
	imgv.at<uchar>(y,x) = 255;
	if(abs(y - aim.y) + abs(x - aim.x) <= 5*delta)
	{
		n.addToStack(s);
		while(!open.empty())
			open.pop();
		return;
	}
	for(int j=-delta; j<=delta; j+=delta)
	{
		for(int i=-delta; i<=delta; i+=delta)
		{
			if(j && i)
				if(isValid(y+j,x+i))
					if(imgv.at<uchar>(y+j,x+i) < 128 && imgg.at<uchar>(y+j,x+i) < 128)
					{
						Node *newNode = new Node(Point(x+i,y+j), &n);
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
	roundToDelta(start);
	roundToDelta(finish);
	if(abs(start.x - finish.x) + abs(start.y - finish.y) <= 2*delta)
		return;
	aim = finish;
	Node *init = new Node(start);
	open.push(*init);
	aStar(*init, path);
}
