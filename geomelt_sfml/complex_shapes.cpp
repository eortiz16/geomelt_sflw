#include "complex_shapes.h"

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

void RoundCornerBox::set_color(geomelt::Color clr)
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

Cloud Cloud::make_cloud(Direction dir)
{
	// clr represents the color of cloud object
	// size assigns a uniform size to cloud object
	// level assigns a Y coor for the cloud object to travel

	Cloud cloud;
	int color = rand() % 55 + 200;
	GLfloat size = (GLfloat)(rand() % CLOUD_RANGE) + CLOUD_START;

	// Define one uniform cloud subshape
	geomelt::Circle circle;
	circle.color.r = color;
	circle.color.g = color;
	circle.color.b = color;
	circle.color.alpha = 255;
	circle.radius = size;
	circle.center.y = (GLfloat)(rand() % (4 * SCRN_HT)) - (2 * SCRN_HT);

	//Assign computed attributes to object
	for (int i = 0; i < SUBCLOUD_SIZE; i++)
		cloud.body.push_back(make_unique<geomelt::Circle>(circle));

	// Assign x of middle circle // Start ar right or left
	cloud.body[1]->center.x = (dir == LEFT) ? 2.0f * SCRN_WD + 1.5f * size : -2.0f * SCRN_WD - 1.5f * size;

	// Assign center of first and last circle, bassed on middle
	cloud.body[0]->center.x = cloud.body[1]->center.x - cloud.body[1]->radius;
	cloud.body[2]->center.x = cloud.body[1]->center.x + cloud.body[1]->radius;

	//Set Speed - Based on Size (Kind of)
	cloud.speed = rand() % MAX_SPEED + (int)size % MAX_SPEED + 1;
	
	//for erasing cloud when off screen
	cloud.offScreen = false;

	return cloud;
}

void Cloud::update(Direction dir)
{
	vector<unique_ptr<geomelt::Circle>>::iterator it;

	for (it = body.begin(); it != body.end(); ++it) 
		it->get()->center.x += (dir == LEFT) ? -speed : speed;
}

void Cloud::render()
{
	vector<unique_ptr<geomelt::Circle>>::iterator it;

	for (it = body.begin(); it != body.end(); ++it)
		it->get()->render();
}

Star::Star() 
{
	body = make_unique<geomelt::Circle>();
	body->radius = 3;
	body->color.r = 255;
	body->color.g = 255;
	body->color.b = 255;
	body->color.alpha = 255;
}

void Star::change_color()
{
	//Stars Flicker in the Night Sky
	if (rand() % 2 == 0) {
		body->color.r = 255;
		body->color.g = rand() % 255;
		body->color.b = 215;
	}
	else {
		body->color.r = rand() % 255;
		body->color.g = 215;
		body->color.b = 255;
	}
}

Star::Star(unsigned int seed) : Star() 
{
	float w = 4.0f * SCRN_WD;
	float h = 5.0f * SCRN_HT;
	float x = 0.0f, y = 0.0f;

	float horizonalPartition = w / 16.0f;
	float verticalPartition = h / 10.0f;

	//Produces Randomly arranged stars
	do {
		int r1 = (int)(((rand() % 400) - 800) * rnd());
		x = ((seed % 16 + 1) * horizonalPartition) - w / 2.0f - horizonalPartition / 2.0f + r1;
		y = ((seed % 10 + 1) * verticalPartition) - h / 2.0f - verticalPartition / 2.0f + r1;
	} while (x < (-w / 2.0f) || x > (w / 2.0f) || y < (-h / 2.0f) || y > (h / 2.0f));

	/* //Produces Uniform Stars
	x = ((seed % 16 + 1) * horizonalPartition) - w / 2 - horizonalPartition / 2;
	y = ((seed % 10 + 1) * verticalPartition) - h / 2 - verticalPartition / 2;
	*/
	body->center = geomelt::Vec(x, y, 0);
}

StarGroup::StarGroup() 
{
	for (unsigned int i = 0; i < MAX_STAR; i++) 
		star.push_back(Star(i));
}

void StarGroup::render()
{
	for (vector<Star>::iterator it = star.begin(); it != star.end(); ++it) {
		it->change_color();
		it->body->render();
	}
}

void StarGroup::update()
{
	//for moving accross the screen;
}

void TexturedQuad::set_texture_attributes(sf::Texture tex)
{
	myTexture = tex;
	
	width = (float)tex.getSize().x;
	height = (float)tex.getSize().y;

	center.x = 0;
	center.y = 0; 
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
		glTexCoord2f(0.0f, 1.0f); glVertex2i((GLint)(center.x - width / 2), (GLint)(center.y - height / 2)); // BL
		glTexCoord2f(0.0f, 0.0f); glVertex2i((GLint)(center.x - width / 2), (GLint)(center.y + height / 2)); // TL
		glTexCoord2f(1.0f, 0.0f); glVertex2i((GLint)(center.x + width / 2), (GLint)(center.y + height / 2)); // TR
		glTexCoord2f(1.0f, 1.0f); glVertex2i((GLint)(center.x + width / 2), (GLint)(center.y - height / 2)); // BR
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
}



CloudGroup::CloudGroup()
{
	//Wind Direction
	(rand() % 2 == 0) ? windDirection = RIGHT : windDirection = LEFT;

	for (int i = 0; i < MAX_CLOUDS; ++i)
		clouds.push_back(Cloud::make_cloud(windDirection));

	// Initially only Assign random X coordinate
	for (vector<Cloud>::iterator it = clouds.begin(); it != clouds.end(); ++it) {
		int randX = rand() % (4 * SCRN_WD) - 2 * SCRN_WD; // get random position for cloud
		it->body[1]->center.x = (float)randX;
		it->body[0]->center.x = randX - it->body[1]->radius;
		it->body[2]->center.x = randX + it->body[1]->radius;
	}
}

void CloudGroup::update()
{
	GLfloat arg1, arg2;
	int offset = 0;

	for (vector<Cloud>::iterator it = clouds.begin(); it != clouds.end(); ++it) {
		// Parameters to determine when a cloud is off screen
		arg1 = it->body[1]->center.x + (it->body[1]->radius  * 1.5f);
		arg2 = it->body[1]->center.x - (it->body[1]->radius  * 1.5f);
		offset = it - clouds.begin();

		//Reset if Last Cloud Offscreen
		if (arg1 < -2.0f * SCRN_WD && windDirection == LEFT) {
			clouds.erase(clouds.begin() + offset);
			clouds.push_back(Cloud::make_cloud(windDirection));
		}
		else if (arg2 > 2.0f * SCRN_WD && windDirection == RIGHT) {
			clouds.erase(clouds.begin() + offset);
			clouds.push_back(Cloud::make_cloud(windDirection));
		}
		it->update(windDirection);
	}

	purge();
}

void CloudGroup::purge()
{
	for (vector<Cloud>::iterator it = clouds.begin(); it != clouds.end(); ++it) {
		if (it->offScreen) {
			clouds.erase(it);
			clouds.push_back(Cloud::make_cloud(windDirection));
		}
	}
}

void CloudGroup::render()
{
	for (vector<Cloud>::iterator it = clouds.begin(); it != clouds.end(); ++it)
		it->render();
}
