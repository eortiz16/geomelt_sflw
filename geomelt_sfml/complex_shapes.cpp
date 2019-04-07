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

void RoundCornerBox::set_color(Color clr)
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
	// Define one uniform cloud subshape
	uint8_t color = rand() % 55 + 200;
	GLfloat radius = (GLfloat)(rand() % CLOUD_RANGE) + CLOUD_START;
	
	Circle circle = Circle(
		radius,
		Color(color, color, color, 255),
		Vec(0, (GLfloat)(rand() % (4 * SCRN_HT)) - (2 * SCRN_HT), 0)
	);

	return Cloud(circle, rand() % MAX_SPEED + (int)radius % MAX_SPEED + 1, dir);
}

Cloud::Cloud()
{
	for (int i = 0; i < SUBCLOUD_SIZE; ++i)
		body.push_back(Circle());
}

Cloud::~Cloud()
{
	body.clear();
}

Cloud::Cloud(Circle circle, int spd, Direction dir) : Cloud()
{
	float radius = circle.radius;

	for (int i = 0; i < SUBCLOUD_SIZE; i++)
		this->body.at(i) = Circle(circle);

	this->body[1].center.x = (dir == LEFT) ? 2.0f * SCRN_WD + 1.5f * radius : -2.0f * SCRN_WD - 1.5f * radius;
	this->body[0].center.x = body[1].center.x - radius;
	this->body[2].center.x = body[1].center.x + radius;

	this->speed = spd;
	this->offScreen = false;
}

void Cloud::update(Direction dir)
{
	for (auto &circle : body)
		circle.center.x += (dir == LEFT) ? -speed : speed;
}

void Cloud::render()
{
	for (auto &circle : body)
		circle.render();
}

Star::Star() 
{
	body = make_unique<Circle>();
	body->radius = 3;
	body->color = Color(255, 255, 255, 255);
}

void Star::change_color()
{
	//Stars Flicker in the Night Sky
	if (rand() % 2 == 0) 
		body->color = Color(255, rand() % 255, 215, 255);
	else 
		body->color = Color(rand() % 255, 215, 255, 255);
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
	body->center = Vec(x, y, 0);
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

TexturedQuad::TexturedQuad(sf::Texture texture, float w, float h, Vec v) : Shape(w, h, 0.0f, v) {}
