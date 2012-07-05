// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_RemoteAgentResolver_h
#define plugins_hla_RemoteAgentResolver_h

#include "RemoteAgentResolver_ABC.h"
#include "ClassListener_ABC.h"
#include "ObjectListener_ABC.h"
#include "ResponseObserver_ABC.h"
#pragma warning( push, 0 )
#include <boost/bimap.hpp>
#pragma warning( pop )
#include <set>

namespace sword
{
    class UnitCreation;
}

namespace plugins
{
namespace hla
{
    class RemoteAgentSubject_ABC;
    template< typename ResponseMessage > class ContextHandler_ABC;

// =============================================================================
/** @class  RemoteAgentResolver
    @brief  RemoteAgentResolver
*/
// Created: SLI 2011-09-22
// =============================================================================
class RemoteAgentResolver : public RemoteAgentResolver_ABC
                          , private ResponseObserver_ABC< sword::UnitCreation >
                          , private ClassListener_ABC
                          , private ObjectListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             RemoteAgentResolver( RemoteAgentSubject_ABC& subject, ContextHandler_ABC< sword::UnitCreation >& unitCreation );
    virtual ~RemoteAgentResolver();
    //@}

    //! @name Operations
    //@{
    virtual std::string Resolve( unsigned int identifier ) const;
    virtual unsigned int Resolve( const std::string& identifier ) const;
    //@}

private:
    //! @name Operations
    //@{
    virtual void RemoteCreated( const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object );
    virtual void RemoteDestroyed( const std::string& identifier );
    virtual void LocalCreated( const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object );
    virtual void LocalDestroyed( const std::string& identifier );
    virtual void Moved( const std::string& identifier, double latitude, double longitude );
    virtual void SideChanged( const std::string& identifier, rpr::ForceIdentifier side );
    virtual void NameChanged( const std::string& identifier, const std::string& name );
    virtual void TypeChanged( const std::string& identifier, const rpr::EntityType& type );
    virtual void EquipmentUpdated( const std::string& identifier, const rpr::EntityType& equipmentType, unsigned int number );
    virtual void UniqueIdChanged( const std::string& identifier, const std::string& uniqueId );
    virtual void CallsignChanged( const std::string& identifier, const std::string& callsign );
    virtual void EmbeddedUnitListChanged( const std::string& identifier, const std::vector< std::string >& units );
    //@}

    //! @name Operations
    //@{
    virtual void Notify( const sword::UnitCreation& message, const std::string& identifier );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< std::string > T_Pendings;
    typedef boost::bimap< unsigned int, std::string > T_Identifiers;
    //@}

private:
    //! @name Member data
    //@{
    RemoteAgentSubject_ABC& subject_;
    ContextHandler_ABC< sword::UnitCreation >& unitCreation_;
    T_Pendings pendings_;
    T_Identifiers identifiers_;
    //@}
};

}
}

#endif // plugins_hla_RemoteAgentResolver_h
