#pragma once
#include <list>
#include <mutex>
#include "Button.h"

class Window
{
public:
	//Recordatori: Constructor crea la Finestra. No es pot tocar des d'un altre thread
	Window(unsigned int width = 800, unsigned int height = 600, std::string title = "Window");

	void AddButton(Button* button);
	void RunWindowLoop();

private:
	std::mutex _mutex;
	sf::RenderWindow _window;

	Button* _lastClickedDownButton;
	std::list<Button*> _buttons;
	std::list<sf::Drawable*> _objectsToDraw;
};

