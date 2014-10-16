// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GridLayer_h_
#define __GridLayer_h_

#include "Layer.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include <boost/optional.hpp>

namespace kernel
{
    class Controllers;
    class CoordinateConverter_ABC;
    class ModelLoaded;
    class ModelUnLoaded;
}

namespace gui
{
// =============================================================================
/** @class  GridLayer
    @brief  GridLayer
*/
// Created: AGE 2006-08-22
// =============================================================================
class GridLayer : public Layer2D
                , public kernel::OptionsObserver_ABC
                , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                , public tools::ElementObserver_ABC< kernel::ModelUnLoaded >
{
public:
    //! @name Constructors/Destructor
    //@{
             GridLayer( kernel::Controllers& controllers,
                        GLView_ABC& tools,
                        const kernel::CoordinateConverter_ABC& converter );
    virtual ~GridLayer();
    //@}

private:
    // An MGRS point in a 100,000-meter square
    struct Point
    {
        Point()
            : x_( 0 )
            , y_( 0 )
        {}
        Point( geometry::Point2f coord, int x, int y )
            : coord_( coord )
            , x_( x )
            , y_( y )
        {}
        geometry::Point2f coord_; // screen coordinates
        int x_, y_;               // MGRS xy coordinates
    };

    // An MGRS 100,000-meter square
    // See http://en.wikipedia.org/wiki/Military_grid_reference_system#100.2C000-meter_square_identification
    struct Square
    {
        Point topLeft_;
        Point topRight_;
        Point bottomRight_;
        Point bottomLeft_;
    };

private:
    //! @name Operations
    //@{
    virtual void Paint( const geometry::Rectangle2f& viewport );
    virtual void Paint( Viewport_ABC& viewport );
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& );
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

    //! @name Helpers
    //@{
    int GetSize() const;
    float Snap( float value ) const;

    void DrawSquares( const geometry::Rectangle2f& viewport ) const;
    void DrawVerticalSides( const geometry::Rectangle2f& viewport, const Square& s ) const;
    void DrawHorizontalSides( const geometry::Rectangle2f& viewport, const Square& s ) const;

    geometry::Rectangle2d ComputeBoundingExtent() const;
    std::string ExtractPrefix( const geometry::Rectangle2d& extent, double longitude, double latitude ) const;
    boost::optional< std::string > Convert( double longitude, double latitude, const std::string& prefix, int x, int y ) const;

    template< typename C >
    Point Bisect( int good, int bad, const C& convert ) const;

    Square MakeSquare( const std::string& prefix, double longitude, double latitude ) const;
    Point MakePoint( double longitude, double latitude ) const;
    Point MakePoint( const std::string& mgrs ) const;

    bool IsVisible( const Square& s ) const;
    void DrawLine( const geometry::Rectangle2f& viewport, const geometry::Point2f& from, const geometry::Point2f& to ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    geometry::Rectangle2f extent_;
    E_CoordinateSystem gridType_;
    float gridSize_;
    std::vector< Square > squares_;
    //@}
};

}

#endif // __GridLayer_h_
