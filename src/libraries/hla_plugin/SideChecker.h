// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_SideChecker_h
#define plugins_hla_SideChecker_h

#include "SideChecker_ABC.h"
#include "AgentListener_ABC.h"
#include "RemoteAgentListener_ABC.h"
#include <map>

namespace plugins
{
namespace hla
{
    class AgentSubject_ABC;
    class RemoteAgentSubject_ABC;
    class RemoteAgentResolver_ABC;

// =============================================================================
/** @class  SideChecker
    @brief  Side checker
*/
// Created: SLI 2011-10-11
// =============================================================================
class SideChecker : public SideChecker_ABC
                  , private AgentListener_ABC
                  , private RemoteAgentListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SideChecker( AgentSubject_ABC& localSubject, RemoteAgentSubject_ABC& remoteSubject, const RemoteAgentResolver_ABC& remoteResolver );
    virtual ~SideChecker();
    //@}

    //! @name Operations
    //@{
    virtual bool AreSameSide( unsigned long firstIdentifier, unsigned long secondIdentifier ) const;
    //@}

private:
    //! @name Operations
    //@{
    virtual void AggregateCreated( Agent_ABC& agent, unsigned int identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type );
    virtual void SurfaceVesselCreated( Agent_ABC& agent, unsigned int identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type );
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

private:
    //! @name Helpers
    //@{
    int Find( unsigned int identifier ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< unsigned int, rpr::ForceIdentifier > T_LocalSides;
    typedef std::map< std::string, rpr::ForceIdentifier > T_RemoteSides;
    //@}

private:
    //! @name Member data
    //@{
    AgentSubject_ABC& localSubject_;
    RemoteAgentSubject_ABC& remoteSubject_;
    const RemoteAgentResolver_ABC& remoteResolver_;
    T_LocalSides localSides_;
    T_RemoteSides remoteSides_;
    //@}
};

}
}

#endif // plugins_hla_SideChecker_h
