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
#include "ClassListener_ABC.h"
#include "ObjectListener_ABC.h"
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
                  , private ClassListener_ABC
                  , private ObjectListener_ABC
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
    virtual void AggregateCreated( Agent_ABC& agent, unsigned long identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type, const std::string& symbol, bool isLocal, const std::vector< char >& uniqueId );
    virtual void PlatformCreated( Agent_ABC& agent, unsigned int identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type, const std::string& symbol, const std::vector< char >& /*uniqueId*/ );
    virtual void SurfaceVesselCreated( Agent_ABC& agent, unsigned int identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type );
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
    virtual void SubAgregatesChanged( const std::string& rtiIdentifier, const ObjectListener_ABC::T_EntityIDs& children );
    virtual void SubEntitiesChanged( const std::string& rtiIdentifier, const ObjectListener_ABC::T_EntityIDs& children );
    virtual void PropagationChanged( const std::string& rtiIdentifier, const std::vector< ObjectListener_ABC::PropagationData >& data,
            int col, int lig, double xll, double yll, double dx, double dy );
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
