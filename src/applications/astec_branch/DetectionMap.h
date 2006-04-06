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

#include "WorldParameters.h"
#include "graphics/ElevationMap.h"

// =============================================================================
/** @class  DetectionMap
    @brief  DetectionMap
*/
// Created: AGE 2006-04-04
// =============================================================================
class DetectionMap : private WorldParameters, public ElevationMap
{

public:
    //! @name Types
    //@{
    struct Environment
    {
        bool          IsInTown() const    { return data_ & town_; };
        bool          IsInForest() const  { return data_ & forest_; };
        unsigned char MeteoEffect() const { return data_ & meteoMask_; };
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
    explicit DetectionMap( const std::string& scipioXml );
    virtual ~DetectionMap();
    //@}

    //! @name Operations
    //@{
    const Environment* EnvironmentData( unsigned x, unsigned y ) const;
    Environment        EnvironmentAt( const geometry::Point2f& point ) const;
    float              GetCellSize() const;
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
    Environment* environment_;
    float cellsize_;
    //@}
};

#endif // __DetectionMap_h_
