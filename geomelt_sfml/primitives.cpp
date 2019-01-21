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

geomelt::Boundary geomelt::Boundary::setBounds(float t, float b, float l, float r)
{
	geomelt::Boundary bnd;
	bnd.top = t;
	bnd.bottom = b;
	bnd.left = l;
	bnd.right = r;

	return bnd;
}

Vec Vec::set_Vec(float xx, float yy, float zz)
{
	Vec vector;
	vector.x = xx;
	vector.y = yy;
	vector.z = zz;

	return vector;
}
