// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NetnRemoteCallsignListener_h
#define plugins_hla_NetnRemoteCallsignListener_h

#include "RemoteAgentListener_ABC.h"
#include "ResponseObserver_ABC.h"

namespace sword
{
    class UnitCreation;
}

namespace plugins
{
namespace hla
{
    class CallsignResolver_ABC;
    class RemoteAgentSubject_ABC;
    template< typename Response > class ContextHandler_ABC;

// =============================================================================
/** @class  NetnRemoteCallsignListener
    @brief  Netn remote callsign listener
*/
// Created: SLI 2011-10-10
// =============================================================================
class NetnRemoteCallsignListener : private RemoteAgentListener_ABC
                                 , private ResponseObserver_ABC< sword::UnitCreation >
{
public:
    //! @name Constructors/Destructor
    //@{
             NetnRemoteCallsignListener( CallsignResolver_ABC& resolver, RemoteAgentSubject_ABC& subject,
                                         ContextHandler_ABC< sword::UnitCreation >& handler );
    virtual ~NetnRemoteCallsignListener();
    //@}

private:
    //! @name Operations
    //@{
    virtual void Created( const std::string& identifier );
    virtual void Destroyed( const std::string& identifier );
    virtual void Moved( const std::string& identifier, double latitude, double longitude );
    virtual void SideChanged( const std::string& identifier, rpr::ForceIdentifier side );
    virtual void NameChanged( const std::string& identifier, const std::string& name );
    virtual void TypeChanged( const std::string& identifier, const rpr::EntityType& type );
    virtual void EquipmentUpdated( const std::string& identifier, const rpr::EntityType& equipmentType, unsigned int number );
    virtual void UniqueIdChanged( const std::string& identifier, const std::string& uniqueId );
    virtual void CallsignChanged( const std::string& identifier, const std::string& callsign );
    //@}

    //! @name Operations
    //@{
    virtual void Notify( const sword::UnitCreation& message, const std::string& identifier );
    //@}

private:
    //! @name Helpers
    //@{
    void Add( const std::string& identifier );
    //@}

private:
    //! @name Types
    //@{
    struct T_Add
    {
    public:
        T_Add()
            : simulationIdentifier( 0u )
        {}
        unsigned int simulationIdentifier;
        std::string callsign;
        std::string uniqueId;
    };
    typedef std::map< std::string, T_Add > T_Adds;
    //@}

private:
    //! @name Member data
    //@{
    CallsignResolver_ABC& resolver_;
    RemoteAgentSubject_ABC& subject_;
    ContextHandler_ABC< sword::UnitCreation >& handler_;
    T_Adds adds_;
    //@}
};

}
}

#endif // plugins_hla_NetnRemoteCallsignListener_h
