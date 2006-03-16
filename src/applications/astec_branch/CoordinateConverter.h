// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CoordinateConverter_h_
#define __CoordinateConverter_h_

#include "Types.h"
#include "ASN_Types.h"
#include "WorldParameters.h"
#include "geocoord/PlanarCartesian.h"
#include "geocoord/MGRS.h"
#include "geocoord/Geodetic.h"

// =============================================================================
// Created: APE 2004-07-19
// $$$$ AGE 2006-03-15: make an iface
// =============================================================================
class CoordinateConverter : private WorldParameters
{
public:
    //! @name Constructor / Destructor
    //@{
    explicit CoordinateConverter( const std::string& scipioXml );
    virtual ~CoordinateConverter();
    //@}

    //! @name Coordinate conversions
    //@{
    std::string       ConvertToMgrs( const geometry::Point2f& pos  ) const;
    geometry::Point2f ConvertToXY  ( const ASN1T_CoordUTM& mgrs ) const;
    //@}

private:
    //! @name Copy / Assignment
    //@{
    CoordinateConverter( const CoordinateConverter& );
    CoordinateConverter& operator=( const CoordinateConverter& );
    //@}

    //! @name Initialization
    //@{
    void ReadTerrain( const std::string& terrain );
    void ReadWorld( const std::string& strArchive );
    //@}
    
private:
    geometry::Rectangle2f extent_;
    geometry::Vector2f    translation_;

            geocoord::PlanarCartesian::Parameters parameters_;
    mutable geocoord::PlanarCartesian             planar_;
    mutable geocoord::MGRS                        mgrs_;
    mutable geocoord::Geodetic                    geodetic_;
};

#endif // __CoordinateConverter_h_