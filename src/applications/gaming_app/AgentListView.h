// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentListView_h_
#define __AgentListView_h_

#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_gui/HierarchyListView.h"

class Publisher_ABC;
class AutomatDecisions;

// =============================================================================
/** @class  AgentListView
    @brief  AgentListView
*/
// Created: SBO 2006-08-18
// =============================================================================
class AgentListView : public gui::HierarchyListView< kernel::CommunicationHierarchies >
                    , public kernel::ElementObserver_ABC< AutomatDecisions >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentListView( QWidget* pParent, kernel::Controllers& controllers, Publisher_ABC& publisher, gui::ItemFactory_ABC& factory );
    virtual ~AgentListView();
    //@}

    //! @name Operations
    //@{
    virtual void Display( const kernel::Entity_ABC& entity, gui::ValuedListItem* item );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const AutomatDecisions& decisions );
    virtual bool Drop( const kernel::Entity_ABC& item, const kernel::Entity_ABC& target );
    virtual bool Drop( const kernel::Agent_ABC& item,  const kernel::Agent_ABC& target );
    virtual bool Drop( const kernel::Agent_ABC& item,  const kernel::KnowledgeGroup_ABC& target );
    //@}

private:
    //! @name Member data
    //@{
    Publisher_ABC& publisher_;
    //@}
};

#endif // __AgentListView_h_
