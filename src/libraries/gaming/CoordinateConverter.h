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

#include "clients_kernel/Types.h"
#include "ASN_Types.h"
#include "clients_kernel/WorldParameters.h"
#include "geocoord/PlanarCartesian.h"
#include "geocoord/MGRS.h"
#include "geocoord/Geodetic.h"
#include "clients_kernel/CoordinateConverter_ABC.h"

// =============================================================================
// Created: APE 2004-07-19
// $$$$ AGE 2006-04-28: warning : not thread safe
// $$$$ AGE 2006-08-07: passer dans kernel
// =============================================================================
class CoordinateConverter : public CoordinateConverter_ABC
{
public:
    //! @name Constructor / Destructor
    //@{
             CoordinateConverter();
    virtual ~CoordinateConverter();
    //@}

    //! @name Operations
    //@{
    void Load( const std::string& scipioXml );

    virtual bool IsInBoundaries( const geometry::Point2f& point ) const;

    virtual std::string       ConvertToMgrs( const geometry::Point2f& pos ) const;
    virtual geometry::Point2f ConvertToXY  ( const ASN1T_CoordUTM& mgrs ) const;
    virtual geometry::Point2f ConvertToGeo ( const geometry::Point2f& pos ) const;
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
    //! @name Member data
    //@{
    WorldParameters world_;
    geometry::Rectangle2f extent_;
    geometry::Vector2f    translation_;

            geocoord::PlanarCartesian::Parameters parameters_;
    mutable geocoord::PlanarCartesian             planar_;
    mutable geocoord::MGRS                        mgrs_;
    mutable geocoord::Geodetic                    geodetic_;
    //@}
};

#endif // __CoordinateConverter_h_