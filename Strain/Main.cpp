#include <SFML/Graphics.hpp>

#include "StrainGauge.h"
#include "StrainVisualization.h"

int main()
{
	constexpr int limitX = 1000;
	constexpr int limitY = 500;

	sf::RenderWindow window(sf::VideoMode(limitX, 2 * limitY), "Tenzo!");

	StrainVisualization ten(limitX, limitY, L"COM5");

	ten.connect();

	ten.whatsPeriod();  // *for test our connection* reading the work period

	ten.strainCalibrating();

	while (window.isOpen())
	{
		ten.getPoints();

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

