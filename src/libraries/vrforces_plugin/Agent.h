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

#include <vlpi/entityidentifier.h>

class DtAggregatePublisher;
class DtExerciseConn;
class DtReflectedAggregate;
class DtSimulationAddress;
class DtString;
class DtVrfRemoteController;

namespace sword
{
    class UnitAttributes;
    class UnitCreation;
}

namespace plugins
{
namespace vrforces
{
    class Facade;

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
             Agent( DtExerciseConn& connection, Facade& vrForces, const sword::UnitCreation& message );
    virtual ~Agent();
    //@}

    //! @name Accessors
    //@{
    unsigned long GetId() const;
    //@}

    //! @name Operations
    //@{
    void Update( const sword::UnitAttributes& message );
    void SetAggregated( bool aggregated );
    void CreatePseudoAggregate( DtVrfRemoteController& controller, const DtSimulationAddress& address );
    //@}

    //! @name Callbacks
    //@{
    bool OnCreateReflected( DtReflectedAggregate* obj );
    static void OnCreatePseudoAggregate( const DtString& name, const DtEntityIdentifier& id, void* usr );
    static void OnDestroyPseudoAggregate( const DtString& name, const DtEntityIdentifier& id, void* usr );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Agent( const Agent& );            //!< Copy constructor
    Agent& operator=( const Agent& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    bool IsTrueAggregate() const;
    //@}

private:
    //! @name Member data
    //@{
    DtExerciseConn& connection_;
    Facade& vrForces_;
    const unsigned long id_;
    unsigned short heading_;
    std::auto_ptr< DtAggregatePublisher > publisher_;
    DtEntityIdentifier reflectedId_;
    const DtReflectedAggregate* reflected_;
    //@}
};

}
}

#endif // __Agent_h_
