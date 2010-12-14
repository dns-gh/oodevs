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
#include "EntityIdentifier.h"
#include "clients_kernel/Updatable_ABC.h"
#include "protocol/Protocol.h"

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
                     , public kernel::Updatable_ABC< sword::UnitAttributes >
                     , public kernel::Updatable_ABC< sword::UnitEnvironmentType >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentExtension( dispatcher::Agent_ABC& holder, const EntityIdentifier& id );
    virtual ~AgentExtension();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( ::hla::UpdateFunctor_ABC& functor, bool bUpdateAll ) const;
    virtual void DoUpdate( const sword::UnitAttributes& attributes );
    virtual void DoUpdate( const sword::UnitEnvironmentType& attributes );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentExtension( const AgentExtension& );            //!< Copy constructor
    AgentExtension& operator=( const AgentExtension& ); //!< Assignment operator
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
    EntityIdentifier id_;
    Formation formation_;
    mutable bool spatialChanged_;
    mutable bool compositionChanged_;
    //@}
};

}
}

#endif // __AgentExtension_h_
