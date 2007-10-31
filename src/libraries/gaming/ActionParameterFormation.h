// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterFormation_h_
#define __ActionParameterFormation_h_

#include "ActionParameterEntity.h"
#include "clients_kernel/Formation_ABC.h"

namespace kernel
{
    class Formation_ABC;
}

// =============================================================================
/** @class  ActionParameterFormation
    @brief  ActionParameterFormation
*/
// Created: SBO 2007-10-23
// =============================================================================
class ActionParameterFormation : public ActionParameterEntity< kernel::Formation_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterFormation( const kernel::OrderParameter& parameter, const kernel::Formation_ABC& formation );
             ActionParameterFormation( const kernel::OrderParameter& parameter, const ASN1T_OID& asn, const kernel::Resolver_ABC< kernel::Formation_ABC >& resolver );
             ActionParameterFormation( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::Resolver_ABC< kernel::Formation_ABC >& resolver );
    virtual ~ActionParameterFormation();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterFormation( const ActionParameterFormation& );            //!< Copy constructor
    ActionParameterFormation& operator=( const ActionParameterFormation& ); //!< Assignment operator
    //@}
};

#endif // __ActionParameterFormation_h_
