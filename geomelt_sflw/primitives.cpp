#include "primitives.h"

Line::Line()
{
	width = 2.0;
	color.r = 0;
	color.g = 0;
	color.b = 0;
}

void Line::render()
{
	glLineWidth(width);
	glColor3i((GLubyte)color.r, (GLubyte)color.g, (GLubyte)color.b);
	glBegin(GL_LINES);
	glVertex2f(from.x, from.y);
	glVertex2f(to.x, to.y);
	glEnd();
}

void Shape_::boundary_assignment()
{
	boundary.top = center.y + height / 2;
	boundary.bottom = center.y - height / 2;
	boundary.left = center.x - width / 2;
	boundary.right = center.x + width / 2;
}

void Circle_::render()
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

void Quad_::render()
{
	glColor4ub((GLubyte)color.r, (GLubyte)color.g, (GLubyte)color.b, (GLubyte)color.alpha);
	glBegin(GL_QUADS);
		glVertex2f(center.x - width / 2, center.y - height / 2);
		glVertex2f(center.x - width / 2, center.y + height / 2);
		glVertex2f(center.x + width / 2, center.y + height / 2);
		glVertex2f(center.x + width / 2, center.y - height / 2);
	glEnd();
}

