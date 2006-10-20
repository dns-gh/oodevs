// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AutomatTacticalHierarchies_h_
#define __AutomatTacticalHierarchies_h_

#include "clients_kernel/EntityHierarchies.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Resolver_ABC.h"
#include "ASN_Types.h"

namespace kernel
{
    class PropertiesDictionary;
    class Formation_ABC;
}

// =============================================================================
/** @class  AutomatTacticalHierarchies
    @brief  AutomatTacticalHierarchies
*/
// Created: AGE 2006-10-19
// =============================================================================
class AutomatTacticalHierarchies : public kernel::EntityHierarchies< kernel::TacticalHierarchies >
                                 , public kernel::Updatable_ABC< ASN1T_MsgAutomateCreation >
{

public:
    //! @name Constructors/Destructor
    //@{
             AutomatTacticalHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder,
                                        const kernel::Resolver_ABC< kernel::Formation_ABC >& resolver,
                                        kernel::PropertiesDictionary& dictionary );
    virtual ~AutomatTacticalHierarchies();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AutomatTacticalHierarchies( const AutomatTacticalHierarchies& );            //!< Copy constructor
    AutomatTacticalHierarchies& operator=( const AutomatTacticalHierarchies& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgAutomateCreation& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Resolver_ABC< kernel::Formation_ABC >& resolver_;
    //@}
};

#endif // __AutomatTacticalHierarchies_h_
