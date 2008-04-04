// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CoordinateConverter_ABC_h_
#define __CoordinateConverter_ABC_h_

namespace kernel
{

// =============================================================================
/** @class  CoordinateConverter_ABC
    @brief  CoordinateConverter_ABC
*/
// Created: AGE 2006-05-17
// =============================================================================
class CoordinateConverter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             CoordinateConverter_ABC() {};
    virtual ~CoordinateConverter_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual bool IsInBoundaries             ( const geometry::Point2f& point ) const = 0;
    virtual std::string       ConvertToMgrs ( const geometry::Point2f& pos ) const = 0;
    virtual geometry::Point2f ConvertToGeo  ( const geometry::Point2f& pos ) const = 0;
    virtual geometry::Point2d ConvertToGeo  ( const geometry::Point2d& pos ) const = 0;
    virtual geometry::Point2f ConvertFromGeo( const geometry::Point2f& pos ) const = 0;
    virtual geometry::Point2f ConvertToXY   ( const std::string& mgrs ) const = 0;

    template< typename T >
    geometry::Point2f ConvertToXY( const T& latlong ) const
    {
        return ConvertFromGeo( geometry::Point2f( float( latlong.longitude ), float( latlong.latitude ) ) );
    }

    template< typename T >
    void ConvertToGeo( const geometry::Point2f& pos, T& latlong ) const
    {
        const geometry::Point2d geo = ConvertToGeo( geometry::Point2d( pos.X(), pos.Y() ) );
        latlong.longitude = geo.X();
        latlong.latitude  = geo.Y();
    }
    //@}

private:
    //! @name Helpers
    //@{
    
    //@}
};

}

#endif // __CoordinateConverter_ABC_h_
