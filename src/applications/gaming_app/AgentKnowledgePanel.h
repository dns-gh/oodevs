// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentKnowledgePanel_h_
#define __AgentKnowledgePanel_h_

#include "clients_gui/InfoPanel_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "gaming/KnowledgeGroupSelectionObserver.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Controllers;
    class KnowledgeGroup_ABC;
    class Displayer_ABC;
    class Team_ABC;
}

namespace gui
{
    template< typename T > class ListDisplayer;
    class DisplayBuilder;
    class ValuedListItem;
    class ItemFactory_ABC;
}

class AgentKnowledges;
class AgentKnowledge;
class PerceptionMap;
class Perception;

// =============================================================================
/** @class  AgentKnowledgePanel
    @brief  AgentKnowledgePanel
*/
// Created: APE 2004-05-03
// =============================================================================
class AgentKnowledgePanel : public gui::InfoPanel_ABC
                          , public kernel::Observer_ABC
                          , public kernel::ElementObserver_ABC< AgentKnowledges >
                          , public kernel::ElementObserver_ABC< AgentKnowledge >
                          , public kernel::ElementObserver_ABC< PerceptionMap >
                          , public kernel::SelectionObserver_Base< AgentKnowledge >
                          , public KnowledgeGroupSelectionObserver
{
    Q_OBJECT;
public:
    //! @name Constructors/Destructor
    //@{
             AgentKnowledgePanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~AgentKnowledgePanel();
    //@}

    //! @name Operations
    //@{
    void Display( const AgentKnowledge& k, kernel::Displayer_ABC& displayer, gui::ValuedListItem* );
    void Display( const Perception& perception, kernel::Displayer_ABC& displayer, gui::ValuedListItem* );
    //@}

private:
    //! @name Copy / Assignment
    //@{
    AgentKnowledgePanel( const AgentKnowledgePanel& );
    AgentKnowledgePanel& operator=( const AgentKnowledgePanel& );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const AgentKnowledges& knowledges );
    virtual void NotifyUpdated( const AgentKnowledge& knowledge );
    virtual void NotifyUpdated( const PerceptionMap& perceptions );
    virtual void Select( const AgentKnowledge& k );
    virtual void BeforeSelection();
    virtual void AfterSelection();
    void Select( const kernel::KnowledgeGroup_ABC* group );
    void Display( const AgentKnowledge& k );
    void showEvent( QShowEvent* );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChanged( QListViewItem* );
    void OnContextMenuRequested( QListViewItem*, const QPoint& );
    void ToggleDisplayOwnTeam();
    void OnRequestCenter( QListViewItem* );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;

    gui::ListDisplayer< AgentKnowledgePanel >* pKnowledgeListView_;
    gui::DisplayBuilder* display_;
    gui::ListDisplayer< AgentKnowledgePanel >* pPerceptionListView_;

    kernel::SafePointer< kernel::Team_ABC >        owner_;
    kernel::SafePointer< AgentKnowledges > selected_;
    kernel::SafePointer< AgentKnowledge >  subSelected_;
    kernel::SafePointer< AgentKnowledge >  selectionCandidate_;
    QCheckBox* pOwnTeamCheckBox_;
    //@}
};

#endif // __AgentKnowledgePanel_h_
