// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentHierarchies_h_
#define __AgentHierarchies_h_

#include "clients_kernel/EntityHierarchies.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Controller.h"
#include "ASN_Types.h"

namespace kernel
{
    class Controller;
    class Automat_ABC;
}

// =============================================================================
/** @class  AgentHierarchies
    @brief  AgentHierarchies
*/
// Created: AGE 2006-09-20
// =============================================================================
template< typename I >
class AgentHierarchies : public kernel::EntityHierarchies< I  >
                       , public kernel::Updatable_ABC< ASN1T_MsgPionCreation >
                       , public kernel::Updatable_ABC< ASN1T_MsgChangeAutomateAck >
                       , public kernel::Updatable_ABC< ASN1T_MsgChangeAutomate >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder,
                               const kernel::Resolver_ABC< kernel::Automat_ABC >& automatResolver );
    virtual ~AgentHierarchies();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentHierarchies( const AgentHierarchies& );            //!< Copy constructor
    AgentHierarchies& operator=( const AgentHierarchies& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgPionCreation& message );
    virtual void DoUpdate( const ASN1T_MsgChangeAutomateAck& message );
    virtual void DoUpdate( const ASN1T_MsgChangeAutomate& message );

    void UpdateSuperior( kernel::Entity_ABC& superior );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Resolver_ABC< kernel::Automat_ABC >& automatResolver_; 
    //@}
};

// -----------------------------------------------------------------------------
// Name: AgentHierarchies constructor
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
template< typename I >
AgentHierarchies< I >::AgentHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, const kernel::Resolver_ABC< kernel::Automat_ABC >& automatResolver )
    : kernel::EntityHierarchies< I  >( controller, holder )
    , controller_   ( controller )
    , automatResolver_( automatResolver ) 
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchies destructor
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
template< typename I >
AgentHierarchies< I >::~AgentHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchies::DoUpdate
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
template< typename I >
void AgentHierarchies< I >::DoUpdate( const ASN1T_MsgPionCreation& message )
{
    UpdateSuperior( automatResolver_.Get( message.oid_automate ) );
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchies::DoUpdate
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
template< typename I >
void AgentHierarchies< I >::DoUpdate( const ASN1T_MsgChangeAutomateAck& message )
{
    if( message.error_code == EnumObjectErrorCode::no_error )
        UpdateSuperior( automatResolver_.Get( message.oid_automate ) );
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchies::DoUpdate
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
template< typename I >
void AgentHierarchies< I >::DoUpdate( const ASN1T_MsgChangeAutomate& message )
{   
    UpdateSuperior( automatResolver_.Get( message.oid_automate ) );
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchies::UpdateSuperior
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
template< typename I >
void AgentHierarchies< I >::UpdateSuperior( kernel::Entity_ABC& superior )
{
    SetSuperior( &superior );
    controller_.Update( *(I*)this );
}

#endif // __AgentHierarchies_h_
