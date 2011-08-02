// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentList_h_
#define __AgentList_h_

namespace kernel
{
    class Controllers;
    class Profile_ABC;
    class Time_ABC;
}

namespace gui
{
    class ItemFactory_ABC;
    class EntitySymbols;
}

namespace actions
{
    class ActionsModel;
}

class AutomatDecisions;
class StaticModel;

// =============================================================================
/** @class  AgentList
    @brief  AgentList
*/
// Created: AGE 2006-08-29
// =============================================================================
class AgentList : public Q3VBox
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentList( kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const StaticModel& staticModel, const kernel::Time_ABC& simulation, gui::ItemFactory_ABC& factory, const kernel::Profile_ABC& profile, gui::EntitySymbols& icons );
    virtual ~AgentList();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentList( const AgentList& );            //!< Copy constructor
    AgentList& operator=( const AgentList& ); //!< Assignment operator
    //@}
};

#endif // __AgentList_h_
