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

#include <graphics/ElevationMap.h>
#include <boost/noncopyable.hpp>

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

    const Environment* EnvironmentData( unsigned int x, unsigned int y ) const;
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

inline
geometry::Rectangle2f DetectionMap::SubExtent( unsigned int x, unsigned int y, unsigned int width, unsigned int height ) const
{
    return map_ ? map_->SubExtent( x, y, width, height ) : geometry::Rectangle2f();
}

inline
geometry::Point2f DetectionMap::Map( unsigned int x, unsigned int y ) const
{
    return map_ ? map_->Map( x, y ) : geometry::Point2f();
}

inline
std::pair< unsigned int, unsigned int > DetectionMap::Unmap( const geometry::Point2f& point ) const
{
    return map_ ? map_->Unmap( point ) : std::pair< unsigned int, unsigned int >();
}

inline
const short* DetectionMap::Data( unsigned int x, unsigned int y ) const
{
    return map_ ? map_->Data( x, y ) : 0;
}

inline
unsigned int DetectionMap::Unmap( float distance ) const
{
    return map_ ? map_->Unmap( distance ) : 0;
}

inline
double DetectionMap::ElevationAt( const geometry::Point2f& point, bool applyOffsetOnCell /*= false*/ ) const
{
    return map_ ? map_->ElevationAt( point, applyOffsetOnCell ) : 0;
}

inline
void DetectionMap::SetAltitudeOffset( unsigned int id, const geometry::Polygon2f::T_Vertices& points, bool isPolygon, short offset )
{
    if( map_ )
        map_->SetAltitudeOffset( id, points, isPolygon, offset );
}

inline
short DetectionMap::MaximumElevation() const
{
    return map_ ? map_->MaximumElevation() : 0;
}

inline
const short* DetectionMap::Data() const
{
    return map_ ? map_->Data() : 0;
}

inline
unsigned int DetectionMap::Width() const
{
    return map_ ? map_->Width() : 0;
}

inline
unsigned int DetectionMap::Height() const
{
    return map_ ? map_->Height() : 0;
}

inline
geometry::Rectangle2f DetectionMap::Extent() const
{
    return map_ ? map_->Extent() : geometry::Rectangle2f();
}

inline
const ElevationMap& DetectionMap::GetMap() const// $$$$ AGE 2006-04-28: prolly tmp
{
    if( ! map_ )
    throw MASA_EXCEPTION( "Map not initialized" );
    return *map_;
}

}

#endif // __DetectionMap_h_
