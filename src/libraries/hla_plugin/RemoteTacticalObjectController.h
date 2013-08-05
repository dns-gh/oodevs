// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_RemoteTacticalObjectController_H
#define plugins_hla_RemoteTacticalObjectController_H

#include "ClassListener_ABC.h"
#include "ObjectListener_ABC.h"

#include "rpr/Coordinates.h"
#include "tools/MessageObserver.h"

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <map>

namespace tools
{
    template< typename T > class MessageController_ABC;
}
namespace dispatcher
{
    class Logger_ABC;
}
namespace rpr
{
    class EntityTypeResolver_ABC;
}
namespace simulation
{
    class ObjectMagicAction;
}
namespace sword
{
    class SimToClient_Content;
    class ObjectMagicActionAck;
}
namespace plugins
{
namespace hla
{
    class ExtentResolver_ABC;
    class SideResolver_ABC;
    class RemoteTacticalObjectSubject_ABC;
    class PropagationManager_ABC;
    template< typename ResponseMessage > class ContextHandler_ABC;

/// =============================================================================
/// @class hla::RemoteTacticalObjectController
/// @brief hla::RemoteTacticalObjectController
/// @thread This type is not thread safe
///
/// Created: ahc 6 sept. 2012
/// =============================================================================
class RemoteTacticalObjectController : private ClassListener_ABC
                                     , private ObjectListener_ABC
                                     , private boost::noncopyable
{
public:
    //! @name Contructors destructors
    //@{
    RemoteTacticalObjectController( const ExtentResolver_ABC& extent, const SideResolver_ABC& sideResolver, const rpr::EntityTypeResolver_ABC& objectEntityTypeResolver,
            ContextHandler_ABC< sword::ObjectMagicActionAck >& contextHandler, RemoteTacticalObjectSubject_ABC& subject, dispatcher::Logger_ABC& logger, PropagationManager_ABC& propMgr );
    virtual ~RemoteTacticalObjectController();
    //@}

private:
    //! @name Types
    //@{
    typedef boost::shared_ptr< simulation::ObjectMagicAction > T_ObjectCreation;
    typedef std::map< std::string, T_ObjectCreation > T_ObjectCreations;
    typedef std::map< std::string, std::pair< double, double > > T_Centers;
    typedef std::map< std::string, std::vector< rpr::WorldLocation > > T_Perimeters;
    //@}

private:
    //! @name Operations
    //@{
    virtual void RemoteCreated( const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object );
    virtual void RemoteDestroyed( const std::string& identifier );
    virtual void LocalCreated( const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object );
    virtual void LocalDestroyed( const std::string& identifier );
    virtual void Divested( const std::string& identifier, const ::hla::T_AttributeIdentifiers& attributes );
    virtual void Acquired( const std::string& identifier, const ::hla::T_AttributeIdentifiers& attributes );
    virtual void Moved( const std::string& identifier, double latitude, double longitude );
    virtual void SideChanged( const std::string& identifier, rpr::ForceIdentifier side );
    virtual void NameChanged( const std::string& identifier, const std::string& name );
    virtual void TypeChanged( const std::string& identifier, const rpr::EntityType& type );
    virtual void EquipmentUpdated( const std::string& identifier, const rpr::EntityType& equipmentType, unsigned int available,
            unsigned int dead, unsigned int lightDamages, unsigned int heavyDamages );
    virtual void UniqueIdChanged( const std::string& identifier, const T_UniqueId& uniqueId );
    virtual void CallsignChanged( const std::string& identifier, const std::string& callsign );
    virtual void EmbeddedUnitListChanged( const std::string& identifier, const std::vector< T_UniqueId >& units );
    virtual void GeometryChanged( const std::string& identifier, const std::vector< rpr::WorldLocation >& perimeter, ObjectListener_ABC::GeometryType type );
    virtual void ParentChanged( const std::string& rtiIdentifier, const std::string& parentRtiId );
    virtual void SubAgregatesChanged( const std::string& rtiIdentifier, const std::set< std::string >& children );
    virtual void SubEntitiesChanged( const std::string& rtiIdentifier, const std::set< std::string >& children );
    virtual void PropagationChanged( const std::string& rtiIdentifier, const std::vector< ObjectListener_ABC::PropagationData >& data,
                int col, int lig, double xll, double yll, double dx, double dy );
    //@}

private:
    //! @name Helpers
    //@{
    void Send( simulation::ObjectMagicAction& message, const std::string& identifier );
    //@}

    //! @name Attributes
    //@{
    const ExtentResolver_ABC& extent_;
    const SideResolver_ABC& sideResolver_;
    const rpr::EntityTypeResolver_ABC& objectEntityTypeResolver_;
    ContextHandler_ABC< sword::ObjectMagicActionAck >& contextHandler_;
    RemoteTacticalObjectSubject_ABC& subject_;
    dispatcher::Logger_ABC& logger_;
    PropagationManager_ABC& propagationManager_;
    T_ObjectCreations objectCreations_;
    T_Centers centers_;
    T_Perimeters perimeters_;
    //@}
};

}
} // namespace hla
#endif // plugins_hla_RemoteTacticalObjectController_H
