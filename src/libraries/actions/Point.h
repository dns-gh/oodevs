// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterPoint_h_
#define __ActionParameterPoint_h_

#include "Location.h"

namespace Common
{
    class MsgCoordLatLong;
    class MsgMissionParameter;
    class MsgPoint;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  Point
    @brief  Point
*/
// Created: SBO 2007-05-22
// =============================================================================
class Point : public Location
{
public:
    //! @name Constructors/Destructor
    //@{
             Point( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location );
             Point( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const Common::MsgPoint& message );
             Point( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis );
    virtual ~Point();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( Common::MsgMissionParameter& message ) const;
    virtual void CommitTo( Common::MsgMissionParameter_Value& message ) const;
    void CommitTo( Common::MsgCoordLatLong& message ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Point( const Point& );            //!< Copy constructor
    Point& operator=( const Point& ); //!< Assignment operator
    //@}
};

    }
}

#endif // __ActionParameterPoint_h_
