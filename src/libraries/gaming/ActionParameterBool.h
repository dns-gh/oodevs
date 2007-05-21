// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterBool_h_
#define __ActionParameterBool_h_

#include "ActionParameter.h"

// =============================================================================
/** @class  ActionParameterBool
    @brief  ActionParameterBool
*/
// Created: SBO 2007-05-21
// =============================================================================
class ActionParameterBool : public ActionParameter< bool >
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterBool( const kernel::OrderParameter& parameter, xml::xistream& xis );
             ActionParameterBool( const kernel::OrderParameter& parameter, bool value );
    virtual ~ActionParameterBool();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterBool( const ActionParameterBool& );            //!< Copy constructor
    ActionParameterBool& operator=( const ActionParameterBool& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    //@}
};

#endif // __ActionParameterBool_h_
