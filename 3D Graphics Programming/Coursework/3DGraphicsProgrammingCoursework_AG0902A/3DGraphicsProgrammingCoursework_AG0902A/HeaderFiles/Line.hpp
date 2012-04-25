// Line.h
#ifndef LINE_HPP_
#define LINE_HPP

class Line
{
public:
	Line() {}
	Line(float gradient, float zIntercept): mGradient(gradient), mZintercept(zIntercept) {}
	float ReturnZ(float x) { return ( (mGradient*x) + mZintercept );}				// Follows Y = mx + c
private:
	float mGradient;
	float mZintercept;
};

#endif