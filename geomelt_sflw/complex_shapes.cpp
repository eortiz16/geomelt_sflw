#include "complex_shapes.h"

void Background::render()
{
	glBegin(GL_QUADS);
		glColor4ub((GLubyte)color[0].r, (GLubyte)color[0].g, (GLubyte)color[0].b, 255);
		glVertex2f(body.center.x - body.width / 2, body.center.y - body.height / 2);
		glColor4ub((GLubyte)color[1].r, (GLubyte)color[1].g, (GLubyte)color[1].b, 255);
		glVertex2f(body.center.x - body.width / 2, body.center.y + body.height / 2);
		glColor4ub((GLubyte)color[2].r, (GLubyte)color[2].g, (GLubyte)color[2].b, 255);
		glVertex2f(body.center.x + body.width / 2, body.center.y + body.height / 2);
		glColor4ub((GLubyte)color[3].r, (GLubyte)color[3].g, (GLubyte)color[3].b, 255);
		glVertex2f(body.center.x + body.width / 2, body.center.y - body.height / 2);
	glEnd();
}

void Background::set_color(Color_ *clr)
{
	for (int i = 0; i < CORNERS; i++)
		color[i] = clr[i];
}

void RoundCornerBox::render()
{
	vRectangle.render();
	hRectangle.render();

	for (int j = 0; j < 4; j++)
		corner[j].render();
}

RoundCornerBox::RoundCornerBox()
{
	int size = 250; // diameter of circle

	vRectangle.height = height; // actual height
	hRectangle.width = width; // actual width

	//Set center of both boxes
	vRectangle.center.x = center.x;
	vRectangle.center.y = center.y;
	hRectangle.center.x = center.x;
	hRectangle.center.y = center.y;

	vRectangle.width = hRectangle.width - size;
	hRectangle.height = vRectangle.height - size;
	hRectangle.boundary_assignment();
	vRectangle.boundary_assignment();

	//bottom left
	corner[0].radius = size / 2.0f;
	corner[0].center.x = vRectangle.boundary.left;
	corner[0].center.y = hRectangle.boundary.bottom;

	//top left
	corner[1].radius = size / 2.0f;
	corner[1].center.x = vRectangle.boundary.left;
	corner[1].center.y = hRectangle.boundary.top;

	//top right
	corner[2].radius = size / 2.0f;
	corner[2].center.x = vRectangle.boundary.right;
	corner[2].center.y = hRectangle.boundary.top;

	//bottom right
	corner[3].radius = size / 2.0f;
	corner[3].center.x = vRectangle.boundary.right;
	corner[3].center.y = hRectangle.boundary.bottom;
}

void RoundCornerBox::set_color(Color_ clr)
{
	vRectangle.color = clr;
	hRectangle.color = clr;

	for (int j = 0; j < CORNERS; j++)
		corner[j].color = clr;
}

void RoundCornerBox::build()
{
	int size = 250; // diameter of circle

	vRectangle.height = height; // actual height
	hRectangle.width = width; // actual width

	 //Set center of both boxes
	vRectangle.center.x = center.x;
	vRectangle.center.y = center.y;
	hRectangle.center.x = center.x;
	hRectangle.center.y = center.y;

	vRectangle.width = hRectangle.width - size;
	hRectangle.height = vRectangle.height - size;
	hRectangle.boundary_assignment();
	vRectangle.boundary_assignment();

	//bottom left
	corner[0].radius = size / 2.0f;
	corner[0].center.x = vRectangle.boundary.left;
	corner[0].center.y = hRectangle.boundary.bottom;

	//top left
	corner[1].radius = size / 2.0f;
	corner[1].center.x = vRectangle.boundary.left;
	corner[1].center.y = hRectangle.boundary.top;

	//top right
	corner[2].radius = size / 2.0f;
	corner[2].center.x = vRectangle.boundary.right;
	corner[2].center.y = hRectangle.boundary.top;

	//bottom right
	corner[3].radius = size / 2.0f;
	corner[3].center.x = vRectangle.boundary.right;
	corner[3].center.y = hRectangle.boundary.bottom;
}

CloudGroup::CloudGroup()
{
	srand((unsigned int)time(NULL));

	//Set Wind Direction for All Clouds
	(rand() % 2 == 0) ?
		direction = RIGHT :
		direction = LEFT;

	for (int i = 0; i < MAX_CLOUD; i++)
	{
		//Assign Uniform Cloud Groups
		cloud[i].reset_cloud(direction);
	}
}

void CloudGroup::render()
{
	for (int i = 0; i < MAX_CLOUD; i++)
		cloud[i].render();
}

void CloudGroup::update()
{
	GLfloat arg1;
	GLfloat arg2;

	for (int i = 0; i < MAX_CLOUD; i++)
	{
		//Use Data from Center Cloud
		arg1 = cloud[i].body[1].center.x + (cloud[i].body[1].radius * 3);
		arg2 = cloud[i].body[1].center.x - (cloud[i].body[1].radius * 3);

		//Reset if Last Cloud Offscreen
		if (arg1 < (-1.0f * HDX) && direction == LEFT)
			cloud[i].reset_cloud(direction);
		else if (arg2 > HDX && direction == RIGHT)
			cloud[i].reset_cloud(direction);

		//Change Position Based on Established Speed
		for (int i = 0; i < MAX_CLOUD; i++)
			cloud[i].update(direction);
	}
}

