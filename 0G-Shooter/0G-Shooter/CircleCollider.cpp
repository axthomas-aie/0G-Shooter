#include "CircleCollider.h"


#include "sfwdraw.h"

#include "Circle.h"


CircleCollider::CircleCollider()
{
	pos = { 0, 0 };
	radius = 2;

	canMove = false;
	vel = { 0, 0 };
	drag = 0.99;
	mass = 10;
}


CircleCollider::~CircleCollider()
{
}




void CircleCollider::update()
{
	vel *= canMove;
	pos += vel * sfw::getDeltaTime() * canMove;

	vel *= drag;

}

void CircleCollider::draw(vec2 playerPos, vec2 playerFacing)
{
	Circle::draw(playerPos, playerFacing, pos, radius);
}



bool CircleCollider::collide(CircleCollider& a, CircleCollider& b)
{
	if ((a.pos.x + a.radius + b.radius > b.pos.x &&
		b.pos.x + a.radius + b.radius > a.pos.x &&
		a.pos.y + a.radius + b.radius > b.pos.y &&
		b.pos.y + a.radius + b.radius > a.pos.y) &&
		//theoretical AABBs are overlapping, possible collision
		(distance(a.pos, b.pos) < a.radius + b.radius)
		//confirmed collision
		)
	{
		float v1x = (a.vel.x * (a.mass - b.mass) + (2 * b.mass * b.vel.x) / (a.mass + b.mass));
		float v2x = (b.vel.x * (b.mass - a.mass) + (2 * a.mass * a.vel.x) / (b.mass + a.mass));
		float v1y = (a.vel.y * (a.mass - b.mass) + (2 * b.mass * b.vel.y) / (a.mass + b.mass));
		float v2y = (b.vel.y * (b.mass - a.mass) + (2 * a.mass * a.vel.y) / (b.mass + a.mass));
		
		a.vel = { v1x, v1y };
		b.vel = { v2x, v2y };

		a.update();
		b.update();


		return true;
	}

	return false;
}



vec2 CircleCollider::closestPointOnLineToPoint(vec2 start, vec2 end, vec2 point)
{
	//formula taken from http://ericleong.me/research/circle-circle/
	vec2 ret;
	ret.x = 0;
	ret.y = 0;

	float A1 = end.y - start.y;
	float B1 = start.x - end.x;

	double C1 = (end.y - start.y) * start.x + (start.x - end.x) * start.y;
	double C2 = -B1 * point.x + A1 * point.y;
	double det = A1 * A1 - -B1 * B1;

	if (det != 0)
	{
		ret.x = (float)((A1*C1 - B1*C2) / det);
		ret.y = (float)((A1*C2 - -B1*C2) / det);
	}
	else
	{
		ret = point;
	}


	return ret;
}
