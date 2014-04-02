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

namespace sword
{
    class CoordLatLong;
    class MissionParameter;
    class Point;
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
             Point( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter );
             Point( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location );
             Point( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const sword::Point& message );
    virtual ~Point();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& message ) const;
    void CommitTo( sword::CoordLatLong& message ) const;
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual std::string SerializeType() const;
    //@}
};

}
}

#endif // __ActionParameterPoint_h_
