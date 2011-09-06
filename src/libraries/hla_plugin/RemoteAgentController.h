// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_RemoteAgentController_h
#define plugins_hla_RemoteAgentController_h

#include "tools/MessageObserver.h"
#include <set>

namespace dispatcher
{
    class Model_ABC;
    class SimulationPublisher_ABC;
}

namespace sword
{
    class ControlEndTick;
    class AutomatCreation;
    class FormationCreation;
    class SimToClient_Content;
}

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  RemoteAgentController
    @brief  Remote agent controller
*/
// Created: SLI 2011-09-01
// =============================================================================
class RemoteAgentController : private tools::MessageObserver< sword::ControlEndTick >
                            , private tools::MessageObserver< sword::FormationCreation >
                            , private tools::MessageObserver< sword::AutomatCreation >
{
public:
    //! @name Constructors/Destructor
    //@{
             RemoteAgentController( tools::MessageController_ABC< sword::SimToClient_Content >& controller,
                                    dispatcher::Model_ABC& model, dispatcher::SimulationPublisher_ABC& publisher );
    virtual ~RemoteAgentController();
    //@}

private:
    //! @name Operations
    //@{
    virtual void Notify( const sword::ControlEndTick& message, int context );
    virtual void Notify( const sword::FormationCreation& message, int context );
    virtual void Notify( const sword::AutomatCreation& message, int context );
    //@}

private:
    //! @name Helpers
    //@{
    void AddFormation( unsigned long party );
    void AddAutomat( unsigned long formation, unsigned long knowledgeGroup );
    unsigned long FindKnowledgeGroup( unsigned long party ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< int > T_Contexts;
    //@}

private:
    //! @name Member data
    //@{
    tools::MessageController_ABC< sword::SimToClient_Content >& controller_;
    dispatcher::Model_ABC& model_;
    dispatcher::SimulationPublisher_ABC& publisher_;
    T_Contexts contexts_;
    //@}
};

}
}

#endif // plugins_hla_RemoteAgentController_h
