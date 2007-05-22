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

#include "ActionParameterLocation.h"

// =============================================================================
/** @class  ActionParameterPoint
    @brief  ActionParameterPoint
*/
// Created: SBO 2007-05-22
// =============================================================================
class ActionParameterPoint : public ActionParameterLocation
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterPoint( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location );
             ActionParameterPoint( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const ASN1T_Point& asn );
             ActionParameterPoint( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis );
    virtual ~ActionParameterPoint();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterPoint( const ActionParameterPoint& );            //!< Copy constructor
    ActionParameterPoint& operator=( const ActionParameterPoint& ); //!< Assignment operator
    //@}
};

#endif // __ActionParameterPoint_h_
