// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_CulturalFeature_H_
#define plugins_hla_CulturalFeature_H_

#include "HlaObject_ABC.h"
#include "TacticalObjectEventListener_ABC.h"
#include "Spatial.h"

#include "rpr/ForceIdentifier.h"
#include "rpr/EntityType.h"
#include "rpr/EntityIdentifier.h"

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

/// =============================================================================
/// @class CulturalFeature
/// @brief CulturalFeature
/// @thread This type is not thread safe
///
/// Created: 2013-01-21
/// =============================================================================
class CulturalFeature : public HlaObject_ABC
    , public TacticalObjectEventListener_ABC
    , public ObjectLocationEventListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    CulturalFeature( TacticalObject_ABC& object, unsigned int identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type,
        const rpr::EntityIdentifier& entityId, const std::string& rtiId );
    CulturalFeature( const std::string& identifier, EntityIdentifierResolver_ABC& entityIdentifierResolver, FOM_Serializer_ABC& fomSerializer );
    virtual  ~CulturalFeature();
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
    Spatial spatial_;
    //@}
};

}
}
#endif // plugins_hla_CulturalFeature_H_
