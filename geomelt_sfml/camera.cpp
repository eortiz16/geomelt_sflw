#include "camera.h"

float Camera::_ZOOM = 1.25f;
float Camera::_aspect_ratio = (float)SCRN_WD / (float)SCRN_HT;
float Camera::_xMin = (float)INT_MAX;
float Camera::_xMax = (float)INT_MIN;
float Camera::_yMin = (float)INT_MAX;
float Camera::_yMax = (float)INT_MIN;
float Camera::_competitionXLeft = -400.0f;
float Camera::_competitionXRight = 400.0f;
Boundary Camera::_ortho = Boundary(SCRN_HT, -SCRN_HT, -SCRN_WD, SCRN_WD);
Boundary Camera::_edges = Boundary(SCRN_HT, -SCRN_HT, -SCRN_WD, SCRN_WD);
Vec Camera::center = Vec(0, 0, 0);

//Default, need to add level camera
void Camera::set_center()
{
	//find furthest player away from center (x and y)
	_xMin = (float)INT_MAX;
	_xMax = (float)INT_MIN;
	_yMin = (float)INT_MAX;
	_yMax = (float)INT_MIN;

	// obtain center (x,y) between ALL players
	for (map<unsigned int, shared_ptr<Player>>::iterator it = PlayerMap::_map.begin(); it != PlayerMap::_map.end(); ++it)
	{
		if (it->second->stats.lifeState == ALIVE)	{
			//Set all min and max
			if (it->second->body->center.x < _xMin)
				_xMin = it->second->body->center.x;

			if (it->second->body->center.x > _xMax)
				_xMax = it->second->body->center.x;

			if (it->second->body->center.y < _yMin)
				_yMin = it->second->body->center.y;

			if (it->second->body->center.y > _yMax)
				_yMax = it->second->body->center.y;
		}
	}

	//consider adapting this into a background where the four points are compeing vs player positions
	for (int i = 0; i < 2; i++)
	{
		//Do this for Y axis too!!!!!!!!!!!!!!
		if (_competitionXLeft < _xMin)
			_xMin = _competitionXLeft;

		if (_competitionXRight > _xMax)
			_xMax = _competitionXRight;
	}

	//update_value();
	
	center.x = (_xMin + _xMax) / 2.0f;
	center.y = (_yMin + _yMax) / 2.0f;
}

/*
//float val = 0.1f;
bool flag = true;
double val = 0.0;
void Camera::update_value()
{
	if (rand() % 2 == 0)
		flag = false;
	else
		flag = true;
		
	val += 0.001;

	if (flag)
	{
		competitionXLeft -= 1.0 / (1.0 / sin(val));
		competitionXRight += 1.0 / (1.0 / sin(val));
	}
	else
	{
		cout << "SUb";
		competitionXLeft += 1.0 / (1.0 / sin(val));
		competitionXRight -= 1.0 / (1.0 / sin(val));
	}
}
*/

void Camera::set_edges()
{
	// For "outside of boundary camera"
	// Add conditions
	// define minimum camera and maximum
	/*
	if (xMax - xMin < HDX / 2.0f && yMax - yMin < SCRN_HT / 2.0f)
	{
		edges.left = ortho.left;
		edges.right = ortho.right;
		edges.top = ortho.top;
		edges.bottom = ortho.bottom;
	}
	else
	{
	*/
	float left = 0, right = 0, top = 0, bottom = 0;

		if (_xMax - _xMin > _yMax - _yMin)
		{

			left = center.x - (_xMax - _xMin) * _ZOOM;
			right = center.x + (_xMax - _xMin) * _ZOOM;
			top = center.y + ((right - left) / _aspect_ratio) / 2.0f;
			bottom = center.y - ((right - left) / _aspect_ratio) / 2.0f;
		}
		else
		{
			top = center.y + (_yMax - _yMin) * _ZOOM / _aspect_ratio;
			bottom = center.y - (_yMax - _yMin) * _ZOOM / _aspect_ratio;
			left = center.x - (top - bottom) * _aspect_ratio / 2.0f;
			right = center.x + (top - bottom) * _aspect_ratio / 2.0f;
		}

		_edges = Boundary(top, bottom, left, right);
	//}
}

void Camera::transition()
{
	float left = 0, right = 0, top = 0, bottom = 0;

	if (_edges.right - _edges.left > _ortho.right - _ortho.left)
	{
		left = left * FAC1;
		right = _edges.right * FAC1;
		top = _edges.top * FAC1;
		bottom = _edges.bottom * FAC1;
	}
	else if (_edges.right - _edges.left < _ortho.right - _ortho.left)
	{
		left = _edges.left * FAC2;
		right = _edges.right * FAC2;
		top = _edges.top * FAC2;
		bottom = _edges.bottom * FAC2;
	}

	_ortho = Boundary(top, bottom, left, right);
}

int Camera::translateX(int x)
{
	int range = abs(_edges.left) + abs(_edges.right);
	
}

int Camera::translatey(int x)
{
	int range = abs(_edges.top) + abs(_edges.bottom);
}
