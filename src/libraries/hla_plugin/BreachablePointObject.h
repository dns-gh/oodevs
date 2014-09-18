// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_BreachablePointObject_H_
#define plugins_hla_BreachablePointObject_H_

#include "HlaObject_ABC.h"
#include "TacticalObjectEventListener_ABC.h"

#include "rpr/ForceIdentifier.h"
#include "rpr/EntityType.h"
#include "rpr/EntityIdentifier.h"
#include "rpr/Coordinates.h"

namespace hla
{
    class AttributeIdentifier;
    class Deserializer_ABC;
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
    class AttributesUpdater;
    class ObjectListenerComposite;
    class TacticalObject_ABC;
    class EntityIdentifierResolver_ABC;
    class FOM_Serializer_ABC;

class BreachablePointObject : public HlaObject_ABC
    , public TacticalObjectEventListener_ABC
    , public ObjectLocationEventListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    BreachablePointObject( TacticalObject_ABC& object, unsigned int identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type,
        const rpr::EntityIdentifier& entityId, const std::string& rtiId );
    BreachablePointObject( const std::string& identifier, EntityIdentifierResolver_ABC& entityIdentifierResolver, FOM_Serializer_ABC& fomSerializer );
    virtual  ~BreachablePointObject();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const;
    virtual void Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer_ABC& deserializer );
    virtual const std::string& GetIdentifier() const;
    virtual void Attach( Agent_ABC* agent, unsigned long simId );
    virtual Agent_ABC* const GetAgent() const;
    virtual void Register( ObjectListener_ABC& listener );
    virtual void Unregister( ObjectListener_ABC& listener );
    virtual void ResetAttributes();
    //@}

private:
    //! @name Operations
    //@{
    virtual void SpatialChanged( const ObjectLocationEventListener_ABC::T_PositionVector& pos );
    virtual void ResourcesChanged( const ObjectLocationEventListener_ABC::T_ResourceVector& res );
    void RegisterAttributes();
    //@}
    //! @name Attributes
    //@{
    std::unique_ptr< ObjectListenerComposite > listeners_;
    TacticalObject_ABC* object_;
    std::string identifier_;
    std::unique_ptr< AttributesUpdater > attributes_;
    rpr::EntityType type_;
    rpr::EntityIdentifier entityIdentifier_;
    rpr::ForceIdentifier force_;
    rpr::WorldLocation center_;
    //@}
};

}
}
#endif // plugins_hla_BreachablePointObject_H_
