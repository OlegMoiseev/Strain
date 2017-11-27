#ifndef STRAIN_H
#define STRAIN_H

#include <iostream>
#include <Windows.h>
#include <array>

int initComStrain(HANDLE& hSerial, LPCTSTR sPortName)
{
	hSerial = ::CreateFile(sPortName, GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
	                       nullptr);
	if (hSerial == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			std::cout << "Serial port does not exist" << std::endl;
			return 1001;
		}
		std::cout << "Some other error occurred" << std::endl;
		return 1002;
	}
	DCB dcbSerialParams = {0};
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (!GetCommState(hSerial, &dcbSerialParams))
	{
		std::cout << "Getting state error" << std::endl;
		return 1003;
	}
	dcbSerialParams.BaudRate = CBR_9600;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;
	if (!SetCommState(hSerial, &dcbSerialParams))
	{
		std::cout << "Error setting serial port state" << std::endl;
		return 1004;
	}
	std::cout << "Connected successfully!" << std::endl;
	DWORD iSize;
	char sStopChar[5] = {0x7, 0x0, 0x0, 0x0, 0x0};
	WriteFile(hSerial, &sStopChar, 5, &iSize, nullptr);
	return 0;
}

void readCOMStrain(HANDLE hSerial, std::array<int, 6>& data)
{
	DWORD iSize;
	int16_t sReceivedChar; // For recieve data from COM port
	char skip;
	char period = 0x000186A0;

	char sSendChar[5] = {0x1, 0x0, 0x0, 0x0, 0x0};
	WriteFile(hSerial, &sSendChar, 5, &iSize, nullptr);

	ReadFile(hSerial, &skip, 1, &iSize, nullptr); // получаем 1 байт кода ответа
	//std::cout << "Skipped: " << skip + '0' - 48 << std::endl;
	for (size_t i = 0; i < 6; ++i)
	{
		ReadFile(hSerial, &sReceivedChar, 2, &iSize, nullptr); // получаем 2 байтa
		data.at(i) = sReceivedChar;
	}
}

void whatsPeriod(HANDLE hSerial)
{
	DWORD iSize;
	uint16_t sReceivedChar;
	char skip;
	uint32_t period;

	char sStopChar[5] = {0x7, 0x0, 0x0, 0x0, 0x0};
	WriteFile(hSerial, &sStopChar, 5, &iSize, nullptr);

	char sSendChar[5] = {0x8, 0x0, 0x0, 0x0, 0x0};
	WriteFile(hSerial, &sSendChar, 5, &iSize, nullptr);
	ReadFile(hSerial, &skip, 1, &iSize, nullptr); // получаем 1 байт кода ответа
	std::cout << "Skipped: " << skip + '0' - 48 << std::endl;
	ReadFile(hSerial, &period, 4, &iSize, nullptr); // получаем 4 байта
	std::cout << "Period: " << period << std::endl;
}


class TenzoVisualization : public sf::Drawable
{
	std::array<int, 6> _data;
	const int _limitX;
	const int _limitY;
	std::vector<int> _y1, _y2, _y3, _m1, _m2, _m3;
	std::array<int, 6> _calibrate = {{0, 0, 0, 0, 0, 0}};

	sf::VertexArray _powerX;
	sf::VertexArray _powerY;
	sf::VertexArray _powerZ;
	sf::VertexArray _momentX;
	sf::VertexArray _momentY;
	sf::VertexArray _momentZ;

	sf::VertexArray _coordSystem;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(_coordSystem);
		target.draw(_momentX);
		target.draw(_momentY);
		target.draw(_momentZ);
		target.draw(_powerX);
		target.draw(_powerY);
		target.draw(_powerZ);
	};

public:
	TenzoVisualization(const int maxX, const int maxY) :
		_limitX(maxX),
		_limitY(maxY)
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
	};

	void strainCalibrating(HANDLE& hSerial)
	{
		const int numberOfReadings = 50;

		std::array<int, 6> tmp;
		for (std::size_t i = 0; i < numberOfReadings; ++i)
		{
			readCOMStrain(hSerial, tmp);
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

	void getPoints(HANDLE& hSerial)
	{
		readCOMStrain(hSerial, _data);

		//system("cls");
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
			_powerZ.append(sf::Vertex(sf::Vector2f(i, _limitY - _y3.at(i) / 4), sf::Color::Green));

			_momentX.append(sf::Vertex(sf::Vector2f(i, _limitY - (_m1.at(i) - _calibrate.at(3)) / 4), sf::Color::Magenta));
			_momentY.append(sf::Vertex(sf::Vector2f(i, _limitY - (_m2.at(i) - _calibrate.at(4)) / 4), sf::Color::Cyan));
			_momentZ.append(sf::Vertex(sf::Vector2f(i, _limitY - (_m3.at(i) - _calibrate.at(5)) / 4), sf::Color::Black));

		}
	}
};
#endif // STRAIN_H
