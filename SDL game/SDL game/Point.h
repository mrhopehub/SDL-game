#ifndef _POINT_H_
#define _POINT_H_
class Point{
public:
	Point(int x, int y);
	~Point();
	int get_x();
	int get_y();
	void set_x(int x);
	void set_y(int y);
private:
	//x,y×ø±ê
	int m_x;
	int m_y;
};
#endif