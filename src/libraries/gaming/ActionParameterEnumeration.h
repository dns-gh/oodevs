// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterEnumeration_h_
#define __ActionParameterEnumeration_h_

#include "ActionParameter.h"

// =============================================================================
/** @class  ActionParameterEnumeration
    @brief  ActionParameterEnumeration
*/
// Created: SBO 2007-05-21
// =============================================================================
class ActionParameterEnumeration : public ActionParameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterEnumeration( const kernel::OrderParameter& parameter, xml::xistream& xis );
             ActionParameterEnumeration( const kernel::OrderParameter& parameter, unsigned int value );
    virtual ~ActionParameterEnumeration();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterEnumeration( const ActionParameterEnumeration& );            //!< Copy constructor
    ActionParameterEnumeration& operator=( const ActionParameterEnumeration& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::OrderParameterValue& value_;
    //@}
};

#endif // __ActionParameterEnumeration_h_
