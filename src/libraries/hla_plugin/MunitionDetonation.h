// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_MunitionDetonation_h
#define plugins_hla_MunitionDetonation_h

#include "RemoteAgentListener_ABC.h"
#include "rpr/Coordinates.h"
#include "tools/MessageObserver.h"
#include <map>

namespace sword
{
    class SimToClient_Content;
    class StartUnitFire;
    class StopUnitFire;
}

namespace hla
{
    template< typename T > class Interaction;
    template< typename T > class InteractionNotification_ABC;
}

namespace tools
{
    template< typename Category > class MessageController_ABC;
}

namespace plugins
{
namespace hla
{
    class Federate_ABC;
    class RemoteAgentResolver_ABC;
    class RemoteAgentSubject_ABC;

// =============================================================================
/** @class  MunitionDetonation
    @brief  Munition detonation
*/
// Created: SLI 2011-06-24
// =============================================================================
class MunitionDetonation : private RemoteAgentListener_ABC
                         , private tools::MessageObserver< sword::StartUnitFire >
                         , private tools::MessageObserver< sword::StopUnitFire >
{
public:
    //! @name Constructors/Destructor
    //@{
             MunitionDetonation( Federate_ABC& federate, tools::MessageController_ABC< sword::SimToClient_Content >& controller,
                                 const RemoteAgentResolver_ABC& resolver, RemoteAgentSubject_ABC& remoteAgentSubject,
                                 const std::string& federateName );
    virtual ~MunitionDetonation();
    //@}

private:
    //! @name Messages
    //@{
    virtual void Notify( const sword::StartUnitFire& message, int context );
    virtual void Notify( const sword::StopUnitFire& message, int context );
    //@}

    //! @name Operations
    //@{
    virtual void Created( const std::string& identifier );
    virtual void Destroyed( const std::string& identifier );
    virtual void Moved( const std::string& identifier, double latitude, double longitude );
    virtual void SideChanged( const std::string& identifier, rpr::ForceIdentifier side );
    virtual void NameChanged( const std::string& identifier, const std::string& name );
    virtual void TypeChanged( const std::string& identifier, const rpr::EntityType& type );
    //@}

private:
    //! @name Types
    //@{
    struct Parameters;
    typedef std::map< std::string, rpr::WorldLocation > T_Positions;
    typedef std::map< unsigned int, sword::StartUnitFire > T_Fires;
    //@}

private:
    //! @name Member data
    //@{
    const RemoteAgentResolver_ABC& resolver_;
    RemoteAgentSubject_ABC& remoteAgentSubject_;
    const std::string federateName_;
    std::auto_ptr< ::hla::InteractionNotification_ABC< Parameters > > pNotification_;
    std::auto_ptr< ::hla::Interaction< Parameters > > pInteraction_;
    T_Fires fires_;
    T_Positions positions_;
    //@}
};

}
}

#endif // plugins_hla_MunitionDetonation_h