void Cloud::update(Direction dir)
{
	if (dir == LEFT)
	{
		for (int j = 0; j < CLOUD_GROUP; j++)
			body[j].center.x -= speed;
	}
	else
	{
		for (int j = 0; j < CLOUD_GROUP; j++)
			body[j].center.x += speed;
	}
}

void Cloud::reset_cloud(Direction dir)
{
	// clr represents the color of cloud object
	// size assigns a uniform size to cloud object
	// level assigns a Y coor for the cloud object to travel

	float clr = (float)(rand() % 55 + 200);
	GLfloat size = (GLfloat)(rand() % CLOUD_RANGE) + CLOUD_START;
	GLfloat level = (GLfloat)(rand() % (2 * HDY)) - HDY;

	cout << "clr = " << clr << ", size = " << size << ", level = " << level << endl;

	//Assign computed attributes to object
	for (int i = 0; i < CLOUD_GROUP; i++)
	{
		body[i].color.r = clr;
		body[i].color.g = clr;
		body[i].color.b = clr;
		body[i].color.alpha = 255;
		body[i].radius = size;
		body[i].center.y = level;
	}

	// Assign x of middle circle
	if (dir == LEFT) //Starting on Right
		body[1].center.x = 2.0f * HDX + body[0].radius;
	else //Starting on Left
		body[1].center.x = -2.0f * HDX - body[0].radius;

	// Assign center of first and last circles, bassed on middle
	body[0].center.x = body[1].center.x - body[1].radius;
	body[2].center.x = body[1].center.x + body[1].radius;

	//Set Speed - Based on Size (Kind of)
	speed = (int)body[0].radius % MAX_SPEED + 1;
}

void Cloud::render()
{
	for (int i = 0; i < CLOUD_GROUP; i++)
		body[i].render();
}

void Star::compute_coordinates(int count)
{
	int w = 4 * HDX;
	int h = 5 * HDY;

	float horizonalPartition = (float)w / 8;
	float verticalPartition = (float)h / 5;

	//Produces Randomly arranged stars
	do {
		int r1 = rand() % 800;
		body.center.x = ((count % 8 + 1) * horizonalPartition) - w / 2 - horizonalPartition / 2 + r1;
		body.center.y = ((count % 5 + 1) * verticalPartition) - h / 2 - verticalPartition / 2 + r1;
	} while (body.center.x < -w / 2 || body.center.x > w / 2 || body.center.y < -h / 2 || body.center.y > h / 2);

	//Produces Uniform Stars
	//body.center.x = ((count % 8 + 1) * horizonalPartition) - w / 2 - horizonalPartition / 2;
	//body.center.y = ((count % 5 + 1) * verticalPartition) - h / 2 - verticalPartition / 2;
}

void Star::change_color()
{
	//Stars Flicker in the Night Sky
	if (rand() % 2 == 0)
	{
		body.color.r = 255;
		body.color.g = (float)(rand() % (255 - 0));
		body.color.b = 215;
	}
	else
	{
		body.color.r = (float)(rand() % (255 - 0));
		body.color.g = 215;
		body.color.b = 255;
	}
}

StarGroup::StarGroup() 
{
	//Star Attributes
	for (int i = 0; i < MAX_STAR; i++)
	{
		star[i].offset = rnd();
		star[i].body.radius = 3;
		star[i].body.color.r = 255;
		star[i].body.color.g = 255;
		star[i].body.color.b = 255;
		star[i].body.color.alpha = 255;
		star[i].compute_coordinates(i);
	}
}

void StarGroup::render()
{
	for (int i = 0; i < MAX_STAR; i++)
	{
		star[i].change_color();
		star[i].body.render();
	}
}

void StarGroup::update()
{

}

void TexturedQuad::set_texture_attributes(sf::Texture asset)
{
	myTexture = asset;
	body.width = (float)asset.getSize().x;
	body.height = (float)asset.getSize().y;

	body.center.x = 0;
	body.center.y = 0; 
}

void TexturedQuad::render()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	sf::Texture::bind(&myTexture);

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();

	glEnable(GL_ALPHA_TEST);
	glEnable(GL_TEXTURE_2D);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255, 255, 255, 255);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2i((GLint)(body.center.x - body.width / 2), (GLint)(body.center.y - body.height / 2)); // BL
		glTexCoord2f(0.0f, 0.0f); glVertex2i((GLint)(body.center.x - body.width / 2), (GLint)(body.center.y + body.height / 2)); // TL
		glTexCoord2f(1.0f, 0.0f); glVertex2i((GLint)(body.center.x + body.width / 2), (GLint)(body.center.y + body.height / 2)); // TR
		glTexCoord2f(1.0f, 1.0f); glVertex2i((GLint)(body.center.x + body.width / 2), (GLint)(body.center.y - body.height / 2)); // BR
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
}
