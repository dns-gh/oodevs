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

class Agent_ABC;
class Population_ABC;
class Object_ABC;
class AgentKnowledge;
class PopulationKnowledge;
class ObjectKnowledge;
class Controllers;

// =============================================================================
/** @class  LinkInterpreter
    @brief  LinkInterpreter
*/
// Created: AGE 2006-08-11
// =============================================================================
class LinkInterpreter : public LinkInterpreter_ABC
                      , public Observer_ABC
                      , public ElementObserver_ABC< Agent_ABC >
                      , public ElementObserver_ABC< Population_ABC >
                      , public ElementObserver_ABC< Object_ABC >
                      , public ElementObserver_ABC< AgentKnowledge >
                      , public ElementObserver_ABC< PopulationKnowledge >
                      , public ElementObserver_ABC< ObjectKnowledge >
{

public:
    //! @name Constructors/Destructor
    //@{
             LinkInterpreter( QObject* parent, Controllers& controllers );
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
    virtual void NotifyCreated( const Agent_ABC& agent );
    virtual void NotifyDeleted( const Agent_ABC& agent );
    virtual void NotifyCreated( const Population_ABC& popu );
    virtual void NotifyDeleted( const Population_ABC& popu );
    virtual void NotifyCreated( const Object_ABC& object );
    virtual void NotifyDeleted( const Object_ABC& object );
    virtual void NotifyCreated( const AgentKnowledge& agent );
    virtual void NotifyDeleted( const AgentKnowledge& agent );
    virtual void NotifyCreated( const PopulationKnowledge& popu );
    virtual void NotifyDeleted( const PopulationKnowledge& popu );
    virtual void NotifyCreated( const ObjectKnowledge& object );
    virtual void NotifyDeleted( const ObjectKnowledge& object );
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    //@}

};

#endif // __LinkInterpreter_h_
