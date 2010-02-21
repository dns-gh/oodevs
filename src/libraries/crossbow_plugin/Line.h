// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Line_h_
#define __Line_h_

#include "PointCollection.h"

namespace Common
{
    class MsgCoordLatLongList;
    class MsgLocation;
}

namespace plugins
{
namespace crossbow
{
    class Point;

// =============================================================================
/** @class  Line
    @brief  Line
*/
// Created: SBO 2007-08-30
// =============================================================================
class Line : public PointCollection
{
public:
    //! @name Constructors/Destructor
    //@{
             Line();
    explicit Line( const Common::MsgCoordLatLongList& message );
    explicit Line( IGeometryPtr geometry );
    virtual ~Line();
    //@}

    //! @name Operations
    //@{
    void Serialize( IGeometryPtr geometry, ISpatialReferencePtr spatialReference ) const;
    void Serialize( std::ostream& geometry ) const;
    void Serialize( Common::MsgLocation& message ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Line( const Line& );            //!< Copy constructor
    Line& operator=( const Line& ); //!< Assignment operator
    //@}
};

}
}

#endif // __Line_h_
