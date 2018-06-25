#pragma once
/*
switch (event.type)
{
// Key events
case Event::KeyPressed:  myKeys[event.Key.Code] = true;  break;
case Event::KeyReleased: myKeys[event.Key.Code] = false; break;

// Mouse event
case Event::MouseButtonPressed:  myMouseButtons[event.MouseButton.Button] = true;  break;
case Event::MouseButtonReleased: myMouseButtons[event.MouseButton.Button] = false; break;

// Mouse move event
case Event::MouseMoved:
myMouseX = event.MouseMove.X;
myMouseY = event.MouseMove.Y;
break;

// Joystick button events
case Event::JoyButtonPressed:  myJoystickButtons[event.JoyButton.JoystickId][event.JoyButton.Button] = true;  break;
case Event::JoyButtonReleased: myJoystickButtons[event.JoyButton.JoystickId][event.JoyButton.Button] = false; break;

// Joystick move event
case Event::JoyMoved:
myJoystickAxis[event.JoyMove.JoystickId][event.JoyMove.Axis] = event.JoyMove.Position;
break;

// Lost focus event : we must reset all persistent states
case Event::LostFocus:
ResetStates();
break;

default:
break;
}

void create_font()
{
	// Create some text to draw on top of our OpenGL object
	Font font;
	if (!font.loadFromFile("resources/sansation.ttf"))
		cout << "Error loading font\n";
	Text text("SFML / OpenGL demo", font);
	Text sRgbInstructions("Press space to toggle sRGB conversion", font);
	Text mipmapInstructions("Press return to toggle mipmapping", font);
	text.setFillColor(Color(255, 255, 255, 170));
	sRgbInstructions.setFillColor(Color(255, 255, 255, 170));
	mipmapInstructions.setFillColor(Color(255, 255, 255, 170));
	text.setPosition(250.f, 450.f);
	sRgbInstructions.setPosition(150.f, 500.f);
	mipmapInstructions.setPosition(180.f, 550.f);
}

void draw_text(RenderWindow win, Text txt)
{
	// Draw some text on top of our OpenGL object
	win.pushGLStates();
	win.draw(txt);
	//win.draw(sRgbInstructions);
	//win.draw(mipmapInstructions);
	win.popGLStates();
}

void key_press()
{
	if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space))
	{
		game.sRgb = !game.sRgb;
		window.close();
	}
}

// Adjust the viewport when the window is resized
if (event.type == sf::Event::Resized)
{
// Make the window the active window for OpenGL calls
window.setActive(true);

glViewport(0, 0, event.size.width, event.size.height);

// Make the window no longer the active window for OpenGL calls
window.setActive(false);
}
*/