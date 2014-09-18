// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef plugins_hla_GroundVehicle_h
#define plugins_hla_GroundVehicle_h

#include "HlaObject_ABC.h"
#include "EventListener_ABC.h"
#include "IsPartOfStruct.h"
#include "rpr/ForceIdentifier.h"
#include "rpr/EntityType.h"

namespace hla
{
    class AttributeIdentifier;
    class Deserializer;
    class UpdateFunctor_ABC;
}

namespace rpr
{
    class EntityType;
    class EntityIdentifier;
}

namespace plugins
{
namespace hla
{
    class Agent_ABC;
    class AttributesSerializer;
    class ObjectListener_ABC;
    class ObjectListenerComposite;
    class MarkingFactory_ABC;
    class EntityIdentifierResolver_ABC;
    class FOM_Serializer_ABC;

// =============================================================================
/** @class  GroundVehicle
    @brief  Surface vessel
*/
    // Created: AHC 2012-07-26
// =============================================================================
class GroundVehicle : public HlaObject_ABC
                    , private EventListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             GroundVehicle( Agent_ABC& agent, const std::string& name,
                            rpr::ForceIdentifier force, const rpr::EntityType& type, const MarkingFactory_ABC& markingFactory,
                            const rpr::EntityIdentifier& entityId, EntityIdentifierResolver_ABC& entityIdentifierResolver,
                            FOM_Serializer_ABC& fomSerializer, const std::string& rtiId, unsigned long simId );
    virtual ~GroundVehicle( );
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const;
    virtual void Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer_ABC& deserializer );
    virtual const std::string& GetIdentifier() const;
    virtual void Register( ObjectListener_ABC& listener );
    virtual void Unregister( ObjectListener_ABC& listener );
    virtual void Attach( Agent_ABC* agent, unsigned long simId );
    virtual Agent_ABC* const GetAgent() const;
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

private:
    //! @name Member data
    //@{
    std::string identifier_;
    FOM_Serializer_ABC& fomSerializer_;
    std::unique_ptr< ObjectListenerComposite > listeners_;
    Agent_ABC& agent_;
    std::unique_ptr< AttributesSerializer > attributes_;
    IsPartOfStruct isPartOf_;
    //@}
};

}
}

#endif // plugins_hla_GroundVehicle_h
