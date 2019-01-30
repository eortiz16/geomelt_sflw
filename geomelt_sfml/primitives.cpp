#include "primitives.h"

geomelt::Line::Line()
{
	width = 2.0;
	color.r = 0;
	color.g = 0;
	color.b = 0;
}

void geomelt::Line::render()
{
	glLineWidth(width);
	glColor3i((GLubyte)color.r, (GLubyte)color.g, (GLubyte)color.b);
	glBegin(GL_LINES);
	glVertex2f(from.x, from.y);
	glVertex2f(to.x, to.y);
	glEnd();
}

void geomelt::Shape::boundary_assignment()
{
	boundary.top = center.y + height / 2;
	boundary.bottom = center.y - height / 2;
	boundary.left = center.x - width / 2;
	boundary.right = center.x + width / 2;
}

geomelt::Shape::Shape(float w, float h, float r, Color clr, Vec v)
{
	width = w;
	height = h;
	radius = r;
	color = clr;
	center = v;

	boundary_assignment();
}

void geomelt::Circle::render()
{
	GLfloat arg1, arg2;
	glColor4ub((GLubyte)color.r, (GLubyte)color.g, (GLubyte)color.b, (GLubyte)color.alpha);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(center.x, center.y);
	for (int i = 0; i < 240; i++)
	{
		arg1 = center.x + (radius * (GLfloat)cos(i * 2 * M_PI / TRI_NUM));
		arg2 = center.y + (radius * (GLfloat)sin(i * 2 * M_PI / TRI_NUM));
		glVertex2f(arg1, arg2);
	}
	glEnd();
}

void geomelt::Quad::render()
{
	glColor4ub((GLubyte)color.r, (GLubyte)color.g, (GLubyte)color.b, (GLubyte)color.alpha);
	glBegin(GL_QUADS);
		glVertex2f(center.x - width / 2, center.y - height / 2);
		glVertex2f(center.x - width / 2, center.y + height / 2);
		glVertex2f(center.x + width / 2, center.y + height / 2);
		glVertex2f(center.x + width / 2, center.y - height / 2);
	glEnd();
}

geomelt::Boundary::Boundary(float t, float b, float l, float r)
{
	top = t;
	bottom = b;
	left = l;
	right = r;
}

bool geomelt::Boundary::isWithin(int x, int y)
{
	if (x >= left && x <= right && y <= top && y >= bottom)
		return true;

	return false;
}

/* Will need modification */
bool geomelt::Boundary::isWithin(Boundary bnd)
{
	if (this->bottom <= bnd.top	&& this->bottom > bnd.bottom
		&& this->left <= bnd.right	&& this->right > bnd.left)
		return true;
	else
		return false;
}

bool geomelt::Boundary::isWithin(Boundary bnd, float center)
{
	if (this->bottom <= bnd.top	&& this->bottom > bnd.bottom
		&& center >= bnd.left && center <= bnd.right)
		return true;
	else
		return false;
}

geomelt::Vec::Vec(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

geomelt::Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->alpha = a;
}

geomelt::Color & geomelt::Color::operator=(const Color & clr)
{
	r = clr.r;
	g = clr.g;
	b = clr.b;
	alpha = clr.alpha;
	return *this;
}
