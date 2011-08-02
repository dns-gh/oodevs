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
#include "tools/ElementObserver_ABC.h"
#include "gaming/KnowledgeGroupSelectionObserver.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Controllers;
    class KnowledgeGroup_ABC;
    class Displayer_ABC;
    class Entity_ABC;
    class AgentKnowledge_ABC;
}

namespace gui
{
    template< typename T > class ListDisplayer;
    class DisplayBuilder;
    class ValuedListItem;
    class ItemFactory_ABC;
}

class AgentKnowledges;
class PerceptionMap;
class Perception;

// =============================================================================
/** @class  AgentKnowledgePanel
    @brief  AgentKnowledgePanel
*/
// Created: APE 2004-05-03
// =============================================================================
class AgentKnowledgePanel : public gui::InfoPanel_ABC
                          , public tools::Observer_ABC
                          , public tools::ElementObserver_ABC< AgentKnowledges >
                          , public tools::ElementObserver_ABC< kernel::AgentKnowledge_ABC >
                          , public tools::ElementObserver_ABC< PerceptionMap >
                          , public tools::SelectionObserver_Base< kernel::AgentKnowledge_ABC >
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
    void Display( const kernel::AgentKnowledge_ABC& k, kernel::Displayer_ABC& displayer, gui::ValuedListItem* );
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
    virtual void NotifyUpdated( const kernel::AgentKnowledge_ABC& knowledge );
    virtual void NotifyUpdated( const PerceptionMap& perceptions );
    virtual void Select( const kernel::AgentKnowledge_ABC& k );
    virtual void BeforeSelection();
    virtual void AfterSelection();
    void Select( const kernel::KnowledgeGroup_ABC* group );
    void Display( const kernel::AgentKnowledge_ABC& k );
    void showEvent( QShowEvent* );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChanged( Q3ListViewItem* );
    void OnContextMenuRequested( Q3ListViewItem*, const QPoint& );
    void OnRequestCenter( Q3ListViewItem* );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;

    gui::ListDisplayer< AgentKnowledgePanel >* pKnowledgeListView_;
    gui::DisplayBuilder* display_;
    gui::ListDisplayer< AgentKnowledgePanel >* pPerceptionListView_;

    kernel::SafePointer< AgentKnowledges >     selected_;
    kernel::SafePointer< kernel::AgentKnowledge_ABC >  subSelected_;
    kernel::SafePointer< kernel::AgentKnowledge_ABC >  selectionCandidate_;
    //@}
};

#endif // __AgentKnowledgePanel_h_
