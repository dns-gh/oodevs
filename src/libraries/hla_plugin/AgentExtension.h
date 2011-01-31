// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentExtension_h_
#define __AgentExtension_h_

#include "HlaExtension_ABC.h"
#include "Formation.h"
#include "clients_kernel/Updatable_ABC.h"
#include "dispatcher/Observer.h"
#include "protocol/Protocol.h"
#include "rpr_tools/EntityIdentifier.h"

namespace dispatcher
{
    class Agent_ABC;
}

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  AgentExtension
    @brief  Agent extension
*/
// Created: SBO 2008-02-18
// =============================================================================
class AgentExtension : public HlaExtension_ABC
                     , private dispatcher::Observer< sword::UnitAttributes >
                     , private dispatcher::Observer< sword::UnitEnvironmentType >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentExtension( dispatcher::Observable< sword::UnitAttributes >& attributes
                           , dispatcher::Observable< sword::UnitEnvironmentType >& environment
                           , dispatcher::Agent_ABC& holder, const rpr::EntityIdentifier& id );
    virtual ~AgentExtension();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( ::hla::UpdateFunctor_ABC& functor, bool bUpdateAll ) const;
    //@}

private:
    //! @name Observer
    //@{
    virtual void Notify( const sword::UnitAttributes& attributes );
    virtual void Notify( const sword::UnitEnvironmentType& attributes );
    //@}

    //! @name Helpers
    //@{
    void UpdateEntityType( ::hla::UpdateFunctor_ABC& functor ) const;
    void UpdateEntityIdentifier( ::hla::UpdateFunctor_ABC& functor ) const;
    void UpdateSpatial( ::hla::UpdateFunctor_ABC& functor ) const;
    void UpdateAggregateMarking( ::hla::UpdateFunctor_ABC& functor ) const;
    void UpdateAggregateState( ::hla::UpdateFunctor_ABC& functor ) const;
    void UpdateForceIdentifier( ::hla::UpdateFunctor_ABC& functor ) const;
    void UpdateComposition( ::hla::UpdateFunctor_ABC& functor ) const;
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Agent_ABC& holder_;
    rpr::EntityIdentifier id_;
    Formation formation_;
    mutable bool spatialChanged_;
    mutable bool compositionChanged_;
    //@}
};

}
}

#endif // __AgentExtension_h_
