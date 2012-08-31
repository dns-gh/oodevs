// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_MINEFIELD_H_
#define plugins_hla_MINEFIELD_H_

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

/// =============================================================================
/// @class Minefield
/// @brief Minefield
/// @thread This type is not thread safe
///
/// Created: ahc  7 ao�t 2012
/// =============================================================================
class Minefield : public HlaObject_ABC
    , private TacticalObjectEventListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    Minefield( TacticalObject_ABC& object, unsigned int identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type,
            unsigned short siteID, unsigned short applicationID );
    Minefield( const std::string& identifier, EntityIdentifierResolver_ABC& entityIdentifierResolver );
    virtual  ~Minefield();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const;
    virtual void Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer_ABC& deserializer );
    virtual void SetIdentifier( const std::string& id );
    virtual const std::string& GetIdentifier() const;
    virtual void Attach( Agent_ABC* agent, unsigned long simId );
    virtual void Register( ObjectListener_ABC& listener );
    virtual void Unregister( ObjectListener_ABC& listener );
    virtual void ResetAttributes();
    //@}

private:
    //! @name Operations
    //@{
    virtual void SpatialChanged( const TacticalObjectEventListener_ABC::T_PositionVector& pos );
    void RegisterAttributes();
    //@}
    //! @name Attributes
    //@{
    std::auto_ptr< ObjectListenerComposite > listeners_;
    TacticalObject_ABC* object_;
    std::string identifier_;
    std::auto_ptr< AttributesUpdater > attributes_;
    rpr::EntityType type_;
    rpr::EntityIdentifier entityIdentifier_;
    rpr::ForceIdentifier force_;
    std::vector< rpr::EntityType > mineTypes_;
    rpr::WorldLocation center_;
    std::vector< rpr::PerimeterPoint > perimeter_;
    //@}
};

}
}
#endif // plugins_hla_MINEFIELD_H_
