// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentConditions_h_
#define __AgentConditions_h_

#include "dispatcher/Registrable_ABC.h"
#include "dispatcher/Position.h"
#include <boost/shared_ptr.hpp>

namespace kernel
{
    class Controller;
    class CoordinateConverter_ABC;
}

namespace dispatcher
{
    class Agent;
}

namespace script
{
    class Condition_ABC;

// =============================================================================
/** @class  AgentConditions
    @brief  AgentConditions
*/
// Created: AGE 2008-06-13
// =============================================================================
class AgentConditions : public dispatcher::Registrable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentConditions( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter );
    virtual ~AgentConditions();
    //@}

    //! @name Operations
    //@{
    virtual void RegisterIn( directia::Brain& brain );
    //@}

private:
    //! @name Copy / Assignment
    //@{
    AgentConditions( const AgentConditions& );
    AgentConditions& operator=( const AgentConditions& );
    //@}

    //! @name Helpers
    //@{
    boost::shared_ptr< Condition_ABC > AgentEnters( const dispatcher::Zone& zone );
    boost::shared_ptr< Condition_ABC > KnowledgeEnters( const dispatcher::Zone& zone );
    boost::shared_ptr< Condition_ABC > OperationalStateChanged();
    boost::shared_ptr< Condition_ABC > MissionStarted();
    boost::shared_ptr< Condition_ABC > KnowledgeCreated();
    boost::shared_ptr< Condition_ABC > KnowledgeCreatedIn( const dispatcher::Zone& zone );
    boost::shared_ptr< Condition_ABC > PerceptionChanged();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::CoordinateConverter_ABC& converter_;
    //@}
};

}

#endif // __AgentConditions_h_
