#include "player.h"
map<unsigned int, unique_ptr<Player>> PlayerMap::_map;

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
	availableIDs.push_back(this->myID);
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

void Player::physics(PlatformGroup plat)
{
	if (stats.lifeState == ALIVE)
	{
		//This character is less affected by gravity
		velocity.y -= weight;
		body->center.y += velocity.y;
		body->center.x += velocity.x;

		//Check contact with each platform in level
		for (vector<Platform>::iterator it = plat._platforms.begin(); it != plat._platforms.end(); ++it) {
			if (body->boundary.isWithin(it->_body.boundary, body->center.x)
				&& velocity.y <= 0.0) {
				//Reset attributes
				toggle.on_ground = true;
				jumpCount = 0;
				velocity.y *= -0.25f;
				body->center.y = it->_body.boundary.top + body->height / 2;
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
		}
		else if (velocity.x > 0.0 && toggle.on_ground) {
			velocity.x -= FRICTION;
			if (velocity.x < 0.0)
				velocity.x = 0.0;
		}

		//Reset size
		if (toggle.on_ground) {
			body->radius = body->width / 2; // divide by 1.999f to slowly inflate
			body->width = body->radius * 2;
			body->height = body->width;
		}
	}
}

// p1 is attacking p2
bool Player::isAttacking(const Player& p)
{
	// GRAB (x,y) of arm corner coors
	vector<pair<float, float>> coordinates; //attacking player coordinates

	coordinates.push_back({ arm.center.x + arm.width / 2.0, arm.center.y + (arm.height / 2.0) });
	coordinates.push_back({ arm.center.x + arm.width / 2.0, arm.center.y - (arm.height / 2.0) });
	coordinates.push_back({ arm.center.x - arm.width / 2.0, arm.center.y + (arm.height / 2.0) });
	coordinates.push_back({ arm.center.x - arm.width / 2.0, arm.center.y - (arm.height / 2.0) });

	bool within = false;
	for (auto coor : coordinates) {
		if (coor.first <= p.body->boundary.right && coor.first >= p.body->boundary.left &&
			(coor.second <= p.body->boundary.top || coor.second >= p.body->boundary.bottom) ) {
			within = true;
		}
	}

	return within;
}

// p1 is attacked by p2
void Player::attackedBy(const Player& p)
{
	const float baseDamageX = 5.0;
	const float baseDamageY = 5.0;

	Direction flightDirection = p.direction;

	if (flightDirection == RIGHT) {
		this->velocity.x = baseDamageX * damageMultiplier;
		this->velocity.y = baseDamageY * damageMultiplier;
	}
	else {
		this->velocity.x = -baseDamageX * damageMultiplier;
		this->velocity.y = baseDamageY * damageMultiplier;
	}
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
		arm.center.x = body->center.x + 3 * body->width / 4;
	else if (direction == LEFT)
		arm.center.x = body->center.x - 3 * body->width / 4;

	arm.center.y = body->center.y;
	armOutline.center.x = arm.center.x;
	armOutline.center.y = arm.center.y;

	// After 25 milliseconds, stop rendering
	if (toggle.attackTimer.getElapsedTime().asMilliseconds() > 35)
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
	outline->center = Vec(body->center.x, body->center.y, 0);

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
		velocity.x = -speed_x;
	else
		velocity.x = speed_x;

	//Small hop when on ground
	if (toggle.on_ground)
		velocity.y = speed_y;
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

Player::Player()
{
	myID = extract_lowest_ID();
	weight = 0.0f;
	jumpMax = 3;
	jumpCount = 0;
	velocity = Vec(0, 0, 0);
	direction = RIGHT;
	mDimension = 100;
	myColor = (CharColorOptions)myID;
	mColor = Assets::characterPalette.colors.at(myColor);
	
	eye = Circle(
		mDimension,
		mDimension,
		(mDimension / 20.0f) + 1,
		mColor.outline,
		Vec(0, 0, 0)
	);

	arm = Quad(
		mDimension,
		mDimension / 4.0f,
		0,
		mColor.body,
		velocity
	);

	armOutline = Quad(
		mDimension + THICKNESS,
		mDimension / 4.0f + THICKNESS / 2.0f,
		0,
		mColor.outline,
		velocity
	);
}

Ball::Ball() : Player()
{
	// Movement Parameters
	weight = 3.0f * GLfloat(GRAVITY) / 4.0f;
	speed_x = 10.0f;
	speed_y = 5.0f;
	jumpMax = 4;
	damageMultiplier = 1.5f;

	body = unique_ptr<Circle>(
		new Circle(
			mDimension,
			mDimension,
			mDimension / 2.0f,
			mColor.body,
			Vec(0, 0, 0)
		)
	);

	outline = unique_ptr<Circle>(
		new Circle(
			mDimension,
			mDimension,
			mDimension / 2.0f,
			mColor.outline,
			Vec(0, 0, 0)
		)
	);

	reflection = unique_ptr<Circle>(
		new Circle(
			mDimension,
			mDimension,
			body->radius - body->radius / 4.0f,
			mColor.reflection,
			Vec(0, 0, 0)
		)
	);
}

void Player::render()
{
	if (stats.lifeState == ALIVE) {
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

void Player::update(PlatformGroup plat)
{
	if (stats.lifeState == ALIVE) {
		physics(plat);
		simple_update();

		if (toggle.attacking)
			attack();

		if (toggle.walking)
			move(direction);

		body->boundary_assignment();
	}
}

void Ball::update(PlatformGroup plat)
{
	Player::update(plat);
}

void Ball::jump()
{
	toggle.on_ground = false;

	if (jumpCount < jumpMax) {
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
	direction = LEFT;
	weight = GLfloat(GRAVITY * 0.5f);
	jumpMax = 2;
	speed_x = 15.0f;
	speed_y = 5.0f;
	damageMultiplier = 2.0f;

	body = unique_ptr<Shape>(
		new Quad(
			mDimension,
			mDimension,
			mDimension / 2.0f,
			mColor.body,
			Vec(0, 0, 0)
		)
	);

	outline = unique_ptr<Shape>(
		new Quad(
			mDimension + THICKNESS,
			mDimension + THICKNESS,
			mDimension / 2.0f,
			mColor.outline,
			Vec(0, 0, 0)
		)
	);

	reflection = unique_ptr<Shape>(
		new Quad(
			body->width / 1.5f,
			body->width / 1.5f,
			body->width / 3.0f,
			mColor.reflection,
			Vec(0, 0, 0)
		)
	);
}

void Boxy::update(PlatformGroup plat)
{
	Player::update(plat);

	if (stats.lifeState == ALIVE) 
		reflection->center.y = body->center.y + sqrt(body->radius) * 1.5f;
}

void Boxy::jump()
{
	if (toggle.on_ground)
		toggle.on_ground = false;

	//Check if jumpcount is less than jumpmax
	if (jumpCount < jumpMax) {
		velocity.y = JUMP_PARAM; // vertical velosity modification
		jumpCount++; // add one to jumpcount
	}
}

Attributes::Attributes()
{
	health = 0.0f; // Should be relabeled as damage
	lifeCount = 4; // 5 total
	lifeState = ALIVE;
}

Toggle::Toggle()
{
	initDeath = false;
	attacking = false;
	walking = false;
	on_ground = false;
}

/* Add player corresponding to joyID */
void PlayerMap::add(unsigned int joyID)
{
	bool is_created_already = false;

	if (_map.size() >= 0 && _map.size() < 8) {
		map<unsigned int, unique_ptr<Player>>::iterator it;

		for (it = _map.begin(); it != _map.end(); ++it) {
			if (it->first == joyID) // If map exists
				is_created_already = true; //Don't create
		}

		if (is_created_already == false) {
			_map[joyID] = unique_ptr<Player>(new Ball());

			Player *plyr = _map[joyID].get();
			plyr->mColor = Assets::characterPalette.colors.at(plyr->myColor);
		}
	}
}

/* Erase player[id] */
void PlayerMap::purge(unsigned int id)
{
	_map.erase(id);
}

/* Function that allows for character to transform to different concrete player type*/
void PlayerMap::transform(unsigned int id)
{
	//if present
	if (_map.find(id) != _map.end()) {
		//check type
		if (typeid(*_map[id]).name() == typeid(Ball).name()) {
			_map[id].reset();
			_map[id] = unique_ptr<Player>(new Boxy);
		}
		else {
			_map[id].reset();
			_map[id] = unique_ptr<Player>(new Ball);
		}
	}
}

/* Function that toggles an attack */
void PlayerMap::attack(unsigned int id)
{
	if (_map.find(id) != _map.end()) {
		_map[id]->toggle.attacking ^= 1;
		_map[id]->toggle.attackTimer.restart();
	}
}

/* Function that triggers player[id] to jump */
void PlayerMap::jump(unsigned int id)
{
	if (_map.find(id) != _map.end())
		_map[id]->jump();
}

/* Function that triggers player[id] to move */
void PlayerMap::move(unsigned int id, Direction dir)
{
	if (_map.find(id) != _map.end()) {
		_map[id]->toggle.walking = true;
		_map[id]->direction = dir;
	}
}

/* Function that triggers player[id] to change to next or previous color */
void PlayerMap::change_color(unsigned int id, SelectColor dir)
{
	if (_map.find(id) != _map.end())
		_map[id]->change_color(dir);
}

/* Function that toggles player[id] to stop */
void PlayerMap::stop(unsigned int id)
{
	if (_map.find(id) != _map.end()) {
		_map[id]->toggle.walking = false;
	}
}

/* Function that erases all players */
void PlayerMap::clear()
{
	_map.clear();
}

/* Function that returns # of players present */
unsigned int PlayerMap::size()
{
	return _map.size();
}

void PlayerMap::render()
{
	for (map<unsigned int, unique_ptr<Player>>::iterator it = _map.begin(); it != _map.end(); ++it) {
		if (it->second->stats.lifeState == ALIVE)
			it->second->render();
	}
}

void PlayerMap::options_render(vector<CharSelBox> selectBox)
{
	map<unsigned int, unique_ptr<Player>>::iterator it = _map.begin();

	while (it != _map.end()) {
		it->second->mColor = Assets::characterPalette.colors.at(it->second->myColor);
		it->second->body->center = selectBox[it->second->myID].box.center;
		it->second->simple_update_menu();
		it->second->render();

		it++;
	}
}

void PlayerMap::phys_handler(PlatformGroup plat)
{
	for (map<unsigned int, unique_ptr<Player>>::iterator it = _map.begin(); it != _map.end(); ++it) {
		if (it->second->stats.lifeState != ELIMINATED)
			it->second->update(plat);
		else
			_map.erase(it);

		it->second->death_handler();
	}
}

void PlayerMap::reset()
{
	for (map<unsigned int, unique_ptr<Player>>::iterator it = _map.begin(); it != _map.end(); ++it) {
		it->second->body->height = 100;
		it->second->body->width = 100;
		it->second->body->radius = 50;
		it->second->body->boundary_assignment();
		it->second->toggle.on_ground = true;
		it->second->update_reflection_x();
		it->second->simple_update();

		if (it->second->myID % 2 == 0)
			it->second->body->center = Vec(500.0f, 500.0f, 0);
		else
			it->second->body->center = Vec(-500.0f, 500.0f, 0);

	}
}
