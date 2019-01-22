#include "player.h"

unsigned int Player::count = 0;
Player::Player()
{
	myID = extract_lowest_ID();
	count++;

	JUMP_MAX = 3;
	jumpCount = 0;
	velocity.x = 0.0;
	velocity.y = 0.0;

	myColor = (CharColorOptions)myID;
}

vector<int> Player::availableIDs = {0,1,2,3,4,5,6,7};

unsigned int Player::extract_lowest_ID()
{
	int ID = 0;

	std::sort(availableIDs.begin(), availableIDs.end());
	ID = availableIDs.front();
	std::swap(availableIDs.at(0), availableIDs.back());
	availableIDs.pop_back();
	std::sort(availableIDs.begin(), availableIDs.end());
	return ID;
}

Player::~Player()
{
	availableIDs.push_back(this->myID); //push to back
	count--;
}

void Player::change_color(SelectColor option)
{
	switch (option)
	{
	case NEXT:
		myColor++;
		break;
	case PREV:
		myColor--;
		break;
	case RANDOM:

		break;
	default:
		break;
	}
}

void Player::read_buttons(unsigned int button)
{
	switch (button) {
	case A:
	case B:
	{
		toggle.attacking ^= 1;
		toggle.attackTimer.restart();
	}
		break;
	case X:
	case Y:
	{
		jump();
	}
		break;
	case LB:
		break;
	case RB:
		break;
	case START:
		break;
	case SELECT:
		break;
	}
}

void Player::read_axes(unsigned int joyID)
{
	float axis_position1 = sf::Joystick::getAxisPosition(joyID, sf::Joystick::PovX);
	float axis_position2 = sf::Joystick::getAxisPosition(joyID, sf::Joystick::X);

	//toggle move 
	if (axis_position1 == 100 || axis_position2 == 100)	{
		toggle.walking = true;
		direction = RIGHT;
	}
	else if (axis_position1 == -100 || axis_position2 == -100) {
		toggle.walking = true;
		direction = LEFT;
	}
	else
		toggle.walking = false;
}

/*Takes the width resolution and scales down to a factor controls reflection of character*/
void Player::update_reflection_x()
{
	GLfloat OldMax, OldMin, OldValue;
	GLfloat NewMax, NewMin;
	GLfloat offset = 0.0f;

	OldValue = body->center.x;
	OldMin = 0;
	OldMax = SCRN_HT;
	NewMin = 0;
	NewMax = sqrt(body->radius / 8);
	offset = (OldValue - OldMin) * (NewMax - NewMin);
	offset /= (OldMax - OldMin);
	offset += NewMin;
	offset *= -1;

	reflection->center.x = body->center.x + offset;
}

void Player::attack()
{ 
	// Extend arm in direction character is facing
	if (direction == RIGHT)
		arm.center.x = body->center.x + body->width / 2;
	else if (direction == LEFT)
		arm.center.x = body->center.x - body->width / 2;

	arm.center.y = body->center.y;
	armOutline.center.x = arm.center.x;
	armOutline.center.y = arm.center.y;

	// After 25 milliseconds, stop rendering
	if (toggle.attackTimer.getElapsedTime().asMilliseconds() > 25)
		toggle.attacking ^= 1;
}

void Player::simple_update()
{
	update_reflection_x();

	outline->radius = body->radius + 4;
	outline->width = 2 * outline->radius;
	outline->height = outline->width;

	reflection->radius = body->radius - body->radius / 4;
	reflection->width = 2 * reflection->radius;
	reflection->height = reflection->width;

	reflection->center.y = body->center.y + sqrt(body->radius);
	outline->center.x = body->center.x;
	outline->center.y = body->center.y;

	arm.center.y = body->center.y;
	armOutline.center.y = body->center.y;

	(direction == LEFT) ?
		eye.center.x = body->center.x - body->radius / 2 :
		eye.center.x = body->center.x + body->radius / 2;
	eye.center.y = body->center.y + body->radius / 2;
	eye.radius = (body->radius / 10) + 1;
}

void Player::simple_update_menu()
{
	update_reflection_x();

	body->height = SCRN_HT / 4.0f;
	body->width = SCRN_HT / 4.0f;
	body->radius = body->height / 2;

	outline->radius = body->radius * 1.04f;
	outline->width = 2 * outline->radius;
	outline->height = outline->width;

	reflection->radius = body->radius - body->radius / 4;
	reflection->width = 2 * reflection->radius;
	reflection->height = reflection->width;

	reflection->center.y = body->center.y + 3 * sqrt(body->radius);
	outline->center.x = body->center.x;
	outline->center.y = body->center.y;

	arm.center.y = body->center.y;
	armOutline.center.y = body->center.y;

	(direction == LEFT) ?
		eye.center.x = body->center.x - body->radius / 2 :
		eye.center.x = body->center.x + body->radius / 2;
	eye.center.y = body->center.y + body->radius / 2;
	eye.radius = (body->radius / 10) + 1;
}


