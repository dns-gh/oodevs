// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentListView_h_
#define __AgentListView_h_

#include "ListView.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/SelectionObserver_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include "clients_kernel/ActivationObserver_ABC.h"

class Team_ABC;
class KnowledgeGroup_ABC;
class Agent_ABC;
class ActionContext;
class Controllers;
class ValuedListItem;
class AutomatDecisions;
class ItemFactory_ABC;
class Publisher_ABC;

// =============================================================================
/** @class  AgentListView
    @brief  Displays the agent list sorted by team, gtia and group.
*/
// Created: APE 2004-03-15
// =============================================================================
class AgentListView : public ListView< AgentListView >
                    , public Observer_ABC
                    , public ElementObserver_ABC< Team_ABC >
                    , public ElementObserver_ABC< KnowledgeGroup_ABC >
                    , public ElementObserver_ABC< Agent_ABC >
                    , public ElementObserver_ABC< AutomatDecisions >
                    , public SelectionObserver_Base< Team_ABC > // $$$$ AGE 2006-03-21: refactor these crap
                    , public SelectionObserver_Base< KnowledgeGroup_ABC >
                    , public SelectionObserver_Base< Agent_ABC >
                    , public SelectionObserver_ABC
                    , public ActivationObserver_ABC< Agent_ABC >
                    , public OptionsObserver_ABC
{
   Q_OBJECT;

public:
    //! @name Constants
    //@{
    static const char* agentMimeType_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             AgentListView( QWidget* pParent, Controllers& controllers, Publisher_ABC& publisher, ItemFactory_ABC& factory );
    virtual ~AgentListView();

    QSize sizeHint() const;
    //@}

    //! @name Operations
    //@{
    void Display( const Team_ABC& team,            ValuedListItem* item );
    void Display( const KnowledgeGroup_ABC& group, ValuedListItem* item );
    void Display( const Agent_ABC& agent,          ValuedListItem* item );
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
    virtual void NotifyCreated( const Team_ABC& team );
    virtual void NotifyUpdated( const Team_ABC& team );
    virtual void NotifyDeleted( const Team_ABC& team );

    virtual void NotifyUpdated( const KnowledgeGroup_ABC& group );
    virtual void NotifyUpdated( const Agent_ABC& agent );
    virtual void NotifyUpdated( const AutomatDecisions& decisions );

    virtual void Select( const Team_ABC& element );
    virtual void Select( const KnowledgeGroup_ABC& element );
    virtual void Select( const Agent_ABC& element );
    virtual void BeforeSelection();
    virtual void AfterSelection();
    virtual void NotifyActivated( const Agent_ABC& element );
    virtual void OptionChanged( const std::string& name, const OptionVariant& value );

    template< typename ParentType, typename ChildType >
    void RecursiveDisplay( const ParentType& value, ValuedListItem* item );

    template< typename Type >
    void Update( const Type& value );

    QDragObject* dragObject();
    void dropEvent( QDropEvent* pEvent );
    void dragEnterEvent( QDragEnterEvent* pEvent );

    bool Drop( ValuedListItem& item, ValuedListItem& target );
    template< typename T >
    bool DoDrop( ValuedListItem& item, ValuedListItem& target );

    virtual bool Drop( const Agent_ABC& item, const Agent_ABC& target );
    virtual bool Drop( const Agent_ABC& item, const KnowledgeGroup_ABC& target );
    virtual bool Drop( const Agent_ABC& item, const Team_ABC& target );
    virtual bool Drop( const KnowledgeGroup_ABC& item, const Agent_ABC& target );
    virtual bool Drop( const KnowledgeGroup_ABC& item, const KnowledgeGroup_ABC& target );
    virtual bool Drop( const KnowledgeGroup_ABC& item, const Team_ABC& target );
    virtual bool Drop( const Team_ABC& item, const Agent_ABC& target );
    virtual bool Drop( const Team_ABC& item, const KnowledgeGroup_ABC& target );
    virtual bool Drop( const Team_ABC& item, const Team_ABC& target );
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    Publisher_ABC& publisher_;
    ItemFactory_ABC& factory_;
    const Team_ABC* currentTeam_;
    //@}
};

#endif // __AgentListView_h_
