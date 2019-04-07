#include "camera.h"

float Camera::ZOOM = 1.25f;
float Camera::aspect_ratio = (float)SCRN_WD / (float)SCRN_HT;
float Camera::xMin = (float)INT_MAX;
float Camera::xMax = (float)INT_MIN;
float Camera::yMin = (float)INT_MAX;
float Camera::yMax = (float)INT_MIN;
float Camera::competitionXLeft = -400.0f;
float Camera::competitionXRight = 400.0f;
Boundary Camera::ortho = Boundary(SCRN_HT, -SCRN_HT, -SCRN_WD, SCRN_WD);
Boundary Camera::edges = Boundary(SCRN_HT, -SCRN_HT, -SCRN_WD, SCRN_WD);
Vec Camera::center = Vec(0, 0, 0);

//Default, need to add level camera
void Camera::set_center()
{
	//find furthest player away from center (x and y)
	xMin = (float)INT_MAX;
	xMax = (float)INT_MIN;
	yMin = (float)INT_MAX;
	yMax = (float)INT_MIN;

	// obtain center (x,y) between ALL players
	for (map<unsigned int, unique_ptr<Player>>::iterator it = PlayerMap::_map.begin(); it != PlayerMap::_map.end(); ++it)
	{
		if (it->second->stats.lifeState == ALIVE)	{
			//Set all min and max
			if (it->second->body->center.x < xMin)
				xMin = it->second->body->center.x;

			if (it->second->body->center.x > xMax)
				xMax = it->second->body->center.x;

			if (it->second->body->center.y < yMin)
				yMin = it->second->body->center.y;

			if (it->second->body->center.y > yMax)
				yMax = it->second->body->center.y;
		}
	}

	//consider adapting this into a background where the four points are compeing vs player positions
	for (int i = 0; i < 2; i++)
	{
		//Do this for Y axis too!!!!!!!!!!!!!!
		if (competitionXLeft < xMin)
			xMin = competitionXLeft;

		if (competitionXRight > xMax)
			xMax = competitionXRight;
	}

	//update_value();
	
	center.x = (xMin + xMax) / 2.0f;
	center.y = (yMin + yMax) / 2.0f;
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

		if (xMax - xMin > yMax - yMin)
		{

			left = center.x - (xMax - xMin) * ZOOM;
			right = center.x + (xMax - xMin) * ZOOM;
			top = center.y + ((right - left) / aspect_ratio) / 2.0f;
			bottom = center.y - ((right - left) / aspect_ratio) / 2.0f;
		}
		else
		{
			top = center.y + (yMax - yMin) * ZOOM / aspect_ratio;
			bottom = center.y - (yMax - yMin) * ZOOM / aspect_ratio;
			left = center.x - (top - bottom) * aspect_ratio / 2.0f;
			right = center.x + (top - bottom) * aspect_ratio / 2.0f;
		}

		edges = Boundary(top, bottom, left, right);
	//}
}

void Camera::transition()
{
	float left = 0, right = 0, top = 0, bottom = 0;

	if (edges.right - edges.left > ortho.right - ortho.left)
	{
		left = left * FAC1;
		right = edges.right * FAC1;
		top = edges.top * FAC1;
		bottom = edges.bottom * FAC1;
	}
	else if (edges.right - edges.left < ortho.right - ortho.left)
	{
		left = edges.left * FAC2;
		right = edges.right * FAC2;
		top = edges.top * FAC2;
		bottom = edges.bottom * FAC2;
	}

	ortho = Boundary(top, bottom, left, right);
}
