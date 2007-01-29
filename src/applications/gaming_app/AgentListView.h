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

namespace kernel
{
    class Entity_ABC;
    class Agent_ABC;
    class Automat_ABC;
    class KnowledgeGroup_ABC;
}

class Publisher_ABC;
class AutomatDecisions;

// =============================================================================
/** @class  AgentListView
    @brief  AgentListView
    // $$$$ SBO 2006-11-30: Factorize additional icon column
*/
// Created: SBO 2006-08-18
// =============================================================================
class AgentListView : public gui::HierarchyListView< kernel::CommunicationHierarchies >
                    , public kernel::ElementObserver_ABC< AutomatDecisions >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentListView( QWidget* pParent, kernel::Controllers& controllers, Publisher_ABC& publisher, gui::ItemFactory_ABC& factory, const kernel::Profile_ABC& profile, gui::SymbolIcons& icons );
    virtual ~AgentListView();
    //@}

    //! @name Operations
    //@{
    virtual void Display( const kernel::Entity_ABC& entity, gui::ValuedListItem* item );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void viewportResizeEvent( QResizeEvent* e );
    virtual void setColumnWidth( int column, int w );

    virtual void NotifyUpdated( const AutomatDecisions& decisions );
    virtual bool Drop( const kernel::Entity_ABC& item, const kernel::Entity_ABC& target );
    virtual bool Drop( const kernel::Agent_ABC& item,  const kernel::Agent_ABC& target );
    virtual bool Drop( const kernel::Agent_ABC& item,  const kernel::Automat_ABC& target );
    virtual bool Drop( const kernel::Automat_ABC& item,  const kernel::KnowledgeGroup_ABC& target );
    //@}

private:
    //! @name Member data
    //@{
    Publisher_ABC& publisher_;
    //@}
};

#endif // __AgentListView_h_
