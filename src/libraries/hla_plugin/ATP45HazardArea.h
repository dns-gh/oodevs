// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_ATP45HazardArea_H_
#define plugins_hla_ATP45HazardArea_H_

#include "HlaObject_ABC.h"
#include "TacticalObjectEventListener_ABC.h"
#include "rpr/ForceIdentifier.h"
#include "CBRNHazardArea.h"


namespace hla
{
    class AttributeIdentifier;
    class Deserializer_ABC;
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
    class AttributesUpdater;
    class ObjectListenerComposite;
    class TacticalObject_ABC;
    class EntityIdentifierResolver_ABC;
    class FOM_Serializer_ABC;

/// =============================================================================
/// @class ATP45HazardArea
/// @brief ATP45HazardArea
/// @thread This type is not thread safe
///
/// Created: 2013-06-28
/// =============================================================================
class ATP45HazardArea : public HlaObject_ABC
    , public TacticalObjectEventListener_ABC
    , public ObjectLocationEventListener_ABC
    , public ObjectPropagationEventListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    ATP45HazardArea( TacticalObject_ABC& object, unsigned int identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type,
        const rpr::EntityIdentifier& entityId, const std::string& rtiId );
    ATP45HazardArea( const std::string& identifier, EntityIdentifierResolver_ABC& entityIdentifierResolver, FOM_Serializer_ABC& fomSerializer );
    virtual  ~ATP45HazardArea();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const;
    virtual void Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer_ABC& deserializer );
    virtual const std::string& GetIdentifier() const;
    virtual void Attach( Agent_ABC* agent, unsigned long simId );
    virtual void Register( ObjectListener_ABC& listener );
    virtual void Unregister( ObjectListener_ABC& listener );
    virtual void ResetAttributes();
    //@}

private:
    //! @name Operations
    //@{
    virtual void SpatialChanged( const ObjectLocationEventListener_ABC::T_PositionVector& pos );
    virtual void ResourcesChanged( const ObjectLocationEventListener_ABC::T_ResourceVector& res );
    virtual void PropagationChanged( const ObjectPropagationEventListener_ABC::T_DataVector& pos );
    void RegisterAttributes();
    //@}
    //! @name Attributes
    //@{
    std::auto_ptr< ObjectListenerComposite > listeners_;
    TacticalObject_ABC* object_;
    std::string identifier_;
    std::auto_ptr< AttributesUpdater > attributes_;
    std::vector< rpr::WorldLocation > locations_;
    int16_t areaType_;
    int16_t agentClass_;
    //@}
};

}
}
#endif // plugins_hla_ATP45HazardArea_H_
