#include "StrainVisualization.h"

#include <iostream>


void StrainVisualization::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_coordSystem);
	target.draw(_torqueX);
	target.draw(_torqueY);
	target.draw(_torqueZ);
	target.draw(_forceX);
	target.draw(_forceY);
	target.draw(_forceZ);
}

StrainVisualization::StrainVisualization(const int maxX, const int maxY, const LPCTSTR port)
	: StrainGauge(port),
	  _limitX(maxX),
	  _limitY(maxY)
{
	// Fill array with zeros.
	_calibrate.fill(0);

	_coordSystem = sf::VertexArray(sf::LineStrip, 4);
	_coordSystem.append(sf::Vertex(sf::Vector2f(1, 0), sf::Color::Red));
	_coordSystem.append(sf::Vertex(sf::Vector2f(1, 2 * _limitY), sf::Color::Red));
	_coordSystem.append(sf::Vertex(sf::Vector2f(1, _limitY), sf::Color::Red));
	_coordSystem.append(sf::Vertex(sf::Vector2f(_limitX, _limitY), sf::Color::Red));

	_forceX = sf::VertexArray(sf::LineStrip, _limitX);
	_forceY = sf::VertexArray(sf::LineStrip, _limitX);
	_forceZ = sf::VertexArray(sf::LineStrip, _limitX);
	_torqueX = sf::VertexArray(sf::LineStrip, _limitX);
	_torqueY = sf::VertexArray(sf::LineStrip, _limitX);
	_torqueZ = sf::VertexArray(sf::LineStrip, _limitX);

	_y1 = std::vector<int>(_limitX);
	_y2 = std::vector<int>(_limitX);
	_y3 = std::vector<int>(_limitX);
	_m1 = std::vector<int>(_limitX);
	_m2 = std::vector<int>(_limitX);
	_m3 = std::vector<int>(_limitX);
}

void StrainVisualization::strainCalibrating()
{
	constexpr int numberOfReadings = 50;

	// 6 - number of sensor's measurements.
	std::array<int, 6> tmp;
	for (std::size_t i = 0; i < numberOfReadings; ++i)
	{
		readComStrain(tmp);
		for (std::size_t j = 0; j < tmp.size(); ++j)
		{
			_calibrate.at(j) += tmp.at(j);
		}
	}
	for (std::size_t i = 0; i < tmp.size(); ++i)
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

	_forceX.clear();
	_forceY.clear();
	_forceZ.clear();
	_torqueX.clear();
	_torqueY.clear();
	_torqueZ.clear();

	for (int i = 0; i < _limitX; ++i)
	{
		_forceX.append(sf::Vertex(sf::Vector2f(i, _limitY - (_y1.at(i) - _calibrate.at(0)) / 4),
		                          sf::Color::Red));
		_forceY.append(sf::Vertex(sf::Vector2f(i, _limitY - (_y2.at(i) - _calibrate.at(1)) / 4),
		                          sf::Color::Blue));
		_forceZ.append(sf::Vertex(sf::Vector2f(i, _limitY - (_y3.at(i) - _calibrate.at(2)) / 4),
		                          sf::Color::Green));

		_torqueX.append(sf::Vertex(sf::Vector2f(i, _limitY - (_m1.at(i) - _calibrate.at(3)) / 4),
		                           sf::Color::Magenta));
		_torqueY.append(sf::Vertex(sf::Vector2f(i, _limitY - (_m2.at(i) - _calibrate.at(4)) / 4),
		                           sf::Color::Cyan));
		_torqueZ.append(sf::Vertex(sf::Vector2f(i, _limitY - (_m3.at(i) - _calibrate.at(5))),
		                           sf::Color::Black));
	}
}
