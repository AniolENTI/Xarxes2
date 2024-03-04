#include "Window.h"

Window::Window(unsigned int width, unsigned int height, std::string title)
{
	_window.create(sf::VideoMode(width, height), title);
}

void Window::AddButton(Button* button)
{
	_mutex.lock();

	_buttons.push_back(button);
	_objectsToDraw.push_back(button);

	_mutex.unlock();
}

void Window::RunWindowLoop()
{
	while (_window.isOpen())
	{
		_window.clear(sf::Color::Black);
		for (sf::Drawable * drawable : _objectsToDraw)
		{
			if (drawable != nullptr)
			{
				_window.draw(*drawable);
			}
		}
		_window.display();

		sf::Event event;
		while (_window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				{
				_window.close();
				break;
				}
			case sf::Event::MouseButtonPressed:
				{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					sf::Vector2i clickPixelPos = { event.mouseButton.x, event.mouseButton.y };
					sf::Vector2f worldPos = _window.mapPixelToCoords(clickPixelPos);

					for (auto it = _buttons.rbegin(); it != _buttons.rend(); it++)
					{
						Button* bt = *it;

						if (bt->CheckBounds(worldPos.x, worldPos.y))
						{
							_lastClickedDownButton = bt;
							break;
						}
					}
				}
				break;
				}
			case sf::Event::MouseButtonReleased:
				{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					sf::Vector2i clickPixelPos = { event.mouseButton.x, event.mouseButton.y };
					sf::Vector2f worldPos = _window.mapPixelToCoords(clickPixelPos);

					if (_lastClickedDownButton != nullptr && _lastClickedDownButton->CheckBounds(worldPos.x, worldPos.y))
					{
						_lastClickedDownButton->onClick();
						_lastClickedDownButton = nullptr;
					}
					else
					{
						//Cancel click or drag or others
					}
				}
				break;
				}
			case sf::Event::MouseWheelScrolled:
				{
				//Todo
				break;
				}
			default:
				{
				break;
				}
			}
		}
	}
}
