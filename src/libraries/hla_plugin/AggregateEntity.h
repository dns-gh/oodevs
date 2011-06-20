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
#include "rpr/ForceIdentifier.h"
#include <vector>

namespace hla
{
    class AttributeIdentifier;
    class Deserializer;
    class UpdateFunctor_ABC;
}

namespace rpr
{
    class EntityIdentifier;
    class EntityType;
}

namespace plugins
{
namespace hla
{
    class Agent_ABC;
    class AttributesSerializer;

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
    T_Equipments equipments_;
    std::auto_ptr< AttributesSerializer > attributes_;
    //@}
};

}
}

#endif // plugins_hla_AgentExtension_h
