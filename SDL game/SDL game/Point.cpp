#include "Point.h"
#include <assert.h>
Point::Point(int x, int y)
{
	assert(x >= 0 && y >= 0 && x <= 2 && y <= 2);
	m_x = x;
	m_y = y;
}
Point::~Point()
{
}
int Point::get_x()
{
	return m_x;
}
int Point::get_y()
{
	return m_y;
}
void Point::set_x(int x)
{
	assert(x >= 0 && x <=2);
	m_x = x;
}
void Point::set_y(int y)
{
	assert(y >= 0 && y <=2);
	m_y = y;
}