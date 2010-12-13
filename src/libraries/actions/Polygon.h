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
             Polygon( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location );
             Polygon( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const sword::MsgLocation& message );
             Polygon( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis );
    virtual ~Polygon();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( sword::MsgMissionParameter& message ) const;
    virtual void CommitTo( sword::MsgMissionParameter_Value& message ) const;
    void CommitTo( sword::MsgLocation& message ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Polygon( const Polygon& );            //!< Copy constructor
    Polygon& operator=( const Polygon& ); //!< Assignment operator
    //@}
};

    }
}

#endif // __ActionParameterPolygon_h_
