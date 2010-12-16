// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __plugins_crossbow_Line_h_
#define __plugins_crossbow_Line_h_

#include "PointCollection.h"

namespace sword
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
    explicit Line( const sword::MsgCoordLatLongList& message );
    explicit Line( const OGRLineString& line );
    virtual ~Line();
    //@}

    //! @name Operations
    //@{
    void Serialize( std::ostream& geometry ) const;
    void SerializeWkt( std::ostream& geometry ) const;
    void Serialize( kernel::Location_ABC& message, const kernel::CoordinateConverter_ABC& converter ) const;
    //@}

    //! @name
    //@{
    virtual void Serialize( OGRFeature& feature, OGRSpatialReference* spatialReference ) const;
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

#endif // __plugins_crossbow_Line_h_
