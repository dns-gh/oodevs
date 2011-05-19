// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Agent_h_
#define __Agent_h_

#include "ReflectedCreationListener_ABC.h"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <vlpi/entityidentifier.h>

class DtAggregatePublisher;
class DtExerciseConn;
class DtReflectedAggregate;
class DtSimulationAddress;
class DtString;
class DtVrfRemoteController;

namespace dispatcher
{
    class Logger_ABC;
    class SimulationPublisher_ABC;
}

namespace kernel
{
    class Agent_ABC;
    class AgentType;
    class ComponentType;
}

namespace rpr
{
    class EntityTypeResolver_ABC;
}

namespace sword
{
    class UnitAttributes;
    class UnitCreation;
    class UnitPathFind;
}

namespace plugins
{
namespace vrforces
{
    class AggregatedPosition_ABC;
    class AggregatedState_ABC;
    class DisaggregationStrategy_ABC;
    class Facade;
    class ForceResolver_ABC;
    class Subordinate;

// =============================================================================
/** @class  Agent
    @brief  Agent
*/
// Created: SBO 2011-01-21
// =============================================================================
class Agent : private boost::noncopyable
            , public ReflectedCreationListener_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Agent( const kernel::Agent_ABC& agent, DtExerciseConn& connection, Facade& vrForces, const sword::UnitCreation& message
                  , const ForceResolver_ABC& forces, const DisaggregationStrategy_ABC& disaggregation
                  , const rpr::EntityTypeResolver_ABC& entityTypes, dispatcher::SimulationPublisher_ABC& simulation, dispatcher::Logger_ABC& logger );
    virtual ~Agent();
    //@}

    //! @name Accessors
    //@{
    unsigned long GetId() const;
    //@}

    //! @name Operations
    //@{
    void Update( const sword::UnitAttributes& message );
    void Update( const sword::UnitPathFind& message );
    void CreatePseudoAggregate( DtVrfRemoteController& controller, const DtSimulationAddress& address );
    void MoveTo( const geometry::Point2d& position ) const;
    void NotifyStateChanged() const;
    //@}

    //! @name Callbacks
    //@{
    virtual bool NotifyCreated( DtReflectedAggregate& aggregate );
    virtual void NotifyUpdated( const Subordinate& subordinate );
    //@}

private:
    //! @name Callbacks
    //@{
    static void OnCreatePseudoAggregate( const DtString& name, const DtEntityIdentifier& id, void* usr );
    //@}

    //! @name Helpers
    //@{
    void CreateSubordinates( const kernel::AgentType& type );
    void AddSubordinates( unsigned int index, unsigned int count, const kernel::ComponentType& type );
    void AddSubordinateEntity( DtVrfRemoteController& controller, const DtSimulationAddress& address, const DtEntityType& type, const std::string& identifier );
    void UpdateLocation( const sword::UnitAttributes& message );
    void SetAggregated( bool aggregated );
    bool IsTrueAggregate() const;
    void DestroyPseudoAggregate();
    void CancelMission();
    //@}

    //! @name Types
    //@{
    typedef std::vector< boost::shared_ptr< Subordinate > > T_Subordinates;
    //@}

private:
    //! @name Member data
    //@{
    const DisaggregationStrategy_ABC& disaggregation_;
    dispatcher::SimulationPublisher_ABC& swordPublisher_;
    DtExerciseConn& connection_;
    dispatcher::Logger_ABC& logger_;
    Facade& vrForces_;
    const unsigned long id_;
    unsigned short heading_;
    std::auto_ptr< DtAggregatePublisher > aggregatePublisher_;
    DtEntityIdentifier reflectedId_;
    const DtReflectedAggregate* reflected_;
    T_Subordinates subordinates_;
    const kernel::AgentType& type_;
    const rpr::EntityTypeResolver_ABC& entityTypes_;
    std::auto_ptr< AggregatedPosition_ABC > position_;
    std::auto_ptr< AggregatedState_ABC > state_;
    DtVector destination_;
    //@}
};

}
}

#endif // __Agent_h_
