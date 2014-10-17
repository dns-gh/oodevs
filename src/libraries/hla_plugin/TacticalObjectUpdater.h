// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_TacticalObjectUpdater_h
#define plugins_hla_TacticalObjectUpdater_h

#include "ClassListener_ABC.h"
#include "ObjectListener_ABC.h"
#include "ResponseObserver_ABC.h"
#include "tools/MessageObserver.h"

namespace tools
{
    template< typename T > class MessageController_ABC;
}

namespace sword
{
    class ObjectMagicActionAck;
    class SimToClient_Content;
}

namespace dispatcher
{
    class SimulationPublisher_ABC;
    class Logger_ABC;
}

namespace plugins
{
namespace hla
{
    template< typename ResponseMessage > class ContextHandler_ABC;
    class PropagationManager_ABC;
    class RemoteTacticalObjectSubject_ABC;
    class SimulationTimeManager_ABC;

// =============================================================================
/** @class  TacticalObjectUpdater
    @brief  TacticalObjectUpdater
*/
// Created: AHC 2013-09-09
// =============================================================================
class TacticalObjectUpdater : private ResponseObserver_ABC< sword::ObjectMagicActionAck >
                            , private ClassListener_ABC
                            , private ObjectListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
            TacticalObjectUpdater( dispatcher::SimulationPublisher_ABC& publisher, ContextHandler_ABC< sword::ObjectMagicActionAck >& contextHandler, dispatcher::Logger_ABC& logger,
                    PropagationManager_ABC& propMgr, RemoteTacticalObjectSubject_ABC& subject, const SimulationTimeManager_ABC& timeManager );
    virtual ~TacticalObjectUpdater();
    //@}

private:
    //! @name Operations
    //@{
    virtual void RemoteCreated( const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object );
    virtual void RemoteDestroyed( const std::string& identifier );
    virtual void LocalCreated( const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object );
    virtual void LocalDestroyed( const std::string& identifier );
    virtual void Divested( const std::string& identifier, const T_AttributeIdentifiers& attributes );
    virtual void Acquired( const std::string& identifier, const T_AttributeIdentifiers& attributes );
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
    virtual void SubAgregatesChanged( const std::string& rtiIdentifier, const ObjectListener_ABC::T_EntityIDs& children );
    virtual void SubEntitiesChanged( const std::string& rtiIdentifier, const ObjectListener_ABC::T_EntityIDs& children );
    virtual void PropagationChanged( const std::string& rtiIdentifier, const std::vector< ObjectListener_ABC::PropagationData >& data,
                int col, int lig, double xll, double yll, double dx, double dy );
    //@}

    //! @name Operations
    //@{
    virtual void Notify( const sword::ObjectMagicActionAck& message, const std::string& identifier );
    //@}

private:
    //! @name Helpers
    //@{
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, unsigned long > T_Identifiers;
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::SimulationPublisher_ABC& publisher_;
    ContextHandler_ABC< sword::ObjectMagicActionAck >& contextHandler_;
    dispatcher::Logger_ABC& logger_;
    PropagationManager_ABC& propagationManager_;
    RemoteTacticalObjectSubject_ABC& subject_;
    const SimulationTimeManager_ABC& timeManager_;
    T_Identifiers identifiers_;
    //@}
};

}
}

#endif // plugins_hla_TacticalObjectUpdater_h
