// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectHierarchies_h_
#define __ObjectHierarchies_h_

#include "ASN_Messages.h"
#include "clients_kernel/EntityHierarchies.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Team_ABC;
}

// =============================================================================
/** @class  ObjectHierarchies
    @brief  Object hierarchies. Duh !
*/
// Created: AGE 2006-10-16
// =============================================================================
class ObjectHierarchies : public kernel::EntityHierarchies< kernel::TacticalHierarchies >
                        , public kernel::Updatable_ABC< ASN1T_MsgObjectCreation >
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectHierarchies( kernel::Controller& controller, kernel::Entity_ABC& entity, const kernel::Resolver_ABC< kernel::Team_ABC >& teamResolver );
    virtual ~ObjectHierarchies();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const ASN1T_MsgObjectCreation& );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectHierarchies( const ObjectHierarchies& );            //!< Copy constructor
    ObjectHierarchies& operator=( const ObjectHierarchies& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Resolver_ABC< kernel::Team_ABC >& teamResolver_;
    //@}
};

#endif // __ObjectHierarchies_h_
