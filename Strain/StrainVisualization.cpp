#include "StrainVisualization.h"

#include <iostream>


void StrainVisualization::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_coordSystem);
	target.draw(_momentX);
	target.draw(_momentY);
	target.draw(_momentZ);
	target.draw(_powerX);
	target.draw(_powerY);
	target.draw(_powerZ);
}

StrainVisualization::StrainVisualization(const int maxX, const int maxY, const LPCTSTR port)
	: StrainGauge(port),
	  _limitX(maxX),
	  _limitY(maxY),
	  _calibrate({{0, 0, 0, 0, 0, 0}})
{
	_coordSystem = sf::VertexArray(sf::LineStrip, 4);
	_coordSystem.append(sf::Vertex(sf::Vector2f(1, 0), sf::Color::Red));
	_coordSystem.append(sf::Vertex(sf::Vector2f(1, 2 * _limitY), sf::Color::Red));
	_coordSystem.append(sf::Vertex(sf::Vector2f(1, _limitY), sf::Color::Red));
	_coordSystem.append(sf::Vertex(sf::Vector2f(_limitX, _limitY), sf::Color::Red));

	_powerX = sf::VertexArray(sf::LineStrip, _limitX);
	_powerY = sf::VertexArray(sf::LineStrip, _limitX);
	_powerZ = sf::VertexArray(sf::LineStrip, _limitX);
	_momentX = sf::VertexArray(sf::LineStrip, _limitX);
	_momentY = sf::VertexArray(sf::LineStrip, _limitX);
	_momentZ = sf::VertexArray(sf::LineStrip, _limitX);

	_y1 = std::vector<int>(_limitX);
	_y2 = std::vector<int>(_limitX);
	_y3 = std::vector<int>(_limitX);
	_m1 = std::vector<int>(_limitX);
	_m2 = std::vector<int>(_limitX);
	_m3 = std::vector<int>(_limitX);
}

void StrainVisualization::strainCalibrating()
{
	const int numberOfReadings = 50;

	std::array<int, 6> tmp;
	for (std::size_t i = 0; i < numberOfReadings; ++i)
	{
		readComStrain(tmp);
		for (std::size_t j = 0; j < 6; ++j)
		{
			_calibrate.at(j) += tmp.at(j);
		}
	}
	for (std::size_t i = 0; i < 6; ++i)
	{
		_calibrate.at(i) /= numberOfReadings;
	}
}

void StrainVisualization::getPoints()
{
	readComStrain(_data);

	std::cout << _data.at(0) - _calibrate.at(0) << '\t' << _data.at(1) - _calibrate.at(1) << '\t' <<
		_data.at(2) << '\t' << _data.at(3) - _calibrate.at(3) << '\t' <<
		_data.at(4) - _calibrate.at(4) << '\t' << _data.at(5) - _calibrate.at(5) << '\n';

	_y1.erase(_y1.begin());
	_y1.emplace_back(_data.at(0));

	_y2.erase(_y2.begin());
	_y2.emplace_back(_data.at(1));

	_y3.erase(_y3.begin());
	_y3.emplace_back(_data.at(2));

	_m1.erase(_m1.begin());
	_m1.emplace_back(_data.at(3));

	_m2.erase(_m2.begin());
	_m2.emplace_back(_data.at(4));

	_m3.erase(_m3.begin());
	_m3.emplace_back(_data.at(5));

	_powerX.clear();
	_powerY.clear();
	_powerZ.clear();
	_momentX.clear();
	_momentY.clear();
	_momentZ.clear();

	for (int i = 0; i < _limitX; ++i)
	{
		_powerX.append(sf::Vertex(sf::Vector2f(i, _limitY - (_y1.at(i) - _calibrate.at(0)) / 4), sf::Color::Red));
		_powerY.append(sf::Vertex(sf::Vector2f(i, _limitY - (_y2.at(i) - _calibrate.at(1)) / 4), sf::Color::Blue));
		_powerZ.append(sf::Vertex(sf::Vector2f(i, _limitY - (_y3.at(i) - _calibrate.at(2)) / 4), sf::Color::Green));

		_momentX.append(sf::Vertex(sf::Vector2f(i, _limitY - (_m1.at(i) - _calibrate.at(3)) / 4), sf::Color::Magenta));
		_momentY.append(sf::Vertex(sf::Vector2f(i, _limitY - (_m2.at(i) - _calibrate.at(4)) / 4), sf::Color::Cyan));
		_momentZ.append(sf::Vertex(sf::Vector2f(i, _limitY - (_m3.at(i) - _calibrate.at(5))), sf::Color::Black));
	}

	/*for (int i = 0; i < _limitX; ++i)
	{
	_powerX.append(sf::Vertex(sf::Vector2f(i, _limitY - _y1.at(i) / 4), sf::Color::Red));
	_powerY.append(sf::Vertex(sf::Vector2f(i, _limitY - _y2.at(i) / 4), sf::Color::Blue));
	_powerZ.append(sf::Vertex(sf::Vector2f(i, _limitY - _y3.at(i) / 4), sf::Color::Green));

	_momentX.append(sf::Vertex(sf::Vector2f(i, _limitY - _m1.at(i) / 4), sf::Color::Magenta));
	_momentY.append(sf::Vertex(sf::Vector2f(i, _limitY - _m2.at(i) / 4), sf::Color::Cyan));
	_momentZ.append(sf::Vertex(sf::Vector2f(i, _limitY - _m3.at(i) / 4), sf::Color::Black));
	}*/
}
