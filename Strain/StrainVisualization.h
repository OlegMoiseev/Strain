#ifndef STRAIN_VISUALIZATION_H
#define STRAIN_VISUALIZATION_H

#include <SFML/Graphics.hpp>

#include "StrainGauge.h"


/**
 * \brief Class of visualising data from the strain gauge
 */
class StrainVisualization : public sf::Drawable, public StrainGauge
{
	/**
	 * \brief Array of data from the sensor
	 */
	std::array<int, 6> _data;

	/**
	 * \brief Limit of the X axis on the chart
	 */
	const int _limitX;

	/**
	* \brief Limit of the Y axis on the chart
	*/
	const int _limitY;

	/**
	* \brief Vectors of the data on 6 axises
	*/
	std::vector<int> _y1, _y2, _y3, _m1, _m2, _m3;

	/**
	 * \brief Data for correcting
	 */
	std::array<int, 6> _calibrate;

	/**
	* \brief Arrays of vertexes to draw on the screen
	*/
	sf::VertexArray _forceX, _forceY, _forceZ, _torqueX, _torqueY, _torqueZ;

	/**
	* \brief Arrays of vertexes to draw coordinate system on the screen
	*/
	sf::VertexArray _coordSystem;

	/**
	 * \brief Function to draw in the screen
	 * \param target Hui znaet
	 * \param states Tozhe samoe
	 */
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	/**
	 * \brief Constructor with the indication of the window size and fort of the strain gauge
	 * \param[in] maxX Window width
	 * \param[in] maxY Window height
	 * \param[in] port Name of the port
	 */
	StrainVisualization(const int maxX, const int maxY, const LPCTSTR port);

	/**
	 * \brief Function of the data calibrating
	 */
	void strainCalibrating();

	/**
	 * \brief Function of the getting and drawing data from the strain gauge
	 */
	void getPoints();
};
#endif // STRAIN_VISUALIZATION_H
