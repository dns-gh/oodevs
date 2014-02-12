// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_RemoteOrbatShaper_h
#define plugins_hla_RemoteOrbatShaper_h

#include "ClassListener_ABC.h"
#include "ObjectListener_ABC.h"
#include "ResponseObserver_ABC.h"
#include <boost/shared_ptr.hpp>
#include <map>

namespace tools
{
    template< typename T, typename Identifier > class Resolver_ABC;
}

namespace dispatcher
{
    class KnowledgeGroup_ABC;
    class SimulationPublisher_ABC;
}

namespace sword
{
    class FormationCreation;
    class AutomatCreation;
    class UnitCreation;
}

namespace plugins
{
namespace hla
{
    class RemoteAgentSubject_ABC;
    class SideResolver_ABC;
    class UnitTypeResolver_ABC;
    template< typename ResponseMessage > class ContextHandler_ABC;

// =============================================================================
/** @class  RemoteOrbatShaper
    @brief  RemoteOrbatShaper
*/
// Created: AHC 2012-10-03
// =============================================================================
class RemoteOrbatShaper : private ClassListener_ABC
                        , private ObjectListener_ABC
                        , private ResponseObserver_ABC< sword::FormationCreation >
                        , private ResponseObserver_ABC< sword::AutomatCreation >
                        , private ResponseObserver_ABC< sword::UnitCreation >
{
public:
    //! @name Constructors/Destructor
    //@{
             RemoteOrbatShaper( RemoteAgentSubject_ABC& agentSubject, ContextHandler_ABC< sword::FormationCreation >& formationCreation,
                                ContextHandler_ABC< sword::AutomatCreation >& automatCreation,
                                ContextHandler_ABC< sword::UnitCreation >& unitCreation, const SideResolver_ABC& sideResolver,
                                const tools::Resolver_ABC< dispatcher::KnowledgeGroup_ABC, unsigned long >& knowledgeGroups,
                                dispatcher::SimulationPublisher_ABC& publisher, const UnitTypeResolver_ABC& automatTypeResolver );
    virtual ~RemoteOrbatShaper();
    //@}

private:
    //! @name Operations
    //@{
    virtual void RemoteCreated(  const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object );
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
    virtual void EmbeddedUnitListChanged( const std::string& identifier, const std::vector< T_UniqueId >& embeddedUnits );
    virtual void GeometryChanged( const std::string& identifier, const std::vector< rpr::WorldLocation >& perimeter, ObjectListener_ABC::GeometryType type );
    virtual void ParentChanged( const std::string& rtiIdentifier, const std::string& parentRtiId );
    virtual void SubAgregatesChanged( const std::string& rtiIdentifier, const std::set< std::string >& children );
    virtual void SubEntitiesChanged( const std::string& rtiIdentifier, const std::set< std::string >& children );
    virtual void PropagationChanged( const std::string& rtiIdentifier, const std::vector< ObjectListener_ABC::PropagationData >& data,
            int col, int lig, double xll, double yll, double dx, double dy );

    void CreateParent( const std::string& childId );
    void MoveChild( const std::string& childId );
    //@}

private:
    //! @name Messages
    //@{
    virtual void Notify( const sword::FormationCreation& message, const std::string& identifier );
    virtual void Notify( const sword::AutomatCreation& message, const std::string& identifier );
    virtual void Notify( const sword::UnitCreation& message, const std::string& identifier );
    //@}

private:
    //! @name Types
    //@{
    struct UnitData;
    typedef std::map< std::string, boost::shared_ptr< UnitData > > T_UnitsData;
    typedef std::map< unsigned long, unsigned long > T_PartyMap;
    //@}

private:
    //! @name Member data
    //@{
    RemoteAgentSubject_ABC& agentSubject_;
    ContextHandler_ABC< sword::FormationCreation >& formationCreation_;
    ContextHandler_ABC< sword::AutomatCreation >& automatCreation_;
    ContextHandler_ABC< sword::UnitCreation >& unitCreation_;
    const SideResolver_ABC& sideResolver_;
    dispatcher::SimulationPublisher_ABC& publisher_;
    const UnitTypeResolver_ABC& automatTypeResolver_;
    T_PartyMap party2Formations_;
    T_PartyMap party2KnowledgeGroups_;
    T_UnitsData units_;
    //@}
};

}
}

#endif // plugins_hla_RemoteOrbatShaper_h
