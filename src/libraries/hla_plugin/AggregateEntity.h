// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef plugins_hla_AggregateEntity_h
#define plugins_hla_AggregateEntity_h

#include "HlaObject_ABC.h"
#include "EventListener_ABC.h"
#include "Omt13String.h"
#include "IsPartOfStruct.h"
#include "AggregateMarking.h"
#include "rpr/ForceIdentifier.h"
#include "rpr/EntityType.h"
#include "rpr/EntityIdentifier.h"
#include "Spatial.h"
#include <vector>
#include <string>
#include <memory>

namespace hla
{
    class AttributeIdentifier;
    class Deserializer;
    class UpdateFunctor_ABC;
}

namespace rpr
{
    class EntityType;
}

namespace plugins
{
namespace hla
{
    class Agent_ABC;
    class AttributesUpdater;
    class MarkingFactory_ABC;
    class ObjectListener_ABC;
    class ObjectListenerComposite;
    class EntityIdentifierResolver_ABC;
    class FOM_Serializer_ABC;

// =============================================================================
/** @class  AggregateEntity
    @brief  Aggregate entity
*/
// Created: SBO 2008-02-18
// =============================================================================
class AggregateEntity : public HlaObject_ABC
                      , private EventListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             AggregateEntity( Agent_ABC& agent, const std::string& name,
                              rpr::ForceIdentifier force, const rpr::EntityType& type, const MarkingFactory_ABC& markingFactory,
                              const rpr::EntityIdentifier& entityId, EntityIdentifierResolver_ABC& entityIdentifierResolver, 
                              FOM_Serializer_ABC& fomSerializer, const std::string& rtiId, unsigned long simId );
             AggregateEntity( const std::string& identifier, EntityIdentifierResolver_ABC& entityIdentifierResolver, FOM_Serializer_ABC& fomSerializer );
    virtual ~AggregateEntity();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const;
    virtual void Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer_ABC& deserializer );
    virtual void Register( ObjectListener_ABC& listener );
    virtual void Unregister( ObjectListener_ABC& listener );
    virtual void Attach( Agent_ABC* agent, unsigned long simId );
    virtual const std::string& GetIdentifier() const;
    virtual void ResetAttributes();
    //@}

private:
    //! @name Observer
    //@{
    virtual void SpatialChanged( double latitude, double longitude, float altitude, float speed, float direction );
    virtual void FormationChanged( bool isOnRoad );
    virtual void EquipmentChanged( unsigned int type, const rpr::EntityType& entityType, unsigned int available,
            unsigned int dead, unsigned int lightDamages, unsigned int heavyDamages );
    virtual void EmbarkmentChanged( bool mounted );
    virtual void PlatformAdded( const std::string& name, unsigned int id );
    virtual void ChildrenChanged( const T_ChildrenIds& children );
    virtual void ParentChanged( const std::string& parentId );
    virtual void StateChanged( rpr::DamageState32 state );
    //@}

    //! @name Operations
    //@{
    void RegisterAttributes( );
    //@}

private:
    //! @name Types
    //@{
    struct T_Equipment
    {
        T_Equipment()
            : type_( 0 )
            , available_( 0 )
            , dead_( 0 )
            , lightDamages_( 0 )
            , heavyDamages_( 0 )
        {}
        T_Equipment( unsigned int type, unsigned int available, unsigned int dead, unsigned int lightDamages, unsigned int heavyDamages, const rpr::EntityType& entityType )
            : type_      ( type )
            , available_ ( available )
            , dead_( dead )
            , lightDamages_( lightDamages )
            , heavyDamages_( heavyDamages )
            , entityType_( entityType )
        {}
        unsigned int type_;
        unsigned int available_;
        unsigned int dead_;
        unsigned int lightDamages_;
        unsigned int heavyDamages_;
        rpr::EntityType entityType_;
    };
    typedef std::vector< T_Equipment >  T_Equipments;
    typedef T_Equipments::iterator     IT_Equipments;
    //@}

private:
    //! @name Member data
    //@{
    std::string identifier_;
    std::unique_ptr< ObjectListenerComposite > listeners_;
    Agent_ABC* agent_;
    EntityIdentifierResolver_ABC& entityIdentifierResolver_;
    FOM_Serializer_ABC& fomSerializer_;
    std::unique_ptr< AttributesUpdater > attributesUpdater_;
    T_Equipments equipments_;
    Omt13StringArray entities_;    
    Omt13StringArray subAggregates_;
    unsigned short numberOfSilentEntities_;
    rpr::ForceIdentifier force_;
    rpr::EntityType type_;
    AggregateMarking marking_;
    rpr::EntityIdentifier entityIdentifier_;
    unsigned char aggregateState_;
    Spatial spatial_;
    //@}
};

}
}

#endif // plugins_hla_AggregateEntity_h
