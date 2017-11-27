#include <SFML/Graphics.hpp>

#include "Strain.h"


int main()
{
	constexpr int limitX = 1000;
	constexpr int limitY = 500;

	sf::RenderWindow window(sf::VideoMode(limitX, 2 * limitY), "Tenzo!");
	TenzoVisualization ten(limitX, limitY);

	HANDLE hSerial;  // For create connect to COM port

	if (initComStrain(hSerial, L"COM5"))
	{
		std::cin.get();
		return 100;
	}

	whatsPeriod(hSerial);  // *for test our connection* reading the work period

	ten.strainCalibrating(hSerial);
	while (window.isOpen())
	{
		ten.getPoints(hSerial);

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear(sf::Color::White);

		window.draw(ten);
		window.display();
	}
	return 0;
}
