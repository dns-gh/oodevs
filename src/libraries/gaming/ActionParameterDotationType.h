// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterDotationType_h_
#define __ActionParameterDotationType_h_

#include "ActionParameter.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class DotationType;
}

// =============================================================================
/** @class  ActionParameterDotationType
    @brief  ActionParameterDotationType
*/
// Created: SBO 2007-05-21
// =============================================================================
class ActionParameterDotationType : public ActionParameter< std::string >
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterDotationType( const kernel::OrderParameter& parameter, unsigned int id, const kernel::Resolver_ABC< kernel::DotationType >& resolver );
             ActionParameterDotationType( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::Resolver_ABC< kernel::DotationType >& resolver );
    virtual ~ActionParameterDotationType();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void CommitTo( std::string& content ) const;
    virtual void Accept( ActionParameterVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterDotationType( const ActionParameterDotationType& );            //!< Copy constructor
    ActionParameterDotationType& operator=( const ActionParameterDotationType& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::DotationType& type_;
    //@}
};

#endif // __ActionParameterDotationType_h_
