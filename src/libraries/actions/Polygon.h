// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterPolygon_h_
#define __ActionParameterPolygon_h_

#include "Location.h"

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  Polygon
    @brief  Polygon
*/
// Created: SBO 2007-05-22
// =============================================================================
class Polygon : public Location
{
public:
    //! @name Constructors/Destructor
    //@{
             Polygon( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter );
             Polygon( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location );
             Polygon( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const sword::Location& message );
    virtual ~Polygon();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& message ) const;
    void CommitTo( sword::Location& message ) const;
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

#endif // __ActionParameterPolygon_h_
