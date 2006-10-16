// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LinkInterpreter_h_
#define __LinkInterpreter_h_

#include "clients_gui/LinkInterpreter_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Agent_ABC;
    class Population_ABC;
    class Object_ABC;
    class Knowledge_ABC;
}

class AgentKnowledge_ABC;
class ObjectKnowledge_ABC;
class PopulationKnowledge_ABC;

// =============================================================================
/** @class  LinkInterpreter
    @brief  LinkInterpreter
*/
// Created: AGE 2006-08-11
// =============================================================================
class LinkInterpreter : public gui::LinkInterpreter_ABC
                      , public kernel::Observer_ABC
                      , public kernel::ElementObserver_ABC< kernel::Agent_ABC >
                      , public kernel::ElementObserver_ABC< kernel::Population_ABC >
                      , public kernel::ElementObserver_ABC< kernel::Object_ABC >
                      , public kernel::ElementObserver_ABC< AgentKnowledge_ABC >
                      , public kernel::ElementObserver_ABC< ObjectKnowledge_ABC>
                      , public kernel::ElementObserver_ABC< PopulationKnowledge_ABC>
{

public:
    //! @name Constructors/Destructor
    //@{
             LinkInterpreter( QObject* parent, kernel::Controllers& controllers );
    virtual ~LinkInterpreter();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LinkInterpreter( const LinkInterpreter& );            //!< Copy constructor
    LinkInterpreter& operator=( const LinkInterpreter& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::Agent_ABC& entity );
    virtual void NotifyDeleted( const kernel::Agent_ABC& entity );
    virtual void NotifyCreated( const kernel::Population_ABC& entity );
    virtual void NotifyDeleted( const kernel::Population_ABC& entity );
    virtual void NotifyCreated( const kernel::Object_ABC& entity );
    virtual void NotifyDeleted( const kernel::Object_ABC& entity );
    virtual void NotifyCreated( const AgentKnowledge_ABC& entity );
    virtual void NotifyDeleted( const AgentKnowledge_ABC& entity );
    virtual void NotifyCreated( const ObjectKnowledge_ABC& entity );
    virtual void NotifyDeleted( const ObjectKnowledge_ABC& entity );
    virtual void NotifyCreated( const PopulationKnowledge_ABC& entity );
    virtual void NotifyDeleted( const PopulationKnowledge_ABC& entity );

    void NotifyCreated( const kernel::Entity_ABC& entity );
    void NotifyDeleted( const kernel::Entity_ABC& entity );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    //@}
};

#endif // __LinkInterpreter_h_
