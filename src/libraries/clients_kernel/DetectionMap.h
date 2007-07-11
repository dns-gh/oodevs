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

#include "graphics/ElevationMap.h"

namespace kernel
{
    class ExerciseConfig;

// =============================================================================
/** @class  DetectionMap
    @brief  DetectionMap
*/
// Created: AGE 2006-04-04
// =============================================================================
class DetectionMap
{

public:
    //! @name Types
    //@{
    struct Environment
    {
        bool          IsInTown() const    { return ( data_ & town_ ) != 0; };
        bool          IsInForest() const  { return ( data_ & forest_ ) != 0; };
        unsigned char MeteoEffect() const { return ( data_ & meteoMask_ ) != 0; };
        short         ElevationDelta() const { return IsInTown() ? 20 : ( IsInForest() ? 10 : 0 ); };

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
    void Load( const ExerciseConfig& config );

    const Environment* EnvironmentData( unsigned x, unsigned y ) const;
    Environment        EnvironmentAt( const geometry::Point2f& point ) const;
    float              GetCellSize() const;
    //@}

    //! @name Operations
    //@{
    geometry::Rectangle2f           SubExtent( unsigned x, unsigned y, unsigned width, unsigned height ) const
    { return map_ ? map_->SubExtent( x, y, width, height ) : geometry::Rectangle2f(); };
    geometry::Point2f               Map( unsigned x, unsigned y ) const
    { return map_ ? map_->Map( x, y ) : geometry::Point2f(); };
    std::pair< unsigned, unsigned > Unmap( const geometry::Point2f& point ) const 
    { return map_ ? map_->Unmap( point ) : std::pair< unsigned, unsigned >(); };
    const short*                    Data( unsigned x, unsigned y ) const
    { return map_ ? map_->Data( x, y ) : 0; };
    unsigned                        Unmap( float distance ) const
    { return map_ ? map_->Unmap( distance ) : 0; };
    short                           ElevationAt( const geometry::Point2f& point ) const
    { return map_ ? map_->ElevationAt( point ) : 0; };
    //@}

    //! @name Accessors
    //@{
    short MaximumElevation() const
    { return map_ ? map_->MaximumElevation() : 0; };
    const short* Data() const
    { return map_ ? map_->Data() : 0; };
    unsigned Width() const
    { return map_ ? map_->Width() : 0; };
    unsigned Height() const
    { return map_ ? map_->Height() : 0; };
    geometry::Rectangle2f Extent() const
    { return map_ ? map_->Extent() : geometry::Rectangle2f(); };

    const ElevationMap& GetMap() const// $$$$ AGE 2006-04-28: prolly tmp
    {
        if( ! map_ )
            throw std::runtime_error( "Map not initialized" );
        return *map_;
    }
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DetectionMap( const DetectionMap& );            //!< Copy constructor
    DetectionMap& operator=( const DetectionMap& ); //!< Assignement operator
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
