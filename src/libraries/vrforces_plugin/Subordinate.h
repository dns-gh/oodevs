// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Subordinate_h_
#define __Subordinate_h_

#include "ReflectedCreationListener_ABC.h"
#include <boost/noncopyable.hpp>
#include <vlpi/entityidentifier.h>
#include <vlutil/vlmachinetypes.h>

class DtAggregatePublisher;
class DtEntityIdentifier;
class DtEntityType;
class DtReflectedEntity;
class DtSimulationAddress;
class DtString;
class DtVector;
class DtVrfRemoteController;

namespace dispatcher
{
    class Logger_ABC;
}

namespace plugins
{
namespace vrforces
{
    class Agent;
    class AggregatedState_ABC;
    class AggregatedPosition_ABC;
    class Facade;

// =============================================================================
/** @class  Subordinate
    @brief  Subordinate
*/
// Created: SBO 2011-03-23
// =============================================================================
class Subordinate : private boost::noncopyable
                  , public ReflectedCreationListener_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Subordinate( const DtEntityType& type, DtAggregatePublisher& publisher, DtReal heading, const std::string& identifier
                        , DtVrfRemoteController& controller, const DtSimulationAddress& address, Facade& vrForces, Agent& superior, dispatcher::Logger_ABC& logger );
    virtual ~Subordinate();
    //@}

    //! @name Callbacks
    //@{
    void SetSuperior( const DtEntityIdentifier& identifier );
    void SetDestination( const DtVector& location );
    virtual bool NotifyCreated( DtReflectedEntity& entity );
    void Update( AggregatedPosition_ABC& position ) const;
    void Update( AggregatedState_ABC& state ) const;
    bool IsUndamaged() const;

    void OnCreate( const DtString& name, const DtEntityIdentifier& id );
    void OnUpdate( DtReflectedEntity* obj );
    //@}

private:
    //! @name Member data
    //@{
    const std::string identifier_;
    DtEntityIdentifier entityId_;
    DtVrfRemoteController& controller_;
    const DtSimulationAddress& address_;
    Facade& vrForces_;
    Agent& superior_;
    dispatcher::Logger_ABC& logger_;
    DtReflectedEntity* reflected_;
    DtEntityIdentifier superiorId_;
    DtVector destination_;
    DtDamageState state_;
    //@}
};

}
}

#endif // __Subordinate_h_
