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

#include "astec_gui/InfoPanel_ABC.h"
#include "astec_kernel/ElementObserver_ABC.h"
#include "astec_gaming/KnowledgeGroupSelectionObserver.h"
#include "astec_kernel/SafePointer.h"

template< typename T > class ListDisplayer;
class Controllers;
class AgentKnowledges;
class AgentKnowledge;
class DisplayBuilder;
class KnowledgeGroup_ABC;
class PerceptionMap;
class Perception;
class Displayer_ABC;
class ValuedListItem;
class Team_ABC;
class ItemFactory_ABC;

// =============================================================================
/** @class  AgentKnowledgePanel
    @brief  AgentKnowledgePanel
*/
// Created: APE 2004-05-03
// =============================================================================
class AgentKnowledgePanel : public InfoPanel_ABC
                          , public Observer_ABC
                          , public ElementObserver_ABC< AgentKnowledges >
                          , public ElementObserver_ABC< AgentKnowledge >
                          , public ElementObserver_ABC< PerceptionMap >
                          , public KnowledgeGroupSelectionObserver
{
    Q_OBJECT;
public:
    //! @name Constructors/Destructor
    //@{
             AgentKnowledgePanel( QWidget* parent, PanelStack_ABC& panel, Controllers& controllers, ItemFactory_ABC& factory );
    virtual ~AgentKnowledgePanel();
    //@}

    //! @name Operations
    //@{
    void Display( const AgentKnowledge& k, Displayer_ABC& displayer, ValuedListItem* );
    void Display( const Perception& perception, Displayer_ABC& displayer, ValuedListItem* );
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
    void Select( const KnowledgeGroup_ABC* group );
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
    Controllers& controllers_;

    ListDisplayer< AgentKnowledgePanel >* pKnowledgeListView_;
    DisplayBuilder* display_;
    ListDisplayer< AgentKnowledgePanel >* pPerceptionListView_;

    SafePointer< Team_ABC >        owner_;
    SafePointer< AgentKnowledges > selected_;
    SafePointer< AgentKnowledge >  subSelected_;
    QCheckBox* pOwnTeamCheckBox_;
    //@}
};

#endif // __AgentKnowledgePanel_h_
