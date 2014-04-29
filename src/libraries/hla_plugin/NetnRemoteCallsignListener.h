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

#include "ClassListener_ABC.h"
#include "ObjectListener_ABC.h"
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
class NetnRemoteCallsignListener : private ResponseObserver_ABC< sword::UnitCreation >
                                 , private ClassListener_ABC
                                 , private ObjectListener_ABC
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
        std::vector< char > uniqueId;
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
