// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterLimits_h_
#define __ActionParameterLimits_h_

#include "game_asn/Asn.h"
#include "ActionParameter.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

// =============================================================================
/** @class  ActionParameterLimits
    @brief  ActionParameterLimits
*/
// Created: SBO 2007-04-25
// =============================================================================
class ActionParameterLimits : public ActionParameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionParameterLimits( const kernel::OrderParameter& parameter );
             ActionParameterLimits( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const ASN1T_Line& line1, const ASN1T_Line& line2 );
             ActionParameterLimits( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis );
    virtual ~ActionParameterLimits();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterLimits( const ActionParameterLimits& );            //!< Copy constructor
    ActionParameterLimits& operator=( const ActionParameterLimits& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadLimit( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter );
    //@}
};

#endif // __ActionParameterLimits_h_
