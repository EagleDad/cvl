#pragma once

// STD includes

// CVL includes
#include <cvl/core/Point.h>
#include <cvl/core/Size.h>
#include <cvl/core/export.h>

namespace cvl::core
{
/**
 * @brief Implementation for a ellipse class
 *
 * Quadratic form:          ax^2 + 2bxy + cy^2 + dx + ey + f = 0
 * Parameter form:          x = a cos(phi); y = b sin(phi)
 * Common form:             x^2 / a^2 + y^2 / b^2 = 1, in center
 * Common form:             (x - x0) ^2 / a^2 + (y - y0)^2 / b^2 = 1, off center
 *
 */
class CVL_CORE_EXPORT Ellipse
{
public:
    /**
     * Create a default ellipse
     *
     */
    Ellipse( ) noexcept = default;

    /**
     * Value construct
     *
     * @param center    The center point of the ellipse
     * @param size      The size of the ellipse. Width = major axis; Height =
     *                  minor axis.
     * @param phi       The angle of the ellipse. The angel between the major
     *                  axis and the x axis.
     *
     */
    explicit Ellipse( const Point2d& center, const SizeD& size, double phi );

    /**
     * Function that returns the center of the ellipse
     *
     * @return The center point of the ellipse
     */
    [[nodiscard]] Point2d getCenter( ) const;

    /**
     * Function that returns the size of the ellipse
     *
     * @return The size of the ellipse
     */
    [[nodiscard]] SizeD getSize( ) const;

    /**
     * Function that returns the angle of the ellipse
     *
     * @return The angle of the ellipse
     */
    [[nodiscard]] double getAngle( ) const;

private:
    Point2d mCenter { };
    SizeD mSize { };
    double mPhi { };
};
} // namespace cvl::core