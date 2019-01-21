#include "camera.h"

float Camera::ZOOM = 1.25f;
float Camera::aspect_ratio = (float)HDX / (float)HDY;
float Camera::xMin = (float)INT_MAX;
float Camera::xMax = (float)INT_MIN;
float Camera::yMin = (float)INT_MAX;
float Camera::yMax = (float)INT_MIN;
float Camera::competitionXLeft = -400.0f;
float Camera::competitionXRight = 400.0f;
medmelt::Boundary Camera::ortho = medmelt::Boundary::setBounds(HDY, -HDY, -HDX, HDX);
medmelt::Boundary Camera::edges = medmelt::Boundary::setBounds(HDY, -HDY, -HDX, HDX);
Vec Camera::center = Vec::set_Vec(0, 0, 0);

//Default, need to add level camera
void Camera::set_center(map<unsigned int, unique_ptr<Player>>& player)
{
	//find furthest player away from center (x and y)
	xMin = (float)INT_MAX;
	xMax = (float)INT_MIN;
	yMin = (float)INT_MAX;
	yMax = (float)INT_MIN;

	map<unsigned int, unique_ptr<Player>>::iterator it;

	// obtain center (x,y) between ALL players
	for (it = player.begin(); it != player.end(); ++it)
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
		cout << "ADD";
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
	if (xMax - xMin < HDX / 2.0f && yMax - yMin < HDY / 2.0f)
	{
		edges.left = ortho.left;
		edges.right = ortho.right;
		edges.top = ortho.top;
		edges.bottom = ortho.bottom;
	}
	else
	{
	*/
		if (xMax - xMin > yMax - yMin)
		{
			edges.left = center.x - (xMax - xMin) * ZOOM;
			edges.right = center.x + (xMax - xMin) * ZOOM;
			edges.top = center.y + ((edges.right - edges.left) / aspect_ratio) / 2.0f;
			edges.bottom = center.y - ((edges.right - edges.left) / aspect_ratio) / 2.0f;
		}
		else
		{
			edges.top = center.y + (yMax - yMin) * ZOOM / aspect_ratio;
			edges.bottom = center.y - (yMax - yMin) * ZOOM / aspect_ratio;
			edges.left = center.x - (edges.top - edges.bottom) * aspect_ratio / 2.0f;
			edges.right = center.x + (edges.top - edges.bottom) * aspect_ratio / 2.0f;
		}
	//}
}

void Camera::transition()
{
	if (edges.right - edges.left > ortho.right - ortho.left)
	{
		ortho.left = edges.left * FAC1;
		ortho.right = edges.right * FAC1;
		ortho.top = edges.top * FAC1;
		ortho.bottom = edges.bottom * FAC1;
	}
	else if (edges.right - edges.left < ortho.right - ortho.left)
	{
		ortho.left = edges.left * FAC2;
		ortho.right = edges.right * FAC2;
		ortho.top = edges.top * FAC2;
		ortho.bottom = edges.bottom * FAC2;
	}
}