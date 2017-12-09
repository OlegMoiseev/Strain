#ifndef STRAIN_GAUGE_H
#define STRAIN_GAUGE_H

#include <Windows.h>
#include <array>


/**
 * \brief Main class of communication between user and strain gauge
 */
class StrainGauge
{
	/**
	 * \brief Declaration of serial port
	 */
	HANDLE _hSerial;

	/**
	 * \brief Name of the serial port in the form: L"COM5"
	 */
	LPCTSTR _sPortName;

	/**
	 * \brief Size of buffer for sending data
	 */
	DWORD _iSize;

	/**
	 * \brief Recieved code from the strain gauge
	 */
	int16_t _sReceivedChar;

	/**
	 * \brief Received code with information about the data packet
	 */
	char _skip;

	/**
	 * \brief Array of chars for sensor with info to get data packet
	 */
	char _sSendChar[5];

	/**
	 * \brief Function of initializing of COM port
	 * \return True if successful
	 */
	bool initializeCom();

public:
	/**
	 * \brief Constructor of Strain gauge with port of connection
	 * \param[in] port Name of the port with sensor in the form: L"COM5"
	 */
	explicit StrainGauge(const LPCTSTR port);

	/**
	 * \brief Function of the connect to the strain gauge
	 * \return True if successful
	 */
	bool connect();

	/**
	 * \brief Function of the getting data array from the strain gauge
	 * \param[out] data 3 strengths and 3 moments from the sensor in conventional units
	 */
	void readComStrain(std::array<int, 6>& data);

	/**
	 * \brief Function of getting period of working and writing it on the screen
	 */
	void whatsPeriod();

	/**
	 * \brief Destructor with closing of the port connection
	 */
	~StrainGauge();
};
#endif // STRAIN_GAUGE_H
