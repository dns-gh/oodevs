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
#include "rpr/EntityIdentifier.h"
#include "rpr/ForceIdentifier.h"
#include <vector>

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
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentExtension( Agent_ABC& agent, const rpr::EntityIdentifier& id,
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
    virtual void SpatialChanged( double latitude, double longitude, float altitude, float speed, float direction );
    virtual void FormationChanged( bool isOnRoad );
    virtual void EquipmentChanged( unsigned int type, unsigned int available );
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
    //! @name Types
    //@{
    typedef std::pair< unsigned int, unsigned int > T_Equipment;
    typedef std::vector< T_Equipment >  T_Equipments;
    typedef T_Equipments::iterator     IT_Equipments;
    //@}

private:
    //! @name Member data
    //@{
    Agent_ABC& agent_;
    rpr::EntityIdentifier id_;
    const std::string name_;
    const rpr::ForceIdentifier force_;
    Formation formation_;
    mutable bool spatialChanged_;
    std::auto_ptr< Spatial > pSpatial_;
    mutable bool compositionChanged_;
    T_Equipments equipments_;
    //@}
};

}
}

#endif // __AgentExtension_h_
