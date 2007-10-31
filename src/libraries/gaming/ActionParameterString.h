// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterString_h_
#define __ActionParameterString_h_

#include "ActionParameter.h"

// =============================================================================
/** @class  ActionParameterString
    @brief  ActionParameterString
*/
// Created: SBO 2007-10-23
// =============================================================================
class ActionParameterString : public ActionParameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterString( const kernel::OrderParameter& parameter, const std::string& value );
             ActionParameterString( const kernel::OrderParameter& parameter, const ASN1VisibleString& asn );
             ActionParameterString( const kernel::OrderParameter& parameter, xml::xistream& xis );
    virtual ~ActionParameterString();
    //@}

    //! @name Operations
    //@{
    void CommitTo( ASN1VisibleString& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterString( const ActionParameterString& );            //!< Copy constructor
    ActionParameterString& operator=( const ActionParameterString& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    //@}
};

#endif // __ActionParameterString_h_
