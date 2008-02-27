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
#include "game_asn/asn.h"
#include "clients_kernel/Updatable_ABC.h"

namespace dispatcher
{
    class Agent;
}

namespace hla
{

// =============================================================================
/** @class  AgentExtension
    @brief  AgentExtension
*/
// Created: SBO 2008-02-18
// =============================================================================
class AgentExtension : public HlaExtension_ABC
                     , public kernel::Updatable_ABC< ASN1T_MsgUnitAttributes >
                     , public kernel::Updatable_ABC< ASN1T_MsgUnitEnvironmentType >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentExtension( dispatcher::Agent& holder, const EntityIdentifier& id );
    virtual ~AgentExtension();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( UpdateFunctor_ABC& functor, bool bUpdateAll ) const;
    virtual void DoUpdate( const ASN1T_MsgUnitAttributes& attributes );
    virtual void DoUpdate( const ASN1T_MsgUnitEnvironmentType& attributes );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentExtension( const AgentExtension& );            //!< Copy constructor
    AgentExtension& operator=( const AgentExtension& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void UpdateEntityType      ( UpdateFunctor_ABC& functor ) const;
    void UpdateEntityIdentifier( UpdateFunctor_ABC& functor ) const;
    void UpdateSpatial         ( UpdateFunctor_ABC& functor ) const;
    void UpdateAggregateMarking( UpdateFunctor_ABC& functor ) const;
    void UpdateAggregateState  ( UpdateFunctor_ABC& functor ) const;
    void UpdateForceIdentifier ( UpdateFunctor_ABC& functor ) const;
    void UpdateComposition     ( UpdateFunctor_ABC& functor ) const;
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Agent& holder_;
    EntityIdentifier id_;
    Formation    formation_;
    mutable bool spatialChanged_;
    mutable bool compositionChanged_;

    //@}
};

}

#endif // __AgentExtension_h_
