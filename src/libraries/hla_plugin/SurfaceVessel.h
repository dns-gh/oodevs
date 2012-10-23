// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef plugins_hla_SurfaceVessel_h
#define plugins_hla_SurfaceVessel_h

#include "HlaObject_ABC.h"
#include "EventListener_ABC.h"
#include "AggregateMarking.h"
#include "Spatial.h"
#include "rpr/ForceIdentifier.h"
#include "rpr/EntityType.h"
#include "rpr/EntityIdentifier.h"
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
    class ObjectListener_ABC;
    class ObjectListenerComposite;
    class MarkingFactory_ABC;
    class EntityIdentifierResolver_ABC;
    class FOM_Serializer_ABC;

// =============================================================================
/** @class  SurfaceVessel
    @brief  Surface vessel
*/
// Created: SLI 2011-10-04
// =============================================================================
class SurfaceVessel : public HlaObject_ABC
                    , private EventListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SurfaceVessel( Agent_ABC& agent, unsigned long identifier,
                            const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type, const MarkingFactory_ABC& markingFactory,
                            unsigned short siteID, unsigned short applicationID, EntityIdentifierResolver_ABC& entityIdentifierResolver, FOM_Serializer_ABC& fomSerializer );
             SurfaceVessel( const std::string& identifier, EntityIdentifierResolver_ABC& entityIdentifierResolver, FOM_Serializer_ABC& fomSerializer );
    virtual ~SurfaceVessel();
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
    virtual void EquipmentChanged( unsigned int type, const rpr::EntityType& entityType, unsigned int available );
    virtual void EmbarkmentChanged( bool mounted );
    virtual void PlatformAdded( const std::string& name, unsigned int id );
    virtual void ChildrenChanged( const T_ChildrenIds& children );
    virtual void ParentChanged( const std::string& parentId );
    //@}

    //! @name Operations
    //@{
    void RegisterAttributes( );
    //@}

private:
    //! @name Member data
    //@{
    std::string identifier_;
    std::auto_ptr< ObjectListenerComposite > listeners_;
    Agent_ABC* agent_;
    EntityIdentifierResolver_ABC& entityIdentifierResolver_;
    FOM_Serializer_ABC& fomSerializer_;
    std::auto_ptr< AttributesUpdater > attributesUpdater_;
    unsigned long simIdentifier_;
    rpr::ForceIdentifier force_;
    rpr::EntityType type_;
    Marking marking_;
    rpr::EntityIdentifier entityIdentifier_;
    Spatial spatial_;
    //@}
};

}
}

#endif // plugins_hla_SurfaceVessel_h
