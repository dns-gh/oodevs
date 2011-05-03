// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Facade_h_
#define __Facade_h_

#include <boost/noncopyable.hpp>
#include <vlpi/simulationaddress.h>
#include <list>

class DtExerciseConn;
class DtEntityIdentifier;
class DtList;
class DtReflectedAggregate;
class DtReflectedAggregateList;
class DtReflectedEntity;
class DtReflectedEntityList;
class DtReflectedObject;
class DtString;
class DtVrfRemoteController;

namespace xml
{
    class xistream;
}

namespace plugins
{
namespace vrforces
{
    class Agent;
    class ReflectedCreationListener_ABC;

// =============================================================================
/** @class  Facade
    @brief  Facade
*/
// Created: SBO 2011-01-20
// =============================================================================
class Facade : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             Facade( DtExerciseConn& connection, xml::xistream& xis );
    virtual ~Facade();
    //@}

    //! @name Operations
    //@{
    void CreatePseudoAggregate( Agent& agent );
    void DestroyPseudoAggregate( const DtEntityIdentifier& reflected );
    void Lookup( const DtEntityIdentifier& id, ReflectedCreationListener_ABC& listener );
    void AddListener( ReflectedCreationListener_ABC& listener );
    void RemoveListener( ReflectedCreationListener_ABC& listener );
    void CreateDisaggregationArea( const DtString& name, const DtList& vertices ) const;
    //@}

private:
    //! @name Helpers
    //@{*
    void StartScenario( const DtSimulationAddress& address );
    bool IsConnected() const;

    static void OnBackendDiscovery( const DtSimulationAddress& address, void* userData );
    static void OnBackendRemoval( const DtSimulationAddress& address, void* userData );
    static void OnAggregateAddition( DtReflectedAggregate* aggregate, void* userData );
    static void OnAggregateRemoval( DtReflectedAggregate* aggregate, void* userData );
    static void OnEntityAddition( DtReflectedEntity* entity, void* userData );
    static void OnEntityRemoval( DtReflectedEntity* entity, void* userData );
    static bool CheckDiscoveryCondition( DtReflectedObject* reflected, void* userData );
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, DtReflectedAggregate* > T_AggregateResolver;
    typedef std::map< std::string, DtReflectedEntity* > T_EntityResolver;
    typedef std::list< ReflectedCreationListener_ABC* > T_ReflectedCreationListeners;
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< DtVrfRemoteController > controller_;
    std::auto_ptr< DtReflectedAggregateList > reflectedAggregates_;
    std::auto_ptr< DtReflectedEntityList > reflectedEntities_;
    const std::string models_;
    const std::string scenario_;
    DtSimulationAddress address_;
    T_ReflectedCreationListeners listeners_;
    T_AggregateResolver addedAggregates_;
    T_EntityResolver addedEntities_;
    //@}
};

}
}

#endif // __Facade_h_
