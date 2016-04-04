#ifndef UTILS_HPP
#define UTILS_HPP

/**@file
 */

#include <glm/glm.hpp>

float random(float a, float b);

glm::vec4 randomColor();

/** @brief Get a color to represent a one dimensional parameter.
 *
 * This function allows to visually represent one dimensional parameter
 * thanks to a color code: cold colors (blue) for small values and hot colors
 * (red) for big values.
 *
 * @param factor The factor to visually represent.
 * @param low The minimum expected value for the factor (will return a blue color).
 * @param high The maximum expected value for the factor (will return a red color).
 */
glm::vec4 getColor( const float& factor, const float& low, const float& high );

#endif //UTILS
