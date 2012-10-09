// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_DirectFireSender_h
#define plugins_hla_DirectFireSender_h

#include "ClassListener_ABC.h"
#include "ObjectListener_ABC.h"
#include "tools/MessageObserver.h"
#include "rpr/Coordinates.h"
#include <map>

namespace sword
{
    class SimToClient_Content;
    class StartUnitFire;
    class StopUnitFire;
}

namespace tools
{
    template< typename Category > class MessageController_ABC;
}

namespace plugins
{
namespace hla
{
    class RemoteAgentResolver_ABC;
    class LocalAgentResolver_ABC;
    class RemoteAgentSubject_ABC;
    template< typename Interaction > class InteractionSender_ABC;

namespace interactions
{
    struct MunitionDetonation;
}
// =============================================================================
/** @class  DirectFireSender
    @brief  Direct fire sender
*/
// Created: SLI 2011-09-23
// =============================================================================
class DirectFireSender : private tools::MessageObserver< sword::StartUnitFire >
                       , private tools::MessageObserver< sword::StopUnitFire >
                       , private ClassListener_ABC
                       , private ObjectListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DirectFireSender( InteractionSender_ABC< interactions::MunitionDetonation >& interactionSender,
                               const RemoteAgentResolver_ABC& remoteResolver, const LocalAgentResolver_ABC& localResolver,
                               RemoteAgentSubject_ABC& remoteAgentSubject, tools::MessageController_ABC< sword::SimToClient_Content >& controller, const std::string& federateName );
    virtual ~DirectFireSender();
    //@}

private:
    //! @name Operations
    //@{
    virtual void RemoteCreated( const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object );
    virtual void RemoteDestroyed( const std::string& identifier );
    virtual void LocalCreated( const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object );
    virtual void LocalDestroyed( const std::string& identifier );
    virtual void Divested( const std::string& identifier );
    virtual void Acquired( const std::string& identifier );
    virtual void Moved( const std::string& identifier, double latitude, double longitude );
    virtual void SideChanged( const std::string& identifier, rpr::ForceIdentifier side );
    virtual void NameChanged( const std::string& identifier, const std::string& name );
    virtual void TypeChanged( const std::string& identifier, const rpr::EntityType& type );
    virtual void EquipmentUpdated( const std::string& identifier, const rpr::EntityType& equipmentType, unsigned int number );
    virtual void UniqueIdChanged( const std::string& identifier, const std::string& uniqueId );
    virtual void CallsignChanged( const std::string& identifier, const std::string& callsign );
    virtual void EmbeddedUnitListChanged( const std::string& identifier, const std::vector< std::string >& units );
    virtual void PerimeterChanged( const std::string& identifier, const std::vector< rpr::PerimeterPoint >& perimeter );
    virtual void ParentChanged( const std::string& rtiIdentifier, const std::string& parentRtiId );
    virtual void SubAgregatesChanged( const std::string& rtiIdentifier, const ObjectListener_ABC::T_EntityIDs& children );
    virtual void SubEntitiesChanged( const std::string& rtiIdentifier, const ObjectListener_ABC::T_EntityIDs& children );
    //@}

private:
    //! @name Messages
    //@{
    virtual void Notify( const sword::StartUnitFire& message, int context );
    virtual void Notify( const sword::StopUnitFire& message, int context );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, rpr::WorldLocation > T_Positions;
    typedef std::map< unsigned int, sword::StartUnitFire > T_Fires;
    //@}

private:
    //! @name Member data
    //@{
    InteractionSender_ABC< interactions::MunitionDetonation >& interactionSender_;
    const RemoteAgentResolver_ABC& remoteResolver_;
    const LocalAgentResolver_ABC& localResolver_;
    RemoteAgentSubject_ABC& remoteAgentSubject_;
    const std::string federateName_;
    T_Fires fires_;
    T_Positions positions_;
    //@}
};

}
}

#endif // plugins_hla_DirectFireSender_h
