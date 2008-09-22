// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterPathPoint_h_
#define __ActionParameterPathPoint_h_

#include "Location.h"

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  PathPoint
    @brief  PathPoint
*/
// Created: SBO 2007-05-15 
// =============================================================================
class PathPoint : public Location
{

public:
    //! @name Constructors/Destructor
    //@{
             PathPoint( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location );
             PathPoint( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter );
    virtual ~PathPoint();
    //@}

    //! @name Operations
    //@{
    void CommitTo( ASN1T_CoordLatLong& asn ) const;
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PathPoint( const PathPoint& );            //!< Copy constructor
    PathPoint& operator=( const PathPoint& ); //!< Assignment operator
    //@}
};

    }
}

#endif // __ActionParameterPathPoint_h_
