// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterLimit_h_
#define __ActionParameterLimit_h_

#include "ActionParameterLocation.h"

// =============================================================================
/** @class  ActionParameterLimit
    @brief  ActionParameterLimit
*/
// Created: SBO 2007-04-13
// =============================================================================
class ActionParameterLimit : public ActionParameterLocation
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterLimit( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const ASN1T_Line& line );
             ActionParameterLimit( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location );
             ActionParameterLimit( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis );
    virtual ~ActionParameterLimit();
    //@}

    //! @name Operations
    //@{
    void CommitTo( ASN1T_MissionParameter& asn ) const;
    void Clean( ASN1T_MissionParameter& asn ) const;
    //@}

protected:
    //! @name Operations
    //@{
    virtual void DisplayInToolTip( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterLimit( const ActionParameterLimit& );            //!< Copy constructor
    ActionParameterLimit& operator=( const ActionParameterLimit& ); //!< Assignment operator
    //@}
};

#endif // __ActionParameterLimit_h_
