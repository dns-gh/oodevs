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
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/App6Symbol.h"
#include "game_asn/Simulation.h"
#include "Diplomacies.h"

namespace kernel
{
    class Controller;
    class Automat_ABC;
    class AgentType;
}

// =============================================================================
/** @class  AgentHierarchies
    @brief  AgentHierarchies
*/
// Created: AGE 2006-09-20
// =============================================================================
template< typename I >
class AgentHierarchies : public kernel::EntityHierarchies< I  >
                       , public kernel::Updatable_ABC< ASN1T_MsgUnitCreation >
                       , public kernel::Updatable_ABC< ASN1T_MsgUnitChangeSuperior >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentHierarchies( kernel::Controller& controller, kernel::Agent_ABC& holder,
                               const kernel::Resolver_ABC< kernel::Automat_ABC >& automatResolver );
    virtual ~AgentHierarchies();
    //@}

    //! @name Operations
    //@{
    virtual std::string GetSymbol() const;
    virtual std::string GetLevel() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentHierarchies( const AgentHierarchies& );            //!< Copy constructor
    AgentHierarchies& operator=( const AgentHierarchies& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitCreation& message );
    virtual void DoUpdate( const ASN1T_MsgUnitChangeSuperior& message );

    void UpdateSuperior( kernel::Entity_ABC& superior );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Resolver_ABC< kernel::Automat_ABC >& automatResolver_;
    std::string symbol_;
    std::string level_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: AgentHierarchies constructor
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
template< typename I >
AgentHierarchies< I >::AgentHierarchies( kernel::Controller& controller, kernel::Agent_ABC& holder, const kernel::Resolver_ABC< kernel::Automat_ABC >& automatResolver )
    : kernel::EntityHierarchies< I  >( controller, holder, 0 )
    , controller_                    ( controller )
    , symbol_                        ( holder.GetType().GetSymbol() )
    , level_                         ( holder.GetType().GetLevelSymbol() )
    , automatResolver_               ( automatResolver )
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
void AgentHierarchies< I >::DoUpdate( const ASN1T_MsgUnitCreation& message )
{
    kernel::Automat_ABC& superior = automatResolver_.Get( message.oid_automate );
    const Diplomacies* diplo = superior.Get< I >().GetTop().Retrieve< Diplomacies >();
    if( diplo )
        App6Symbol::SetKarma( symbol_, diplo->GetKarma() );
    SetSuperior( & superior );
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchies::DoUpdate
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
template< typename I >
void AgentHierarchies< I >::DoUpdate( const ASN1T_MsgUnitChangeSuperior& message )
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
    ChangeSuperior( &superior );
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchies::GetSymbol
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
template< typename I >
std::string AgentHierarchies< I >::GetSymbol() const
{
    return symbol_;
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchies::GetLevel
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
template< typename I >
std::string AgentHierarchies< I >::GetLevel() const
{
    return level_;
}

#endif // __AgentHierarchies_h_
