// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef plugins_hla_SurfaceVessel_h
#define plugins_hla_SurfaceVessel_h

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
}

namespace plugins
{
namespace hla
{
    class Agent_ABC;
    class AttributesSerializer;
    class MarkingFactory_ABC;

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
             SurfaceVessel( Agent_ABC& agent, unsigned int identifier,
                            const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type, const MarkingFactory_ABC& markingFactory );
    virtual ~SurfaceVessel();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const;
    virtual void Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer deserializer );
    //@}

private:
    //! @name Observer
    //@{
    virtual void SpatialChanged( double latitude, double longitude, float altitude, float speed, float direction );
    virtual void FormationChanged( bool isOnRoad );
    virtual void EquipmentChanged( unsigned int type, const rpr::EntityType& entityType, unsigned int available );
    virtual void EmbarkmentChanged( bool mounted );
    //@}

private:
    //! @name Member data
    //@{
    Agent_ABC& agent_;
    std::auto_ptr< AttributesSerializer > attributes_;
    //@}
};

}
}

#endif // plugins_hla_SurfaceVessel_h
