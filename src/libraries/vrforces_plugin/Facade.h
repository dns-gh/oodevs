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

#include <vlpi/simulationaddress.h>
#include <list>

class DtExerciseConn;
class DtEntityIdentifier;
class DtReflectedAggregate;
class DtReflectedAggregateList;
class DtReflectedObject;
class DtVrfRemoteController;

namespace plugins
{
namespace vrforces
{
    class Agent;

// =============================================================================
/** @class  Facade
    @brief  Facade
*/
// Created: SBO 2011-01-20
// =============================================================================
class Facade
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Facade( DtExerciseConn& connection );
    virtual ~Facade();
    //@}

    //! @name Operations
    //@{
    void CreatePseudoAggregate( Agent& agent );
    void DestroyPseudoAggregate( const DtEntityIdentifier& reflected );
    const DtReflectedAggregate* Find( const DtEntityIdentifier& id ) const;
    void RegisterReflectedCreationListener( Agent& agent );
    void UnregisterReflectedCreationListener( Agent& agent );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Facade( const Facade& );            //!< Copy constructor
    Facade& operator=( const Facade& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{*
    void StartScenario( const DtSimulationAddress& address );
    bool IsConnected() const;

    static void OnBackendDiscovery( const DtSimulationAddress& addr, void* usr );
    static void OnBackendRemoval( const DtSimulationAddress& addr, void* usr );
    static void OnAggregateAddition( DtReflectedAggregate* obj, void* userData );
    static void OnAggregateRemoval( DtReflectedAggregate* obj, void* userData );
    static bool CheckAggregateDiscoveryCondition( DtReflectedObject* refObj, void* usr );
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, DtReflectedAggregate* > T_Reflected;
    typedef std::list< Agent* > T_ReflectedCreationListeners;
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< DtVrfRemoteController > controller_;
    std::auto_ptr< DtReflectedAggregateList > reflectedAggregates_;
    T_Reflected reflected_;
    DtSimulationAddress address_;
    T_ReflectedCreationListeners reflectedCreationListeners_;
    //@}
};

}
}

#endif // __Facade_h_
