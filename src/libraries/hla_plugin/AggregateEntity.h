// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef plugins_hla_AgentExtension_h
#define plugins_hla_AgentExtension_h

#include "EventListener_ABC.h"
#include "rpr/EntityIdentifier.h"
#include "rpr/EntityType.h"
#include "rpr/ForceIdentifier.h"
#include "Spatial.h"
#include "AggregateMarking.h"
#include "SilentEntity.h"
#include "Formation.h"
#include "Dimension.h"
#include <vector>
#include "Dynamic.h"
#include "DynamicArray.h"

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
    virtual void EmbarkmentChanged( bool mounted );
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
    T_Equipments equipments_;
    Dynamic< Spatial > spatial_;
    Dynamic< Formation > formation_;
    Dynamic< Dimension > dimensions_;
    Dynamic< unsigned short > numberOfSilentEntities_;
    DynamicArray< SilentEntity > silentEntities_;
    Dynamic< double > mounted_;
    //@}
};

}
}

#endif // plugins_hla_AgentExtension_h
