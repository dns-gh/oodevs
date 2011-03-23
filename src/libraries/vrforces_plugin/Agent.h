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

#include <boost/shared_ptr.hpp>
#include <vlpi/entityidentifier.h>

class DtAggregatePublisher;
class DtExerciseConn;
class DtReflectedAggregate;
class DtSimulationAddress;
class DtString;
class DtVrfRemoteController;

namespace kernel
{
    class Agent_ABC;
    class AgentType;
    class ComponentType;
}

namespace sword
{
    class UnitAttributes;
    class UnitCreation;
}

namespace plugins
{
namespace vrforces
{
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
class Agent
{

public:
    //! @name Constructors/Destructor
    //@{
             Agent( const kernel::Agent_ABC& agent, DtExerciseConn& connection, Facade& vrForces, const sword::UnitCreation& message
                  , const ForceResolver_ABC& forces, const DisaggregationStrategy_ABC& disaggregation );
    virtual ~Agent();
    //@}

    //! @name Accessors
    //@{
    unsigned long GetId() const;
    //@}

    //! @name Operations
    //@{
    void Update( const sword::UnitAttributes& message );
    void CreatePseudoAggregate( DtVrfRemoteController& controller, const DtSimulationAddress& address );
    //@}

    //! @name Callbacks
    //@{
    bool OnCreateReflected( DtReflectedAggregate* obj );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Agent( const Agent& );            //!< Copy constructor
    Agent& operator=( const Agent& ); //!< Assignment operator
    //@}

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
    //@}

    //! @name Types
    //@{
    typedef std::vector< boost::shared_ptr< Subordinate > > T_Subordinates;
    //@}

private:
    //! @name Member data
    //@{
    const DisaggregationStrategy_ABC& disaggregation_;
    DtExerciseConn& connection_;
    Facade& vrForces_;
    const unsigned long id_;
    unsigned short heading_;
    std::auto_ptr< DtAggregatePublisher > publisher_;
    DtEntityIdentifier reflectedId_;
    const DtReflectedAggregate* reflected_;
    T_Subordinates subordinates_;
    const kernel::AgentType& type_;
    //@}
};

}
}

#endif // __Agent_h_
