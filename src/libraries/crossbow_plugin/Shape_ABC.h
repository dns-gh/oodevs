// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Shape_ABC_h_
#define __Shape_ABC_h_

#include <gdal/ogr_geometry.h>
#include <gdal/ogr_feature.h>

namespace kernel
{
    class Location_ABC;
    class CoordinateConverter_ABC;
}

namespace plugins
{
namespace crossbow
{

// =============================================================================
/** @class  Shape_ABC
    @brief  Shape_ABC
*/
// Created: SBO 2007-08-30
// =============================================================================
class Shape_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Shape_ABC() {}
    virtual ~Shape_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( std::ostream& geometry ) const = 0;
    virtual void Serialize( kernel::Location_ABC& location, const kernel::CoordinateConverter_ABC& converter ) const = 0;
    //@}

    //! @name
    //@{
    virtual void Serialize( OGRFeature& feature, OGRSpatialReference* spatialReference ) const = 0;
    //@}
};

}
}

#endif // __Shape_ABC_h_