void Player::reset_attributes()
{
	stats.lifeState = ALIVE;
	velocity.x = 0;
	velocity.y = -25.0f;
	jumpCount = 0;
	toggle.on_ground = false;
	toggle.attacking = false;

	body->radius = body->width / 2;
}

void Player::move(Direction dir)
{
	direction = dir;
	//Horizontal Velocity
	if (direction == LEFT)
		velocity.x = -move_param_x;
	else
		velocity.x = move_param_x;

	//Small hop when on ground
	if (toggle.on_ground)
		velocity.y = move_param_y;
}

void Player::respawn()
{
	// Remove one life
	--stats.lifeCount;

	// If player is not eliminated and life count drops below 0
	if (stats.lifeState != ELIMINATED && stats.lifeCount < 0)
		stats.lifeState = ELIMINATED; // Eliminate player
	else {
		//respawn and reset attributes
		body->center.x = 0;
		body->center.y = 550;
		reset_attributes();
		velocity.y = -50.0;
	}
}

void Player::death_handler()
{
	if (stats.lifeState != ELIMINATED)
	{
		// If outside boundaries
		if (body->boundary.right < SCRN_WD * -2.0f || body->boundary.left > SCRN_WD * 2.0f
			|| body->boundary.top < SCRN_HT * -2.5f || body->boundary.bottom > SCRN_HT * 2.5f)
		{
			//If player has lives and is not eliminated and not respawning
			if (stats.lifeCount >= 0 && toggle.initDeath == false) {
				// start respawn timer and update stats
				toggle.initDeath = true;
				stats.lifeState = DEAD;
				toggle.deathTimer.restart();
			}
		}

		// If player is dead and not eliminated
		if (stats.lifeState == DEAD) {
			// Check respawn timer (in seconds)
			if (toggle.deathTimer.getElapsedTime().asSeconds() >= 5.0) {
				// After 5 seconds, respawn
				toggle.initDeath = false;
				respawn();
			}
		}
	}
}

Ball::Ball() : Player()
{
	//Default Character Values
	JUMP_MAX = 4;

	//Polymorphism
	body = unique_ptr<geomelt::Circle>(new geomelt::Circle);
	outline = unique_ptr<geomelt::Circle>(new geomelt::Circle);
	reflection = unique_ptr<geomelt::Circle>(new geomelt::Circle);

	//Default Player Dimensions
	body->width = 100;
	body->height = body->width;
	body->radius = body->width / 2;
	
	outline->width = body->width;
	outline->height = body->height;
	outline->radius = body->radius;

	//Arm
	arm.width = body->width;
	arm.height = 25;
	arm.color = Assets::palette.moon;
	arm.boundary_assignment();
	armOutline.width = body->width + THICKNESS;
	armOutline.height = arm.height + THICKNESS / 2.0f;
	armOutline.color = Assets::palette.black;

	// Movement Parameters
	move_param_x = 10.0f;
	move_param_y = 5.0f;

	//Player Boundaries
	body->boundary_assignment();

	outline->radius = body->radius + 4;
	eye.radius = (body->radius / 10) + 1;
	reflection->radius = body->radius - body->radius / 4;

	body->center.x = 0;
	body->center.y = 200;
	outline->center.x = body->center.x;
	outline->center.y = body->center.y;

	update_reflection_x();
	reflection->center.y = body->center.y + sqrt(body->radius);

	//Color Assignment
	outline->color = Assets::palette.black;
	eye.color = Assets::palette.black;

	//Default Center
	body->center.x = 0;
	body->center.y = 0;

	//Default Direction
	direction = RIGHT;
}

void Ball::render()
{
	if (stats.lifeState == ALIVE)	{
		outline->render();
	
		if (toggle.attacking) {
			armOutline.render();
			arm.render();
		}

		body->render();
		reflection->render();
		eye.render();
	}
}

void Ball::update_position(vector<Platform> plat)
{
	if (stats.lifeState == ALIVE)	{
		physics(plat);
		simple_update();

		if (toggle.attacking)
			attack();

		if (toggle.walking)
			move(direction);

		body->boundary_assignment();
	}
}

void Ball::physics(vector<Platform> plat)
{
	if (stats.lifeState == ALIVE)
	{
		//This character is less affected by gravity
		velocity.y -= 3.0f * GLfloat(GRAVITY) / 4.0f;

		body->center.y += velocity.y;
		body->center.x += velocity.x;

		//Check contact with each platform in level
		for (unsigned int i = 0; i < plat.size(); i++) {
			if (body->boundary.bottom <= plat.at(i).body.boundary.top
				&& body->boundary.bottom > plat.at(i).body.boundary.bottom
				&& body->center.x >= plat.at(i).body.boundary.left
				&& body->center.x <= plat.at(i).body.boundary.right
				&& velocity.y <= 0.0)
			{
				//Reset attributes
				toggle.on_ground = true;
				jumpCount = 0;
				velocity.y *= -0.25f;
				body->center.y = plat.at(i).body.boundary.top + body->height / 2;
				break;
			}
			else
				toggle.on_ground = false;
		}

		//affect horizontal momentum with friction
		if (velocity.x < 0.0 && toggle.on_ground) {
			velocity.x += FRICTION;
			if (velocity.x > 0.0)
				velocity.x = 0.0;
		} else if (velocity.x > 0.0 && toggle.on_ground) {
			velocity.x -= FRICTION;
			if (velocity.x < 0.0)
				velocity.x = 0.0;
		}

		//Reset size
		if (toggle.on_ground)
			body->radius = body->width / 2;
	}
}

