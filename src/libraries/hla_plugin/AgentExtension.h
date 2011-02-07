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
#include "EventListener_ABC.h"
#include "Formation.h"
#include "dispatcher/Observer.h"
#include "protocol/Protocol.h"
#include "rpr/EntityIdentifier.h"
#include "rpr/ForceIdentifier.h"

namespace plugins
{
namespace hla
{
    class Agent_ABC;
    class Spatial;

// =============================================================================
/** @class  AgentExtension
    @brief  Agent extension
*/
// Created: SBO 2008-02-18
// =============================================================================
class AgentExtension : public HlaExtension_ABC
                     , private EventListener_ABC
                     , private dispatcher::Observer< sword::UnitAttributes >
                     , private dispatcher::Observer< sword::UnitEnvironmentType >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentExtension( dispatcher::Observable< sword::UnitAttributes >& attributes,
                             dispatcher::Observable< sword::UnitEnvironmentType >& environment,
                             Agent_ABC& holder, const rpr::EntityIdentifier& id,
                             const std::string& name, rpr::ForceIdentifier force );
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
    virtual void SpatialChanged( double latitude, double longitude, float altitude, float speed, float direction );
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
    Agent_ABC& holder_;
    rpr::EntityIdentifier id_;
    const std::string name_;
    const rpr::ForceIdentifier force_;
    Formation formation_;
    mutable bool spatialChanged_;
    std::auto_ptr< Spatial > pSpatial_;
    mutable bool compositionChanged_;
    //@}
};

}
}

#endif // __AgentExtension_h_
