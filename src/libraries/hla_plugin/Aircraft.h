// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef plugins_hla_Aircraft_h
#define plugins_hla_Aircraft_h

#include "HlaObject_ABC.h"
#include "EventListener_ABC.h"
#include "AggregateMarking.h"
#include "Spatial.h"
#include "IsPartOfStruct.h"
#include "rpr/EntityType.h"
#include "rpr/EntityIdentifier.h"
#include "rpr/ForceIdentifier.h"

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
    class ObjectListener_ABC;
    class ObjectListenerComposite;
    class MarkingFactory_ABC;
    class EntityIdentifierResolver_ABC;
    class FOM_Serializer_ABC;

// =============================================================================
/** @class  Aircraft
    @brief  Surface vessel
*/
// Created: SLI 2011-10-04
// =============================================================================
class Aircraft : public HlaObject_ABC
                    , private EventListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Aircraft( Agent_ABC& agent, const std::string& name,
                            rpr::ForceIdentifier force, const rpr::EntityType& type, const MarkingFactory_ABC& markingFactory,
                            const rpr::EntityIdentifier& entityId, EntityIdentifierResolver_ABC& entityIdentifierResolver, 
                            FOM_Serializer_ABC& fomSerializer, const std::string& rtiId, unsigned long simId );
             Aircraft( const std::string& identifier, EntityIdentifierResolver_ABC& entityIdentifierResolver, FOM_Serializer_ABC& fomSerializer );
    virtual ~Aircraft();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const;
    virtual void Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer_ABC& deserializer );
    virtual void Register( ObjectListener_ABC& listener );
    virtual void Unregister( ObjectListener_ABC& listener );
    virtual void Attach( Agent_ABC* agent, unsigned long simId );
    virtual Agent_ABC* const GetAgent() const;
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
    //! @name Member data
    //@{
    std::string identifier_;
    std::unique_ptr< ObjectListenerComposite > listeners_;
    Agent_ABC* agent_;
    EntityIdentifierResolver_ABC& entityIdentifierResolver_;
    FOM_Serializer_ABC& fomSerializer_;
    std::unique_ptr< AttributesUpdater > attributesUpdater_;
    rpr::ForceIdentifier force_;
    rpr::EntityType type_;
    Marking marking_;
    rpr::EntityIdentifier entityIdentifier_;
    Spatial spatial_;
    IsPartOfStruct isPartOf_;
    //@}
};

}
}

#endif // plugins_hla_Aircraft_h
