// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __SimpleEntityCondition_h_
#define __SimpleEntityCondition_h_

#include "EventCondition.h"
#include "clients_kernel/ElementObserver_ABC.h"

namespace dispatcher
{
    class Agent;
    class AgentKnowledge;
    class Automat;
}

namespace script
{
// =============================================================================
/** @class  SimpleEntityCondition
    @brief  Simple entity condition
*/
// Created: AGE 2008-06-17
// =============================================================================
class EntityConditionBase
{
public:
    static void Trigger( BaseCondition& that, const dispatcher::Agent& agent );
    static void Trigger( BaseCondition& that, const dispatcher::AgentKnowledge& knowledge );
    static void Trigger( BaseCondition& that, const dispatcher::Automat& knowledge );
};

// =============================================================================
/** @class  SimpleEntityCondition
    @brief  Simple agent condition
*/
// Created: AGE 2008-06-17
// =============================================================================
template< typename E >
class SimpleEntityCondition : public EventCondition
                            , public EntityConditionBase
                            , public kernel::ElementObserver_ABC< E >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit SimpleEntityCondition( kernel::Controller& controller )
        : EventCondition( controller ) {}
    virtual ~SimpleEntityCondition() { Activate( false ); };
    //@}

public:
    //! @name Helpers
    //@{
    void Trigger( const dispatcher::Agent& entity )
    {
        EntityConditionBase::Trigger( *this, entity );
    }
    void Trigger( const dispatcher::AgentKnowledge& entity )
    {
        EntityConditionBase::Trigger( *this, entity );
    }
    void Trigger( const dispatcher::Automat& entity )
    {
        EntityConditionBase::Trigger( *this, entity );
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SimpleEntityCondition( const SimpleEntityCondition& );            //!< Copy constructor
    SimpleEntityCondition& operator=( const SimpleEntityCondition& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const E& event )
    {
        Trigger( event.entity );
    }
    //@}
};

}

#endif // __SimpleEntityCondition_h_
