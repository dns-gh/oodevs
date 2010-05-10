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
    class Team_ABC; // LTO
    class Controllers; // LTO
    class Time_ABC;
}

namespace actions
{
    class ActionsModel;
}

class AutomatDecisions;
class StaticModel;

// =============================================================================
/** @class  AgentListView
    @brief  AgentListView
    // $$$$ SBO 2006-11-30: Factorize additional icon column
*/
// Created: SBO 2006-08-18
// =============================================================================
class AgentListView : public gui::HierarchyListView< kernel::CommunicationHierarchies >
                    , public tools::ElementObserver_ABC< AutomatDecisions >
                    , public tools::ElementObserver_ABC< kernel::KnowledgeGroup_ABC > // LTO
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentListView( QWidget* pParent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const StaticModel& staticModel, const kernel::Time_ABC& simulation, gui::ItemFactory_ABC& factory, const kernel::Profile_ABC& profile, gui::EntitySymbols& icons );
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
    virtual void NotifyUpdated( const kernel::KnowledgeGroup_ABC& knowledgeGroup ); // LTO
    virtual bool Drop( const kernel::Entity_ABC& item, const kernel::Entity_ABC& target );
    virtual bool Drop( const kernel::Agent_ABC& item,  const kernel::Agent_ABC& target );
    virtual bool Drop( const kernel::Agent_ABC& item,  const kernel::Automat_ABC& target );
    virtual bool Drop( const kernel::Automat_ABC& item,  const kernel::KnowledgeGroup_ABC& target );
    virtual bool Drop( const kernel::KnowledgeGroup_ABC& item,  const kernel::KnowledgeGroup_ABC& target ); // LTO
    virtual bool Drop( const kernel::KnowledgeGroup_ABC& item,  const kernel::Team_ABC& target ); // LTO
    //@}

private:
    //! @name Member data
    //@{
    actions::ActionsModel& actionsModel_;
    const StaticModel& static_;
    const kernel::Time_ABC& simulation_;
    QPixmap lock_, commandPost_, scisors_;
    kernel::Controllers& controllers_; // LTO
    //@}
};

#endif // __AgentListView_h_
