// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef plugins_hla_Human_h
#define plugins_hla_Human_h

#include "HlaObject_ABC.h"
#include "EventListener_ABC.h"
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
/** @class  Human
    @brief  Human
*/
    // Created: AHC 2012-11-12
// =============================================================================
class Human : public HlaObject_ABC
                    , private EventListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
        Human( Agent_ABC& agent, const std::string& name,
                            rpr::ForceIdentifier force, const rpr::EntityType& type, const MarkingFactory_ABC& markingFactory,
                            const rpr::EntityIdentifier& entityId, EntityIdentifierResolver_ABC& entityIdentifierResolver,
                            FOM_Serializer_ABC& fomSerializer, const std::string& rtiId, unsigned long simId );
    virtual ~Human( );
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const;
    virtual void Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer_ABC& deserializer );
    virtual const std::string& GetIdentifier() const;
    virtual void Register( ObjectListener_ABC& listener );
    virtual void Unregister( ObjectListener_ABC& listener );
    virtual void Attach( Agent_ABC* agent, unsigned long simId );
    virtual void ResetAttributes();
    //@}

private:
    //! @name Observer
    //@{
    virtual void SpatialChanged( double latitude, double longitude, float altitude, float speed, float direction );
    virtual void FormationChanged( bool isOnRoad );
    virtual void EquipmentChanged( unsigned int type, const rpr::EntityType& entityType, unsigned int available );
    virtual void EmbarkmentChanged( bool mounted );
    virtual void PlatformAdded( const std::string& name, unsigned int id );
    virtual void ChildrenChanged( const T_ChildrenIds& children );
    virtual void ParentChanged( const std::string& parentId );
    //@}

private:
    //! @name Member data
    //@{
    std::string identifier_;
    FOM_Serializer_ABC& fomSerializer_;
    std::auto_ptr< ObjectListenerComposite > listeners_;
    Agent_ABC& agent_;
    std::auto_ptr< AttributesSerializer > attributes_;
    //@}
};

}
}

#endif // plugins_hla_Human_h
