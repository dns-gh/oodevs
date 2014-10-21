// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DetectionMap_h_
#define __DetectionMap_h_

#include <boost/noncopyable.hpp>

class ElevationMap;

namespace tools
{
    class ExerciseConfig;
}

namespace kernel
{

// =============================================================================
/** @class  DetectionMap
    @brief  DetectionMap
*/
// Created: AGE 2006-04-04
// =============================================================================
class DetectionMap : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    struct Environment
    {
        Environment() : data_( 0 ) {}
        bool IsInTown() const { return ( data_ & town_ ) != 0; }
        bool IsInForest() const { return ( data_ & forest_ ) != 0; }
        unsigned char MeteoEffect() const { return ( data_ & meteoMask_ ) != 0; }
        short ElevationDelta() const { return IsInTown() ? 20 : ( IsInForest() ? 10 : 0 ); }

        static const unsigned char town_      = 0x80;
        static const unsigned char forest_    = 0x40;
        static const unsigned char meteoMask_ = 0x3f;
        unsigned char data_;
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             DetectionMap();
    virtual ~DetectionMap();
    //@}

    //! @name Operations
    //@{
    void Load( const tools::ExerciseConfig& config );

    // Returns the environment data for (x, y) cell if it is within terrain
    // boundaries, a valid but zero environment otherwise.
    Environment EnvironmentData( unsigned int x, unsigned int y ) const;
    Environment EnvironmentAt( const geometry::Point2f& point ) const;
    float GetCellSize() const;
    //@}

    //! @name Operations
    //@{
    geometry::Rectangle2f SubExtent( unsigned int x, unsigned int y, unsigned int width, unsigned int height ) const;
    geometry::Point2f Map( unsigned int x, unsigned int y ) const;
    std::pair< unsigned int, unsigned int > Unmap( const geometry::Point2f& point ) const;
    const short* Data( unsigned int x, unsigned int y ) const;
    unsigned int Unmap( float distance ) const;
    double ElevationAt( const geometry::Point2f& point, bool applyOffsetOnCell = false ) const;
    void SetAltitudeOffset( unsigned int id, const geometry::Polygon2f::T_Vertices& points, bool isPolygon, short offset );
    //@}

    //! @name Accessors
    //@{
    short MaximumElevation() const;
    const short* Data() const;
    unsigned int Width() const;
    unsigned int Height() const;
    geometry::Rectangle2f Extent() const;
    const ElevationMap& GetMap() const;// $$$$ AGE 2006-04-28: prolly tmp
    //@}

private:
    //! @name Member data
    //@{
    ElevationMap* map_;
    Environment* environment_;
    float cellsize_;
    //@}
};

}

#endif // __DetectionMap_h_
