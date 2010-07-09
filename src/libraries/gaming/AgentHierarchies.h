// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __AgentHierarchies_h_
#define __AgentHierarchies_h_

#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/App6Symbol.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/EntityHierarchies.h"
#include "clients_kernel/Updatable_ABC.h"

namespace MsgsSimToClient
{
    class MsgUnitCreation;
}
namespace Common
{
    class MsgUnitChangeSuperior;
}
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
                       , public kernel::Updatable_ABC< MsgsSimToClient::MsgUnitCreation >
                       , public kernel::Updatable_ABC< Common::MsgUnitChangeSuperior >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentHierarchies( kernel::Controller& controller, kernel::Agent_ABC& holder,
                               const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver );
    virtual ~AgentHierarchies();
    //@}

    //! @name Operations
    //@{
    virtual std::string GetSymbol() const;
    virtual std::string GetLevel() const;

    virtual void DoUpdate( const MsgsSimToClient::MsgUnitCreation& message );
    virtual void DoUpdate( const Common::MsgUnitChangeSuperior& message );
    //@}
    //! @name Helpers
    //@{
    void UpdateSuperior( kernel::Entity_ABC& superior );
    //@}


private:
    //! @name Copy/Assignment
    //@{
    AgentHierarchies( const AgentHierarchies& );            //!< Copy constructor
    AgentHierarchies& operator=( const AgentHierarchies& ); //!< Assignment operator
    //@}


    //! @name Member data
    //@{
protected:
    const tools::Resolver_ABC< kernel::Automat_ABC >& GetAutomatResolver();

private:
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver_;
    std::string symbol_;
    std::string level_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: AgentHierarchies constructor
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
template< typename I >
AgentHierarchies< I >::AgentHierarchies( kernel::Controller& controller, kernel::Agent_ABC& holder, const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver )
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
void AgentHierarchies< I >::DoUpdate( const MsgsSimToClient::MsgUnitCreation& message )
{
    kernel::Automat_ABC& superior = automatResolver_.Get( message.oid_automate() );
    const kernel::Diplomacies_ABC* diplo = superior.Get< I >().GetTop().Retrieve< kernel::Diplomacies_ABC >();
    if( diplo )
        kernel::App6Symbol::SetKarma( symbol_, diplo->GetKarma() );
    SetSuperior( & superior );
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchies::DoUpdate
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
template< typename I >
void AgentHierarchies< I >::DoUpdate( const Common::MsgUnitChangeSuperior& message )
{
    UpdateSuperior( automatResolver_.Get( message.oid_automate() ) );
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


// -----------------------------------------------------------------------------
// Name: AgentHierarchies::GetAutomatResolver
// Created: FDS 2010-03-29
// -----------------------------------------------------------------------------
template< typename I >
const tools::Resolver_ABC< kernel::Automat_ABC >& AgentHierarchies< I >::GetAutomatResolver()
{
    return automatResolver_;
}

#endif // __AgentHierarchies_h_
