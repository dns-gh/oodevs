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

namespace Common
{
    class MsgLocation;
    class MsgCoordLatLongList;
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
    explicit Area( const Common::MsgCoordLatLongList& message );
    explicit Area( const OGRPolygon& area );
    virtual ~Area();
    //@}

    //! @name Operations
    //@{
    void Serialize( std::ostream& geometry ) const;
    void Serialize( Common::MsgLocation& message ) const;
    //@}

    //! @name 
    //@{
    virtual OGRPolygon* Extract( OGRSpatialReference* spatialReference ) const;
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
