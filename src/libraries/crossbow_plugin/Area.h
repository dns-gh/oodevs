// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __crossbow_Area_h_
#define __crossbow_Area_h_

#include "PointCollection.h"

class OGRPolygon;

namespace sword
{
    class CoordLatLongList;
}

namespace plugins
{
namespace crossbow
{
// =============================================================================
/** @class  Area
*/
// Created: JCR 2008-04-25
// =============================================================================
class Area : public PointCollection
{
public:
    //! @name Constructors/Destructor
    //@{
             Area();
    explicit Area( const sword::CoordLatLongList& message );
    explicit Area( const OGRPolygon& area );
    virtual ~Area();
    //@}

    //! @name Operations
    //@{
    void SerializeWkt( std::ostream& geometry ) const;

    virtual void Serialize( std::ostream& geometry ) const;
    virtual void Serialize( kernel::Location_ABC& message, const kernel::CoordinateConverter_ABC& converter ) const;
    virtual void Serialize( OGRFeature& feature, OGRSpatialReference* spatialReference ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Area( const Area& );            //!< Copy constructor
    Area& operator=( const Area& ); //!< Assignement operator
    //@}
};

}
}

#endif // __Area_h_