void Ball::jump()
{
	toggle.on_ground = false;

	if (jumpCount < JUMP_MAX) {
		exhale(); //Change character's size
		velocity.y = JUMP_PARAM; //Vertical Velocity
		jumpCount++;
	}
}

void Ball::exhale()
{
	body->radius *= 0.9f;
}

Boxy::Boxy() : Player()
{
	//Default Character Values
	JUMP_MAX = 2;

	body = unique_ptr<geomelt::Quad>(new geomelt::Quad);
	outline = unique_ptr<geomelt::Quad>(new geomelt::Quad);
	reflection = unique_ptr<geomelt::Quad>(new geomelt::Quad);

	//Default Player Dimensions
	body->width = 100;
	body->height = body->width;
	body->radius = body->width / 2;

	//Arm
	arm.width = 100;
	arm.height = 25;
	arm.boundary_assignment();
	armOutline.width = 108;
	arm.height = 29;
	armOutline.color = Assets::palette.black;

	// Movement Parameters
	move_param_x = 15.0f;
	move_param_y = 5.0f;

	//Player Boundaries
	body->boundary_assignment();
	body->center.x = 0;
	body->center.y = 0;

	eye.width = body->width / 20 + 1;
	eye.height = eye.width;
	eye.radius = eye.width;

	outline->width = body->width + 8;
	outline->height = outline->width;
	outline->center.x = body->center.x;
	outline->center.y = body->center.y;

	reflection->width = body->width / 1.5f;
	reflection->height = reflection->width;
	reflection->radius = reflection->width / 2;
	update_reflection_x();
	reflection->center.y = body->center.y + sqrt(body->radius) * 2;

	//Color Assignment
	outline->color = Assets::palette.black;
	eye.color = Assets::palette.black;

	//Default Direction
	direction = LEFT;
}

void Boxy::render()
{
	//Render player while alive
	if (stats.lifeState == ALIVE)
	{
		// Render all player objects
		outline->render();

		// If player is attacking
		if (toggle.attacking) {
			armOutline.render();
			arm.render();
		}

		body->render();
		reflection->render();
		eye.render();
	}
}

void Boxy::update_position(vector<Platform> plat)
{
	// If player is alive
	if (stats.lifeState == ALIVE) {
		// Compute physics
		physics(plat);
		simple_update();

		if (toggle.attacking)
			attack();

		if (toggle.walking)
			move(direction);

		body->boundary_assignment();

		reflection->center.y = body->center.y + sqrt(body->radius) * 1.5f;
	}
}

void Boxy::physics(vector<Platform> plat)
{
	// If player is alive
	if (stats.lifeState == ALIVE) {
		// Apply gravity
		// Manipulate x and y velocity

		velocity.y -= GLfloat(GRAVITY);
		body->center.y += velocity.y;
		body->center.x += velocity.x;

		for (unsigned int i = 0; i < plat.size(); i++) {
			if (body->boundary.bottom <= plat.at(i).body.boundary.top
				&& body->boundary.bottom > plat.at(i).body.boundary.bottom
				&& body->center.x >= plat.at(i).body.boundary.left
				&& body->center.x <= plat.at(i).body.boundary.right
				&& velocity.y <= 0.0)
			{
				// Player is on ground
				// Reset jumpcount
				// Lose vertical momentum 
				// Assign player center

				toggle.on_ground = true;
				jumpCount = 0;
				velocity.y *= -0.25f;
				body->center.y = plat.at(i).body.boundary.top + body->height / 2;
				break;
			}
			else // Else not on ground
				toggle.on_ground = false;
		}

		//apply friction
		if (toggle.on_ground) {
			if (velocity.x < 0.0) {
				velocity.x += FRICTION;
				if (velocity.x > 0.0)
					velocity.x = 0.0;
			}
			else if (velocity.x > 0.0) {
				velocity.x -= FRICTION;
				if (velocity.x < 0.0)
					velocity.x = 0.0;
			}
		}
	}
}

void Boxy::jump()
{
	toggle.on_ground = false;

	//Check if jumpcount is less than jumpmax
	if (jumpCount < JUMP_MAX) {
		// vertical velosity modification
		// add one to jumpcount
		velocity.y = JUMP_PARAM;
		jumpCount++;
	}
}

Attributes::Attributes()
{
	// Default attribute assignment
	health = 0.0f;
	lifeCount = 4;
	lifeState = ALIVE;
}

Toggle::Toggle()
{
	initDeath = false;
	attacking = false;
	walking = false;
	on_ground = false;
}