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
#include "AgentListener_ABC.h"
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
    class DotationTypeResolver_ABC;
    class AgentSubject_ABC;
    struct ChildListener;

namespace interactions
{
    struct MunitionDetonation;
    struct WeaponFire;
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
                       , private AgentListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DirectFireSender( InteractionSender_ABC< interactions::MunitionDetonation >& interactionSender,
                               InteractionSender_ABC< interactions::WeaponFire >& weaponFireSender,
                               const RemoteAgentResolver_ABC& remoteResolver, const LocalAgentResolver_ABC& localResolver,
                               RemoteAgentSubject_ABC& remoteAgentSubject, tools::MessageController_ABC< sword::SimToClient_Content >& controller,
                               const std::string& federateName, const DotationTypeResolver_ABC& munitionTypeResolver, AgentSubject_ABC& agentSubject );
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
    // AgentListener_ABC
    virtual void AggregateCreated( Agent_ABC& agent, unsigned long identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type, const std::string& symbol, bool isLocal, const std::vector< char >& uniqueId );
    virtual void PlatformCreated( Agent_ABC& agent, unsigned int identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type, const std::string& symbol, const std::vector< char >& uniqueId );
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
    typedef std::map< unsigned long, std::pair< boost::shared_ptr< ChildListener >, Agent_ABC* > > T_LocalListeners;
    typedef std::map< std::string, std::set< std::string > > T_ChildrenRtiIds;
    //@}

private:
    //! @name Helpers
    //@{
    void ComputeChildrenRtiIds( unsigned int parentId );
    void DoPlatformsFire( unsigned long fireIdentifier, const std::string& firingRtiId, unsigned long firingSimId, 
        const std::string& targetIdentifier, const interactions::MunitionDetonation& parentDeto );
    //@}

    //! @name Member data
    //@{
    InteractionSender_ABC< interactions::MunitionDetonation >& interactionSender_;
    InteractionSender_ABC< interactions::WeaponFire >& weaponFireSender_;
    const RemoteAgentResolver_ABC& remoteResolver_;
    const LocalAgentResolver_ABC& localResolver_;
    RemoteAgentSubject_ABC& remoteAgentSubject_;
    const std::string federateName_;
    const DotationTypeResolver_ABC& munitionTypeResolver_;
    AgentSubject_ABC& localAgentSubject_;
    T_Fires fires_;
    T_Positions positions_;
    T_LocalListeners listeners_;
    T_ChildrenRtiIds childrenRtiIds_;
    //@}
};

}
}

#endif // plugins_hla_DirectFireSender_h
