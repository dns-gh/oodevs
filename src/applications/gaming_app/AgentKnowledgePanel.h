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
#include "gaming/KnowledgeGroupSelectionObserver.h"
#include "clients_kernel/SafePointer.h"
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class AgentKnowledge_ABC;
    class Automat_ABC;
    class Controllers;
    class Displayer_ABC;
    class KnowledgeGroup_ABC;
    class ModelUnLoaded;
}

namespace gui
{
    class DisplayBuilder;
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
                          , public tools::ElementObserver_ABC< kernel::Automat_ABC >
                          , public tools::ElementObserver_ABC< PerceptionMap >
                          , public tools::ElementObserver_ABC< kernel::ModelUnLoaded >
                          , public tools::SelectionObserver_Base< kernel::AgentKnowledge_ABC >
                          , public KnowledgeGroupSelectionObserver
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             AgentKnowledgePanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~AgentKnowledgePanel();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const AgentKnowledges& knowledges );
    virtual void NotifyUpdated( const kernel::AgentKnowledge_ABC& knowledge );
    virtual void NotifyUpdated( const kernel::Automat_ABC& automat );
    virtual void NotifyUpdated( const PerceptionMap& perceptions );
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& );
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
    void OnSelectionChanged();
    void OnKnowledgeContextMenuEvent( const QPoint & pos );
    void OnPerceptionContextMenuEvent( const QPoint & pos );
    void OnKnowledgeRequestCenter();
    void OnPerceptionRequestCenter();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;

    QTreeView* pKnowledgeListView_;
    QStandardItemModel knowledgeModel_;
    gui::DisplayBuilder* display_;
    QTreeView* pPerceptionListView_;
    QStandardItemModel perceptionModel_;

    kernel::SafePointer< AgentKnowledges >     selected_;
    kernel::SafePointer< kernel::AgentKnowledge_ABC >  subSelected_;
    kernel::SafePointer< kernel::AgentKnowledge_ABC >  selectionCandidate_;
    //@}
};

#endif // __AgentKnowledgePanel_h_
