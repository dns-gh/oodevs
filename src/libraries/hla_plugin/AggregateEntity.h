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

#include "EventListener_ABC.h"
#include "rpr/EntityIdentifier.h"
#include "rpr/EntityType.h"
#include "rpr/ForceIdentifier.h"
#include <vector>

namespace hla
{
    class AttributeIdentifier;
    class Deserializer;
    class UpdateFunctor_ABC;
}

namespace plugins
{
namespace hla
{
    class Agent_ABC;
    class Spatial;

// =============================================================================
/** @class  AggregateEntity
    @brief  Aggregate entity
*/
// Created: SBO 2008-02-18
// =============================================================================
class AggregateEntity : private EventListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             AggregateEntity( Agent_ABC& agent, const rpr::EntityIdentifier& identifier,
                              const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type );
    virtual ~AggregateEntity();
    //@}

    //! @name Operations
    //@{
    void Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const;
    void Deserialize( const ::hla::AttributeIdentifier& identifier, const ::hla::Deserializer& deserializer );
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
    void UpdateFormation( ::hla::UpdateFunctor_ABC& functor ) const;
    void UpdateDimensions( ::hla::UpdateFunctor_ABC& functor ) const;
    void UpdateEchelon( ::hla::UpdateFunctor_ABC& functor ) const;
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
    rpr::EntityIdentifier identifier_;
    const std::string name_;
    const rpr::ForceIdentifier force_;
    const rpr::EntityType type_;
    mutable bool formationChanged_;
    mutable bool dimensionsChanged_;
    bool isOnRoad_;
    mutable bool spatialChanged_;
    std::auto_ptr< Spatial > pSpatial_;
    mutable bool compositionChanged_;
    T_Equipments equipments_;
    //@}
};

}
}

#endif // __AgentExtension_h_
