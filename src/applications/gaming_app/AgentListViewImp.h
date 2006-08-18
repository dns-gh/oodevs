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


// =============================================================================
/** @class  AgentListViewImp
    @brief  AgentListViewImp
*/
// Created: SBO 2006-08-18
// =============================================================================
class AgentListViewImp : public AgentListView
                       , public ElementObserver_ABC< AutomatDecisions >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentListViewImp( QWidget* pParent, Controllers& controllers, Publisher_ABC& publisher, ItemFactory_ABC& factory );
    virtual ~AgentListViewImp();
    //@}

    //! @name Operations
    //@{
    virtual void Display( const Agent_ABC& agent, ValuedListItem* item );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const AutomatDecisions& decisions );
    //@}
};

#endif // __AgentListViewImp_h_
