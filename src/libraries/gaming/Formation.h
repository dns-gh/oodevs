// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Formation_h_
#define __Formation_h_

#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Resolver_ABC.h"
#include "network/Simulation_Asn.h"

// =============================================================================
/** @class  Formation
    @brief  Formation
*/
// Created: AGE 2006-10-19
// =============================================================================
class Formation : public kernel::EntityImplementation< kernel::Formation_ABC > 
{

public:
    //! @name Constructors/Destructor
    //@{
             Formation( const ASN1T_MsgFormationCreation& message, kernel::Controller& controller, const kernel::Resolver_ABC< kernel::HierarchyLevel_ABC >& resolver );
    virtual ~Formation();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::HierarchyLevel_ABC& GetLevel() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Formation( const Formation& );            //!< Copy constructor
    Formation& operator=( const Formation& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    kernel::HierarchyLevel_ABC& level_;
    //@}
};

#endif // __Formation_h_
