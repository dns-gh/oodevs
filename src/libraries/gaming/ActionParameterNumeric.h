// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterNumeric_h_
#define __ActionParameterNumeric_h_

#include "ActionParameter.h"

// =============================================================================
/** @class  ActionParameterNumeric
    @brief  ActionParameterNumeric
*/
// Created: SBO 2007-05-21
// =============================================================================
class ActionParameterNumeric : public ActionParameter< float >
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterNumeric( const QString& name, float value );
             ActionParameterNumeric( const kernel::OrderParameter& parameter, float value );
             ActionParameterNumeric( const kernel::OrderParameter& parameter, xml::xistream& xis );
    virtual ~ActionParameterNumeric();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterNumeric( const ActionParameterNumeric& );            //!< Copy constructor
    ActionParameterNumeric& operator=( const ActionParameterNumeric& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    //@}
};

#endif // __ActionParameterNumeric_h_
