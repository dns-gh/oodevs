// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-15 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentListView.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:42 $
// $Revision: 9 $
// $Workfile: AgentListView.h $
//
// *****************************************************************************

#ifndef __AgentListView_h_
#define __AgentListView_h_

#include "ListView.h"
#include "ElementObserver_ABC.h"
#include "SelectionObserver_ABC.h"
#include "OptionsObserver_ABC.h"
#include "ActivationObserver_ABC.h"

class Team;
class KnowledgeGroup;
class Agent;
class ActionContext;
class Controllers;
class ValuedListItem;
class AutomatDecisions;
class ItemFactory_ABC;

// =============================================================================
/** @class  AgentListView
    @brief  Displays the agent list sorted by team, gtia and group.
*/
// Created: APE 2004-03-15
// =============================================================================
class AgentListView : public ListView< AgentListView >
                    , public Observer_ABC
                    , public ElementObserver_ABC< Team >
                    , public ElementObserver_ABC< KnowledgeGroup >
                    , public ElementObserver_ABC< Agent >
                    , public ElementObserver_ABC< AutomatDecisions >
                    , public SelectionObserver_Base< Team > // $$$$ AGE 2006-03-21: refactor these crap
                    , public SelectionObserver_Base< KnowledgeGroup >
                    , public SelectionObserver_Base< Agent >
                    , public SelectionObserver_ABC
                    , public ActivationObserver_ABC< Agent >
                    , public OptionsObserver_ABC
{
   Q_OBJECT;
public:
    //! @name Constructors/Destructor
    //@{
             AgentListView( QWidget* pParent, Controllers& controllers, ItemFactory_ABC& factory );
    virtual ~AgentListView();

    QSize sizeHint() const;
    //@}

    //! @name Operations
    //@{
    void Display( const Team& team,            ValuedListItem* item );
    void Display( const KnowledgeGroup& group, ValuedListItem* item );
    void Display( const Agent& agent,          ValuedListItem* item );
    //@}

private:
    //! @name Copy / Assignment
    //@{
    AgentListView( const AgentListView& );
    AgentListView& operator=( const AgentListView& );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChange( QListViewItem* item );
    void OnContextMenuRequested( QListViewItem*, const QPoint&, int );
    void OnRequestCenter();
    //@}

    //! @name Helpers
    //@{
private:
    virtual void NotifyCreated( const Team& team );
    virtual void NotifyUpdated( const Team& team );
    virtual void NotifyDeleted( const Team& team );

    virtual void NotifyUpdated( const KnowledgeGroup& group );
    virtual void NotifyUpdated( const Agent& agent );
    virtual void NotifyUpdated( const AutomatDecisions& decisions );

    virtual void Select( const Team& element );
    virtual void Select( const KnowledgeGroup& element );
    virtual void Select( const Agent& element );
    virtual void BeforeSelection();
    virtual void AfterSelection();
    virtual void NotifyActivated( const Agent& element );
    virtual void OptionChanged( const std::string& name, const OptionVariant& value );

    template< typename ParentType, typename ChildType >
    void RecursiveDisplay( const ParentType& value, ValuedListItem* item );

    template< typename Type >
    void Update( const Type& value );

    QDragObject* dragObject();
    void dropEvent( QDropEvent* pEvent );
    void dragEnterEvent( QDragEnterEvent* pEvent );
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    ItemFactory_ABC& factory_;
    const Team* currentTeam_;
    //@}
};

#endif // __AgentListView_h_
