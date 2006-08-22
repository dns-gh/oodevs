// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentListViewImp_h_
#define __AgentListViewImp_h_

#include "clients_gui/AgentListView.h"

class Publisher_ABC;
class AutomatDecisions;

// =============================================================================
/** @class  AgentListViewImp
    @brief  AgentListViewImp
*/
// Created: SBO 2006-08-18
// =============================================================================
class AgentListViewImp : public gui::AgentListView
                       , public kernel::ElementObserver_ABC< AutomatDecisions >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentListViewImp( QWidget* pParent, kernel::Controllers& controllers, Publisher_ABC& publisher, gui::ItemFactory_ABC& factory );
    virtual ~AgentListViewImp();
    //@}

    //! @name Operations
    //@{
    virtual void Display( const kernel::Agent_ABC& agent, gui::ValuedListItem* item );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const AutomatDecisions& decisions );
    virtual bool Drop( const kernel::Agent_ABC& item, const kernel::Agent_ABC& target );
    virtual bool Drop( const kernel::Agent_ABC& item, const kernel::KnowledgeGroup_ABC& target );
    //@}

private:
    //! @name Member data
    //@{
    Publisher_ABC& publisher_;
    //@}
};

#endif // __AgentListViewImp_h_
